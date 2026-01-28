CUBRID HA Errors
================

.. _ERROR-898:

**ERROR CODE: -898, 'Replication error: "%1$s"'**

- This message appears when various problems occur during replication operations in a CUBRID database HA (High Availability) replication environment. That is, it occurs during database replication processes such as memory allocation, class information processing, or replication log generation.


.. _ERROR-970:

**ERROR CODE: -970, 'Server HA mode is changed from '%1$s' to '%2$s'.'**

- This message is a system notification that occurs when the mode is changed in a CUBRID HA (High Availability) replication environment. It occurs when the server state transitions between states such as active, standby, and maintenance, and this is part of the normal HA operation process.


.. _ERROR-975:

**ERROR CODE: -975, 'Current version of replication does not allow changing multiple rows with a single UPDATE statement which can violate the UNIQUE constraint.'**

- This message is related to a special restriction that occurs in a CUBRID HA (High Availability) replication environment. Although changing multiple rows with a single UPDATE statement is normally allowed, in an environment where HA replication is enabled, multi-row UPDATE operations that may violate UNIQUE constraints are restricted. This is a protective error to ensure data consistency in the replication environment and to prevent synchronization problems between the master and the slave. When HA mode is enabled (when `ha_mode` is not `off`), the system rejects multi-row UPDATE operations that may violate UNIQUE constraints.


.. _ERROR-986:

**ERROR CODE: -986, 'CUBRID heartbeat feature started.'**

- This message is an informational notification indicating that the heartbeat subsystem has successfully started in a CUBRID HA (High Availability) replication environment. The heartbeat feature periodically checks the status of each node in a CUBRID cluster environment and plays an essential role in detecting failures and performing automatic failover. This message usually occurs when the CUBRID master process starts, when the `cubrid heartbeat start` command is executed, or when HA mode is enabled, and it means that the system has started to operate normally in high-availability mode.


.. _ERROR-987:

**ERROR CODE: -987, 'CUBRID heartbeat feature stopped.'**

- This message is an event notification indicating that the HA heartbeat subsystem has been deactivated in a CUBRID HA (High Availability) replication environment. It occurs during administrator stop commands, master shutdown or restart, HA mode OFF transitions, or deactivation request processing flows, and after this, failure detection and automatic failover functions are stopped.


.. _ERROR-988:

**ERROR CODE: -988, 'Node event: %1$s.'**

- This message is an informational message that notifies node state changes or event occurrences in the CUBRID HA (High Availability) system. It records various events related to node state monitoring, such as connections, disconnections, and state changes.


.. _ERROR-989:

**ERROR CODE: -989, 'Process event: %1$s. %2$s'**

- This message is a system message that notifies process events in the CUBRID HA (High Availability) system. It is used as a core monitoring message to track and record server state changes or important events.


.. _ERROR-990:

**ERROR CODE: -990, 'Command execution: %1$s. %2$s'**

- This message indicates that a problem occurred during the execution of a specific command inside the CUBRID HA (High Availability) system. In particular, this error suggests that a command execution failure occurred during heartbeat management or request processing, which are core functions of the CUBRID master process. When an actual error occurs, additional information about the specific command content or failure cause is provided, which means that important internal processes did not operate normally and may directly affect database stability and availability.


.. _ERROR-1023:

**ERROR CODE: -1023, 'log applier filter: %1$s.'**

- This message occurs when the log applier encounters a generic error while processing user-defined replication filter rules in a CUBRID HA (High Availability) replication environment. Replication filters define rules to exclude or include specific tables or data change operations (INSERT, UPDATE, DELETE) from replication, and this means that a problem occurred during the processing of those rules.


.. _ERROR-1024:

**ERROR CODE: -1024, 'log writer: failed to get log page(s) starting from page id %1$lld.'**

- This message occurs when the copylogdb process requests replication logs from the replication target server in a CUBRID HA (High Availability) environment.


.. _ERROR-1025:

**ERROR CODE: -1025, 'HA delay info: The replication delay (%1$d secs) has exceeded the limit (%2$d secs).'**

- This message indicates that the replication delay time between the master server and the standby server in a CUBRID HA (High Availability) environment has exceeded the threshold configured by the user (`ha_delay_limit`, `ha_delay_limit_delta`). That is, it means that the standby server is taking longer than the configured time to catch up with the latest changes of the master server. This may be a warning or informational message indicating that the data synchronization status between the master and standby is not good, and depending on the configuration, it may trigger specific actions such as additional transaction delays on the master.


.. _ERROR-1026:

**ERROR CODE: -1026, 'HA delay info: The replication delay (%1$d secs) is decreased below the acceptable level (%2$d secs).'**

- This message is an informational log indicating that a previously occurring replication delay condition in a CUBRID HA (High Availability) environment has been resolved. That is, it means that the replication delay time between the active (master) server and the standby server has been reduced below the user-configured threshold (`ha_delay_limit`, `ha_delay_limit_delta`), and the data synchronization status has returned to a normal range.


.. _ERROR-1027:

**ERROR CODE: -1027, 'log applier: failed to change apply state from '%1$s' to '%2$s'.'**

- This message is an error that occurs when the applylogdb process cannot change its operation state as requested by the internal system in a CUBRID HA (High Availability) environment.


.. _ERROR-1028:

**ERROR CODE: -1028, 'log applier: unexpected EOF record in archive log. LSA: %1$lld|%2$d.'**

- This message is an error message that occurs when the applylogdb process encounters an unexpected end-of-log record in a replicated archive log in a CUBRID HA (High Availability) environment, and the applylogdb process proceeds without applying the corresponding log record.


.. _ERROR-1029:

**ERROR CODE: -1029, 'log applier: invalid replication log page/offset. page HDR: %1$lld|%2$d, final: %3$lld|%4$d, append LSA: %5$lld|%6$d, EOF LSA: %7$lld|%8$d, ha file status: %9$d, is end-of-log: %10$d.'**

- This message occurs when the applylogdb process finds an invalid page ID or offset while processing replication logs in a CUBRID HA (High Availability) environment. It indicates a state where replication cannot proceed because the log record location information is not valid.


.. _ERROR-1030:

**ERROR CODE: -1030, 'log applier: invalid replication record. LSA: %1$lld|%2$d, forw LSA: %3$lld|%4$d, backw LSA: %5$lld|%6$d, Trid: %7$d, prev tran LSA: %8$lld|%9$d, type: %10$d.'**

- This message occurs during the process in which the applylogdb process verifies replication log records in a CUBRID HA (High Availability) environment. If the LSA chain (forward/backward linkage) or transaction information of the log record is determined to be invalid during verification, this error occurs.


.. _ERROR-1031:

**ERROR CODE: -1031, 'log applier: failed to apply the following statement. class: "%1$s", statement: "%2$s", server error: %3$d, %4$s.'**

- This message occurs when the applylogdb process on the standby server in a CUBRID HA (High Availability) environment fails to actually apply (execute) a change log (in the form of an SQL statement) received from the master server to the standby database. That is, it means that although the log itself was received and parsed normally, the corresponding SQL (mainly DDL statements) could not be executed in the current database state of the standby server. The causes of failure vary, including schema mismatch, data conflicts (such as unique key violations), constraint violations, and resource shortages, and the detailed 'server error' information included in the message indicates the specific reason for the failure.


.. _ERROR-1032:

**ERROR CODE: -1032, 'log applier: failed to apply insert replication log. class: "%1$s", key: "%2$s", server error: %3$d, %4$s'**

- This message occurs when the applylogdb process on the standby server in a CUBRID HA (High Availability) environment fails while trying to apply (execute) a replicated `INSERT` log received from the master server to the standby database. The most common cause is that a record with the same key already exists on the standby server, resulting in a primary key or unique key constraint violation (duplicate key). In addition, there may be other causes that prevent execution of the INSERT statement, such as schema mismatches or other constraint violations, and the included 'server error' information shows the direct reason for the failure.


.. _ERROR-1033:

**ERROR CODE: -1033, 'log applier: failed to apply update replication log. class: "%1$s", key: "%2$s", server error: %3$d, %4$s'**

- This message occurs when the applylogdb process on the standby server in a CUBRID HA (High Availability) environment fails while trying to apply (execute) a replicated `UPDATE` log received from the master server to the standby database. This means that the UPDATE operation was rejected on the standby server. Major causes include cases where the target record to be updated does not exist on the standby server, the update result violates a unique key or primary key constraint or other constraints (NOT NULL, FK, etc.), or schema mismatches, and the 'server error' information included in the message provides the specific reason for the failure.


.. _ERROR-1034:

**ERROR CODE: -1034, 'log applier: failed to apply delete replication log. class: "%1$s", key: "%2$s", server error: %3$d, %4$s'**

- This message occurs when the applylogdb process on the standby server in a CUBRID HA (High Availability) environment fails while trying to apply (execute) a replicated `DELETE` log received from the master server to the standby database. This means that the DELETE operation was rejected on the standby server. Major causes include cases where the target record does not exist on the standby server, constraint violations (unique key, primary key, NOT NULL, FK, etc.), or schema mismatches, and the 'server error' information included in the message provides the specific reason for the failure.


.. _ERROR-1035:

**ERROR CODE: -1035, 'log applier: mem size(%1$d MB) of log applier is greater than max mem size (%2$d MB) or has been grow more than 2 times (%3$d MB). required LSA: %4$lld|%5$d. last committed LSA: %6$lld|%7$d.'**

- This message occurs when the amount of memory used by the applylogdb process on the standby server in a CUBRID HA (High Availability) environment exceeds the configured maximum allowed value (`ha_log_applier_max_mem_size`) or increases abnormally fast (more than two times in a short period). This is a protection mechanism to prevent resource exhaustion on the standby server and to maintain system stability. It can occur when a very large transaction (generating many logs) occurs on the master server, or when log application is delayed due to insufficient processing performance or network delays on the standby server. If the difference between the 'required LSA' and the 'last committed LSA' is large, it indicates that log application is significantly delayed.


.. _ERROR-1036:

**ERROR CODE: -1036, 'log applier: log applier shut itself down by signal.'**

- This message indicates a normal termination of the applylogdb process that occurs in the CUBRID HA (High Availability) system. It means that the log replication process performed a normal shutdown procedure after receiving a system signal. This usually occurs due to a planned system shutdown or an intentional shutdown request by an administrator.


.. _ERROR-1037:

**ERROR CODE: -1037, 'log writer: log writer shut itself down by signal.'**

- This message indicates a normal termination of the copylogdb log replication process that occurs in the CUBRID HA (High Availability) system. It means that the log replication process performed a normal shutdown procedure after receiving a system signal. This usually occurs due to a planned system shutdown or an intentional shutdown request by an administrator.


.. _ERROR-1038:

**ERROR CODE: -1038, 'log applier: log applier started. required LSA: %1$lld|%2$d. last committed LSA: %3$lld|%4$d. last committed repl LSA: %5$lld|%6$d.'**

- This message is an informational message printed when the applylogdb process starts in a CUBRID HA (High Availability) environment. It includes LSA information at the start time and shows the starting position of log replication and the last committed state. LSA is an address value that identifies the position of logs and serves as a reference point for data synchronization.


.. _ERROR-1039:

**ERROR CODE: -1039, 'log writer: log writer started. mode: %1$d.'**

- This message is an informational message printed when the copylogdb process starts in a CUBRID HA (High Availability) environment. It includes LSA information at the start time and shows the starting position of log replication and the last committed state. LSA is an address value that identifies the position of logs and serves as a reference point for data synchronization. This message indicates the normal startup of the copylogdb process.


.. _ERROR-1040:

**ERROR CODE: -1040, 'HA generic: %1$s.'**

- This message represents an exceptional or general error situation that occurred within the CUBRID HA (High Availability) system and was not classified under a specific error code. That is, it is a comprehensive error code used when it is no longer possible to proceed due to unexpected internal states or external factors while performing HA-related functions (heartbeat, copylogdb, applylogdb, etc.). Therefore, the detailed error messages included in the error message provide critical clues for diagnosing the actual cause of the error.


.. _ERROR-1105:

**ERROR CODE: -1105, 'Internal error: partially failed to flush dirty object.'**

- This message occurs when only part of the operation succeeds and part fails during the process of writing modified data (dirty objects) in memory to disk in the CUBRID database. That is, it means that some changes of a transaction were successfully written to disk, but some of the remaining changes failed to be written. This is an internal error to ensure data integrity and system stability, and it can mainly occur when data is applied by applylogdb and may occur during the replication process.


.. _ERROR-1122:

**ERROR CODE: -1122, 'Time out in receiving data.'**

- This message is an execution error that occurs in the copylogdb process among the problems that occur within the CUBRID HA (High Availability) system. It occurs when the requested log page is not received within the timeout period after requesting a log page from the cub_server process.


.. _ERROR-1133:

**ERROR CODE: -1133, 'Copylogdb for %1$s already exists.'**

- This message occurs when copylogdb is already running while copylogdb requests a connection to cub_master in a CUBRID HA (High Availability) environment.


.. _ERROR-1134:

**ERROR CODE: -1134, 'Applylogdb for %1$s already exists.'**

- This message occurs when applylogdb is already running while applylogdb requests a connection to cub_master in a CUBRID HA (High Availability) environment.


.. _ERROR-1139:

**ERROR CODE: -1139, 'Handshake error (peer host %1$s): incompatible read/write mode. (client: %2$s, server: %3$s)'**

- This message appears when a compatibility problem occurs with the read/write mode during the handshake process between the database client and the server in a CUBRID HA (High Availability) environment. That is, it occurs when the client and server modes (RW/RO) are different during the handshake process between the database client and the database server.


.. _ERROR-1140:

**ERROR CODE: -1140, 'Handshake error (peer host %1$s): HA replication delayed.'**

- This message appears when a delay occurs during the replication process between database servers in a CUBRID HA (High Availability) environment. It is an error that indicates a situation in which data synchronization between the master and slave is delayed beyond the configured threshold.


.. _ERROR-1141:

**ERROR CODE: -1141, 'Handshake error (peer host %1$s): replica-only client to non-replica server.'**

- This message occurs when a client attempts to directly access a server that is configured as replica-only in a CUBRID HA (High Availability) environment. That is, it occurs when a replica-only client (replica broker) requests a connection to a master or slave server that is not a replica server during the handshake process between the database client and the database server.


.. _ERROR-1142:

**ERROR CODE: -1142, 'Handshake error (peer host %1$s): unidentified server version.'**

- This message occurs when the client cannot identify the server version when attempting to connect to the database server in a CUBRID HA (High Availability) environment. It represents a situation where server version information is not properly delivered or is incompatible during the client-server handshake process.


.. _ERROR-1143:

**ERROR CODE: -1143, 'Handshake error (peer host %1$s): remote access to server not allowed.'**

- This message occurs when the database client (csql, cas, copylogdb, applylogdb, etc.) cannot identify version information between servers in a CUBRID HA (High Availability) environment.


.. _ERROR-1144:

**ERROR CODE: -1144, 'Timed out attempting to connect to %1$s. (timeout: %2$d sec(s))'**

- This message is an error that occurs when a CUBRID client or component (for example, CSQL, JDBC driver, etc.) attempts to connect to a specified server but fails to receive a response within the configured timeout period. It can generally occur due to network failures, abnormal server states, firewall blocking, or port opening failures.

- Reference: This message is summarized as "equipment down" in the CUBRID HA replication inconsistency detection section of the CUBRID manual.


.. _ERROR-1170:

**ERROR CODE: -1170, 'Failed to flush replication items (error code : %1$d).'**

- This message indicates that a failure occurred while flushing replication items, that is, changes generated in the master database, to the slave (replica) database during the replication process in a CUBRID HA (High Availability) environment. This can occur due to various causes such as network problems, disk I/O problems, internal state issues of the slave database, or problems with the replication items themselves.