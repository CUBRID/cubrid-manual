Database File Errors
====================


.. _ERROR-17:

**ERROR CODE: -17, 'Internal error: fetching deallocated pageid %1$d of volume "%2$s".'**

- This message is an internal error that occurs when the CUBRID system attempts to fetch a page ID that has already been deallocated; it means that a problem has occurred in page allocation/deallocation management, memory management, or page access logic, and it can lead to corruption of database consistency and data loss.


.. _ERROR-19:

**ERROR CODE: -19, 'Internal error: pageptr = %1$p of page %2$d of volume "%3$s" is not fixed.'**

- This message is an internal error that occurs when the CUBRID system attempts to access a page pointer for a specific page in a specific volume while that page pointer is not "fixed" in memory; it means that a problem has occurred in page buffer management, memory protection, or page access logic, and it can lead to corruption of database consistency and data loss.


.. _ERROR-35:

**ERROR CODE: -35, 'Internal error: Unknown volume identifier %1$d.'**

- This message is an internal error that occurs when the CUBRID system tries to reference a volume with a specific volume ID (Volume Identifier), but it cannot find a volume corresponding to that volume ID or recognizes it as an invalid volume; it can mainly occur when deleting a temporary volume (temp temp volume).


.. _ERROR-38:

**ERROR CODE: -38, 'Internal error: Unknown file VFID %1$d|%2$d.'**

- This message occurs when, in the CUBRID database, while executing the loaddb command, it cannot find the object file (object file) or cannot open the file due to issues such as file permission problems.


.. _ERROR-43:

**ERROR CODE: -43, 'Internal error: The page %1$d of volume "%2$s" may be corrupted. %3$d records was found when %4$d was expected.'**

- This message is an internal error that occurs when the CUBRID system, while inspecting a slotted page of a specific volume, detects that the number of actual records found in the page does not match the expected number of records recorded in the page header; this mismatch strongly suggests that the internal structure of the slotted page has been damaged and indicates that database consistency has been broken; this message mainly occurs while performing a slot cleanup/compaction operation on the page.


.. _ERROR-44:

**ERROR CODE: -44, 'No space available in slotted page %1$d of volume "%2$s".'**

- This message is an error indicating that the CUBRID system attempted to insert new data into a slotted page of a specific volume or to expand existing data, but there is no more available space on that page; it mainly occurs when the page is full, and operations such as inserting new records, updating existing records (size increase), or adding index entries may fail.


.. _ERROR-45:

**ERROR CODE: -45, 'Slot %1$d on page %2$d of volume "%3$s" is allocated to an anchored record. A new record cannot be inserted here.'**

- This message is an internal error indicating that the CUBRID system attempted to insert a new record into a slot on a specific page of a specific volume, but it cannot insert because that slot is already allocated to an "anchored record"; it indicates that there is a problem in the database's internal structure (especially the slotted-page structure) or that there is an error in the record insertion logic, and it can lead to insertion failure, query errors, or data loss.


.. _ERROR-46:

**ERROR CODE: -46, 'Internal error: slot %1$d on page %2$d of volume "%3$s" is not allocated.'**

- This message is an internal error that occurs when the CUBRID system attempts to access a slot on a specific page of a specific volume but detects that the slot is not allocated; it indicates that there is a problem in the database's internal structure (especially the slotted-page structure), and it can lead to data access failure, query errors, or data loss; this mainly occurs when accessing by slot ID on a database page and the corresponding slot does not exist.


.. _ERROR-47:

**ERROR CODE: -47, 'Unable to create a heap file in volume "%1$s".'**

- This message is an error indicating that the CUBRID system attempted to create a new heap file in the specified volume but failed; it may occur due to lack of disk space, file system permission issues, volume corruption, or problems in CUBRID's internal file system management; it directly affects operations such as creating new tables, creating indexes, or adding data to existing tables.


.. _ERROR-48:

**ERROR CODE: -48, 'Accessing deleted object %1$d|%2$d|%3$d.'**

- This message is an internal error that occurs when the CUBRID system attempts to access an object that has already been deleted or is invalid; it represents an internal inconsistency in which a reference to the object remains even after the object itself has been deleted, causing the system to try to access an invalid object; as a result, it can lead to data access failure, query errors, or system instability.


.. _ERROR-49:

**ERROR CODE: -49, 'Internal error: class of object %1$d|%2$d|%3$d is unknown.'**

- This message is an internal error that occurs when the CUBRID system cannot find the table (class) information for a specific object (identified by an OID); it indicates that the link between the object and the table has been damaged, that the table metadata is invalid, or that the database's internal consistency has been broken; as a result, it can lead to data access failure, query errors, or data loss.


.. _ERROR-52:

**ERROR CODE: -52, 'Internal error: object overflow address %1$d|%2$d|%3$d may be corrupted.'**

- This message is an internal error that occurs when the CUBRID system detects the possibility that an object's overflow address may be corrupted; it indicates that there is a problem in internal structure management of the database, and it can lead to data access failure, query errors, or data loss.


.. _ERROR-53:

**ERROR CODE: -53, 'Fetching object %1$d|%2$d|%3$d when only its OID has been assigned.'**

- This message is an error indicating that the CUBRID system attempted to fetch the data of a specific object, but only an OID (Object Identifier) has been assigned to that object and the actual data has not been stored; it indicates a timing issue between object creation and data storage, or a consistency issue that occurs during transaction processing.


.. _ERROR-55:

**ERROR CODE: -55, 'Internal error: A page cycle reference was detected on page %1$d|%2$d of heap file %3$d|%4$d|%5$d.'**

- This message is an internal error that occurs when the CUBRID system detects a cyclic reference between pages in a heap file; it indicates that there is a problem in internal structure management of the database, and it can lead to data access failure, query errors, or data loss.


.. _ERROR-56:

**ERROR CODE: -56, 'Internal error: unknown extendible hashing file (Volid: %1$d Fileid: %2$d) page (Volid: %3$d Pageid: %4$d) was specified.'**

- This message is an internal error that occurs when the CUBRID system tries to reference an unknown or invalid file and page in an extendible hashing structure; it indicates that there is a problem in internal structure management of the database, and it can lead to data access failure, query errors, or data loss.


.. _ERROR-57:

**ERROR CODE: -57, 'Key does not exist in the extendible hashing structure.'**

- This message is an error indicating that the CUBRID system attempted to find a specific key in an extendible hashing structure, but the key does not exist in the structure; it can occur when querying data with a non-existent key, when the key is incorrect, or when the hashing structure itself has a problem.


.. _ERROR-59:

**ERROR CODE: -59, 'Internal error: the specified key type %1$d is not valid for the extendible hashing structure.'**

- This message is an internal error that occurs when the CUBRID system detects that the key type to be used in an extendible hashing structure is an invalid type that is not supported by that structure; it indicates that there is a problem in internal structure management of the database, and it can lead to query execution failure or data access errors.


.. _ERROR-60:

**ERROR CODE: -60, 'Internal error: the extendible hashing structure has been corrupted.'**

- This message is an internal error that occurs when the CUBRID system detects that corruption has occurred in the database's extendible hashing structure; corruption of this structure has a fatal impact on the integrity of the database and may lead to data access failure, query errors, or data loss.


.. _ERROR-61:

**ERROR CODE: -61, 'Internal error: the directory root page of the extendible hashing structure (Volid: %1$d Fileid: %2$d Pageid: %3$d) has been corrupted.'**

- This message means that corruption was detected during an integrity check of the top-level directory page of Extendible Hashing in the CUBRID system; since the starting point of the index/hash directory is broken, access to the entire structure may be impossible or the results may not be trustworthy.


.. _ERROR-63:

**ERROR CODE: -63, 'Internal error: a temporary page was corrupted during sorting.'**

- This message is an internal error that occurs when the CUBRID system detects that a temporary page used during a data sorting operation has been corrupted; when sorting large amounts of data, CUBRID creates and uses temporary files on disk if memory is insufficient, and a page of that temporary file was corrupted during read/write; this can affect database consistency and the correctness of the sort operation and may lead to query failure.


.. _ERROR-66:

**ERROR CODE: -66, 'Internal error: unknown force operation %1$d for object %2$d|%3$d|%4$d.'**

- This message is an internal error that occurs when the CUBRID system attempts to perform an unknown or invalid "force operation" on a specific object; it can occur when there is a problem in the internal logic of the CUBRID engine or when the database's internal consistency has been severely damaged.


.. _ERROR-67:

**ERROR CODE: -67, 'Internal error: a heap file has not been allocated to store object %1$d|%2$d|%3$d.'**

- This message is an internal error that occurs when a heap file to store a specific object (OID) has not been allocated in the CUBRID system; it means that there is a problem in internal structure management of the database and it can have a fatal impact on object storage and access.


.. _ERROR-68:

**ERROR CODE: -68, 'Internal error: different classnames for class with oid = %1$d|%2$d|%3$d were found. Found classnames are "%4$s", "%5$s" using classname hash table and heap, respectively.'**

- This message is an internal error that occurs when the CUBRID system finds two different table names for a table with a specific object (OID); this discrepancy means that the database's internal metadata structure has been severely damaged and indicates that a fatal problem has occurred in the database's consistency and integrity.


.. _ERROR-69:

**ERROR CODE: -69, 'Internal error: different class object identifiers were found for class with name "%1$s". Found OIDS are %2$d|%3$d|%4$d, %5$d|%6$d|%7$d using classname hash table and heap, respectively.'**

- This message is an internal error that occurs when the CUBRID system finds two different object identifiers (OIDs) for a specific table; this discrepancy means that the database's internal metadata structure has been severely damaged and indicates that a fatal problem has occurred in the database's consistency and integrity.


.. _ERROR-70:

**ERROR CODE: -70, 'Internal error: Class with name "%1$s" and oid = %2$d|%3$d|%4$d does not exist in classname hash table.'**

- This message is an internal error that occurs when the CUBRID system attempts to find a specific table (OID) in the classname hash table but fails; the classname hash table is an internal data structure that CUBRID uses to quickly look up table names; this message indicates that an inconsistency or corruption has occurred in the database's metadata management system, meaning that there is a problem in the database's internal structure.


.. _ERROR-71:

**ERROR CODE: -71, 'Internal error: Class with name "%1$s" and oid = %2$d|%3$d|%4$d does not exist in its heap.'**

- This message is an internal error that occurs when the CUBRID system attempts to find a specific table (OID information) in the database heap area but fails; the heap refers to the space where actual data objects are stored; this message indicates that an inconsistency or corruption has occurred between the database metadata (table definition) and the actually stored data, meaning that there is a problem with the integrity of the database.


.. _ERROR-80:

**ERROR CODE: -80, 'Insufficient space in operating system device when writing logical log page %1$lld (physical page %2$lld) of "%3$s". Could not write more than %4$d bytes.'**

- This message is an error indicating that when the CUBRID system tried to write a logical log page to a physical page, the write failed because there was not enough space on the operating system device; it mainly occurs due to insufficient disk space, file system limits, or I/O errors.


.. _ERROR-81:

**ERROR CODE: -81, 'Internal error: logical log page %1$lld may be corrupted.'**

- This message is an internal error that occurs when the CUBRID system detects the possibility that a specific logical log page in the transaction log file of the database may be corrupted; it is a very important warning indicating database stability and the risk of data loss.


.. _ERROR-96:

**ERROR CODE: -96, 'Media recovery may be needed on volume "%1$s".'**

- This message is an error indicating that the CUBRID system detected a consistency or integrity problem in a database volume and that media recovery (Media Recovery) may be needed; it occurs when there is a possibility that a specific volume of the database has been damaged due to abnormal termination, disk damage, log file inconsistency, and so on.


.. _ERROR-97:

**ERROR CODE: -97, 'Internal error: unable to find log page %1$lld in log archives.'**

- This message is an internal error that occurs when the CUBRID system tries to find a specific log page in the log archives but fails; it occurs when a log page required while processing the database transaction logs does not exist in the archive files, the archive files are corrupted, or the archive files cannot be accessed.


.. _ERROR-98:

**ERROR CODE: -98, 'Unable to create archive log "%1$s" to archive pages from %2$lld to %3$lld.'**

- This message is displayed when the CUBRID system fails to create an archive log file while performing log archiving; it is an important error that can affect database consistency and recoverability.


.. _ERROR-296:

**ERROR CODE: -296, 'Invalid property list encountered.'**

- This message is an error that appears when, in the CUBRID database, an object's property list is invalid or corrupted; it occurs during validation of the property list in object representation or the schema management system, and it suggests that internal metadata or object definitions in the CUBRID database may be incorrect or damaged.


.. _ERROR-315:

**ERROR CODE: -315, 'Illegal metaclass definition encountered.'**

- This message is an error that appears when a metaclass definition is incorrect in the CUBRID database; a metaclass refers to a structure that defines CUBRID's system catalog tables, and this message occurs when the metaclass attribute definition is not correct.


.. _ERROR-316:

**ERROR CODE: -316, 'Transformer size calculation mismatch, expected %1$d calculated %2$d.'**

- This message is an error that appears when, during object transformation in the CUBRID database, the expected size and the actually calculated size do not match; the system calculates the size before storing the object on disk and verifies it after actual storage; a size mismatch can indicate an internal logic error or data corruption.


.. _ERROR-317:

**ERROR CODE: -317, 'Invalid disk representation encountered for class "%1$s".'**

- This message is an error that appears when the disk representation (representation) of a table is invalid in the CUBRID database; the disk representation is a structure that defines how an object is stored on disk, and it occurs when a representation corresponding to a specific representation ID cannot be found for the table.


.. _ERROR-318:

**ERROR CODE: -318, 'Out of sync during object loading. Database is likely corrupted or out of date.'**

- This message is an error that appears when a synchronization (sync) problem occurs while loading objects in the CUBRID database; it occurs when the size of the data read from disk differs from the expected size; it can generally occur due to database file corruption, version compatibility issues, or memory corruption.


.. _ERROR-406:

**ERROR CODE: -406, 'Invalid B+tree index identifier: (vfid = (%1$d, %2$d), rt_pgid: %3$d).'**

- This message is an error that appears when a B-tree index identifier (BTID) is invalid in the CUBRID database; it mainly occurs during validity checks of index identifiers in B-tree storage management when the vfid (volume file ID) or pageid of the B-tree index identifier is wrong.


.. _ERROR-407:

**ERROR CODE: -407, 'Unknown key %1$s referenced in B+tree index {vfid: (%2$d, %3$d), rt_pgid: %4$d, key_type: %5$s}.'**

- This message is a system error that occurs when trying to reference a non-existent or corrupted key in a B+tree index structure; it is an important error message indicating that there is a problem with the integrity of the database index structure, and it may mean that index consistency has been broken or data corruption has occurred.


.. _ERROR-412:

**ERROR CODE: -412, 'Invalid range search specification.'**

- This message is an error that appears when, while performing a range search using a B-tree index in the CUBRID database, the range search specification is invalid; it occurs when an unsupported range search type or an incorrect range search condition is used in the range search validity check process in B-tree storage management.


.. _ERROR-415:

**ERROR CODE: -415, 'Invalid class identifier: %1$d|%2$d|%3$d.'**

- This message occurs when the internal table identifier (OID, Object Identifier) used in the CUBRID database is invalid or when it references a table that does not exist; it can occur when an incorrect OID is passed in system catalog access, query execution, object access, and so on, when a table has been deleted/changed and is no longer valid, or due to internal data corruption.


.. _ERROR-416:

**ERROR CODE: -416, 'Unknown representation identifier: %1$d.'**

- This message occurs when the ID (REPRID) that identifies the internal representation (Representation) of an object (table, record, etc.) in the CUBRID database is invalid or when it references a value that is not registered in the system; it can occur when an incorrect identifier is passed in system catalog access, object access, query execution, and so on, or when the identifier was lost/mismatched during table structure changes, data corruption, or recovery/migration processes.


.. _ERROR-417:

**ERROR CODE: -417, 'Invalid representation identifier: %1$d.'**

- This message occurs when the ID (REPRID) that identifies the internal representation (Representation) of an object (table, record, etc.) in the CUBRID database is invalid or when an unacceptable value is passed; it can occur when an incorrect identifier is used in system catalog access, object access, query execution, and so on, or when the identifier is mismatched or corrupted due to table structure changes, data corruption, or recovery/migration processes.


.. _ERROR-421:

**ERROR CODE: -421, 'Representations Directory Missing in Catalog for Class: %1$d|%2$d|%3$d.'**

- This message is an error that appears when the representation directory of a table is missing from the system catalog in the CUBRID database; there is a very high possibility that this error occurs when required information cannot be found due to system catalog corruption.


.. _ERROR-422:

**ERROR CODE: -422, 'Representation Information Record Missing in Catalog for Class Repr_Id: %1$d|%2$d|%3$d %4$d.'**

- This message is an error that appears when the representation information record of a specific table is missing from the system catalog in the CUBRID database; there is a very high possibility that this error occurs when required information cannot be found due to system catalog corruption.


.. _ERROR-475:

**ERROR CODE: -475, 'No query specification with index %1$s.'**

- This message is an error that appears when the CUBRID database cannot find a query specification (Query Specification) corresponding to a specific index number; a query specification refers to an SQL query definition used in a virtual table or a view, and each specification is identified by an index number; this message occurs when trying to query or delete a query specification with a non-existent index number.


.. _ERROR-540:

**ERROR CODE: -540, '%1$s.'**

- This message is a general error message that appears when a critical error occurs in the CUBRID database; it is a generic error message mainly used when an internal system error or an unexpected situation occurs; the specific contents of this error may be a trigger compilation error, a B+Tree structure error, or corruption or inconsistency of the index structure.


.. _ERROR-544:

**ERROR CODE: -544, 'Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). B+tree key %6$s entry for object OID: %7$d|%8$d|%9$d was not found on B+tree: %10$d|%11$d|%12$d.'**

- This message is an internal error that appears when the CUBRID database cannot find a specific key entry in a B+tree index; it mainly occurs during an index consistency check when the key of an object that exists in the heap does not exist in the B+tree index; it is an internal error indicating data inconsistency between the index and the heap.


.. _ERROR-545:

**ERROR CODE: -545, 'Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). Key and OID: %6$d|%7$d|%8$d entry on B+tree: %9$d|%10$d|%11$d is incorrect. The object does not exist.'**

- This message is an internal error that appears when the key and OID entry of a B+tree index in the CUBRID database is not correct; it mainly occurs during an index scan when an OID that exists in the B+tree does not actually exist in the heap; it is an internal error indicating data inconsistency between the index and the heap.


.. _ERROR-546:

**ERROR CODE: -546, 'Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). Expecting %6$d OID entry values, but %7$d were found on B+tree: %8$d|%9$d|%10$d.'**

- This message is an internal error that appears when the number of OID entries in a B+tree index in the CUBRID database differs from what is expected; it mainly occurs during an index consistency check when the number of OIDs in the heap does not match the number of OIDs in the B+tree; it is an internal error indicating data inconsistency between the index and the heap.


.. _ERROR-551:

**ERROR CODE: -551, 'Unable to locate volume information path file "%1$s"... Continue reading from internal tables'**

- This message is an error that appears when the CUBRID database cannot find the volume information file (DB_vinf); it mainly occurs when the volume information file cannot be accessed during database boot or during a log page buffer scan process.


.. _ERROR-585:

**ERROR CODE: -585, 'Unknown heap %1$s|%2$d|%3$d'**

- This message is displayed when trying to access a heap file that does not exist or is unknown in the CUBRID database; a heap file is the basic unit for storing data in CUBRID, and each heap file has a unique identifier (HFID).


.. _ERROR-587:

**ERROR CODE: -587, 'Entries of permanent volumes are unsorted in your "%1$s" volinfo file. Entry %2$d: %3$d %4$s is out of sequence.'**

- This message indicates that the permanent volume entries in the CUBRID database's DBname_vinf file are not sorted in the correct order; the DBname_vinf file stores volume information for the database and must be sorted in volume ID order, including file paths and file information.


.. _ERROR-596:

**ERROR CODE: -596, 'The %1$d pages of total temporary space allowed have been exceeded.'**

- This message indicates that the number of pages of total temporary space allowed for the backend (Backend) to use in the CUBRID database has been exceeded; CUBRID uses temporary space to store temporary data when processing complex queries, sorting, joining, and so on; this message occurs when the temporary space limited by a setting parameter such as `temp_file_max_size_in_pages` in the cubrid.conf file has been exceeded.


.. _ERROR-597:

**ERROR CODE: -597, 'Number of pages for heap file %1$d|%2$d|%3$d is inconsistent. \n%4$d and %5$d were found according to heap chain and file table, respectively.'**

- This message is an internal error indicating that, when executing the checkdb command in the CUBRID database, an inconsistency in the number of pages in a heap file was found; a heap file is the space where actual data records are stored, the heap chain is the logical linkage of pages within the heap file, and the file table refers to page information managed at the file-system level.


.. _ERROR-603:

**ERROR CODE: -603, 'Internal Error: Sector/page table of file VFID %1$d|%2$d seems corrupted.'**

- This message occurs when corruption is detected in the internal structure (sector/page table) of the file system in the CUBRID database; it is a fatal internal error indicating that the integrity of the database file has been broken and that normal data access and management may be impossible.


.. _ERROR-610:

**ERROR CODE: -610, 'Your database is likely to be corrupted since logging was turned off when your database crashed.'**

- This message occurs during the log recovery process in the CUBRID database; it means that there is a high possibility that the database has been corrupted because logging was disabled at the time the database crashed.


.. _ERROR-614:

**ERROR CODE: -614, 'Number of active log archives has been exceeded the max desired number of %1$d.'**

- This message appears when the number of active log archives in the CUBRID database has exceeded the configured maximum allowed number; it is a notification that occurs during the process in which the log archive management system automatically deletes old log archives.


.. _ERROR-625:

**ERROR CODE: -625, 'Internal Error. Trying to update the wrong instance object %1$d|%2$d|%3$d attribute information template with instance object %4$d|%5$d|%6$d.'**

- This message is displayed internally in the CUBRID database; it is an internal error caused by an OID mismatch in attribute information for the wrong record object; an OID (Object Identifier) is an 8-byte identifier that uniquely identifies each object in a CUBRID database.


.. _ERROR-626:

**ERROR CODE: -626, 'Internal Error. %1$d requested attributes were not found.'**

- This message is displayed internally in the CUBRID database; it appears when some of the requested attributes cannot be found, and it occurs while caching or retrieving attribute information for a table.


.. _ERROR-638:

**ERROR CODE: -638, 'Warning: Flushing a non-updatable log archive pageid = %1$d'**

- This message is a warning that occurs when trying to flush a log archive page that cannot be updated in the CUBRID database; it occurs when a flush operation is attempted while the log page is outside the archive range or is in a state where it can no longer be updated.


.. _ERROR-644:

**ERROR CODE: -644, 'LOG FATAL ERROR: %1$s'**

- This message indicates a fatal error that occurs in the log system of the CUBRID database; it is used when an unrecoverable error occurs during log processing, and it is an error that can affect the integrity and stability of the database.


.. _ERROR-694:

**ERROR CODE: -694, 'SYSTEM ERROR: Unable to load B+tree.'**

- This message is a system-level error that occurs when trying to load (create) a B+tree index in the CUBRID database; it can occur due to insufficient memory, disk I/O errors, parameter validation failures, or internal algorithm errors during index creation.


.. _ERROR-698:

**ERROR CODE: -698, ' Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). the number of OIDs in the unique hierarchy: %6$d does not equal the number of OIDs: %7$d found in the unique B+tree plus the number of NULLs: %8$d found in the unique hierarchy for B+tree: %9$d|%10$d|%11$d.'**

- This message indicates an internal consistency error in the CUBRID database; it occurs when the total number of OIDs in the hierarchy to which a UNIQUE constraint for an index of a specific table applies does not match the sum of the number of OIDs found in the B+tree and the number of NULLs found in the unique hierarchy; this means a mismatch between database metadata (schema information) and the actual B+tree index structure and suggests that data integrity may have been damaged or that there may be a problem in the index structure.


.. _ERROR-699:

**ERROR CODE: -699, 'Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). the number of OIDs in the unique hierarchy: %6$d does not equal the number of OIDs: %7$d found in the unique B+tree root statistics for B+tree: %8$d|%9$d|%10$d.'**

- This message indicates an internal consistency error in the CUBRID database; it occurs when the number of OIDs (Object Identifiers) in the hierarchy to which a UNIQUE constraint for an index of a specific table applies does not match the number of OIDs recorded in the B+tree root statistics for that index; this means a mismatch between database metadata (schema information) and the actual B+tree index structure and suggests that data integrity may have been damaged or that there may be a problem in the index structure.


.. _ERROR-700:

**ERROR CODE: -700, 'Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). the number of OIDs: %6$d does not equal the number of NULLs: %7$d plus the number of keys: %8$d in the unique B+tree: %9$d|%10$d|%11$d.'**

- This message indicates an internal consistency error in the CUBRID database; it occurs when, in a unique B+tree for an index of a specific table, the total number of OIDs does not match the sum of the number of NULL values and the number of keys; this means that the internal statistics of the B+tree index are inconsistent and suggests that data integrity may have been damaged or that there may be a problem in the index structure.


.. _ERROR-702:

**ERROR CODE: -702, 'Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). the number of NULLs in the unique hierarchy: %6$d does not equal the number of NULLs: %7$d found in the unique B+tree root statistics for B+tree: %8$d|%9$d|%10$d.'**

- This message indicates an internal consistency error in the CUBRID database; it occurs when the number of NULL values in the hierarchy to which a UNIQUE constraint for an index of a specific table applies does not match the number of NULL values recorded in the B+tree root statistics for that index; this means a mismatch between database metadata (schema information) and the actual B+tree index structure and suggests that data integrity may have been damaged or that index statistics may be incorrect.


.. _ERROR-703:

**ERROR CODE: -703, 'Internal error: INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d). OID: %6$d|%7$d|%8$d found in unique B+tree: %9$d|%10$d|%11$d does not belong to one of the classes for the unique constraint.'**

- This message indicates an internal consistency error in the CUBRID database; it occurs when an object OID found in the unique B+tree for an index of a specific table does not belong to one of the classes in the table hierarchy to which the unique constraint applies (UNIQUE hierarchy); this means a mismatch between the database metadata (schema) and the actual data storage structure (B+tree) and suggests that data integrity may have been damaged.


.. _ERROR-714:

**ERROR CODE: -714, 'Query failed due to insufficient temporary file space.'**

- This message is an error that appears when query processing fails because there is insufficient temporary file space during query execution in the CUBRID database; CUBRID uses temporary files to store intermediate results when processing complex queries (e.g., sorting, joins, grouping, etc.).


.. _ERROR-725:

**ERROR CODE: -725, 'Internal error: expected temporary oid and encountered permanent oid.'**

- This message is an internal error that appears when, while processing a collection data type in the CUBRID database, a temporary OID was expected but a permanent OID was encountered; an OID is an identifier that uniquely identifies database objects in CUBRID, and this message occurs when the object's state and the expected OID type do not match.


.. _ERROR-728:

**ERROR CODE: -728, 'Query failed because temporary file vfid is invalid for transaction %1$d.'**

- This message is an error that appears when, during query execution in the CUBRID database, the VFID (Volume File ID) of a temporary file is invalid for a specific transaction; CUBRID uses temporary files to store intermediate results when processing complex queries or large amounts of data, and a VFID is an identifier that uniquely identifies a file in CUBRID and is composed of a volume ID and a file ID; this message occurs when the VFID of the temporary file is corrupted or when the temporary file associated with the transaction is in an unexpected state.


.. _ERROR-858:

**ERROR CODE: -858, 'Volume "%1$s" is unknown at server restart.'**

- This is a warning message indicating that, when the database server starts up or shuts down, it deletes unnecessary temp temp volumes, and it is deleting because there is a temp temp volume that is not registered in the system information.



.. _ERROR-909:

**ERROR CODE: -909, 'Missing or invalid catalog class/vclass is found.'**

- This message occurs when the CUBRID database system cannot find a catalog table or a virtual table (view) that it uses internally, or when the definition of that catalog is not valid; in other words, it means a situation in which the system cannot operate normally because a problem has occurred in internal objects that manage the core metadata of the database (tables, columns, indexes, user information, and other information essential for system operation); such situations generally occur due to database corruption, abnormal system shutdown, database upgrade failure, or errors in CUBRID's internal metadata management logic, and it is a fatal protective error to protect the integrity and stability of the database system.


.. _ERROR-934:

**ERROR CODE: -934, 'Null domain referenced.'**

- This message occurs in the CUBRID database due to the domain information for a data type being NULL or invalid, and it is generally caused by an incorrect data type definition.



.. _ERROR-983:

**ERROR CODE: -983, 'Instances of a reusable OID class are non-referable. This operation is not permitted on non-referable instances.'**

- This message occurs when, in the CUBRID database, you try to reference a record of a table with the `REUSE_OID` option set; `REUSE_OID` is a special CUBRID feature that reuses the OIDs of deleted records to improve the space efficiency of the database; however, records in a table with this feature enabled are restricted so that they cannot be referenced from outside, and this is a protective error to protect data integrity and to prevent reference errors caused by OID reuse; it can especially occur when a foreign key constraint, object reference, or a reference from another table is attempted.


.. _ERROR-1016:

**ERROR CODE: -1016, '%1$s external storage error: %2$s'**

- This message is a general error that occurs when the CUBRID database system processes LOB (Large Object) data or external files using External Storage; that is, it means that when the CUBRID server attempts to access an external storage system, operations such as file creation, reading, writing, and deletion have failed; it can occur due to failure to initialize external storage, file system permission issues, insufficient disk space, network connectivity issues, or errors in the external storage system itself, and it is a protective error that occurs during the integration process with external storage in CUBRID's LOB data management system.


.. _ERROR-1017:

**ERROR CODE: -1017, 'Path for external storage '%1$s' is invalid.'**

- This message occurs when the CUBRID database system, while validating the path of External Storage, determines that the path is invalid; that is, when the CUBRID server attempts to initialize or use an external storage path for storing LOB data, it means that the path format is incorrect, an unsupported external storage type was specified, or a path component is missing, and it is a protective error that occurs during the process of validating the path in CUBRID's external storage system.


.. _ERROR-1019:

**ERROR CODE: -1019, 'External storage is not initialized because the path is not specified in "databases.txt".'**

- This message occurs when initialization fails because the external storage (LOB) path of the CUBRID database is not defined in the databases.txt file; it indicates that the external storage path setting for storing LOB (Large Object) data is missing.


.. _ERROR-1020:

**ERROR CODE: -1020, 'External file "%1$s" was not found.'**

- This message occurs when CUBRID tries to find a specific file in external storage (a LOB repository or another external file system) but the file does not exist; it indicates a situation in which the file cannot be found due to an incorrect file path, the file having actually been deleted, access permission issues, and so on.


.. _ERROR-1075:

**ERROR CODE: -1075, 'Descending index scan aborted because of lower priority on B+tree with index identifier: (vfid = (%1$d, %2$d), rt_pgid: %3$d).'**

- This message occurs when, in the CUBRID database, a descending scan operation on a B+tree index is aborted because the system internally determined that the operation has a lower priority; it mainly occurs when, during a descending scan, the desired B-tree page cannot be obtained.


.. _ERROR-1084:

**ERROR CODE: -1084, 'Skip invalid page in checkpoint. (page id: %1$d, "%2$s", oldest_unflush_lsa: %3$lld|%4$d, previous checkpoint redo lsa: %5$lld|%6$d)'**

- This message is an error message that occurs because, during checkpoint in the CUBRID database, the internal time information does not match; it is an error that has no impact on system operation and data consistency.



.. _ERROR-1125:

**ERROR CODE: -1125, 'Create the overflow key file. INDEX %1$s%2$s ON CLASS %3$s%4$s. key: %5$s%6$s.'**

- This is an informational notice that occurs when, while creating or updating a B-tree index in the CUBRID database system, the index key size exceeds the maximum size that can be stored directly on a page; that is, when the CUBRID server encounters a key larger than a certain page size while processing B-tree index operations, it automatically creates an overflow key file and stores the key in a separate overflow page.
이는 B-tree의 깊이를 제한하고 인덱스 성능을 유지하기 위한 정상적인 동작으로, 오류가 아닌 시스템의 자동 최적화 기능입니다.


.. _ERROR-1126:

**ERROR CODE: -1126, 'Create a new overflow page. INDEX %1$s%2$s ON CLASS %3$s%4$s. key: %5$s%6$s.'**

- This is an informational notice that occurs when, while creating or updating a B-tree index in the CUBRID database system, the number of OIDs (Object Identifiers) that can be stored in an index leaf node exceeds the page limit and an overflow page is created.
즉, CUBRID 서버가 B-tree 인덱스 작업을 처리하는 과정에서, 페이지 특정 크기에 맞는 최대(OID 개수)보다 많은 OID를 하나의 키에 대해 저장해야 할 때 자동으로 오버플로우 페이지를 생성하여 추가 OID를 별도 페이지에 저장합니다. 이는 B-tree의 깊이를 제한하고 인덱스 성능을 유지하기 위한 정상적인 동작으로, 오류가 아닌 시스템의 자동 최적화 기능입니다.


.. _ERROR-1127:

**ERROR CODE: -1127, 'Delete an empty overflow page. INDEX %1$s%2$s ON CLASS %3$s%4$s. key: %5$s%6$s.'**

- This is an informational notice that occurs when, while cleaning up or optimizing a B-tree index in the CUBRID database system, an empty overflow page that is no longer used is deleted; that is, when the CUBRID server performs B-tree index maintenance, if OIDs (Object Identifiers) for a specific key are deleted or changed and the overflow page becomes empty, the system automatically cleans up and deletes the page; this is normal behavior to manage storage space efficiently and to optimize the index structure, and it is an automatic cleanup function of the system, not an error.


.. _ERROR-1128:

**ERROR CODE: -1128, 'Log recovery is started.'**

- This message is actually not an error but an informational message indicating that the log recovery process of the CUBRID database has started.


.. _ERROR-1129:

**ERROR CODE: -1129, 'Log recovery is finished.'**

- This message is actually not an error but an informational message indicating that the log recovery process of the CUBRID database has successfully completed.


.. _ERROR-1146:

**ERROR CODE: -1146, 'volume identifier %1$d does not exist.'**

- This message occurs when, in the CUBRID database, while trying to access a volume using a specific volume ID, the system detects that the volume does not exist in the system; that is, it is a diagnostic error that can occur when executing the `show volume header` statement and occurs when the database cannot find the physical storage space (volume) it is trying to reference; this situation can generally occur when the volume file has been deleted, the path has been changed, or incorrect volume information has been set in the database configuration files.


.. _ERROR-1149:

**ERROR CODE: -1149, 'Cannot find the page %1$d of volume %2$d.'**

- This message occurs when, in the CUBRID database, while trying to access a specific page of a specific volume, the page does not exist or has already been deallocated; that is, it is a diagnostic error that can occur when executing the `show slotted page header/slots` statement and occurs when the system cannot find the requested page; this situation can generally occur when the page has been deleted, the volume has been corrupted, or there is an incorrect page reference.


.. _ERROR-1151:

**ERROR CODE: -1151, 'The page %1$d of volume %2$d is not a slotted page.'**

- This message occurs when, in the CUBRID database, while trying to access a specific page of a specific volume, the system detects that the page is not in the expected "slotted page" (Slotted Page) format; that is, it is a diagnostic error that can occur when executing the show slotted page header/slots statement and occurs when the database expected the page to be a slotted page but the page header information or internal structure failed the validity checks for a slotted page.


.. _ERROR-1181:

**ERROR CODE: -1181, 'Manual vacuum is disabled for client-server mode.'**

- This message occurs when the CUBRID database is running in client-server (Client-Server) mode and the user attempts to perform a manual vacuum operation; in the client-server environment, CUBRID restricts manual vacuum operations in order to maintain database consistency and stability, and internally manages space reclamation and optimization through an automatic vacuum (auto vacuum) mechanism; therefore, this message is a protective error indicating that the manual vacuum command is not permitted in the current operating mode.

