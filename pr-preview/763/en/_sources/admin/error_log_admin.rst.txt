Database Management Errors
==========================


.. _ERROR-1:

**ERROR CODE: -1, 'Missing message for error code %1$d.'**

- This message is an error used when the CUBRID system encounters an error that it cannot define, it is printed together with an error message about the cause that occurred, and it is an error that is used in various ways when it cannot be specifically defined.


.. _ERROR-2:

**ERROR CODE: -2, 'Internal system failure: no more specific information is available.'**

- This message is a general (Generic) system error that occurs when an unexpected error has occurred inside the CUBRID system, or when it is not possible to set a more specific error code for a particular error situation, CUBRID is composed of various internal components and modules, and each component is designed to return a unique error code for a specific failure situation. However, sometimes this error is used when it is difficult to clearly identify a specific error due to exceptional situations or complex internal states, this message may be a sign of a serious internal problem, and the possibility of affecting the stability and consistency of the database is very high.


.. _ERROR-4:

**ERROR CODE: -4, 'Has been interrupted.'**

- This message is an error indicating that a task being executed by the CUBRID system was interrupted by an external signal or a system event, CUBRID can receive an interrupt signal while performing various tasks (query execution, data loading, backup, recovery, etc.), and in this case it safely interrupts the current task and returns this error.


.. _ERROR-83:

**ERROR CODE: -83, 'Length of path "%1$s" plus length of prefix logname "%2$s" is too long; the combined length must be less than %3$d.'**

- This message is displayed when the total length combining the path of the log file and the log name exceeds the maximum length allowed, CUBRID enforces a limit on the total path length of log files, and this message mainly occurs when the database path or log file name is too long and the overall path exceeds the system limit.


.. _ERROR-84:

**ERROR CODE: -84, 'Length of prefix logname "%1$s" is too long; the length must be less than %2$d.'**

- This message is displayed when the length of a log file name exceeds the maximum length allowed, CUBRID enforces a length limit on log file names, and this message mainly occurs when the database name is too long, or when the log file path exceeds the system limit.


.. _ERROR-85:

**ERROR CODE: -85, 'The prefix name "%1$s" is not the same as "%2$s" on the log disk. The log may have been renamed outside the database domain.'**

- This message is displayed when the name of the log file found on the log disk does not match the name expected by the database, this may occur when the database transaction log file has been manually renamed outside the database system, or when the log file is connected to the wrong database.


.. _ERROR-86:

**ERROR CODE: -86, 'Database is incompatible with current "%1$s" release "%2$s".'**

- This message indicates that the currently used CUBRID release (engine) version is not compatible with the database, it occurs when the database was created with a different version of CUBRID, or when the current CUBRID version is not compatible with the database format, this is mainly a compatibility issue that occurs during the CUBRID version upgrade/downgrade process.


.. _ERROR-87:

**ERROR CODE: -87, 'There are recovery actions that must recovered using "%1$s" release "%2$s" instead of release "%3$s". After the recovery, the database can be run on release "%4$s".'**

- This message indicates that the database recovery operation contains log records that are not compatible with the currently used CUBRID release, it is mainly caused by CUBRID version compatibility issues, and occurs when trying to recover log files created with a different version of CUBRID, or during the database upgrade/downgrade process.


.. _ERROR-88:

**ERROR CODE: -88, 'Internal error: Release string "%1$s" cannot exceed %2$d. Must change header log.'**

- This message is an internal error that occurs when the CUBRID system detects that the length of the "release string" stored in the log header exceeds the maximum length allowed, that is, it occurs when the release string length exceeds the allowed length when saving the log header.


.. _ERROR-89:

**ERROR CODE: -89, 'Log "%1$s" does not belong to the given database.'**

- This message is displayed when the CUBRID system determines that a specific log file does not belong to the database currently being worked on, it is an error that occurs during the consistency verification process between the log file and the database, and this can occur when the log file is connected to the wrong database, when the log file is corrupted, or when the database configuration is incorrect.


.. _ERROR-99:

**ERROR CODE: -99, 'Unable to create backup directory information file "%1$s".'**

- This message is displayed when the CUBRID system fails while trying to create the backup information file (DBname_bkvinf) that stores backup directory information when performing a database backup operation.


.. _ERROR-100:

**ERROR CODE: -100, 'Unable to backup database "%1$s".'**

- This message is displayed when the CUBRID system tries to perform a database backup operation, but the backup fails due to an error during the backup process, backup failures can occur due to various causes such as file system permission issues, insufficient disk space, file access errors, or insufficient system resources.


.. _ERROR-115:

**ERROR CODE: -115, 'Database "%1$s" already exists.'**

- This message is displayed when the CUBRID system tries to create a new database, but a database with the same name already exists, CUBRID database names are case-sensitive, and this message means that you cannot create a new database with the same name as an existing database.


.. _ERROR-116:

**ERROR CODE: -116, 'Database "%1$s" is unknown, or the file "databases.txt" cannot be accessed.'**

- This message is displayed when the CUBRID system cannot find the specified database, or cannot access the databases.txt file that manages the database list, it mainly occurs when the database name is incorrect, when the databases.txt file is corrupted, or when access is not possible due to file system permission issues.


.. _ERROR-117:

**ERROR CODE: -117, 'Absolute pathname for the database is too long. The combined length of the path "%1$s" plus the name "%2$s" is %3$d; the combined length must be less than %4$d.'**

- This message is displayed when the CUBRID system tries to handle the full pathname of the database (absolute path + database name), but the length exceeds the maximum length allowed by the system, it means that an overly long pathname cannot be handled due to file system or internal CUBRID limitations.


.. _ERROR-119:

**ERROR CODE: -119, 'Cannot find hostname ("%1$s") in "%2$s". Please check permissions of the file or if there is the hostname.'**

- This message is displayed when the CUBRID system tried to find a specific host name but failed, it mainly occurs in the process of configuring network communication or querying host information, and appears when the specified host name is invalid, or when it cannot access the file that contains host information (e.g., /etc/hosts).


.. _ERROR-120:

**ERROR CODE: -120, 'The maximum number of volumes (%1$d) has been exceeded.'**

- This message is displayed when the CUBRID system tries to add a new volume, but exceeds the maximum number of volumes allowed by the system., CUBRID limits the maximum number of volumes per database for system stability and performance. If this limit is exceeded, the volume addition fails.


.. _ERROR-121:

**ERROR CODE: -121, 'Trying to remove "%1$s" a permanent volume from the database.'**

- This message is displayed when the CUBRID system tries to remove a temporary volume (temp temp volume) from the database, a temporary volume is a space in the database for sorting queries, and deletion is not allowed while the database is running (operating). This error is an event message that informs the user that an attempt to remove a permanent volume has been detected.


.. _ERROR-122:

**ERROR CODE: -122, 'Unable to access system message catalog.'**

- This message is displayed when the CUBRID system tries to initialize message catalog files, but cannot access the message catalog files, the message catalog consists of files that include all of CUBRID's error messages and locale-specific messages, and they must be loaded during system initialization.


.. _ERROR-124:

**ERROR CODE: -124, 'Volume "%1$s" already exists.'**

- This message is displayed when the CUBRID system tries to create a new volume, but a volume with the same name already exists, it means that creation failed due to a duplicate volume name during volume expansion or new volume creation.


.. _ERROR-126:

**ERROR CODE: -126, 'Unknown purpose "%1$s" given on line %2$d.'**

- This message is displayed when the CUBRID system parses a database volume-addition configuration file and the 'volume purpose (PURPOSE)' value specified on a particular line is invalid or unrecognizable, it mainly occurs when using the --more-volume-file option in the `createdb` command, and it means that a string that is not allowed was entered into the PURPOSE parameter that specifies the purpose of a volume in the volume expansion configuration file.


.. _ERROR-127:

**ERROR CODE: -127, 'Incorrect value %1$d for number of pages given on line %2$d.'**

- This message is displayed when the CUBRID system parses a database volume-addition configuration file and the 'number of pages' value specified on a particular line is invalid, it mainly occurs when using the --more-volume-file option in the `createdb` command, and it means that the number of pages that specifies the size of a volume in the volume expansion configuration file was specified with a value that is not allowed.


.. _ERROR-128:

**ERROR CODE: -128, 'Number of pages was not given on line %1$d.'**

- This message is displayed when the CUBRID system parses a database volume-addition configuration file and the required 'number of pages' information is missing or invalid on a particular line, it mainly occurs when using the --more-volume-file option in the `createdb` command, and it means that the number of pages that defines the size of a volume in the volume expansion configuration file was not defined, or was specified in an incorrect format.


.. _ERROR-129:

**ERROR CODE: -129, 'Unknown token "%1$s" was found on line %2$d.'**

- This message is displayed when the CUBRID system parses a database volume-addition configuration file and finds an unexpected, undefined token (keyword), it means that an unsupported keyword or an incorrect syntax was used in the volume expansion configuration file.


.. _ERROR-131:

**ERROR CODE: -131, 'Database system error.'**

- This message indicates that an unexpected error occurred inside the CUBRID database system, it is generally printed when a problem occurs in the core components of the database engine, and mainly occurs when there is no related query or result information in a related query execution request.


.. _ERROR-137:

**ERROR CODE: -137, 'Corrupted authorization objects.'**

- This message occurs when permission information is inconsistent in the CUBRID database, or when a column is missing.


.. _ERROR-138:

**ERROR CODE: -138, 'Authorization class "%1$s" not found.'**

- This message occurs when a table cannot be found due to a permission problem in the CUBRID database.


.. _ERROR-139:

**ERROR CODE: -139, 'Access error on authorization attribute "%2$s" of class "%1$s".'**

- This message occurs when a related column cannot be accessed due to a permission problem in the CUBRID database.


.. _ERROR-140:

**ERROR CODE: -140, 'Operation "%1$s" can only be performed by the DBA or a DBA group member.'**

- This message is displayed when, in the CUBRID system, a user tries to perform a specific operation and that operation requires privileges that only the DBA or a member of the DBA group can have, it occurs when a general user attempts an operation that requires system administrator privileges, and it means that access has been denied for security reasons.


.. _ERROR-141:

**ERROR CODE: -141, 'Cannot add "%1$s" as a member of "%2$s".'**

- This message is a warning-type message that occurs during user creation in the CUBRID database, in the process of registering as a member of public.


.. _ERROR-142:

**ERROR CODE: -142, 'Adding member causes the user hierarchy to become a cyclic graph.'**

- This message is displayed when adding a member to a user or group causes a cyclic reference (cyclic dependency) in the user hierarchy structure in the CUBRID system, for example, it can occur when trying to add group B as a member of group A, and at the same time trying to add group A as a member of group B. Since this can cause an infinite loop or a logical contradiction, it is not allowed by the system.


.. _ERROR-143:

**ERROR CODE: -143, 'Encountered a class with no owner.'**

- This message is displayed when the object whose privileges are being modified has no owner in the CUBRID database.


.. _ERROR-146:

**ERROR CODE: -146, 'Cannot issue GRANT/REVOKE to owner of a %1$s.'**

- This message is displayed when attempting to perform GRANT on the owner's object in the CUBRID database.


.. _ERROR-147:

**ERROR CODE: -147, 'No GRANT option.'**

- This message is displayed when performing GRANT in the CUBRID database without giving the GRANT OPTION.


.. _ERROR-148:

**ERROR CODE: -148, 'Cannot obtain write lock on authorization object.'**

- This message is displayed when the CUBRID database cannot obtain a write lock during GRANT or REVOKE processing and therefore cannot update.


.. _ERROR-150:

**ERROR CODE: -150, 'Cannot revoke privileges from self.'**

- This message is displayed when attempting to perform REVOKE on one's own object in the CUBRID database.


.. _ERROR-151:

**ERROR CODE: -151, 'Cannot revoke privileges from owner of a %1$s.'**

- This message is displayed when attempting to perform REVOKE on the owner's object in the CUBRID database.


.. _ERROR-152:

**ERROR CODE: -152, 'GRANT not found.'**

- This message is displayed when, during REVOKE execution in the CUBRID database, it cannot find the related GRANT information.


.. _ERROR-153:

**ERROR CODE: -153, 'No authorization privileges in effect for the database.'**

- This message is displayed when, in the CUBRID database, there is no permission-related catalog class in the relevant DB.


.. _ERROR-154:

**ERROR CODE: -154, 'Incomplete authorization installation no authorization privileges in effect for the database.'**

- This message is displayed when, in the CUBRID database, the DBA and public users do not exist in the relevant DB.


.. _ERROR-156:

**ERROR CODE: -156, 'Authorization failure.'**

- This message is displayed when an unknown authorization is requested in the CUBRID database.


.. _ERROR-157:

**ERROR CODE: -157, 'SELECT authorization failure.'**

- This message occurs when the SELECT privilege does not exist on the relevant table in the CUBRID database.


.. _ERROR-158:

**ERROR CODE: -158, 'ALTER authorization failure.'**

- This message occurs when the ALTER privilege does not exist on the relevant table in the CUBRID database.


.. _ERROR-159:

**ERROR CODE: -159, 'UPDATE authorization failure.'**

- This message occurs when the UPDATE privilege does not exist on the relevant table in the CUBRID database.


.. _ERROR-160:

**ERROR CODE: -160, 'INSERT authorization failure.'**

- This message occurs when the INSERT privilege does not exist on the relevant table in the CUBRID database.


.. _ERROR-161:

**ERROR CODE: -161, 'DELETE authorization failure.'**

- This message occurs when the DELETE privilege does not exist on the relevant table in the CUBRID database.


.. _ERROR-162:

**ERROR CODE: -162, 'INDEX authorization failure.'**

- This message occurs when the INDEX processing privilege does not exist on the relevant table in the CUBRID database.


.. _ERROR-163:

**ERROR CODE: -163, 'EXECUTE authorization failure.'**

- This message occurs when the EXECUTE privilege does not exist on the relevant table in the CUBRID database.


.. _ERROR-164:

**ERROR CODE: -164, 'User "%1$s" already exists.'**

- This message is displayed when the CUBRID system tries to create a new user, but a user with the same name already exists in the system, it means that user creation failed due to a duplicate user name when executing the CREATE USER statement.


.. _ERROR-165:

**ERROR CODE: -165, 'User "%1$s" is invalid.'**

- This message is an authentication-related error that occurs when, in the CUBRID system, the specified user name is invalid or a user that does not exist in the system, it means that an incorrect user name was used in operations such as connecting to the database, changing user privileges, or specifying an owner of a specific object.


.. _ERROR-166:

**ERROR CODE: -166, 'Invalid user specified.'**

- This message is displayed when performing a user privilege (GRANT) operation in the CUBRID system, if the specified user is invalid or does not exist, it mainly occurs due to a user name error or when referencing a non-existent user.


.. _ERROR-167:

**ERROR CODE: -167, 'DBA, members of DBA group and %1$s owner can perform the operation.'**

- This message is an authentication-related error that occurs when, in the CUBRID system, a user who does not have the privilege to perform a particular operation tries to do so, it informs that only the DBA, members of the DBA group, or the owner of a particular object can perform the operation.


.. _ERROR-168:

**ERROR CODE: -168, '168 Member not found.'**

- This message is displayed in the CUBRID database when requesting deletion of a user that is not a member during drop member.


.. _ERROR-169:

**ERROR CODE: -169, '169 Cannot remove user %1$s from the database.'**

- This message is displayed when requesting deletion of a currently logged-in user during drop user in the CUBRID database.


.. _ERROR-171:

**ERROR CODE: -171, 'Incorrect or missing password.'**

- This message is displayed when, during user authentication in the CUBRID system, the provided password is incorrect, or when no password is provided at all, it means that a password mismatch or omission was detected in the authentication process required to connect to the database or to perform CUBRID command operations.


.. _ERROR-172:

**ERROR CODE: -172, '172 Password string cannot have more than 31 bytes.'**

- This message is displayed when the password length of a CUBRID database DB user exceeds 31 characters.


.. _ERROR-173:

**ERROR CODE: -173, 'Could not locate database file "%1$s".'**

- This message is displayed when the CUBRID system cannot find a specific databases.txt file.


.. _ERROR-174:

**ERROR CODE: -174, 'Could not obtain write access to database file "%1$s".'**

- This message is displayed when the CUBRID system fails to obtain write permission for a specific databases.txt file.


.. _ERROR-183:

**ERROR CODE: -183, 'Unexpected amount of received data; %1$d expected, %2$d received.'**

- This message is displayed when the CUBRID client receives data from the server and the expected data size does not match the actual received data size, it can occur due to data integrity issues during network communication, protocol mismatch, or data transfer errors, and it can indicate incorrect communication where the amount of data received from the server differs from what was expected.


.. _ERROR-184:

**ERROR CODE: -184, 'Cannot allocate communications buffer.'**

- This message is displayed when the CUBRID system cannot allocate a buffer for network communication, it occurs due to insufficient memory, system resource limits, or buffer allocation failure, and occurs when it cannot create a communications buffer for data transmission between the client and server.


.. _ERROR-185:

**ERROR CODE: -185, 'Error receiving data from client.'**

- This message is an error that appears when the CUBRID server encounters an error while receiving data from the client, it occurs due to failures in receiving client data during network communication, protocol mismatch, client response errors, and so on, and occurs when the server did not receive the expected data from the client, or when the received data is not valid.


.. _ERROR-186:

**ERROR CODE: -186, 'Error receiving data from server.'**

- This message is an error that appears when the CUBRID client encounters an error while receiving data from the server, it occurs due to data receiving failures during network communication, protocol mismatch, server response errors, and so on, and occurs when the client did not receive the expected data from the server, or when the received data is not valid.


.. _ERROR-187:

**ERROR CODE: -187, 'Communications buffer not used.'**

- This message is displayed when the CUBRID client receives data from the server and the expected buffer differs from the buffer actually received, it occurs when there is a problem with buffer management during network communication or when unexpected data is received.


.. _ERROR-188:

**ERROR CODE: -188, 'Unknown database "%1$s".'**

- This message occurs when the CUBRID system detects that the specified database name is NULL or exceeds the maximum length.


.. _ERROR-189:

**ERROR CODE: -189, 'Invalid host name "%1$s".'**

- This message is displayed when the host name provided by the CUBRID client when connecting to the server is invalid or in an incorrect format, it occurs when the host name is NULL, exceeds the maximum allowed length, or is not in a valid format, and occurs when validating the server host information during the client initialization process.


.. _ERROR-190:

**ERROR CODE: -190, 'Server host not identified.'**

- This message is displayed when the CUBRID client cannot find server host information, it occurs when the server host name or server name is not set or is empty during the client initialization process, and indicates a state where the client does not know which server to connect to.


.. _ERROR-191:

**ERROR CODE: -191, 'Cannot connect to server "%1$s" on "%2$s".'**

- This message is displayed when the CUBRID client cannot connect to the database server process (cub_server) on the specified host, it can occur due to network connection failure, the server not running, firewall blocking, port issues, and so on, and indicates a state where the client cannot establish communication with the server.


.. _ERROR-193:

**ERROR CODE: -193, 'Server received shutdown command from client.'**

- This message occurs when the CUBRID database receives a shutdown command from a client. It informs that the database server has started the normal shutdown process according to the client's request, this is not an error but an event message that indicates the server's normal shutdown.


.. _ERROR-194:

**ERROR CODE: -194, 'Unknown server request id %1$d.'**

- This message is displayed when the request ID (service ID) received by the CUBRID server from the client is invalid or an undefined request, it occurs when the server receives a request ID it cannot process, and is mainly due to a protocol version mismatch between the client and server or an incorrect request format.


.. _ERROR-195:

**ERROR CODE: -195, 'Server communications error: %1$s.'**

- This message indicates an error that occurs during communication between internal/external clients and the server in the CUBRID database, it occurs when communication with the server fails or the connection is dropped, or when there is a problem with the server communication protocol, and is mainly related to network connection issues, server status issues, or communication protocol errors.


.. _ERROR-196:

**ERROR CODE: -196, 'Server name not identified.'**

- This message is displayed when attempting to connect to the CUBRID database server, if the server name is not set or is not valid, it mainly indicates a state where the name of the server or host information that the client is trying to connect to is not correctly configured.


.. _ERROR-197:

**ERROR CODE: -197, 'Could not make contact with master server.'**

- This message indicates that the CUBRID database attempted to connect to the master (cub_master) process but failed, the master (cub_master) is a core process in the CUBRID system that manages multiple databases, and a connection to the master server is required to perform database operations, it mainly occurs when the master process is in an abnormal operating state or in an environment (such as a firewall) that prevents connection.


.. _ERROR-199:

**ERROR CODE: -199, 'Server no longer responding.'**

- This message is displayed when the CUBRID database server does not respond to the client's request, or when the server process unexpectedly terminates, it indicates a state where the server can no longer communicate with the client, and can occur due to DB connection problems, disconnections, or server process interruption, and so on.


.. _ERROR-204:

**ERROR CODE: -204, 'Function called with missing or invalid arguments.'**

- This message is displayed when, inside the CUBRID database, a function is called with required arguments missing or with invalid argument values, in general it is less likely to be a direct user SQL error and more likely to be an internal CUBRID engine issue or a case where a developer used the CUBRID API incorrectly.


.. _ERROR-210:

**ERROR CODE: -210, 'Internal error: processing object template.'**

- This message is an error that appears when an internal error occurs during object template processing in the CUBRID database, it can mainly occur due to table cache inconsistency, lock upgrade failure, or template state validation errors.


.. _ERROR-217:

**ERROR CODE: -217, 'Operation can only be performed on class objects.'**

- This message indicates that, when a specific operation in the CUBRID database is valid only for table objects, that operation was attempted on an object type other than a table object, it can occur mainly in operations related to table definitions such as schema management, metadata querying, or object template processing.


.. _ERROR-224:

**ERROR CODE: -224, 'A database has not been restarted.'**

- This message is displayed when database work is attempted in the CUBRID database while the database connection is not established or the database has not been restarted, it mainly occurs when the connection to the database server has been lost, or when the database is not started normally and an SQL statement is executed or database work is attempted.


.. _ERROR-231:

**ERROR CODE: -231, 'Invalid API operation attempted on a temporary object.'**

- This message is displayed when attempting an API operation that is not allowed on a temporary object in the CUBRID database, in CUBRID, temporary objects are temporary objects mainly related to schema templates or triggers, and they have a different lifecycle and constraints than general database objects, it mainly occurs when attempting operations (e.g., data access, modification, deletion, etc.) that are allowed only for general objects on a temporary object.


.. _ERROR-232:

**ERROR CODE: -232, 'Not allowed to use object templates over multiple transactions.'**

- This message is displayed when an object template is attempted to be used across multiple transactions in the CUBRID database, an object template is a temporary schema-definition object that must generally be created and used within a single transaction. If it is used across multiple transactions, it can cause data consistency issues or unexpected behavior, so the system prohibits this, it occurs in logic related to lifecycle management of object templates, and appears when a template is used outside the transaction scope in which it was created.


.. _ERROR-247:

**ERROR CODE: -247, 'Invalid arguments to schema manager internal function.'**

- This message is an internal error that occurs when invalid arguments (NULL, an empty string, or an invalid value) are passed to an internal function of the schema manager in the CUBRID database, it mainly occurs during operations such as schema creation, modification, and querying, and indicates a developer- or system-level issue.


.. _ERROR-263:

**ERROR CODE: -263, 'Schema manager internal corruption detected.'**

- This message is a system error indicating that data corruption has been detected inside the schema manager in the CUBRID database, it mainly occurs when an internal consistency check fails during object representation, table definition, or metadata processing, and can occur due to database file corruption, memory errors, or abnormal system shutdown, and so on.


.. _ERROR-314:

**ERROR CODE: -314, 'Object buffer overflow while writing.'**

- This message occurs when, in the CUBRID database, while storing (writing) data of an object (such as a table record), data is attempted to be written that exceeds the size of an internally allocated buffer, it can appear due to large data processing, abnormally large objects, buffer size configuration errors, internal bugs, and so on, and can lead to data loss, transaction failure, or system errors.


.. _ERROR-321:

**ERROR CODE: -321, 'Possible corruption in the workspace detected.'**

- This message is displayed when unwanted information exists during processing in the workspace in the CUBRID database, it occurs during the data integrity verification process in the workspace management system, generally the workspace is a memory-resident object management structure and it must keep consistency, and this error is reported when that consistency is broken.


.. _ERROR-322:

**ERROR CODE: -322, 'MOP not found in the workspace.'**

- This message is an error that appears when a specific MOP cannot be found in the hash table in the workspace in the CUBRID database, it can appear when there is a problem in the internal MOP management logic of CUBRID, or when unexpected problems occur in the concurrency control mechanism


.. _ERROR-324:

**ERROR CODE: -324, 'Object found in workspace without class.'**

- This message is an error that appears when an object (MOP) exists in the workspace without table information in the CUBRID database, it can appear when there is a problem in the internal object-table management logic of CUBRID, or when unexpected problems occur in the concurrency control mechanism.


.. _ERROR-325:

**ERROR CODE: -325, 'Attempt to garbage collect a MOP in the dirty list.'**

- This message is an error that appears when, during the garbage collection process of the workspace in the CUBRID database, it attempts to clean up a MOP that is in the 'dirty list', it occurs during the state consistency verification process of MOPs in the workspace management system, generally, if a MOP is in the 'dirty list' it should not be a garbage collection target, and this error is reported when such an inconsistency occurs.


.. _ERROR-326:

**ERROR CODE: -326, 'Changing class pointer for object in workspace.'**

- This message is an error that occurs due to class MOP during instance caching in the workspace in the CUBRID database, it occurs during the table assignment process of objects in the workspace management system, and it mainly occurs when attempting to reassign an object that already belongs to another table to a different table.


.. _ERROR-327:

**ERROR CODE: -327, 'Cannot create MOP with NULL OID.'**

- This message is an error that appears when, in the CUBRID database, a Managed Object Pointer (MOP) is created with a NULL Object ID (OID), it occurs during the OID validity verification process when creating a MOP in the workspace management system, and it can occur when OID assignment fails, or when the memory structure that includes the OID is corrupted.


.. _ERROR-328:

**ERROR CODE: -328, 'Instance encountered in workspace without class.'**

- This message is an error that appears when, in the CUBRID database, a record without a table definition is found in the workspace, it occurs during the verification process of the relationship between records and tables in the schema management system, and it mainly occurs when the table cannot be loaded or the table cannot be found.


.. _ERROR-335:

**ERROR CODE: -335, 'Ignoring attempt to free object not allocated within the workspace.'**

- This message is an error that appears when, in the CUBRID database, it attempts to free a memory object that was not allocated within the workspace (work area), it occurs during the pointer validity verification process in the memory management system, and it mainly occurs when the memory pointer is not within a valid area block.


.. _ERROR-344:

**ERROR CODE: -344, '%1$s.'**

- This message is a warning-type message indicating that the CUBRID database process (cub_server) is in the middle of shutdown.


.. _ERROR-345:

**ERROR CODE: -345, 'Error occurred during reading a shutdown message from master.'**

- This message occurs when the CUBRID database process (cub_server) checks a shutdown request message from the master process (cub_master).


.. _ERROR-347:

**ERROR CODE: -347, 'Could not establish a pipe to the master server.'**

- This message is displayed when, in the CUBRID database server process (cub_server), it tries to perform communication with the master process (cub_master) and the socket is invalid.


.. _ERROR-350:

**ERROR CODE: -350, 'Cannot find hostname ("%1$s") in "%2$s". Please check permissions of the file or if there is the hostname.'**

- This message is displayed when the CUBRID system tries to obtain an IP address from a hostname, it occurs in cases such as a permission error on /etc/hosts, when there is no hostname information, when use_user_host = yes, and when there is no hostname information in $CUBRID/conf/cubrid_hosts.conf.


.. _ERROR-363:

**ERROR CODE: -363, 'Recvmsg from master socket failed.'**

- This message is displayed when a message is received after connecting between the CUBRID database process (cub_server) and the master process (cub_master).


.. _ERROR-364:

**ERROR CODE: -364, 'Sending a new request to server.'**

- This message occurs when the connection between the CUBRID database process (cub_server) and the master process (cub_master) has been terminated.


.. _ERROR-366:

**ERROR CODE: -366, '366 Error from server: %1$s'**

- This message is displayed when changing mode in a CUBRID HA environment, or when the CAS process is connected to a standby server and the database server process (cub_server) terminates the connection.


.. _ERROR-367:

**ERROR CODE: -367, 'Server %1$s already exists.'**

- This message occurs when, in the CUBRID system, a database with the same name already exists when creating a database.

.. _ERROR-368:

**ERROR CODE: -368, 'Communication error during connect for %1$s.'**

- This message occurs while connecting to the master process (cub_master) when starting the database process (cub_server) in the CUBRID system.


.. _ERROR-423:

**ERROR CODE: -423, 'Invalid session.'**

- This message is an error that appears when an invalid session is used in the CUBRID database, it is when the session is NULL or the parser (parser) is not initialized in the session validation process.


.. _ERROR-430:

**ERROR CODE: -430, 'Unknown variable "%1$s".'**

- This message is an error that appears when an undefined variable is referenced in the CUBRID database, it is when the variable cannot be found during the variable validation process, in the host variable (host variable) handling process.


.. _ERROR-456:

**ERROR CODE: -456, 'Data type references are incompatible.'**

- This message is an error that appears when a reference or operation between different data types is incompatible in the CUBRID database, it occurs during the data type compatibility verification process in a query, that is, it occurs when comparison, operation, assignment, and so on between different data types are not possible.


.. _ERROR-469:

**ERROR CODE: -469, 'Invalid data type used.'**

- This message is an error that appears when an unsupported or incorrect data type is used in the CUBRID database.


.. _ERROR-547:

**ERROR CODE: -547, 'Server release %1$s is different from client release %2$s.'**

- This message is an error that appears when the server and client release versions are different in the CUBRID database, it mainly occurs during the version compatibility check between the server and client in the network connection process, and the connection is rejected when the server and client release versions are not compatible.


.. _ERROR-554:

**ERROR CODE: -554, 'The loader has entered an invalid state. No further operations are possible.'**

- This message is displayed when the CUBRID database loaddb command enters an invalid state, this mainly occurs when the loaddb command session is invalid, or when the loader is in an unexpected state, and if the loader (loaddb) is in this state, no further data load operations can be performed.


.. _ERROR-555:

**ERROR CODE: -555, 'Memory allocation error during database loading.'**

- This message occurs when the CUBRID database loaddb command fails to allocate the required memory, the allocation request may fail due to processing a large amount of data during the loading work, insufficient available memory on the system, or due to memory fragmentation, operating system limits, and so on.


.. _ERROR-556:

**ERROR CODE: -556, 'Too many values supplied. Expecting %1$d.'**

- This message is displayed when, while loading data with the CUBRID database loaddb command, the number of supplied values is greater than the expected number, it mainly occurs when processing attribute values during the data loading process, and it occurs when more values are provided than the number of attributes defined in the table.


.. _ERROR-559:

**ERROR CODE: -559, 'For attribute %1$s of %2$s, expected type %3$s, got %4$s.'**

- This message is displayed when, while loading data with the CUBRID database loaddb command, the domain type of an attribute does not match the expected type, it mainly occurs when validating the domain type during the data loading process, and it occurs when the expected type and the actually received type are different for a specific attribute.


.. _ERROR-560:

**ERROR CODE: -560, 'For attribute %1$s of %2$s, domain is not specific enough to support\n unqualified object references.'**

- This message is displayed when, while loading data with the CUBRID database loaddb command, the domain of an object reference is ambiguous, it mainly occurs when validating an object domain during the data loading process, and it occurs when the domain of a specific attribute is not specific enough to support unqualified object references.


.. _ERROR-561:

**ERROR CODE: -561, 'Nested sets are not allowed.'**

- This message is displayed when, while loading data with the CUBRID database loaddb command, it tries to handle nested sets (collections), it mainly occurs when processing collection elements during the data loading process, and because CUBRID does not allow nested sets, this error occurs when a set is attempted to contain another set.


.. _ERROR-562:

**ERROR CODE: -562, 'System class %1$s cannot be populated using the database loader.'**

- This message is displayed when, while loading data with the CUBRID database loaddb command, it tries to load a system table, it mainly occurs when initializing the table context during the data loading process, system tables are special tables used internally by CUBRID and cannot be loaded through a general loaddb.


.. _ERROR-563:

**ERROR CODE: -563, 'Reference to internal class %1$s converted to NULL.'**

- This message is displayed when, while loading data with the CUBRID database loaddb command, it processes a reference to an internal table, it mainly occurs when creating a table record during the data loading process, and when a reference to an internal table is found, it is converted to NULL and processed.


.. _ERROR-565:

**ERROR CODE: -565, 'Constructor method %1$s is not defined on class %2$s.'**

- This message is displayed when, while loading data with the CUBRID database loaddb command, the specified constructor method is not defined on the table, it mainly occurs when creating a table record during the data loading process, and it occurs when the constructor method does not exist on the table or is not defined.


.. _ERROR-566:

**ERROR CODE: -566, 'No class specified or no attribute in class.'**

- This message is displayed when, while loading data with the CUBRID database loaddb command, no table is specified or there is no attribute in the table, it mainly occurs when creating a table record during the data loading process, and it occurs when the table does not exist, or when the table exists but no attributes are defined.


.. _ERROR-567:

**ERROR CODE: -567, 'Too many constructor method arguments supplied. Expected %1$d.'**

- This message is displayed when more arguments are supplied than the number of arguments expected by the CUBRID database loaddb command, it mainly occurs when creating a table record during the data loading process, and it occurs when more arguments are provided than the number of arguments required for the constructor method.


.. _ERROR-568:

**ERROR CODE: -568, 'Missing constructor method parameters. Expected %1$d, found %2$d.'**

- This message is displayed when the expected number of arguments and the actually supplied number of arguments do not match in the CUBRID database loaddb command, it mainly occurs when creating a table record during the data loading process, and it occurs when fewer arguments are provided than the number of arguments required for the constructor method.


.. _ERROR-569:

**ERROR CODE: -569, 'Missing attribute values.  Expected %1$d, found %2$d.'**

- This message is displayed when, during the process of loading data with the CUBRID database loaddb command, the expected number of attribute values and the actually found number of attribute values do not match, it mainly occurs when creating a table record during the data loading process, and it occurs when fewer attribute values are provided than the number of attributes defined in the table.


.. _ERROR-570:

**ERROR CODE: -570, 'Could not access Glo data file "%1$s".'**

- This message is displayed when, in the CUBRID database loaddb command, it cannot access a GLO (Generalized Large Object) data file, it mainly occurs when processing BLOB (Binary Large Object) or CLOB (Character Large Object) data during the data loading process, and it occurs when the path to the LOB data file is wrong, the file does not exist, or there are no permissions.


.. _ERROR-571:

**ERROR CODE: -571, 'Instance was previously created through a forward reference.\nThis is not allowed for instances created using constructor methods.'**

- This message is displayed when, in the CUBRID database loaddb command, it tries to create again, using a constructor method, a record that has already been created through a forward reference, it mainly occurs when creating a table record during the data loading process.


.. _ERROR-576:

**ERROR CODE: -576, 'Reference to class %3$s is not compatible with the domain of %1$s of %2$s.'**

- This message is displayed when, in the CUBRID database loaddb command, when referencing a table object, the reference is not compatible with the domain of a specific attribute, it mainly occurs when creating or updating a table record during the data loading process, and it occurs when the table reference does not match the attribute's domain.


.. _ERROR-579:

**ERROR CODE: -579, 'Target domain must include type "object" to allow references to classes.'**

- This message is displayed when, in the CUBRID database loaddb command, the domain type is not correct when referencing a table object, to reference a table object, the target domain must include the "object" type, this is a special requirement for table references in CUBRID's type system.


.. _ERROR-581:

**ERROR CODE: -581, 'Attempted to update the database when updates are disabled.'**

- This message is displayed when trying to modify the database while database updates are disabled because the CUBRID database server is in standby or to_be_active status, the state where database updates are disabled can occur in read-only mode or in special situations.


.. _ERROR-582:

**ERROR CODE: -582, 'Unknown storage purpose %1$d. Valid range is from %2$d to %3$d.'**

- This message is a disk management error that occurs when the purpose (purpose) of a volume is invalid in the CUBRID database, the volume purpose is a value indicating whether the data is permanent (permanent) or temporary (temporary), and because a value outside the valid range is passed, the volume cannot be created or managed.


.. _ERROR-588:

**ERROR CODE: -588, 'INTERNAL ERROR: Assertion '%1$s' failed.'**

- This message is an internal error that occurs when, due to an internal error in the CUBRID database, a specific condition has failed, when this error occurs it means there is a problem in CUBRID's internal logic.


.. _ERROR-589:

**ERROR CODE: -589, 'Invalid user name "%1$s".'**

- This message indicates that the user name is invalid or does not exist in the CUBRID database, it occurs when validating a user name in user authentication, privilege management, stored procedure creation, and so on, and it can occur when referencing a user that does not exist.


.. _ERROR-595:

**ERROR CODE: -595, 'The database name "%1$s" is too long. Database name should be less than %2$d characters.'**

- This message is displayed in CUBRID when the database name exceeds the maximum length and it tries to create a volume file name that is the same as the database name.


.. _ERROR-611:

**ERROR CODE: -611, '%1$s cannot be executed in client/server mode.'**

- This message appears when a specific feature cannot be executed in client/server mode in the CUBRID database, it means that the feature can be executed only in standalone mode.


.. _ERROR-612:

**ERROR CODE: -612, 'Logging cannot be disabled at this moment since there are pending actions to recover.'**

- This message occurs when attempting to disable logging in CUBRID database loaddb -S when using --no-logging, it means that logging cannot be disabled when there are pending actions to recover.


.. _ERROR-619:

**ERROR CODE: -619, 'Bad source codeset.'**

- This message appears when the source (source) codeset is invalid or unsupported during codeset conversion of a string in the CUBRID database, it can occur when the source codeset is specified incorrectly when converting a string from one codeset to another codeset.


.. _ERROR-629:

**ERROR CODE: -629, 'Pagesize %1$d is not a power of 2 or pagesize is too small. Pagesize of %2$d is used instead.'**

- This message is a warning that occurs when setting the page size in the CUBRID database, it appears when the page size specified by the user is not a power of 2 or is smaller than the minimum size, and it notifies that the CUBRID system has automatically adjusted it to a valid page size.


.. _ERROR-630:

**ERROR CODE: -630, 'Conversion not supported by %1$s.'**

- This message is displayed when the CUBRID database attempts conversion between certain data types but the conversion is not supported by the system, it occurs when an internal function attempts conversion between types, for example, when attempting an unsupported data type combination or conversion between composite types, this error may occur.


.. _ERROR-632:

**ERROR CODE: -632, '%1$s is not a backup volume.'**

- This message is displayed when, in the CUBRID database, the specified file or directory is not a valid backup volume, it appears when the header of the backup file cannot be read or when the magic number of the backup file is not correct, and it occurs when trying to recognize a file in a format other than a CUBRID backup file as a backup file.


.. _ERROR-633:

**ERROR CODE: -633, '1$s is a backup of database %2$s created on %3$s instead of given database %4$s created on %5$s'**

- This message is displayed when restoring a backup file in the CUBRID database, it appears when the backup file is a backup of another database or a backup of a database created at another point in time, and it occurs when the database specified by the user and the database information contained in the backup file do not match.


.. _ERROR-634:

**ERROR CODE: -634, 'A database volume/file was expected in backup.'**

- This message is displayed when restoring a backup file or checking the list in the CUBRID database, it appears when database volume or file information cannot be found in the backup file, and it occurs when the structure of the backup file is damaged or differs from the expected format.


.. _ERROR-648:

**ERROR CODE: -648, 'Backup is incompatible with current "%1$s" release "%2$s".'**

- This message is displayed when a backup file is not compatible with the current CUBRID version in the CUBRID database, it indicates a compatibility problem between the CUBRID version that created the backup file and the CUBRID version attempting recovery, and it occurs when the backup file is not supported in the current version.


.. _ERROR-669:

**ERROR CODE: -669, 'Server refused client connection: max clients, (%1$d), exceeded.'**

- This message occurs when the CUBRID database server refuses a new client connection because the configured maximum number of client (max_clients) connections has been exceeded, it is a mechanism that limits the number of concurrent connections to protect server resources and maintain performance, and it means that the number of currently active client connections has exceeded the maximum value allowed by the server.


.. _ERROR-675:

**ERROR CODE: -675, 'Cannot read the database location file, '%1$s'.'**

- This message is displayed when the CUBRID database system attempted to read the database location file (e.g., `databases.txt`) but failed, this file is used by CUBRID to identify the location and information of databases registered in the system, and this error occurs when the file cannot be accessed or its contents are damaged.


.. _ERROR-676:

**ERROR CODE: -676, 'Database '%1$s', cannot be found in the database location file, '%2$s''**

- This message is displayed when the CUBRID database system tries to start or connect to a specific database, but information for that database is not found in the database location file (e.g., `databases.txt`), CUBRID manages the location and information of databases registered in the system through a configuration file such as `databases.txt`, and if there is no database information in this file, the system cannot recognize that database.


.. _ERROR-677:

**ERROR CODE: -677, 'Failed to connect to database server, '%1$s', on the following host(s): %2$s'**

- This message is displayed when a client in the CUBRID database attempted to connect to the database server on the specified host(s) but failed, it indicates a situation where the database server cannot be connected to due to various causes such as network connection, server status, authentication, and so on, and for detailed causes it is necessary to check csql, err files, or $CUBRID/log/*.err files, and so on.


.. _ERROR-695:

**ERROR CODE: -695, 'Wrong interface has been used to kill a client.'**

- This message occurs when, in the CUBRID database, when trying to forcibly terminate a client session with the killtran command, the wrong interface or inappropriate parameters are used, to perform a client termination operation, information such as transaction index, user name, host name, and so on is required, but this error occurs when such information is insufficient or incorrect.


.. _ERROR-696:

**ERROR CODE: -696, 'Unknown transaction (index %1$d, %2$s@%3$s|%4$d).'**

- This message occurs when, in the CUBRID database, when trying to forcibly terminate a specific transaction with the killtran command, the transaction corresponding to the requested transaction index does not exist in the system, it can mainly occur when the client uses an incorrect transaction index, or when the transaction has already ended and has been removed from the system.


.. _ERROR-697:

**ERROR CODE: -697, 'Given transaction index (index %1$d, %2$s@%3$s|%4$d) does not match with current transaction index (index %5$d, %6$s@%7$s|%8$d). Likely, the given transaction has finished and its transaction index was re-assigned to another transaction.'**

- This message occurs when, in the CUBRID database, when trying to forcibly terminate a specific transaction with the killtran command, the requested transaction index does not match the transaction information recognized by the current system, it can mainly occur when the client has an old (stale) transaction index, or when the transaction has already ended on the server and the index was reassigned to another transaction.


.. _ERROR-743:

**ERROR CODE: -743, 'Failed on handshake between client and server. (peer host %1$s)'**

- This message is an error that appears when an error occurs during the initial connection setup process (handshake) between the client and the server in the CUBRID database, handshake is the process of checking each other's version, features, and compatibility before the client and server start communication, and it occurs when the client version information cannot be received or when the feature compatibility check between the server and client fails, it can occur due to network connection problems or server status problems.


.. _ERROR-752:

**ERROR CODE: -752, 'Error restoring backup unit_num %1$d.'**

- This message is a general I/O error that occurs while restoring CUBRID database backup, while reading data from the backup file, it can occur due to various causes such as physical damage to the backup file, disk I/O errors, file access permission issues, or backup file format mismatch.


.. _ERROR-753:

**ERROR CODE: -753, 'Error restoring backup unit_num %1$d, pageid %2$d exceeds total number of pages %3$d for volid %4$d.'**

- This message occurs when, during the process of restoring a CUBRID database backup, the page ID in the backup file is outside the valid page range of the target volume to be restored, it can occur when the metadata of the CUBRID backup file is damaged, or when attempting to restore to a different environment than when the backup was created (e.g., a smaller volume size).


.. _ERROR-755:

**ERROR CODE: -755, '%1$s cannot be executed in stand-alone mode.'**

- This message occurs when, while running in stand-alone mode in the CUBRID database, an operation that requires client-server communication is attempted, it indicates this error that occurs when a specific operation or command such as checkpoint cannot be executed in stand-alone mode in the CUBRID database.


.. _ERROR-757:

**ERROR CODE: -757, 'Internal Error: cannot assign permanent OID during object encode.'**

- This message occurs when, in the CUBRID database, during the process of encoding an object, it fails to assign a permanent OID. OID is an important value that uniquely identifies each object in the database. Failure to assign a permanent OID means that there is a problem in the object creation or persistence process, it can occur mainly due to internal data consistency problems, insufficient storage space, transaction problems, or internal logic errors, and so on.


.. _ERROR-767:

**ERROR CODE: -767, 'Cannot create a level %1$d backup without first creating a level %2$d backup.'**

- This message occurs when, in the CUBRID database, when performing incremental backup, continuity of backup levels is not guaranteed, in this case, when specifying the backup level, 2 causes an error when there is no level 1 backup, and 1 causes an error when there is no level 0 backup, and CUBRID supports the following backup levels:
Level 0 (Full Backup): Full database backup
Level 1 (Big Incremental): Incremental backup based on level 0 backup
Level 2 (Small Incremental): Incremental backup based on level 1 backup


.. _ERROR-769:

**ERROR CODE: -769, 'Attribute "%1$s", of class "%2$s" is not a valid class or shared attribute.'**

- This message indicates, when processing table attributes while executing the loaddb command in the CUBRID database, that the specified attribute is not a valid table attribute or shared attribute, it occurs in CUBRID's data loader mechanism, and can mainly occur due to failure of the attribute namespace validation.


.. _ERROR-771:

**ERROR CODE: -771, 'Invalid parameter for function.'**

- This message occurs when, in the CUBRID database, the parameter passed during a function call is invalid or does not follow the expected format, an error occurs when CUBRID's internal specific function fails in the process of validating the parameter validity required to perform its work.


.. _ERROR-778:

**ERROR CODE: -778, 'Cannot create a symbolic link "%1$s" to the file "%2$s".'**

- This message indicates a situation where creation of a symbolic link fails in the CUBRID database, it occurs in CUBRID's file system management mechanism, and can mainly occur when executing createdb, copydb, renamedb commands.


.. _ERROR-820:

**ERROR CODE: -820, 'Cannot initialize a connection.'**

- This message is an internal error that occurs when the CUBRID system fails to initialize a connection between the client and server, connection initialization is the process of creating a connection object when the client connects to the server, allocating necessary resources, and setting the connection state, failure in this process means that memory allocation failure, client ID allocation failure, mutex initialization failure, and so on have occurred, connection initialization failure prevents the client from connecting to the server, making use of the database service impossible.


.. _ERROR-821:

**ERROR CODE: -821, 'Shutdown a connection.'**

- This message is an internal error that occurs when the CUBRID system forcibly terminates a connection between the client and server, rather than an error, it is an informational message that the system is terminating a specific connection, it mainly occurs during the slam transaction process, and it is the process of cleaning up all resources related to that connection.


.. _ERROR-827:

**ERROR CODE: -827, 'Connection list traverse function returned invalid value.'**

- This message is an internal error that occurs when, in the process of traversing the connection list by the CUBRID system, a callback function returns an invalid return value, an invalid return value means that there is a bug in the callback function or an unexpected situation has occurred.


.. _ERROR-829:

**ERROR CODE: -829, 'This mutex lock has been unlocked already.'**

- This message occurs when releasing a critical section in the thread management module of the CUBRID database, that is, it is an error message that occurs when trying to unlock a critical section that has already been unlocked, it indicates a programming error or a problem in the concurrency control logic, and it is a safeguard to prevent double unlocking of a mutex lock.


.. _ERROR-831:

**ERROR CODE: -831, 'All transaction descriptors(%1$d) are in use. Raise up max_clients parameter.'**

- This message occurs when the CUBRID database receives connection requests exceeding the max_clients setting value in its configuration, it can occur when too many clients connect to the database simultaneously beyond the max_clients parameter setting value, or when existing clients continue to remain without ending their transactions.


.. _ERROR-832:

**ERROR CODE: -832, 'Backup is already running. Backup cannot be run by two processes.'**

- This message occurs in the log page buffer management module of the CUBRID database to prevent duplicate execution of backup (backupdb) tasks, it occurs when the database administrator tries another backup before the backup job is completed, or when multiple clients request backup at the same time.


.. _ERROR-835:

**ERROR CODE: -835, 'Undefined statistics item.'**

- This message occurs when trying to reference an undefined statistics item in the statistics management module of the CUBRID database, CUBRID supports only a limited number of statistics items, and it returns this error for requests outside that range.


.. _ERROR-837:

**ERROR CODE: -837, 'Cannot drop the user who owns database objects.'**

- This message occurs when trying to delete a user in the user management module of the CUBRID database, as a constraint to ensure database integrity and security, it prevents objects owned by the user from being deleted indiscriminately. Before deleting a user, you must first delete all database objects owned by that user or transfer ownership to another user.


.. _ERROR-841:

**ERROR CODE: -841, 'Requested operation can be executed by only sole client(transaction).'**

- This message occurs when trying to perform a specific operation in the system parameter management module of the CUBRID database in a situation where multiple clients (transactions) are connected at the same time, it indicates a constraint for operations that must be performed only in a single-client environment to ensure data consistency and stability when changing certain system parameters or performing certain administrative operations.


.. _ERROR-877:

**ERROR CODE: -877, 'No data to be unloaded.'**

- This message occurs during the process of unloading (unload) schema or data in the CUBRID database, such a situation generally appears when there is no schema information in the database, or when the table or object to be unloaded does not exist.


.. _ERROR-878:

**ERROR CODE: -878, 'Invalid database location file, '%1$s'.'**

- This message occurs during the process of reading and parsing the `databases.txt` file in the CUBRID database.
 Such a situation generally appears when, at database startup, the format of the `databases.txt` file is not correct, or when required information (database name, path, host, log path, etc.) is missing.


.. _ERROR-883:

**ERROR CODE: -883, 'Failed to get '%1$s' file status.'**

- This message occurs when, in the CUBRID database, when executing the renamedb command, it tries to obtain status information of a specific file or directory, it can occur due to file system access issues, the file not existing, and so on.


.. _ERROR-884:

**ERROR CODE: -884, 'Volext-pathname '%1$s' is not permitted to write.'**

- This message occurs when, in the CUBRID database, when executing the renamedb command, there is no write permission for a specific path, it means that a file operation essential to database operation has failed due to file system access permission issues.


.. _ERROR-885:

**ERROR CODE: -885, 'Failure to move from '%1$s' to '%2$s'. Use the same device(disk).'**

- This message occurs when, in the CUBRID database, when executing the command with renamedb -E option, it tries to move extension volume files to a different location, it is a protective error to protect database integrity and performance.


.. _ERROR-896:

**ERROR CODE: -896, 'Compression failed. Zip Method: %1$d (%2$s), Zip Level: %3$d (%4$s)'**

- This message occurs because compression failed when compressing varchar data or log data using the LZ4 compression algorithm in the CUBRID database.


.. _ERROR-897:

**ERROR CODE: -897, 'Decompression failed.'**

- This message occurs in the process where CUBRID decompresses data compressed with the LZ4 algorithm. It can occur when decompressing object (varchar) data or log data, and it indicates a failure due to data corruption or memory problems.


.. _ERROR-910:

**ERROR CODE: -910, 'Not allowed access to partition.'**

- This message occurs when, in the CUBRID database, an attempt to access a specific partition is denied due to insufficient privileges, a partition state problem, or an incorrect access method, that is, the database system needs to access a specific partition to perform the operation requested by the user (e.g., data insert, query, update, delete, or schema change), but it occurs when there is no access privilege to that partition or the partition itself is in an inaccessible state (e.g., offline, corrupted).
Such a situation generally appears when performing DML/DDL operations on a partitioned table, when the user account does not have the required privileges, when the partition is damaged and cannot be accessed, or when there is a problem in the partition management logic internally in the system, and it is a protective error to protect database security, data integrity, and system stability.


.. _ERROR-941:

**ERROR CODE: -941, 'The operation is not supported.'**

- This message is a generic error code that can occur in various situations in the client interface of the CUBRID database, that is, it is used when the requested operation is not supported in the current CUBRID version or implementation.


.. _ERROR-962:

**ERROR CODE: -962, 'Out of memory'**

- This message occurs when the DBlink gateway of the CUBRID database fails to allocate memory.


.. _ERROR-969:

**ERROR CODE: -969, 'Server bit platform (%1$d) is different from client bit platform (%2$d).'**

- This message occurs when, when a CUBRID database client attempts to connect to the server, it detects that the client and server are running on different bit platforms (e.g., a 32-bit client and a 64-bit server, or vice versa), CUBRID recommends running in the same bit platform environment for stable communication and data consistency between client and server, and when the bit platforms differ like this, it refuses the connection and raises this error, it is a protective error to prevent data corruption or unexpected behavior.


.. _ERROR-971:

**ERROR CODE: -971, 'Program '%1$s' (pid %2$d) connected to database server '%3$s' on the host '%4$s' (port %5$d).'**

- This message is a notification that occurs when a client successfully connects to a CUBRID database server, it is an informational message indicating a normal connection state rather than an actual error, and it shows in detail the client's connection information and the server access state.


.. _ERROR-972:

**ERROR CODE: -972, 'Program '%1$s' (pid %2$d) was connected from the host '%3$s'. (transaction index %4$d)'**

- This message is an informational log message notifying that a client program has successfully connected to the CUBRID database, it is not an actual error, but records that the database server has accepted the client's connection request and established a session. This message provides important information for system administrators to trace and monitor which client connected to the database when, from where, and with which program, in particular, it can be used usefully during abnormal connection attempts or security audits.


.. _ERROR-973:

**ERROR CODE: -973, 'Server status is %1$s.'**

- This message is an informational message that tells the current state of the server in the CUBRID database, it is not actually an error, but a notification message that records the current state of the server in the log during the boot process or HA (High Availability) state changes, the server starts in DOWN state during the boot process and transitions to UP state, or outputs this message when switching to MAINTENANCE mode in an HA environment, it helps system administrators monitor the current state of the server and debug when problems occur.


.. _ERROR-974:

**ERROR CODE: -974, 'Archive log "%1$s" is created to archive pages from %2$lld to %3$lld.'**

- This message is an informational message rather than an actual error, indicating that the database system has successfully created an archive log file for a specific range of pages. This is part of the normal log archiving process for database backup and recovery.


.. _ERROR-977:

**ERROR CODE: -977, 'Checkpoint started (previous checkpoint page id: %1$lld, previous redo page id: %2$lld).'**

- This message is a system notification that informs that a checkpoint operation has started in the CUBRID database. It indicates the start of an important operation to ensure the database's consistency and recoverability.


.. _ERROR-978:

**ERROR CODE: -978, 'Checkpoint finished (checkpoint page id: %1$lld, checkpoint redo page id: %2$lld, flushed page count: %3$d).'**

- This message is a system notification that informs that a checkpoint operation has been completed in the CUBRID database. It indicates the successful end of the operation together with information about the pages processed during the checkpoint process.


.. _ERROR-982:

**ERROR CODE: -982, 'Set interrupt to the transaction %1$d.'**

- This message indicates that an interrupt (stop signal) has been successfully set for a specific transaction in the CUBRID database, this generally means that there was an attempt to stop execution of the transaction due to external factors (e.g., user request, forced termination command by an administrator, timeout, etc.), and that attempt was processed internally by the system, it is closer to an informational message that the transaction interruption request has been processed rather than an error. When an interrupt is set, the transaction can stop its current work and may be rolled back.


.. _ERROR-984:

**ERROR CODE: -984, 'Cannot set %3$s to "%6$s" in the "[%5$s]" section when it is set to "%4$s" in the "[%2$s]" section of the configuration file "%1$s".'**

- This message occurs when the ha_mode parameter setting value in the CUBRID configuration file ($CUBRID/conf/cubrid.conf) is set to a different value, it is a protective error to ensure consistency of the configuration file and prevent conflicts of parameter values, and it can occur especially for important parameters such as HA (High Availability) mode.


.. _ERROR-985:

**ERROR CODE: -985, 'The hostname on the database connection string should be specified when multihost is set in "databases.txt". ex) csql demodb@localhost'**

- This message occurs when a CUBRID database client attempts to connect to a database for which the `multihost` option is enabled in the `databases.txt` file. The `multihost` option indicates that the database can be serviced across multiple hosts, and in this case the client must explicitly specify the host name of the specific server it wants to connect to, if the client attempts to connect with only the database name, such as `csql demodb`, CUBRID cannot know which host to connect to and therefore raises this error, it is a protective error to guide the client to connect to the correct server record and prevent ambiguous connection attempts.


.. _ERROR-991:

**ERROR CODE: -991, 'Flush victim candidates of page buffer started.'**

- This message is a system message that informs that the flush operation for victim candidate pages has started in the page buffer of the CUBRID database. It indicates the start of an important operation that writes dirty pages to disk for memory management.


.. _ERROR-992:

**ERROR CODE: -992, 'Flush victim candidates of page buffer finished (count: %1$d).'**

- This message is a system message that informs that the flush operation for victim candidate pages has completed in the page buffer of the CUBRID database. It indicates the successful end of the operation together with the number of flushed pages.


.. _ERROR-994:

**ERROR CODE: -994, 'This statement cannot be a prepared statement.'**

- This message occurs when, in the CUBRID database, attempting to use prepared statement syntax (prepare, execute, deallocated/drop) in the prepare from statement.


.. _ERROR-995:

**ERROR CODE: -995, 'A prepared statement with the name %1$s does not exist.'**

- This message occurs when, in the CUBRID database, attempting to execute a prepared statement with a specific name using the `EXECUTE` statement, a prepared statement is an SQL statement prepared in advance through the `PREPARE` statement and executed through `EXECUTE`, but this error occurs when the prepared statement with the requested name does not exist in the current session.
This can occur in the following situations, when the prepared statement has not been created yet, when the session has ended and the prepared statement has been deleted, when the prepared statement name was entered incorrectly, or when trying to execute a prepared statement created in another session in the current session, because prepared statements are managed per session, when the session changes you cannot access statements prepared previously.


.. _ERROR-1001:

**ERROR CODE: -1001, 'An unexpected condition or state has been reached. '%1$s''**

- This message occurs when the CUBRID database system internally encounters an unexpected state or condition, it is a comprehensive error code used for general internal error situations that the developer did not anticipate or that are difficult to assign a specific error code to, it usually provides a description of the specific error situation to help trace the cause of the problem. This message can occur due to logical defects in the database, data corruption, abnormal system state, or environmental issues.


.. _ERROR-1009:

**ERROR CODE: -1009, 'COMPACTDB already started.'**

This error code occurs when there is already a COMPACTDB process running and you attempt a new COMPACTDB operation.


.. _ERROR-1066:

**ERROR CODE: -1066, 'Session expired.'**

- This message occurs when a client session in the CUBRID database has expired by exceeding its validity period. A session is a logical unit that maintains the connection state between the client and server and stores user-specific information. When a session expires, all work in progress through that session is interrupted, and the client loses access rights to the database. This can mainly occur due to session timeout settings, prolonged client inactivity, or unstable network connections, and it is a protective error for database security and resource management.


.. _ERROR-1069:

**ERROR CODE: -1069, 'Too many prepared statements.'**

- This message occurs when, in the CUBRID database, a single session attempts to prepare or use statements by exceeding the maximum allowed number of prepared statements, this is in the PREPARE STMT syntax, the database system limits the number of prepared statements per session to prevent excessive memory use or resource exhaustion. This message mainly appears when the application or queries use more prepared statements than expected, or when related parameters such as `MAX_PREPARED_STMT_COUNT` are set too low compared to the current workload, and it is a protective error for database stability and resource management.


.. _ERROR-1070:

**ERROR CODE: -1070, 'Session variable '@%1$s' not defined.'**

- This message occurs when, in the CUBRID database, trying to use a session variable that does not exist or is not defined in the current session. A session variable is a temporary variable valid only within a specific user session, and the database system returns this error when it cannot find the requested variable. This can occur due to a typo in the variable name, the variable not being defined, or an access attempt outside the valid scope (scope), and it is a protective error to prevent operations on non-existent data and ensure data integrity.


.. _ERROR-1071:

**ERROR CODE: -1071, 'Too many session variables.'**

- This message occurs when, in the CUBRID database, a single session attempts to create or use session variables by exceeding the maximum allowed number of session variables. A session variable is a temporary variable valid only within a specific user session, and the database system limits the number of session variables to prevent excessive memory usage or resource exhaustion.


.. _ERROR-1072:

**ERROR CODE: -1072, 'IP address(%1$s) is not authorized.'**

- This message occurs when the CUBRID database server determines that the client's IP address is not authorized in the Access Control List (Access Control List, ACL). That is, it means that the IP address attempting to connect to the database has been blocked by the server's security policy. This is a protective error to prevent unauthorized access to the database and strengthen security.


.. _ERROR-1073:

**ERROR CODE: -1073, 'Invalid format in file(%1$s).'**

- This message occurs when the CUBRID database system, in the process of reading and parsing a specific file (especially an IP access control file), the contents of that file do not follow the expected format or grammar. This can mainly occur due to typos in the file contents, incorrect syntax, or abnormal modification. If this error occurs, CUBRID cannot apply the settings in the file correctly, so service startup may fail or unexpected access control issues may occur. This is an integrity issue with a configuration file that is essential to system security and stable operation, and immediate checking and correction are required.


.. _ERROR-1074:

**ERROR CODE: -1074, 'Cannot read access list file(%1$s).'**

- This message occurs when the CUBRID database system fails to open or read a specific access list file (Access List File). The access list file is an important configuration file used to define CUBRID's security and access control policy. This message mainly appears when the file does not exist, the file path is incorrect, the user running the CUBRID process does not have read permission for the file, or the file itself is damaged. If this error occurs, CUBRID cannot apply the correct access control policy, so service startup may fail or unexpected access problems may occur.


.. _ERROR-1076:

**ERROR CODE: -1076, 'Could not load system parameter.'**

- This message occurs when the CUBRID database system fails to load essential system parameters (e.g., settings defined in the `cubrid.conf` file) during startup or initialization. System parameters are very important for configuring how the database works, resource allocation, and operational policy. If it cannot load these parameters, the system cannot configure itself properly, leading to startup failure or abnormal behavior, and it is a critical error that prevents the database from functioning correctly.


.. _ERROR-1077:

**ERROR CODE: -1077, 'The '%1$s' parameter at line %2$d in file '%3$s' : Unknown parameter'**

- This message occurs when the CUBRID database system, in the process of reading and parsing a configuration file such as `cubrid.conf`, finds a parameter name that the system does not recognize or support. This can generally occur due to a typo in the parameter name, use of a parameter that is no longer supported in the current CUBRID version, or abnormal modification of the configuration file. If this error occurs, CUBRID cannot process that parameter, so the server or related services may not start normally or may operate in an unexpected way. This is an integrity issue with a configuration file that is essential to stable operation of the system, and immediate checking and correction are required.


.. _ERROR-1078:

**ERROR CODE: -1078, 'Character at offset %1$d is invalid with current codeset.'**

- This message occurs when, in the CUBRID database, when executing the loaddb command, it finds an invalid character that cannot be handled with the currently set character set (codeset). It occurs because the actual encoding of the data and the system's `codeset` setting do not match during the internal string processing process, and it is a protective error to ensure data integrity and correctness of string processing.


.. _ERROR-1087:

**ERROR CODE: -1087, 'Backup active log '%1$s' started.'**

- This message is an informational notification message indicating that the backup operation of an active log file has started in the CUBRID database. It means that the system has started the log backup process and is performing work to secure data stability and recoverability. It is generally an informational message that records the start of an important operation in a database management system.


.. _ERROR-1088:

**ERROR CODE: -1088, 'Backup active log '%1$s' finished.'**

- This message is a success notification message indicating that the backup operation of an active log file has successfully completed in the CUBRID database. It means that the system has performed the log backup normally to secure data stability and recoverability. It is generally an informational message recorded when an important operation in a database management system has been successfully finished.


.. _ERROR-1090:

**ERROR CODE: -1090, 'Locale initialization: %1$s.'**

- This message appears when a problem occurs during the process in which the CUBRID database initializes locale-related information. Locale is an important setting that defines language, country, and character encoding rules so that data sorting, string comparison, and character set conversion can be performed correctly. If an error occurs in this process, it means there is a problem with the system's locale settings, CUBRID's configuration, or the operating system's locale support capabilities. This can affect character set processing, internationalization features, and overall data integrity, and it is an important error.


.. _ERROR-1091:

**ERROR CODE: -1091, 'Locale generation: %1$s.'**

- This message appears when a problem occurs during the process in which the CUBRID database generates or processes locale-related information. Locale is an important setting that defines language, country, and character encoding rules so that data sorting, string comparison, and character set conversion can be performed correctly. If an error occurs in this process, it means there is a problem with the system's locale settings, CUBRID's configuration, or the operating system's locale support capabilities. This can affect character set processing, internationalization features, and overall data integrity, and it is an important error.


.. _ERROR-1093:

**ERROR CODE: -1093, 'The arg '%1$s' for inet_aton is not a valid string formatted ipv4 address.'**

- This message occurs when, in the CUBRID database, when using the `inet_aton` function to convert a string-formatted IP address into a numeric format, the input argument is not a valid IPv4 address string format. The `inet_aton` function is a standard library function that converts a string IPv4 address in the "a.b.c.d" format to a 32-bit integer in network byte order. This message mainly occurs due to incorrect data input, data type mismatch, or abnormal values, and it means that operations requiring IP address conversion (e.g., network-related functions, log recording, specific data processing) have failed. This is a protective error due to data validity check failure.


.. _ERROR-1094:

**ERROR CODE: -1094, 'The arg '%1$lld' for inet_ntoa is not a valid number formatted ipv4 address.'**

- This message occurs when, in the CUBRID database, when using the `inet_ntoa` function to convert a numeric-formatted IP address into a string format, the input argument is not a valid IPv4 address format. The `inet_ntoa` function is a standard library function that converts a 32-bit integer IPv4 address in network byte order to a string in the "a.b.c.d" format. This message mainly occurs due to incorrect data input, data type mismatch, or abnormal values, and it means that operations requiring IP address conversion (e.g., network-related functions, log recording, specific data processing) have failed.


.. _ERROR-1095:

**ERROR CODE: -1095, 'User name is too long.'**

- This message occurs when, in the CUBRID database system, when creating or changing a user name (User Name), the length of the entered user name exceeds the maximum length allowed by the system, that is, during the process where the CUBRID server handles user management operations, if the user name provided through commands such as `CREATE USER` or `ALTER USER` exceeds the internally defined maximum string length, it returns this error. This is a protective constraint to maintain consistency of the database schema and system metadata and to prevent potential issues such as internal buffer overflow.


.. _ERROR-1099:

**ERROR CODE: -1099, 'DDL statement is not allowed by configuration (block_ddl_statement=yes).\n SQL Text: %1$s'**

- This message is displayed when trying to execute a DDL (Data Definition Language) statement in the CUBRID database, that is, when the system parameter `block_ddl_statement` is set to `yes`, it is a protection feature that blocks schema change operations such as creating, modifying, and dropping tables, it is a protective error to ensure stability and security of the database schema and prevents unintended schema changes, it checks whether it is a DDL statement with the `pt_is_ddl_statement` function in a DDL statement and allows or blocks execution according to the system parameter setting.


.. _ERROR-1100:

**ERROR CODE: -1100, 'Statement without WHERE clause is not allowed by configuration (block_nowhere_statement=yes).'**

- This message is displayed when trying to execute a DELETE statement without a WHERE clause in the CUBRID database, that is, when the system parameter `block_nowhere_statement` is set to `yes`, it is a protection feature that blocks execution of dangerous SQL statements that delete all records without conditions, it is a protective error to ensure data integrity and system stability and prevents accidentally deleting data of an entire table, it mainly occurs by detecting the case where `search_cond` (search condition) is NULL in a DELETE statement, and allows or blocks execution according to the system parameter setting.


.. _ERROR-1102:

**ERROR CODE: -1102, 'Unknown system parameter or bad value.'**

- This message is displayed when setting or changing a system parameter in the CUBRID database when there is no change value, that is, it occurs when referencing a system parameter that does not exist, or when the parameter value is outside the allowed range or format, it is a protective error to ensure system stability and data integrity.


.. _ERROR-1103:

**ERROR CODE: -1103, 'Cannot change system parameter.'**

- This message is displayed when trying to change a system parameter in the CUBRID database when there is no change value, that is, it occurs when attempting to change a parameter that cannot be changed dynamically while the database is running, or when trying to modify a parameter for which there is no change permission, it is a protective error to ensure system stability and data integrity.


.. _ERROR-1104:

**ERROR CODE: -1104, 'Slow query (%1$d msec)\n%2$s'**

- This message is not actually an error but an informational message for performance monitoring in the CUBRID database, that is, it detects that a query that took longer than the configured threshold has been executed and provides detailed information for performance analysis, it is an important monitoring feature for database performance optimization and problem diagnosis, and the system collects detailed information such as query execution time, SQL statement, execution plan, buffer statistics, wait time, and so on to help identify performance bottlenecks.


.. _ERROR-1115:

**ERROR CODE: -1115, 'Started to update statistics (class "%1$s", oid : %2$d|%3$d|%4$d).'**

- This message is not actually an error but an informational message that indicates that the statistics update `update statistics` operation in the CUBRID database has started, that is, it means that the operation to update statistics (index statistics, column distribution, cardinality, etc.) of a table has started, it is a positive message that indicates that the process of collecting and updating statistics information necessary for query optimization has started, updating statistics is an important operation to optimize query execution plans by analyzing table data distribution, index structure, column value distributions, and so on.


.. _ERROR-1116:

**ERROR CODE: -1116, 'Finished to update statistics (class "%1$s", oid : %2$d|%3$d|%4$d, error code : %5$d).'**

- This message is not actually an error but an informational message that indicates that the statistics update `update statistics` operation in the CUBRID database has completed, that is, it means that the operation to update statistics (index statistics, column distribution, cardinality, etc.) of a table has ended normally, it is a positive message that indicates that the process of collecting and updating statistics information necessary for query optimization has completed.


.. _ERROR-1117:

**ERROR CODE: -1117, 'Cannot change attribute "%1$s". CUBRID cannot change an attribute as a SHARED and vice versa.'**

- This message occurs when attempting to change the SHARED attribute property of an already defined attribute in CUBRID. The SHARED attribute is a property determined at table definition time, and it cannot be changed afterward.


.. _ERROR-1123:

**ERROR CODE: -1123, 'pthread_cond_timedwait() timed out.'**

- This message occurs when a thread used internally by the CUBRID database waits for a critical section (e.g., buffer latch, etc.) and exceeds the specified time.


.. _ERROR-1130:

**ERROR CODE: -1130, 'Transaction is aborted because transaction lock counts exceeds %1$d.'**

- This message occurs when the number of locks held by a transaction in the CUBRID database exceeds the threshold set in the system, that is, it occurs when, in the $CUBRID/conf/cubrid.conf configuration parameters, rollback_on_lock_escalation is true, and the configured threshold of lock escalation is exceeded, such a situation can generally occur when many row locks are required in queries that process a large amount of data.


.. _ERROR-1132:

**ERROR CODE: -1132, 'Encryption library failure: %1$s'**

- This message is an error that occurs during encryption/decryption related to encryption in the CUBRID dblink server.


.. _ERROR-1135:

**ERROR CODE: -1135, 'A previous link of B+tree(%1$s) is corrupted.: vpid = (%2$d, %3$d)'**

- This message is a warning message indicating that b-tree corruption was detected when executing the CUBRID database checkdb --repair-prev-link command.


.. _ERROR-1136:

**ERROR CODE: -1136, 'A previous link of B+tree(%1$s) is repaired.: vpid = (%2$d, %3$d)'**

- This message is a warning message indicating that the b-tree corruption detected when executing the CUBRID database checkdb --repair-prev-link command has been repaired.

.. _ERROR-1138:

**ERROR CODE: -1138, 'Handshake error (peer host %1$s): incompatible interruptibility. (client: %2$s, server: %3$s)'**

- This message occurs during the handshake process that establishes a network connection between the CUBRID database client and the server when the two sides' 'interruptibility' settings are not compatible with each other; that is, it means that a communication connection cannot be established because the client and the server do not agree on how to handle interrupts (e.g., Ctrl+C or `KILL TRANSACTION`) during transaction or query execution; this is a protective error to ensure the consistency and stability of network communication.


.. _ERROR-1145:

**ERROR CODE: -1145, 'To change the owner of a system class is not allowed.'**

- This message occurs when you attempt to change the owner of a 'system table' that plays an important internal role in the CUBRID database; that is, in order to maintain the core structure and security of the database, it is a protective error that does not allow even a normal user or a user with specific privileges to arbitrarily change the ownership of a system table; this situation generally occurs when you try to change the owner for a system table using a DDL (Data Definition Language) statement such as `ALTER CLASS ... CHANGE OWNER TO ...`; this is an important security policy to protect the stability and integrity of the database.


.. _ERROR-1152:

**ERROR CODE: -1152, 'The owner or a member of DBA group is only allowed to kill the transaction (%1$d).'**

- This message occurs when, in the CUBRID database, a user without the required privileges attempts to forcibly terminate (kill) a specific transaction; that is, transaction termination is designed so that only users with restricted privileges can perform it for database stability and security; this situation generally occurs when a normal user tries to terminate another user's transaction, or when trying to terminate a system transaction; this is a protective error to protect the database security policy and integrity.


.. _ERROR-1158:

**ERROR CODE: -1158, 'Serializable conflict due to concurrent updates'**

- This message occurs when, in the CUBRID database, a transaction running at the `SERIALIZABLE` isolation level conflicts with update work from another concurrent transaction and the serializability is broken. The `SERIALIZABLE` isolation level is the strictest level that guarantees that concurrently executed transactions produce the same result as if they had been executed sequentially. This message occurs when the data that the current transaction tries to access has been changed by another transaction, or when a non-serializable phenomenon such as a phantom read is detected, and, in order to maintain data consistency, it rolls back one of the conflicting transactions. This is normal behavior caused by the strict nature of the `SERIALIZABLE` isolation level.


.. _ERROR-1159:

**ERROR CODE: -1159, 'Timezone compile error: %s'**

- This message indicates that a problem occurred while the CUBRID database internally processes or "compiles" timezone data; it includes logic to parse timezone rules and data and convert them into a form that can be used internally. This message mainly occurs when the timezone data files installed on the system are corrupted, the format is incorrect, or CUBRID fails to interpret the data. This can prevent the database from performing timezone-related operations accurately and is an important issue that can affect the validity and consistency of date/time data.


.. _ERROR-1160:

**ERROR CODE: -1160, 'Timezone library loader error: %s'**

- This message indicates that the CUBRID database failed to load the timezone library or data required to perform timezone-related operations. CUBRID relies on the system's timezone information for accurate processing of date/time data, especially for timezone conversion of `DATETIME WITH TIME ZONE` types or `TIMESTAMP` values; this message mainly occurs when timezone data files are missing, corrupted, lack access permissions, or timezone-related environment settings are incorrect. Failure to load the timezone library prevents the database from performing timezone-related features normally, which can lead to database startup failure or errors in date/time-related queries and operations.


.. _ERROR-1161:

**ERROR CODE: -1161, 'Timezone conversion error.'**

- This message indicates that an unexpected internal problem occurred while converting a date/time value from one timezone to another in the CUBRID database. CUBRID internally manages time based on UTC (Coordinated Universal Time) and converts the displayed time according to the timezone setting of the user session. This message can mainly occur in the following situations:


.. _ERROR-1162:

**ERROR CODE: -1162, 'Invalid or missing timezone.'**

- This message occurs when timezone information is invalid in the CUBRID database, or when timezone information required to perform date/time-related operations is missing. CUBRID requires valid timezone information to accurately process and convert date/time data.


.. _ERROR-1163:

**ERROR CODE: -1163, 'Invalid or missing daylight saving time.'**

- This message occurs when, while processing date/time values in the CUBRID database, Daylight Saving Time (DST) information is invalid or required DST information is missing. CUBRID strictly applies DST rules to ensure the accuracy of timezone conversion and date/time calculations. This message can mainly occur in the following situations:


.. _ERROR-1165:

**ERROR CODE: -1165, 'Invalid combination of date, time, timezone and daylight specifier.'**

- This message occurs when, in the CUBRID database, the combination of date, time, timezone, and daylight saving time (DST) specifiers is logically invalid; CUBRID validates in timezone-related operations whether the input date/time and timezone information and DST rules are consistent with each other. For example, this error occurs when the date and time given conflict with DST rules in the specified timezone, or when a non-existent time combination is specified (e.g., a time that does not exist during the DST transition interval). This is a validation mechanism to ensure the accuracy and consistency of data and prevents incorrect timezone conversion or calculations.


.. _ERROR-1166:

**ERROR CODE: -1166, 'The specified combination of date and time do not exist (during daylight saving interval).'**

- This message occurs when, in the CUBRID database, the date and time combination specified by the user is a time that does not actually exist due to the Daylight Saving Time (DST) transition interval of a particular timezone. For example, when DST starts and the clock jumps forward by one hour from 2 AM to 3 AM, the times from 2:00 AM to 2:59 AM do not exist on that date. When the user attempts to input or calculate such a non-existent time, CUBRID raises this error to ensure the validity of the data. This can mainly occur during date/time data insert, update, or when using date/time functions.


.. _ERROR-1167:

**ERROR CODE: -1167, 'Comment string cannot have more than 1024 bytes.'**

- This message indicates an error that occurs when, in the CUBRID database, the length of a comment (annotation) string for a user or an object exceeds the allowed maximum size of 1024 bytes. Comments are used to store descriptions or memos for database user objects (tables, columns, users, etc.), and there is a length limit for system stability and performance; that is, this message occurs when setting a user comment in the user authentication system of the CUBRID database.


.. _ERROR-1168:

**ERROR CODE: -1168, 'Unable to update statistics on table (%1$s) because a lock is not immediately granted.'**

- This message indicates that, in the CUBRID database, an attempt was made to update the statistics of a specific table, but the operation failed because the required lock could not be acquired immediately. Statistics updates provide essential information for the query optimizer to establish an optimal execution plan; this message mainly occurs when another transaction holds an exclusive lock on the target table for statistics update, or when lock contention is severe and the statistics update cannot obtain the needed lock in time. This means that the statistics update is delayed or fails, which can negatively affect the performance of queries executed afterward.


.. _ERROR-1177:

**ERROR CODE: -1177, 'Incompatible timezone data: %1$s has different checksum from %2$s.'**

- This message is an error indicating that the timezone data between the client and the server in the CUBRID database is not compatible with each other. CUBRID requires the client and server to use the same timezone data to ensure the accuracy of timezone-related operations. This message occurs when the checksums of the client's and server's timezone data differ, which means that different versions of timezone data are being used or the timezone data is corrupted. Since a mismatch of timezone data can cause inaccurate results of date/time conversion functions, the database raises an error in this situation to protect data integrity.


.. _ERROR-1179:

**ERROR CODE: -1179, 'Found not vacuumed entries in heap.'**

- This message is an informational or warning message that can occur when running the CUBRID checkdb command, indicating that there are entries (records) in the heap file (the space where a table's actual data is stored) that have not been vacuumed. When records are deleted or updated in a database, the space is not immediately reused and is marked as "dead" space. The vacuum operation reclaims such dead space and organizes it so it can be reused; this message tells you that vacuum work is needed, or that space that has not yet been cleaned up was found during a particular check process. This is information related to space efficiency and performance maintenance rather than a database consistency issue.


.. _ERROR-1180:

**ERROR CODE: -1180, 'Found not vacuumed OIDs. INDEX %1$s ON CLASS %2$s (CLASS_OID: %3$d|%4$d|%5$d).'**

- This message is an informational or warning message that can occur when running the CUBRID checkdb command, indicating that there are object IDs (OIDs) remaining in an index that have not been vacuumed (i.e., deleted but the space has not yet been reclaimed). When a record is deleted in a database, the OID of that record is not immediately reused and is cleaned up into an inactive or reusable state through vacuum work; this message tells you that such "uncleaned" OIDs exist in a specific index, which can mean inefficient use of index space or potential performance degradation. In general, this is information related to maintenance rather than a database consistency issue.


.. _ERROR-1186:

**ERROR CODE: -1186, 'checksumdb: %1$s (error code: %2$d).'**

- This message indicates that an internal problem occurred while running the `checksumdb` utility of the CUBRID database. `checksumdb` is a tool that checks the checksums of database files to verify whether data corruption exists; this message means that the utility could not complete its work normally due to various causes such as file access problems, data mismatches, internal logic errors, and so on during the checksum checking process.


.. _ERROR-1187:

**ERROR CODE: -1187, 'Invalid access on page %1$d,%2$d which was part of a deleted heap.'**

- This message occurs when, in the CUBRID database, an attempt is made to access a page that belonged to a heap file (the actual data store of a table) that has already been deleted (released). That is, it is a protective error that detects and blocks attempts to refer to or read a data page of a table (or object) that has been logically or physically deleted; this situation generally occurs when a wrong reference remains to the page after a table DROP, TRUNCATE, or after the heap file has been deleted internally.


.. _ERROR-1188:

**ERROR CODE: -1188, 'Dropping an active user '%1$s' is not allowed.'**

- This message occurs when, in the CUBRID database, when performing `DROP USER`, it is not possible to DROP a user who is in the middle of executing a transaction.


.. _ERROR-1189:

**ERROR CODE: -1189, 'The specified combination of date and time do not exist (during transition of timezone offset rules).'**

- This message occurs when, in the CUBRID database, while processing date and time values, a date and time combination that does not exist is entered during a transition period when the offset rules of a particular timezone change (e.g., at the start or end of Daylight Saving Time (DST)). For example, when DST starts and the clock skips from 2 AM to 3 AM, a time such as 2:30 AM does not physically exist on that date. This message occurs to protect the validity of the data by detecting such a non-existent time.


.. _ERROR-1190:

**ERROR CODE: -1190, 'Stand-alone vacuum execution is started.'**

- This message is an informational message indicating that a stand-alone vacuum operation has started in the CUBRID database. This is not an error, but a log message that informs the user that the vacuum operation, which reclaims database space and optimizes performance, has started normally.


.. _ERROR-1191:

**ERROR CODE: -1191, 'Stand-alone vacuum execution is ended.'**

- This message is an informational message indicating that a stand-alone vacuum operation has been successfully completed in the CUBRID database. This is not an error, but a log message that informs the user that the vacuum operation, which reclaims database space and optimizes performance, has ended normally.


.. _ERROR-1194:

**ERROR CODE: -1194, 'Unable to check file %1$d|%2$d because lock on class %3$d|%4$d|%5$d is not immediately granted.'**

- This message indicates a problem that occurs when, in the CUBRID checkdb command, while checking an object (heap, btree, etc.), when trying to inspect or access a specific file (generally a data page file), the lock on the class (table) to which that file belongs cannot be acquired immediately due to a lock acquisition error. This mainly occurs in situations related to concurrency control, such as when another transaction holds an exclusive lock on the table, or when there are too many lock requests and they cannot be processed immediately. It means that the system needs a lock to check the integrity of the file or to access it safely, but it cannot proceed because it could not obtain it.


.. _ERROR-1241:

**ERROR CODE: -1241, 'XASL tree needs recompile.'**

- This message is an internal request or error indicating that, during the query processing process in the CUBRID database system, the XASL (eXtensible Abstract Syntax Language) tree needs to be recompiled; XASL is CUBRID's internal query representation format and is an intermediate stage in which an SQL query is parsed and optimized and then converted into an execution plan. This execution plan can be cached to improve performance; this message means that the cached XASL tree is no longer valid, or that a situation has occurred where the optimized execution plan must be generated again. This mainly occurs due to reasons such as table schema changes, statistics information updates, or changes in the internal system state; in general, rather than being a serious error directly exposed to the user, it occurs as part of a mechanism to maintain consistency and optimization of query processing inside CUBRID.


.. _ERROR-1247:

**ERROR CODE: -1247, 'The time(%1$s) specified must be after the time(%2$s) of the specified backup.'**

- This message occurs when, while the CUBRID system performs a database recovery operation, the recovery time specified by the user is earlier than or equal to the creation time of the backup to be used; database recovery is the process of restoring a database to a specific point in time (Point-In-Time Recovery) or to the latest state by starting from a specific backup point and applying subsequent transaction logs. The recovery time must always be in the future relative to the backup time. Recovering to a time before the backup time is logically impossible, and recovering to the same time as the backup time is the same as using the backup itself, so this error can occur when attempting to recover to a particular point in time after the backup time.


.. _ERROR-1248:

**ERROR CODE: -1248, 'Invalid Key file : %1$s'**

- This message occurs when the CUBRID system detects that the key file used for TDE (Transparent Data Encryption) is invalid or corrupted while reading or processing it.


.. _ERROR-1249:

**ERROR CODE: -1249, 'Cannot find the key (index: %1$d).'**

- This message is an error indicating that the CUBRID system failed because it tried to find the master key corresponding to a specific index while using the TDE (Transparent Data Encryption) feature, but could not find that key.


.. _ERROR-1250:

**ERROR CODE: -1250, 'The master key set on the database and what is given don't match. The key index set on the database: %1$d'**

- This message occurs when, while using the TDE (Transparent Data Encryption) feature, the CUBRID system detects that the master key information (especially the key index) set in the database does not match the master key information provided to the current system; TDE is a feature that strengthens security by encrypting database data, and the master key is the top-level key used to encrypt the Data Encryption Key (DEK). If this master key does not match, encrypted data cannot be accessed; this message mainly occurs when the contents of the TDE key file have been changed, an incorrect key file is used, or the key index recorded in the database metadata differs from the index of the currently loaded key.


.. _ERROR-1251:

**ERROR CODE: -1251, 'Error while TDE-encrypting.'**

- This message is an error indicating that the CUBRID system failed the encryption operation because an unexpected error occurred during the process of encrypting data using the TDE (Transparent Data Encryption) feature; TDE is a feature that strengthens security by encrypting database data, and data is encrypted before being written to disk and decrypted when read. This message means that a problem occurred in this encryption process; encryption failure suggests that data may not be stored properly, data integrity may be damaged, or normal operation of the database may be hindered.


.. _ERROR-1252:

**ERROR CODE: -1252, 'Error while TDE-decrypting.'**

- This message is an error indicating that the CUBRID system failed the decryption operation because an unexpected error occurred during the process of decrypting encrypted data using the TDE (Transparent Data Encryption) feature; TDE is a feature that strengthens security by encrypting database data, and data is encrypted before being written to disk and decrypted when read. This message means that a problem occurred in this decryption process; decryption failure suggests that encrypted data cannot be accessed, data integrity may be damaged, or normal operation of the database may be hindered.


.. _ERROR-1253:

**ERROR CODE: -1253, 'TDE Module is not loaded.'**

- This message is an error indicating that, when the CUBRID system tries to use the TDE (Transparent Data Encryption) feature, the TDE module has not been properly loaded or initialized; that is, it occurs when you perform TDE-related operations in a state where the database was started without a TDE KEY file, or was started with an incorrect TDE KEY file so that the TDE module does not operate properly.


.. _ERROR-1254:

**ERROR CODE: -1254, 'Cannot create a key.'**

- This message is an error indicating that the CUBRID system failed the key creation operation because an unexpected error occurred during the process of generating an encryption key while using the TDE (Transparent Data Encryption) feature; TDE is a feature that strengthens security by encrypting database data, and it requires keys such as the master key or the Data Encryption Key (DEK). This message means that a problem occurred in this key generation process; key generation failure suggests that the TDE feature may not operate normally or that there may be a problem with data security.


.. _ERROR-1255:

**ERROR CODE: -1255, 'Cannot load TDE module. You can't use TDE feature.\n\'**

- This message is an informational message that occurs when the CUBRID system tries to use the TDE (Transparent Data Encryption) feature, informing that the TDE feature cannot be used because there is no TDE KEY file at database startup.


.. _ERROR-1256:

**ERROR CODE: -1256, 'Cannot copy key file (_keys). You can't use TDE feature on the copied database.'**

- This message is an error indicating that, during the process of copying a CUBRID database with the copydb command, copying the key file (_keys) used for TDE (Transparent Data Encryption) failed.


.. _ERROR-1258:

**ERROR CODE: -1258, 'Cannot find the key set on the database from %1$s. The key file from backup volume is going to be used.'**

- This message indicates that, when the CUBRID system starts or accesses a database that uses the TDE (Transparent Data Encryption) feature, it cannot find the master key file (_keys file) set on the database at the specified path; however, the system has fortunately found the key file within the backup volume and is a warning message indicating that it will continue operating the database using this backup key file.


.. _ERROR-1259:

**ERROR CODE: -1259, 'Rename the key file from "%1$s" to "%2$s".'**

- This message is a warning message indicating that, during the process of recovering or restoring the key file of a database that uses the TDE (Transparent Data Encryption) feature, it is changing back to the previous key file.


.. _ERROR-1260:

**ERROR CODE: -1260, 'Copy the key file "%1$s" to "%2$s".'**

- This message is a warning message indicating that, during the process of restoring a database that uses the TDE (Transparent Data Encryption) feature, it restored the key file from the backup.


.. _ERROR-1261:

**ERROR CODE: -1261, 'The first key (key index: %1$d, created time: %2$s) on the restored key file has been set.'**

- This message is an informational or warning message indicating that, during the CUBRID database restore process, the TDE (Transparent Data Encryption) key file has been successfully restored, and the first master key (with the specified index and creation time) in that key file has been set on the database; TDE is a feature that strengthens security by encrypting database data, and the master key is the top-level key used to encrypt the Data Encryption Key (DEK). This message means that the restored database is ready to access encrypted data using the TDE feature; generally this is a positive result, but it is important to check whether the key matches what the user intended.


.. _ERROR-1262:

**ERROR CODE: -1262, 'The key file is full.'**

- This message occurs when, while the CUBRID system uses the TDE (Transparent Data Encryption) feature, it exceeds the maximum number of keys that can be stored when adding a new TDE KEY.


.. _ERROR-1263:

**ERROR CODE: -1263, 'It fails to TDE-encrypt the log page (pageid: %1$lld). It won't be tried to encrypt this page any longer.'**

- This message is an error indicating that the CUBRID system failed the encryption operation because an unexpected error occurred while encrypting a database log page using the TDE (Transparent Data Encryption) feature; log pages are a very important part that record all changes in the database, and in an environment where TDE is enabled, these log pages must also be encrypted for security.


.. _ERROR-1286:

**ERROR CODE: -1286, 'Cannot find the log lsa at the time to extract. (input time : %1$s).'**

- This message is an error indicating that, when the CUBRID system tries to find the log LSA (Log Sequence Address) for a specific time while using the CDC (Change Data Capture) feature, it cannot find the log LSA corresponding to that time; CDC is a feature that captures and tracks database changes in real time, and LSA is used as a log sequence address to identify a specific point in time in the database.


.. _ERROR-1287:

**ERROR CODE: -1287, 'Timed out attempting to extract log info. (elapsed time : %1$d sec, extraction timeout : %2$d sec)'**

- This message is an error indicating that, when the CUBRID system uses the CDC (Change Data Capture) feature to extract log information, the work was stopped because it exceeded the configured timeout value.


.. _ERROR-1289:

**ERROR CODE: -1289, 'Skip producing log info for invalid transaction. (trid : %1$d)'**

- This message means that, while the CUBRID system uses the CDC (Change Data Capture) feature to process log information, it found an invalid transaction and skipped generating log information for that transaction.


.. _ERROR-1290:

**ERROR CODE: -1290, 'Invalid log lsa (%1$lld|%2$d) to extract log info.'**

- This message is an error indicating that, when the CUBRID system tries to extract log information using the CDC (Change Data Capture) feature, it encountered an invalid log LSA (Log Sequence Address); CDC is a feature that captures and tracks database changes in real time, and LSA is used to identify a specific point in time in the database; this message mainly occurs when the specified LSA is outside the log range of the database, log files are corrupted, or the LSA format is incorrect.


.. _ERROR-1291:

**ERROR CODE: -1291, 'Cannot connect to server for extracting log info. Try again with setting 'supplemental_log' parameter.'**

- This message is an error indicating that, when the CUBRID system tries to extract log information using the CDC (Change Data Capture) feature, it cannot connect to the server; CDC is a feature that captures and tracks database changes in real time, and using this feature requires a connection to the server.


.. _ERROR-1292:

**ERROR CODE: -1292, 'Cannot find the log lsa at the time (%1$s). Log lsa is adjusted to the time (%2$s).'**

- This message is a warning message indicating that, when the CUBRID system tries to find the log LSA (Log Sequence Address) for a specific time using the CDC (Change Data Capture) feature, it could not find an LSA that exactly matches that time, so it adjusted to the LSA of the closest valid time; CDC is a feature that captures and tracks database changes in real time, and LSA is used to identify a specific point in time in the database; this message means that the system could not find a log that exactly matches the requested time, but adjusted it so that the work can continue by finding the closest valid time log.


.. _ERROR-1293:

**ERROR CODE: -1293, 'Cannot get log page while producing log infos. Log lsa to generate log info indicates NULL.'**

- This message is an error indicating that, while the CUBRID system generates log information using the CDC (Change Data Capture) feature, it cannot perform the work because the log LSA (Log Sequence Address) has a NULL value when trying to retrieve a log page.


.. _ERROR-1294:

**ERROR CODE: -1294, 'Log info has been generated. (type : "%1$s").'**

- This message is an informational message indicating that the CUBRID system successfully generated log information using the CDC (Change Data Capture) feature; CDC is a feature that captures and tracks database changes in real time, and log information is the data that records such changes; this message generally indicates successful work and informs what type of log information was generated by including the type of the generated log information.


.. _ERROR-1296:

**ERROR CODE: -1296, 'Log recovery: ANALYSIS Phase is started.'**

- This message is an informational message indicating that, when the CUBRID database system is restarted after an abnormal shutdown or when a specific recovery operation starts, the 'ANALYSIS phase', the first step of the transaction log recovery process, has started; log recovery is an essential process to ensure the consistency and integrity of the database, and it rolls back uncommitted transactions due to system crashes and re-applies committed changes that were not reflected on disk; in the 'ANALYSIS phase', it scans the log file to identify all transaction activities after the last checkpoint and collects information such as which transactions were committed and which transactions should be rolled back.


.. _ERROR-1297:

**ERROR CODE: -1297, 'Log recovery: REDO Phase is started. Log pages to redo: %1$lld, Log records to redo: %2$lld.'**

- This message is an informational message indicating that, when the CUBRID database system is restarted after an abnormal shutdown or when a specific recovery operation starts, the 'REDO phase', the second step of the transaction log recovery process, has started; log recovery is an essential process to ensure the consistency and integrity of the database, and it re-applies changes that were committed but not reflected on disk due to system crashes in the order recorded in the logs; in the 'REDO phase', based on the information identified in the previous 'ANALYSIS phase', it applies the changes of committed transactions to the data files; this message provides the number of log pages to redo and the number of log records to redo together to indicate the scale of the recovery work.


.. _ERROR-1298:

**ERROR CODE: -1298, 'Log recovery: UNDO Phase is started. Log pages to undo: %1$lld, transactions to undo: %2$d.'**

- This message is an informational message indicating that, when the CUBRID database system is restarted after an abnormal shutdown or when a specific recovery operation starts, the 'UNDO phase', the third step of the transaction log recovery process, has started; log recovery is an essential process to ensure the consistency and integrity of the database, and it rolls back uncommitted transactions due to system crashes; in the 'UNDO phase', based on the information identified in the previous 'ANALYSIS phase', it reverts the changes of uncommitted transactions; this message provides the number of log pages to undo and the number of transactions to undo together to indicate the scale of the recovery work.


.. _ERROR-1299:

**ERROR CODE: -1299, 'Log recovery: %1$s Phase is being finished up.'**

- This message is an informational message indicating that, when the CUBRID database system is restarted after an abnormal shutdown or when a specific recovery operation starts, a specific phase (e.g., ANALYSIS, REDO, UNDO) of the transaction log recovery process is being successfully wrapped up; log recovery is an essential process to ensure the consistency and integrity of the database, and it rolls back uncommitted transactions due to system crashes and re-applies committed changes that were not reflected on disk; this message indicates that each recovery phase is proceeding successfully and suggests that the overall recovery process is progressing smoothly.


.. _ERROR-1300:

**ERROR CODE: -1300, 'Log recovery: %1$s Phase is finished.'**

- This message is an informational message indicating that, when the CUBRID database system is restarted after an abnormal shutdown or when a specific recovery operation starts, a specific phase (e.g., ANALYSIS, REDO, UNDO) of the transaction log recovery process has been successfully completed; log recovery is an essential process to ensure the consistency and integrity of the database, and it rolls back uncommitted transactions due to system crashes and re-applies committed changes that were not reflected on disk; this message indicates that each recovery phase has been successfully completed and suggests that the overall recovery process is progressing smoothly.


.. _ERROR-1301:

**ERROR CODE: -1301, 'Log recovery: %1$s progress: (%2$lld/%3$lld), %4$.2f percent, elapsed time: %5$.2f (s), estimated remaining time: %6$.2f (s).'**

- This message is an informational message that provides real-time progress of a specific phase (ANALYSIS, REDO, UNDO) of the transaction log recovery process when the CUBRID database system is restarted after an abnormal shutdown or when a specific recovery operation starts; log recovery is an essential process to ensure the consistency and integrity of the database, and it rolls back uncommitted transactions due to system crashes and re-applies committed changes that were not reflected on disk; this message provides detailed information about the current state of the recovery work, the amount completed, the total amount, the progress rate, the elapsed time, and the estimated remaining time, helping the administrator monitor the recovery progress.


.. _ERROR-1302:

**ERROR CODE: -1302, 'dblink %1$s'**

- This message is a general (inclusive) error message that occurs in CUBRID's DBLink feature and is used in various subordinate error situations; it is an error code used by the parser, etc., when a dblink-related error occurs, and the actual cause of the error depends on the detailed message that is delivered by substituting multiple messages; it can occur at all layers related to DBLink, including network communication, authentication, parameters, query execution, and so on.


.. _ERROR-1303:

**ERROR CODE: -1303, 'dblink invalid number of columns specified.'**

- This message occurs when, in CUBRID, while executing a query on an external DB through DBLink, the specified number of columns does not match the number of columns in the actual query result, or does not satisfy the conditions required by the system; for example, it can occur when the number of columns returned by a SELECT query differs from the number of columns specified in the DBLink object, or when column mapping is incorrect.


.. _ERROR-1304:

**ERROR CODE: -1304, 'dblink catalog _db_server class not found.'**

- This message occurs when, in CUBRID, while performing DBLink-related operations (creating, querying, modifying server objects, etc.), the internally referenced system catalog table (_db_server table) does not exist in the database; that is, it is a case where the core information repository of the DBLink feature is damaged, or it is abnormal due to a product bug.


.. _ERROR-1305:

**ERROR CODE: -1305, 'dblink server "%1$s" not found.'**

- This message occurs when, in CUBRID, when trying to reference (e.g., connect, modify, delete, etc.) a DBLink server object, a server object with the specified name does not exist in the database; that is, the server name was entered incorrectly, has already been deleted, or has not yet been created.


.. _ERROR-1306:

**ERROR CODE: -1306, 'dblink server "%1$s" already exists.'**

- This message occurs when, in CUBRID, when trying to create (CREATE SERVER) a DBLink server object, a server object with the specified name already exists; that is, it is a situation where duplicate creation is not possible because the same server name is already registered.


.. _ERROR-1307:

**ERROR CODE: -1307, 'dblink Cannot update server object.'**

- This message occurs when, in CUBRID, when trying to modify (UPDATE) the information of a DBLink server object, the operation is not allowed due to system policy or privilege restrictions; for example, it can be denied due to lack of privileges if not the owner, DBA, or DBA member, or it can be denied for reasons such as ensuring consistency of the server object.


.. _ERROR-1308:

**ERROR CODE: -1308, 'dblink not supported type %1$s.'**

- This message occurs when, in CUBRID, while executing a query on an external DB through DBLink, the data type used is a type that is not supported by the CUBRID DBLink feature; for example, it occurs when a particular data type of an external DB (e.g., large binary, special types, etc.) cannot be mapped/converted/processed in CUBRID DBLink.


.. _ERROR-1309:

**ERROR CODE: -1309, 'dblink invalid bind param.'**

- This message occurs when, in CUBRID, while executing a query on an external DB through DBLink, the bind parameter passed is not valid.
  For example, it can occur when the number of parameters does not match, the type is different, the value is NULL/abnormal, and so on, in cases that are not allowed.


.. _ERROR-1310:

**ERROR CODE: -1310, 'dblink password length exceeds max size.'**

- This message occurs when, in CUBRID, when creating or modifying a DBLink server object, the length of the password entered exceeds the maximum size allowed by the system; this can also occur when the actual byte count exceeds due to an encoding issue even if the password appears not too long.


.. _ERROR-1311:

**ERROR CODE: -1311, 'dblink encrypted password length is incorrect.'**

- This message occurs when, in CUBRID, while decrypting or validating an encrypted password of a DBLink server object, the length of the encrypted string differs from the length required by the system; this can occur when the encrypted password is corrupted, generated/stored in an incorrect way, or an incorrect ciphertext was entered when creating the DBLink object.


.. _ERROR-1312:

**ERROR CODE: -1312, 'dblink the checksum of the encrypted password does not match.'**

- This message occurs when, in CUBRID, while decrypting or validating an encrypted password of a DBLink server object, the checksum value of the encrypted string differs from the expected value; this can occur when the encrypted password is corrupted, the encryption/decryption environment (algorithm, key, etc.) does not match, or data was stored/transmitted incorrectly.


.. _ERROR-1313:

**ERROR CODE: -1313, 'dblink invalid cipher string format.'**

- This message occurs when, in CUBRID, while decrypting or validating an encrypted password of a DBLink server object, the format of the encrypted string (ciphertext) is not correct; it can occur when the encrypted string is corrupted, the format does not match, or it was stored in an unexpected way (e.g., insufficient length, missing delimiter, encoding error, etc.).


.. _ERROR-1314:

**ERROR CODE: -1314, 'dblink Failed to decryption password. error=%1$d".'**

- This message occurs when, in CUBRID, when trying to connect to an external DB through a DBLink server object, the stored password fails internally during the decryption process; it is returned when decryption cannot be performed normally due to various causes such as issues with the crypto library, mismatch of encryption/decryption keys, corruption of encrypted data, system environment issues, internal bugs, and so on.


.. _ERROR-1315:

**ERROR CODE: -1315, 'dblink Failed to encryption password. error=%1$d".'**

- This message occurs when, in CUBRID, when creating or modifying a DBLink server object, the encryption of the password for connecting to an external DB fails internally; it is returned when encryption cannot be performed normally due to various causes such as issues with the crypto library, input value errors, insufficient system resources, unsupported crypto algorithm, internal bugs, and so on.


.. _ERROR-1317:

**ERROR CODE: -1317, 'dblink Not allowed "%1$s" Server.'**

- This message occurs when, in CUBRID, when attempting an operation (ALTER, etc.) that is not allowed on a DBLink server object; depending on certain system policies, privileges, or the attributes of the DBLink server, changes (ALTER, DROP, etc.) to that server may be prohibited.
  For example, it can occur when the server is protected by the system, or when the current session/user does not have the authority to change that server.


.. _ERROR-1318:

**ERROR CODE: -1318, '[%1$5.5s][%2$d] %3$s'**

- This message is a message that, when an error occurs from an external database (such as an ODBC driver) in CUBRID's Gateway feature (DBLink external DB integration, ODBC, etc.), formats and delivers the detailed information of that error as-is. That is, it shows the SQLSTATE, error code, and detailed message of an error that occurred in an external DBMS or ODBC driver, not inside CUBRID.
- This message serves to pass through the error information of the external system without CUBRID directly interpreting the cause of the problem that occurred during DBLink external DB integration.


.. _ERROR-1319:

**ERROR CODE: -1319, 'Parameter binding error.'**

- This message is printed when, in CUBRID's Gateway feature (DBLink external DB integration, ODBC, etc.), a problem occurs during the parameter (input variable) binding process when executing an SQL query. It mainly occurs when the number of parameters to bind does not match, the types do not match, or an internal binding handle (structure, etc.) is not created correctly. For example, it can occur when a query needs 3 parameters but only 2 are bound, or when the type of a bound value does not match the DB column type.


.. _ERROR-1320:

**ERROR CODE: -1320, 'Invalid ODBC handle.'**

- This message occurs when, in CUBRID's Gateway feature (DBLink external DB integration, ODBC, etc.), an ODBC handle (identifier, pointer, etc.) is invalid. That is, it can occur when referencing a handle that has already been closed (freed), using a handle that was not created correctly, or when the handle is corrupted internally. It mainly occurs in cases such as reuse after disconnect, missing handle, memory corruption, driver bugs, and so on.


.. _ERROR-1322:

**ERROR CODE: -1322, 'not supported type %1$s(%2$d).'**

- This message occurs when, in CUBRID's Gateway feature (DBLink external DB integration, ODBC, etc.), a data type not supported by CUBRID is used during integration with an external database or during query execution. For example, it is a case where an external DB table contains a type that CUBRID cannot recognize (e.g., BLOB, CLOB, user-defined types, etc.), or where type mapping is not defined. This message mainly occurs due to data type compatibility issues, unimplemented type conversion logic, or limitations of the driver/integration module.


.. _ERROR-1323:

**ERROR CODE: -1323, 'Invalid Statement handle.'**

- This message occurs when, in CUBRID's Gateway feature (DBLink external DB integration, ODBC, etc.), the Statement handle (identifier, pointer, etc.) that manages an SQL statement is invalid. That is, it can occur when referencing a Statement that has already been closed, using a handle that was not created correctly, or when the handle is corrupted internally. It mainly occurs in cases such as reuse after Statement free, missing handle, memory corruption, driver bugs, and so on.


.. _ERROR-1324:

**ERROR CODE: -1324, 'Invalid db connection handle.'**

- This message occurs when, in CUBRID's Gateway feature (DBLink external DB integration, ODBC, etc.), a handle (identifier, pointer, etc.) that manages a database connection is invalid. That is, it can occur when referencing a connection that has already been closed, using a handle that was not created correctly, or when the handle is corrupted internally. It mainly occurs in cases such as reuse after disconnect, missing handle, memory corruption, driver bugs, and so on.


.. _ERROR-1325:

**ERROR CODE: -1325, 'Link server connection url do not exist.'**

- This message occurs when, in CUBRID's Gateway feature (DBLink external DB integration, ODBC, etc.), when attempting to connect to an external database, the URL (connection information) required for the connection is not defined or the system cannot find the information. That is, it is a case where the connection URL was omitted when creating the DBLink or Gateway server object, or where the information was not properly registered internally.


.. _ERROR-1326:

**ERROR CODE: -1326, 'Invalid numeric value.'**

- This message occurs when, in CUBRID's Gateway feature (DBLink external DB integration, ODBC, etc.), processing an invalid numeric type value in a query.


.. _ERROR-1327:

**ERROR CODE: -1327, 'Invalid precision value of "%1$s" column.'**

- This message occurs when, in CUBRID's Gateway feature (DBLink external DB integration, ODBC, etc.), while processing an external DB table or query result, the precision value of a particular column is specified as an invalid value (out of range, negative, 0, too large, etc.). It can mainly occur when the external DB schema definition is wrong, or when the ODBC driver returns column precision information incorrectly.


.. _ERROR-1329:

**ERROR CODE: -1329, 'Not supported dbms.'**

- This message occurs when, in CUBRID's Gateway feature (DBLink external DB integration, ODBC, etc.), the external DBMS you attempted to connect to is not an officially supported DBMS in CUBRID. That is, it occurs when you attempt to connect to a DBMS type that the CUBRID Gateway cannot recognize, or a DBMS that is not included in the supported list. For example, if CUBRID Gateway supports only some DBMS such as Oracle, MySQL, MSSQL, and so on, and you attempt to connect to an unsupported DBMS (e.g., SQLite, MariaDB, special-purpose DB, etc.), this error is returned.


.. _ERROR-1330:

**ERROR CODE: -1330, 'Unable to allocate an environment handle.'**

- This message occurs when, in CUBRID's Gateway feature (DBLink external DB integration, ODBC, etc.), while starting a connection to an external database, it fails during the process of creating (allocating) an environment handle (such as an ODBC environment handle). Since the environment handle is the base resource for all work such as sessions, connections, and transactions with an external DB, if allocation fails, all subsequent DB integration work becomes impossible. It can mainly occur due to insufficient system memory, ODBC driver/library issues, internal resource exhaustion, and so on.


.. _ERROR-1331:

**ERROR CODE: -1331, 'A null column binder was referenced.'**

- This message occurs when, in CUBRID's Gateway feature (DBLink external DB integration, ODBC, etc.), while binding a query execution result to an application variable, a column binder (binding handle) is referenced in a NULL (not initialized) state. That is, it is a programming error that occurs when the column binder was not created/allocated properly or when you attempt to access it after it has already been freed. It can mainly occur due to issues in DBLink external DB integration, the ODBC driver, or internal binding logic.


.. _ERROR-1332:

**ERROR CODE: -1332, 'The active log volume (%1$s) is too sane to recreate. If you remove the active log, it could cause an unexpected and irreversible problem.'**

- This message occurs when, in CUBRID, when using the `cubrid emergencylog -r` command, if the active log is normal, it indicates that it cannot be recreated; this occurs when you try to recreate the active log volume and the system does not allow recreation because the log file is in a sane state and not corrupted.


.. _ERROR-1333:

**ERROR CODE: -1333, ' Invalid time (%1$s) to start flashback. Time is required to be set between (%2$s) and (%3$s).'**

- This message occurs when, in CUBRID, when executing the Flashback feature, the recovery point in time (time) specified by the user is outside the range allowed by the system. Flashback is limited to a recoverable time range depending on log retention, database state, backup time, and so on, and if a request is made for a time outside this range, the operation is denied to ensure data consistency and integrity.


.. _ERROR-1334:

**ERROR CODE: -1334, 'Can not find the class to flashback (%1$s).'**

- This message occurs when, in CUBRID, when executing the Flashback feature, the target table (table) to be recovered does not exist in the database, or the system cannot find the table; it can mainly occur when the table has been deleted, the name is specified incorrectly, or there is a problem in the system catalog.


.. _ERROR-1335:

**ERROR CODE: -1335, 'Too many transactions to flashback. Less than %1$d transactions can be flashback.'**

- This message occurs when, in CUBRID, when executing the Flashback feature, the number of transactions to be recovered exceeds the maximum value allowed by the system. Flashback work has a limit on the number of transactions that can be processed at once for database consistency and performance. If this limit is exceeded, problems such as system overload, performance degradation, and recovery failure can occur, so the system imposes a policy limit.


.. _ERROR-1336:

**ERROR CODE: -1336, 'Cannot support flashback for this class (name : %1$s , OID : %2$d|%3$d|%4$d). Class schema has been changed.'**

- This message occurs when, in CUBRID, when executing the Flashback feature, the schema (structure) of the target table (table) has been changed after the flashback target time. For example, if the schema is changed such as adding/deleting columns, changing types, changing constraints, and so on, data recovery becomes impossible because the data at the flashback time and the current structure do not match. To ensure data integrity and consistency, flashback is not supported for tables whose schema has been changed.


.. _ERROR-1337:

**ERROR CODE: -1337, 'Log record does not exist (lsa : %1$lld|%2$d). The archive log volume has been deleted.'**

- This message occurs when, in the CUBRID database, a specific log record (needed for transaction recovery, flashback, etc.) does not exist, or the archive log volume storing that log has already been deleted so it can no longer be accessed; it can mainly occur when old logs have been deleted automatically/manually, when log files have been cleaned up according to backup/recovery/archive policies, or due to log file corruption, incorrect LSA access, and so on.


.. _ERROR-1338:

**ERROR CODE: -1338, 'Flashback is already running. Flashback cannot be run by more than one user at the same time.'**

- This message occurs when, in CUBRID, when trying to execute the Flashback feature, another session or user is already running a flashback operation. Flashback is restricted so that only a single user can execute it concurrently to ensure database consistency and integrity. Therefore, only one flashback operation is allowed at a time, and if a duplicate request comes in, this error is returned.


.. _ERROR-1341:

**ERROR CODE: -1341, 'Invalid arguments to authorization internal function.'**

- This message occurs in the CUBRID database when, while changing the owner of a class or method, the desired information is missing.


.. _ERROR-1342:

**ERROR CODE: -1342, 'AUTO_INCREMENT does not allow change of owner.'**

- This message occurs in the CUBRID database when attempting to change the owner of a serial object of auto increment.


.. _ERROR-1345:

**ERROR CODE: -1345, 'DBA, members of DBA group, and owner can perform CREATE TRIGGER.'**

- This message occurs when, in CUBRID, when attempting to create a trigger (CREATE TRIGGER), the user executing the command is not the DBA, a member of the DBA group, or the owner of the target object for the trigger. That is, it is a privilege-related error that occurs when a user without trigger creation privileges tries to execute CREATE TRIGGER.


.. _ERROR-1350:

**ERROR CODE: -1350, 'Invalid synonym value.'**

- This message occurs from a synonym (SYNONYM) object that has incorrect information when running the umloaddb command in CUBRID.


.. _ERROR-1356:

**ERROR CODE: -1356, 'There is no hint in the DBLink query, or the statement type is incorrect. %1$s'**

- This message occurs because, in CUBRID, a DBLink query internally adds the statement type as a hint and passes it to the dblink gateway to determine what kind of query it is, and an unknown statement type was delivered.


.. _ERROR-1357:

**ERROR CODE: -1357, 'Converting SQL string to wide string failed.'**

- This message occurs when, while CUBRID communicates with an external database through the DBLink Gateway, it cannot convert the SQL query string from UTF8 to Unicode; it mainly occurs when the character encoding does not match, or when the input string contains invalid (broken) characters.


.. _ERROR-1358:

**ERROR CODE: -1358, 'Number of rows affected is unknown.'**

- This message occurs when CUBRID cannot correctly determine the number of rows actually affected when executing DML (INSERT, UPDATE, DELETE). It can occur when, during inter-database integration work in a DBLink query, the external system does not return the number of affected rows, or when the returned value is not clear.


.. _ERROR-1362:

**ERROR CODE: -1362, 'Locale '%1$s' is unavailable.'**

- This message occurs when CUBRID uses locale information that has not been loaded on the server when converting a string to date/time,

