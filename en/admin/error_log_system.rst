System Management Errors
========================


.. _ERROR-3:

**ERROR CODE: -3, 'Out of virtual memory: unable to allocate %1$zu memory bytes.'**

- This message indicates an error where the CUBRID system attempted to allocate a certain amount of virtual memory from the operating system, but the allocation failed because there was not enough virtual memory available on the system; virtual memory includes physical RAM and disk swap space, and CUBRID uses a large amount of memory for database work (e.g., query execution, buffer cache, transaction management, internal data structures); this message typically occurs when the system has insufficient physical RAM, the swap space is exhausted, or the memory limit assigned to the CUBRID process has been exceeded, and as a result it can interfere with normal database service.


.. _ERROR-7:

**ERROR CODE: -7, 'Trying to format disk volume "%1$s" with an incorrect value %2$d for number of pages.'**

- This message indicates an error where the CUBRID system attempted to format a particular disk volume, but the format operation failed because the specified number of pages is invalid or not an allowed value; it mainly occurs in the file-open stage due to incorrect settings, exceeding system limits, or an internal logic error, and it can interfere with database creation or volume expansion operations.


.. _ERROR-8:

**ERROR CODE: -8, 'Unable to format disk volume "%1$s" with %2$d pages (%3$lld bytes).'**

- This message indicates an error where the CUBRID system attempted to format a particular disk volume to the specified size, but the format operation failed due to a file-system–level problem; it mainly occurs in the file-open stage due to a file system error, a permission problem, a hardware problem, or a volume-file path problem, and it can interfere with database creation or volume expansion operations.


.. _ERROR-9:

**ERROR CODE: -9, 'Unable to format disk volume "%1$s" with %2$d pages (%3$lld Kbytes) due to insufficient space. Current space available is %4$d pages (%5$lld Kbytes).'**

- This message indicates an error where the CUBRID system attempted to format a particular disk volume, but the format operation failed because the available disk space is less than the requested size; it mainly occurs due to insufficient disk space, an error in volume-size configuration, or file-system limits, and it can interfere with database creation or volume expansion operations.


.. _ERROR-10:

**ERROR CODE: -10, 'Unable to mount disk volume "%1$s".'**

- This message indicates an error where the CUBRID system attempted to mount a particular disk volume, but the operation failed due to an operating-system or file-system–level problem; it mainly occurs due to file system corruption, a missing volume file, insufficient permissions, or a hardware problem, and it can interfere with normal database startup or recovery operations.


.. _ERROR-11:

**ERROR CODE: -11, 'Unable to mount disk volume "%1$s". The database "%2$s", to which the disk volume belongs, is in use by user %3$s on process %4$d of host %5$s since %6$s.'**

- This message indicates an error where the CUBRID system attempted to mount a particular disk volume, but the mount failed because the database to which that volume belongs is already in use by another CUBRID process; it is mainly an error that occurs when creating a lock file, and it happens when a lock file has already been created by another process or when the lock file cannot be created; this message typically occurs when two or more CUBRID server instances try to access the same database at the same time, and it is an important mechanism for protecting database consistency.


.. _ERROR-12:

**ERROR CODE: -12, 'Unable to dismount disk volume "%1$s".'**

- This message indicates an error where the CUBRID system attempted to dismount a particular volume, but the operation failed due to an operating-system or file-system–level problem; it mainly occurs in the file-close stage due to issues such as file system errors, permission problems, or hardware problems, and it can interfere with safe database shutdown or volume management operations.


.. _ERROR-13:

**ERROR CODE: -13, 'Unable to read page %1$d of disk volume "%2$s".'**

- This message indicates an error where the CUBRID system attempted to read a specific page of a particular volume from disk, but the read operation failed due to an error during the I/O operation; it mainly occurs due to disk errors, file system problems, permission problems, or hardware problems, and it can affect database stability and can lead to data corruption.


.. _ERROR-14:

**ERROR CODE: -14, 'Unable to write page %1$d of disk volume "%2$s".'**

- This message indicates an error where the CUBRID system attempted to write a specific page of a particular volume to disk, but the write operation failed due to an error during the I/O operation; it mainly occurs due to disk errors, file system problems, permission problems, or hardware problems, and it can affect database stability and can lead to data corruption.


.. _ERROR-15:

**ERROR CODE: -15, 'Unable to write page %1$d of disk volume "%2$s" due to insufficient space.'**

- This message indicates an error where the CUBRID system attempted to write a specific page of a particular volume to disk, but the write operation failed because there was not enough disk space at the operating-system level; it mainly occurs due to a full disk, file-system quota/limits, or improper disk-space management, and it can interfere with normal database operation and volume growth.


.. _ERROR-16:

**ERROR CODE: -16, 'Unable to rename disk volume "%1$s" to "%2$s".'**

- This message indicates an error where the CUBRID system attempted to rename a particular disk volume, but the operation failed due to a file-system–level problem; it mainly occurs due to insufficient permissions, conflicts with an existing target file/directory, the source file being in use, or other file-system issues, and it can interfere with volume management operations such as database maintenance, backup/restore, or storage migration.


.. _ERROR-78:

**ERROR CODE: -78, 'Internal error: an I/O error occurred while reading logical log page %1$lld (physical page %2$lld) of "%3$s".'**

- This message is an internal error indicating that, when the CUBRID system tried to read a logical log page from a physical page, an I/O (input/output) error occurred on the specified operating-system device and the read operation failed; this mainly occurs due to hardware issues (disk failure), file system corruption, OS-level I/O problems, or driver errors.


.. _ERROR-79:

**ERROR CODE: -79, 'Internal error: an I/O error occurred while writing logical log page %1$lld (physical page %2$lld) of "%3$s".'**

- This message is an internal error indicating that, when the CUBRID system tried to write a logical log page to a physical page, an I/O (input/output) error occurred on the specified operating-system device and the write operation failed; this mainly occurs due to hardware issues (disk failure), file system corruption, OS-level I/O problems, or driver errors.


.. _ERROR-82:

**ERROR CODE: -82, 'Unable to mount log volume "%1$s".'**

- This message indicates an error where the CUBRID database system failed to “mount” or access the specified log disk volume or file; this mainly occurs due to file-system permission problems, missing or corrupted files, disk-space shortages, or other storage-related I/O problems, and it can interfere with database startup, recovery, and transaction logging.


.. _ERROR-101:

**ERROR CODE: -101, 'Not a database volume file - %1$s.'**

- This message indicates an error that occurs when the CUBRID system tries to access or process a particular user file, but the file does not exist, the path is incorrect, or it is a file that the system cannot recognize; it mainly occurs when the file specified in the ``createdb`` command with the ``--more-volume-file`` option cannot be found, or when the volume information file (DBname_vinf) cannot be found.


.. _ERROR-102:

**ERROR CODE: -102, 'Unexpected EOF in volume information file.'**

- This message is an error that occurs while the CUBRID system reads the volume information (DBname_vinf) file; it mainly occurs when the file is incomplete or corrupted, or when it has been recorded/entered in a way that does not match the actual database volume information.


.. _ERROR-103:

**ERROR CODE: -103, 'The first volume in volume information file is not in order.'**

- This message is an error that occurs while the CUBRID system searches for the first volume in the database volume information (DBname_vinf) file; it means that the entries in the volume information file must be sorted sequentially, but their order has been changed or there are missing entries.


.. _ERROR-104:

**ERROR CODE: -104, 'The first volume in volume information file is not found.'**

- This message is an error that occurs while the CUBRID system searches for the first volume in the database volume information (DBname_vinf) file; it may indicate a database integrity issue, file corruption, or that the file has possibly been modified manually.


.. _ERROR-105:

**ERROR CODE: -105, 'Unable to open backup file "%1$s".'**

- This message is an error that occurs when the CUBRID system tries to access a backup file but cannot access it due to file-system permission problems, file corruption, or the file not existing; backup or restore operations can fail because this file-access failure blocks access to the backup file.


.. _ERROR-111:

**ERROR CODE: -111, 'Transaction was aborted.'**

- This message is an error that occurs when the CUBRID system unilaterally aborts an active transaction due to a server failure or a mode change; it means the system forcibly terminated the transaction without the client’s request or consent, and it mainly occurs when the server process is terminated unexpectedly or during failover/failback or other mode transitions, which can lead to client errors and rollback of in-flight work.


.. _ERROR-113:

**ERROR CODE: -113, 'Cannot restart server.'**

- This message is an error that occurs when the CUBRID system fails while trying to restart or initialize the database server; during server restart/initialization, this can occur due to insufficient system resources, permission problems, port conflicts, or other system-level errors, and it can prevent normal database service from starting.


.. _ERROR-114:

**ERROR CODE: -114, 'Unknown database "%1$s".'**

- This message is an error that occurs when the CUBRID system cannot recognize the specified path or name as a database; it means that database volumes or files have been renamed or copied outside of the database domain, so CUBRID cannot find the correct database structure.


.. _ERROR-118:

**ERROR CODE: -118, 'Unable to determine the current working directory.'**

- This message is an error that occurs when the CUBRID system fails while checking the current working directory; such failures mainly occur due to file-system permission problems, the directory having been deleted, or process permission issues.


.. _ERROR-123:

**ERROR CODE: -123, 'Unable to create database volumes.'**

- This message is an error that occurs when the CUBRID system tries to create volumes or files required for the database but fails due to file-system permissions, insufficient disk space, path problems, and similar issues; database volume creation can fail because the needed files cannot be created, which can block database creation, volume expansion, or certain maintenance operations.


.. _ERROR-130:

**ERROR CODE: -130, 'Unable to allocate %1$zu bytes.'**

- This message is an error that occurs when the CUBRID database system cannot allocate the required memory; it is printed when the system’s virtual memory is insufficient or when a memory allocation request fails, which indicates that the system’s memory resources have been exhausted.


.. _ERROR-313:

**ERROR CODE: -313, 'Memory allocation failure.'**

- This message occurs during the process of reading database object data, and it is an important memory-related error that mainly occurs during object loading or unloading.


.. _ERROR-320:

**ERROR CODE: -320, 'Encountered corrupted disk representation of object.'**

- This message is an error that appears when the representation of an object stored on disk is corrupted in a CUBRID database; it occurs when reading object data during the B-tree loading process, and it can be caused by disk I/O errors, memory corruption, or database file corruption.


.. _ERROR-542:

**ERROR CODE: -542, 'Corrupted disk volume. The number of remaining free sectors in volume header does not match the bitmap's.'**

- This message occurs on a particular volume (disk file) in the CUBRID database when the number of remaining free sectors recorded in the volume header (metadata) differs from the number of remaining free sectors calculated from the actual bitmap (the internal table that represents sector allocation status); such a mismatch means a consistency error in disk-space management, and it can appear when there were problems in the process of allocating/freeing storage space, or when the metadata and actual state became inconsistent due to physical damage, abnormal shutdown, bugs, and similar causes.


.. _ERROR-543:

**ERROR CODE: -543, 'Corrupted disk volume. Disk volume header is broken.'**

- This message occurs on a particular volume (disk file) in the CUBRID database when the disk header (metadata that represents the volume’s structure and state) is damaged or when the internally expected values differ from the actual values; because the disk header contains important metadata such as volume size, allocation information, version, and checksums, any mismatch can affect the entire storage structure of the database; it can mainly occur due to abnormal shutdown, disk errors, file-system corruption, bugs, and similar causes.


.. _ERROR-599:

**ERROR CODE: -599, 'I/O error while synchronizing the volume status.'**

- This message appears when an I/O error occurs while the CUBRID database synchronizes a volume’s status with disk; a volume is the physical storage unit of a CUBRID database and refers to a data file, and synchronization is the process of forcibly writing in-memory data to disk in order to guarantee data consistency and durability.


.. _ERROR-705:

**ERROR CODE: -705, 'Permanent volume number mismatch. Permanent volumes in boot page (%1$d). Permanent volumes in volume information file (%2$d).'**

- This message is an error that appears when, during CUBRID database boot or recovery, the number of permanent volumes recorded in the boot page (boot page) does not match the number of permanent volumes actually found by the system; this means there is a problem with the database’s volume configuration and it can affect database consistency and integrity; this message occurs when the CUBRID database server starts up or recovers after a crash.


.. _ERROR-708:

**ERROR CODE: -708, 'Not enough space in temporary volume.'**

- This message is an error that appears when the CUBRID database attempts to extend a temporary disk volume but fails because there is not enough disk space; it can occur when CUBRID extends temporary volumes for large query processing or temporary table creation.


.. _ERROR-780:

**ERROR CODE: -780, 'Cannot get system time.'**

- This message indicates that, when CUBRID tries to obtain the system time, it is in a situation where the system time cannot be obtained; this occurs in CUBRID’s system-time mechanism and can mainly be caused by failures in OS-level time-related function calls.


.. _ERROR-792:

**ERROR CODE: -792, 'Internal error: connection entry allocation error.'**

- This message is an internal error that occurs when the CUBRID system fails to allocate memory related to a connection entry; a memory allocation failure prevents the module from functioning normally and has a critical impact on client connection management and server communication; this mainly occurs due to insufficient system memory, memory fragmentation, or operating-system memory-allocation limits.


.. _ERROR-864:

**ERROR CODE: -864, 'Cannot open backupdb/restoredb verbose file '%1$s'.'**

This error occurs when, during a database backup or restore operation, the detailed (verbose) log file cannot be created or opened; it is an I/O-related error that can be caused by file-system permissions, insufficient disk space, path problems, and similar issues.


.. _ERROR-879:

**ERROR CODE: -879, 'Failed to get permission for "%1$s".'**

- This message occurs when CUBRID tries to query file-system permission information for a particular file or directory; this typically occurs when access permissions for database files or log files need to be checked, and it mainly appears when the user account running the CUBRID process does not have permission to query information about that file.


.. _ERROR-880:

**ERROR CODE: -880, 'Failed to set permission for "%1$s".'**

- This message occurs when CUBRID tries to change file-system permissions for a particular file or directory; this typically occurs when access permissions for database files or log files need to be adjusted, and it mainly appears when the user account running the CUBRID process does not have permission to modify that file.


.. _ERROR-881:

**ERROR CODE: -881, 'Failed to acquire lock for "%1$s".'**

- This message occurs when CUBRID tries to acquire a lock for a particular file; this typically occurs when opening or mounting database volumes and is related to the file system’s lock-management mechanism; it can be caused by file-system access problems, permission problems, a lock being held by another process, insufficient system resources, and similar issues.


.. _ERROR-882:

**ERROR CODE: -882, 'Failed to release lock for "%1$s".'**

- This message occurs when CUBRID tries to release a lock set on a particular file; this typically occurs when closing or unmounting database volumes and is related to the file system’s lock-management mechanism; it can be caused by file-system access problems, permission problems, insufficient system resources, and similar issues.


.. _ERROR-980:

**ERROR CODE: -980, 'Thread %1$u waited %2$lld milliseconds during %3$s.'**

- This message is a warning that occurs when a thread in the CUBRID database waits longer than the configured threshold while performing a particular task (entering a critical section, acquiring a lock, file I/O, and so on); it is part of a performance monitoring feature that detects situations where a thread is waiting longer than expected and notifies the system administrator; this is not an error but an informational message that helps discover performance issues early and respond to them, and it mainly occurs in critical sections, lock management, and file I/O operations and is useful for identifying bottlenecks or resource contention.


.. _ERROR-1015:

**ERROR CODE: -1015, 'Specified directory '%1$s' does not exist.'**

- This message occurs when the path of volume files used by the database server is incorrect, or when the directory cannot be created/used and therefore the required directory does not exist.


.. _ERROR-1113:

**ERROR CODE: -1113, 'Element index out of range.'**

- This message occurs when, in a CUBRID database, an attempt is made to add an element to a Binary Heap data structure and the heap’s maximum capacity has been exceeded; that is, it means an insertion was attempted while the binary heap was already full; this is a protective error intended to ensure memory management and system stability, because the binary heap is an important data structure used for sorting, priority queues, heap sort, and so on, and exceeding its capacity could lead to memory overflow or unexpected behavior.


.. _ERROR-1185:

**ERROR CODE: -1185, 'Unexpected page refix on page %1$d,%2$d, while trying to fix page %3$d,%4$d.'**

- This message occurs in the page buffer management system of the CUBRID database. When trying to fix a particular page in memory for access, it indicates that another page was unexpectedly refixed; this means that there is a problem with the internal consistency or state management of the page buffer, and in general it is an abnormal situation that should not occur. Because the page buffer is a core component of the database, this kind of error can lead to data integrity issues or system instability.
