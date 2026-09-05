Query (SQL) Related Errors
==========================


.. _ERROR-64:

**ERROR CODE: -64, 'Unknown class "%1$s".'**

- This message is displayed when the specified table name cannot be found in the CUBRID system.


.. _ERROR-65:

**ERROR CODE: -65, 'Class "%1$s" already exists.'**

- This message is displayed when you attempt to create a new table with a table name that already exists in the CUBRID system.


.. _ERROR-176:

**ERROR CODE: -176, 'Conversion error in date format.'**

- This message is an error that appears when an error occurs during the format conversion process for date/time-related data types in CUBRID; it occurs in the process of converting a string to a date/time type, or validating the validity of a date/time value; it can be caused by an invalid date format, an out-of-range value, or data in a format that cannot be converted.


.. _ERROR-181:

**ERROR CODE: -181, 'Cannot coerce value of domain "%1$s" to domain "%2$s".'**

- This message is displayed when converting a value of one data type to another data type in CUBRID when the conversion is not possible or is incompatible; it occurs due to compatibility issues between data types or values that do not match the conversion rules; for example, it can occur when converting a string to a number, or performing an operation between different data types.


.. _ERROR-182:

**ERROR CODE: -182, 'Cannot coerce value of domain "%1$s" to domain "%2$s" without overflow.'**

- This message is an error that appears when converting a value of one data type to another data type in CUBRID when overflow occurs and the conversion cannot be performed; it means that in the data type conversion process the value range exceeds the allowed range of the target data type and therefore cannot be converted; for example, it can occur when attempting to convert a very large integer value to a small integer type.


.. _ERROR-202:

**ERROR CODE: -202, 'Attribute "%1$s" was not found.'**

- This message is displayed when a column with the specified name cannot be found in the CUBRID database; it can mainly appear due to an incorrect column name reference, access to a non-existent column, or a schema definition error.


.. _ERROR-203:

**ERROR CODE: -203, 'Value for attribute "%1$s" must be of type "%2$s", not type "%3$s".'**

- This message is displayed when the data type of a column and the type of the value you are trying to assign do not match in the CUBRID database; it can mainly appear when trying to assign a value of an incorrect type, due to a data type definition issue or a data type conversion failure.


.. _ERROR-205:

**ERROR CODE: -205, 'Attribute "%1$s" cannot be made NULL.'**

- This message is displayed when you attempt to assign a NULL value to a column on which a NOT NULL constraint is set in the CUBRID database; it can mainly appear due to incorrect data insertion, assigning a NULL value during an update, or a mismatch between the schema definition and the data.


.. _ERROR-208:

**ERROR CODE: -208, 'Method "%1$s" not found.'**

- This message is displayed when a method with the specified name cannot be found in the CUBRID database; it can mainly appear due to an incorrect method name reference, access to a non-existent method, or a schema definition error.


.. _ERROR-218:

**ERROR CODE: -218, 'Invalid object in db_get path expression.'**

- This message is displayed when an invalid object is found while processing the path expression of the `db_get` function in the CUBRID database.



.. _ERROR-219:

**ERROR CODE: -219, 'Invalid db_get path expression.'**

- This message is displayed when the path expression of the `db_get` function is invalid in the CUBRID database.



.. _ERROR-220:

**ERROR CODE: -220, 'Invalid set in db_get path expression.'**

- This message is displayed when, while processing the path expression of the `db_get` function in the CUBRID database, a set-related operation is attempted on an object that is not of a set type.


.. _ERROR-221:

**ERROR CODE: -221, 'Invalid set index in db_get path expression.'**

- This message is displayed when, while processing the path expression of the `db_get` function in the CUBRID database, an invalid index is used for a collection object of type set, multiset, or sequence.


.. _ERROR-222:

**ERROR CODE: -222, 'Attempt to assign string value greater than %2$d characters to attribute "%1$s".'**

- This message is displayed when, in the CUBRID database, you attempt to assign a string value to a specific column of an object and the length of the string to be assigned exceeds the maximum string length allowed by that column; it mainly occurs when inserting or updating data that exceeds the maximum length defined for a column of type `VARCHAR` or `CHAR`.


.. _ERROR-225:

**ERROR CODE: -225, 'Missing value for attribute "%1$s" with the NOT NULL constraint.'**

- This message is displayed when, in the CUBRID database, a value is not provided for a column with a `NOT NULL` constraint or when a `NULL` value is attempted to be assigned.


.. _ERROR-226:

**ERROR CODE: -226, 'Empty component list in class.'**

- This message is displayed when, while performing an operation on a table or object in the CUBRID database, the list of components (columns, attributes, etc.) that the table or object must have is empty or invalid; it can mainly occur in operations such as schema unload, query result processing, or retrieving specific object information.


.. _ERROR-227:

**ERROR CODE: -227, 'Attempt to assign value to attribute "%1$s" more than once.'**

- This message is displayed when, in the CUBRID database, you create an object using an object template and attempt to assign a value to the same column more than once.


.. _ERROR-228:

**ERROR CODE: -228, 'Method "%1$s" called with %2$d arguments. The system allows a maximum of %3$d arguments.'**

- This message is displayed when, in the CUBRID database, you call a method and pass more arguments than the maximum number of arguments allowed.


.. _ERROR-229:

**ERROR CODE: -229, 'Method "%1$s" requires argument %2$d to be within the domain "%3$s". An invalid value of domain "%4$s" was given.'**

- This message is displayed when, in the CUBRID database, the value of a specific argument falls outside the expected domain when calling a method; it can mainly appear when a value of an incorrect type is passed as a method argument, or when data type conversion (coercion) is not possible.


.. _ERROR-233:

**ERROR CODE: -233, 'Cannot create class with basic type name "%1$s".'**

- This message is a schema definition error that occurs when, in the CUBRID database, you attempt to use a basic type name as a table name when creating a table; CUBRID prohibits using primitive type names and reserved words as table names. This is to maintain consistency of the type system and prevent name collisions; it can appear when an invalid table name is used or when attempting to use a basic type or reserved word as a table name.


.. _ERROR-234:

**ERROR CODE: -234, 'Method file "%1$s" not found.'**

- This message is a schema definition error that occurs when, in the CUBRID database, a method file with a specific name cannot be found in the schema template; it can appear when an incorrect method file name is used, when the method file has already been deleted, or when the schema template has not been initialized correctly.


.. _ERROR-235:

**ERROR CODE: -235, '%1$d unresolved method functions after linking.'**

- This message is a schema definition error indicating that, even after the dynamic linking process is completed in the CUBRID database, there are still unresolved method functions (implementations cannot be found); it can appear when there is a problem with the method file, the function name is wrong, the required library has not been loaded, or a function referenced within the method file is missing.


.. _ERROR-236:

**ERROR CODE: -236, 'Unresolved method "%1$s".'**

- This message is a schema definition error that occurs when, in the CUBRID database, the implementation of a specific method (mainly a Java stored function/procedure) cannot be found during the dynamic linking process; it can appear when the method file is corrupted, the function name is wrong, or there is a problem with the linker settings.


.. _ERROR-239:

**ERROR CODE: -239, 'Attribute "%1$s" was not found.'**

- This message is a schema definition error that occurs when, in the CUBRID database, a column with a specific name cannot be found.


.. _ERROR-240:

**ERROR CODE: -240, 'Method "%1$s" was not found.'**

- This message is a schema definition error that occurs when, in the CUBRID database, a method with a specific name cannot be found.


.. _ERROR-241:

**ERROR CODE: -241, 'There is no attribute or method with name "%1$s".'**

- This message is a schema definition error that occurs when, in the CUBRID database, a column or method with a specific name cannot be found.


.. _ERROR-242:

**ERROR CODE: -242, 'Signature for %2$s not found for method "%1$s".'**

- This message is a schema definition error that occurs when, in the CUBRID database, the specified signature cannot be found for a specific method.


.. _ERROR-243:

**ERROR CODE: -243, 'Argument %2$d of method "%1$s" is undefined.'**

- This message is a schema definition error that occurs when, in the CUBRID database, the argument corresponding to the specified index of a specific method is not defined.


.. _ERROR-244:

**ERROR CODE: -244, 'Domain name "%1$s" is not the name of a class or basic type.'**

- This message is a schema definition error that occurs when, in the CUBRID database, the name specified as the data type of a column or method is not a valid table name or a basic data type.


.. _ERROR-245:

**ERROR CODE: -245, 'The name "%1$s" has already been used as an attribute name.'**

- This message is a schema definition error that occurs when, in the CUBRID database, you attempt to define the name of a new method, table method, or other schema component, but that name is already used as the name of an existing column.


.. _ERROR-246:

**ERROR CODE: -246, 'The name "%1$s" has already been used as a method name.'**

- This message is a schema definition error that occurs when, in the CUBRID database, you attempt to define the name of a new column, table column, or other schema component, but that name is already used as the name of an existing method.


.. _ERROR-248:

**ERROR CODE: -248, 'The attribute domain "%1$s" is not suitable for use with the %2$s constraint.'**

- This message is a schema definition error that occurs when, in the CUBRID database, you attempt to apply a UNIQUE constraint to a specific column but the data type is not suitable for a UNIQUE constraint; CUBRID restricts UNIQUE constraints to certain data types. Only indexable data types can have a UNIQUE constraint.


.. _ERROR-250:

**ERROR CODE: -250, 'Cannot create index on attribute "%1$s", defined with a shared value.'**

- This message is a schema definition error that occurs when, in the CUBRID database, you attempt to create an index on a column defined with a shared value (shared value).


.. _ERROR-251:

**ERROR CODE: -251, 'Signature %2$s already exists for method "%1$s".'**

- This message is a schema definition error that occurs when, in the CUBRID database, a definition with the same signature already exists for a specific method.


.. _ERROR-252:

**ERROR CODE: -252, 'Attribute "%1$s" must have a set domain for that operation.'**

- This message is a schema definition error that occurs when, in the CUBRID database, you attempt to perform an operation that requires a SET type (collection types such as SET, MULTISET) on a specific column, but the data type of that column is not a SET type; it can occur mainly when defining or modifying a column using `CREATE TABLE` or `ALTER TABLE`, or when a specific method or function requires a SET-type column as an argument.


.. _ERROR-253:

**ERROR CODE: -253, 'Attribute "%1$s" cannot be defined with a nested set domain.'**

- This message is a schema definition error that occurs when, in the CUBRID database, you attempt to define a column as a nested SET data type; CUBRID does not allow nesting another SET type inside a SET-type collection (e.g., SET, MULTISET); it mainly occurs when defining or modifying a column using `CREATE TABLE` or `ALTER TABLE`.


.. _ERROR-254:

**ERROR CODE: -254, 'Unable to remove domain "%2$s" from attribute "%1$s".'**

- This message is displayed when, in the CUBRID database, you attempt to remove a specific data type; it mainly occurs when trying to remove a column's data type using a `DROP TABLE` statement, in a situation where the data type cannot be removed from the column (e.g., the data type does not exist, is referenced by another constraint, or is the column's basic data type).


.. _ERROR-260:

**ERROR CODE: -260, 'Argument %2$d of method "%1$s" is not of a set domain.'**

- This message is a schema definition error that occurs when, in the CUBRID database, the domain of a specific argument is not a set type when defining a method; in CUBRID, set types mean collection types such as set and multiset; it can occur mainly when defining or modifying a method using `CREATE TABLE` or `ALTER TABLE`.


.. _ERROR-269:

**ERROR CODE: -269, 'Cannot define index on domain "%1$s".'**

- This message is a schema definition error that occurs when, in the CUBRID database, you attempt to create an index on a specific data type; CUBRID does not allow index creation for all data types, and the data types on which index creation is not allowed (such as "SET", "MULTISET", "BLOB", "CLOB", etc.) are restricted.


.. _ERROR-272:

**ERROR CODE: -272, 'Index "%2$s" already defined for class "%1$s".'**

- This message indicates a schema management error that occurs when, in the CUBRID database, you attempt to create an index with the same name redundantly; it occurs when you try to create a new index with the same name as an index that already exists on a specific table.


.. _ERROR-273:

**ERROR CODE: -273, 'Index "%1$s" does not exist.'**

- This message indicates a schema management error that occurs when, in the CUBRID database, a specific index cannot be found; it can mainly occur during index DDL operations such as `DROP INDEX`, `ALTER INDEX`, or query execution that uses an index.


.. _ERROR-274:

**ERROR CODE: -274, 'Illegal characters in name: "%1$s".'**

- This message occurs when, in the CUBRID database, when defining or changing the name of a schema object (table, column, method, index, etc.), the name is NULL or an empty string; it can mainly occur when executing DDL (Data Definition Language) statements such as `CREATE TABLE`, `ALTER TABLE`, and `CREATE VIEW`.


.. _ERROR-293:

**ERROR CODE: -293, 'Operation not allowed for this class type.'**

- This message is displayed when, in the CUBRID database, you attempt an operation that is not allowed for a specific table type; it can occur mainly in the process of schema definition, table alteration, or performing certain object-related operations, and indicates that you must understand the characteristics of the table and perform allowed operations.


.. _ERROR-294:

**ERROR CODE: -294, 'Undefined environment variable "%1$s" referenced in method file.'**

- This message is displayed when, in the CUBRID database, an environment variable included in the path of a method file is not defined; a method file is an external library file that implements user-defined methods (functions) in CUBRID, and this message mainly indicates a problem that occurs because the method file cannot be found during the dynamic linking process.


.. _ERROR-305:

**ERROR CODE: -305, 'Sequence index %1$d is out of bounds.'**

- This message is an error that appears when, in the CUBRID database, the index of a sequence collection is outside the valid range; it occurs during the index range validation process in the sequence object management system, mainly when accessing a sequence element with an index less than 0 or greater than the sequence size.


.. _ERROR-308:

**ERROR CODE: -308, 'Element value is not within the domain of the set.'**

- This message is an error that appears when, in the CUBRID database, when adding an element to a set-type collection, the value of that element is not compatible with the domain of the set; it mainly occurs when the element value violates the domain rules of the set.


.. _ERROR-309:

**ERROR CODE: -309, 'Illegal set element index given: %1$d.'**

- This message is an error that appears when, in the CUBRID database, an invalid index is passed to a set-type collection; it mainly occurs when a negative index or an index exceeding the collection size is passed.


.. _ERROR-311:

**ERROR CODE: -311, 'Element not found in sequence.'**

- This message is an error that appears when, in the CUBRID database, a specific element cannot be found in a sequence (an ordered collection); a sequence is an ordered data structure, and this message mainly occurs when the value does not exist in the sequence during an element search.


.. _ERROR-312:

**ERROR CODE: -312, '"%1$s" is not a valid set domain.'**

- This message is an error that appears when, in the CUBRID database, a set type is invalid; it can mainly occur when an incorrect set type is passed or when it is NULL.


.. _ERROR-424:

**ERROR CODE: -424, 'No statement to execute.'**

- This message is an error that appears when there is no SQL statement to execute in the CUBRID database; it occurs when there is no statement in the session or the statement array is empty during the statement validation process.


.. _ERROR-427:

**ERROR CODE: -427, 'Data overflow on data type "%1$s".'**

- This message is an error that appears when overflow occurs for a specific data type in the CUBRID database; it occurs when, during the data type conversion process, the data value exceeds the allowed range of that data type.


.. _ERROR-428:

**ERROR CODE: -428, 'Statement is not updatable.'**

- This message is an error that appears when, in the CUBRID database, you attempt to modify a statement that cannot be updated; it occurs when trying to modify a non-updatable statement on a view table during the statement updatability check process.


.. _ERROR-435:

**ERROR CODE: -435, 'Multiple statements not allowed.'**

- This message is an error that appears when, in the CUBRID database, multiple SQL statements are provided in a context where multiple statements cannot be executed or processed at the same time; it occurs when multiple statements separated by a semicolon, etc., are entered in an API call or execution environment where only a single statement is allowed during SQL statement processing.


.. _ERROR-454:

**ERROR CODE: -454, 'Invalid data type referenced.'**

- This message is an error that appears when, in the CUBRID database, a non-existent or invalid data type is referenced; it occurs during the data type validation process in a query, that is, when an undefined data type or an invalid data type identifier is used.


.. _ERROR-457:

**ERROR CODE: -457, 'Invalid query result type.'**

- This message is an error that appears when, in the CUBRID database, the type of the query result structure is invalid; it occurs during the query result type validation process, that is, when an unsupported query result type is used.


.. _ERROR-458:

**ERROR CODE: -458, 'Overflow occurred in addition context.'**

- This message is an error that appears when overflow occurs while performing addition among arithmetic operations in the CUBRID database; it occurs during arithmetic operation processing in a query, that is, when an addition operation is performed that exceeds the maximum value of the data type.


.. _ERROR-459:

**ERROR CODE: -459, 'Query result contains more than a single tuple.'**

- This message is an error that appears when a query that must return a single tuple (row) returns multiple tuples in the CUBRID database; it occurs during the single-tuple validation process in a query, that is, when exactly one result must be returned in a subquery or certain operation but multiple results are returned.


.. _ERROR-482:

**ERROR CODE: -482, '"%1$s" is not a virtual class.'**

- This message is an error that appears when, in the CUBRID database, you attempt to use an object that is not a 'virtual class' (virtual table, view table) as a virtual table; a 'virtual class' is a table that does not store actual data but shows query results, such as a virtual table or view table; this message occurs when performing a virtual-table-related operation and the object is not a virtual table.


.. _ERROR-486:

**ERROR CODE: -486, 'Object is not an updatable virtual class instance.'**

- This message is an error that appears when, in the CUBRID database, an instance of a virtual class (virtual table) is not updatable; it occurs when attempting to perform a modification operation on a non-updatable virtual class instance (record).


.. _ERROR-492:

**ERROR CODE: -492, '%1$s'**

- This message is an error that appears when a general error occurs in the parser in the CUBRID database; it is an error used to display the line and column when an error occurs during query parsing.


.. _ERROR-493:

**ERROR CODE: -493, 'Syntax: %1$s %2$s'**

- This message is an error that appears when a syntax error occurs in the parser in the CUBRID database; it is an error that occurs at the parser's syntactic analysis stage, and it occurs when a problem arises during the grammatical analysis of an SQL query.


.. _ERROR-494:

**ERROR CODE: -494, 'Semantic: %1$s %2$s'**

- This message is an error that appears when a semantic error occurs in the parser in the CUBRID database; it is an error that occurs at the parser's semantic analysis stage and is used when a problem arises during the meaning analysis of a query; it mainly occurs in situations such as semantic errors in a query or type mismatches.


.. _ERROR-495:

**ERROR CODE: -495, 'Execute: %1$s %2$s'**

- This message is an error that appears when an execution error occurs in the parser in the CUBRID database; it is an error that occurs at the parser's execution stage, and it occurs when processing cannot be performed as XASL; it mainly occurs in situations such as query execution failure or an internal parser error.


.. _ERROR-501:

**ERROR CODE: -501, 'Invalid trigger priority.'**

- This message is an error that appears when the trigger priority is invalid in the CUBRID database; it occurs during the process of validating a trigger definition.


.. _ERROR-502:

**ERROR CODE: -502, 'Missing target class in trigger.'**

- This message is an error that appears when, when defining a trigger in the CUBRID database, the target table is not specified; since a trigger is defined to respond to events (INSERT, UPDATE, DELETE) on a specific table, the target table must be specified; it occurs during the process of validating a trigger definition.


.. _ERROR-503:

**ERROR CODE: -503, 'Trigger "%1$s" was not found.'**

- This message is an error that appears when a trigger with the specified name cannot be found in the CUBRID database; it occurs when the trigger does not exist, or when the user does not have permission to access the trigger; it occurs during the trigger lookup and access process.


.. _ERROR-505:

**ERROR CODE: -505, 'A trigger with the name "%1$s" already exists.'**

- This message is an error that appears when a trigger with the same name already exists in the CUBRID database; because trigger names must be globally unique, you cannot create a trigger with a duplicate name or change it to a name that already exists; it occurs during the process of validating a trigger definition.


.. _ERROR-506:

**ERROR CODE: -506, 'A trigger cannot be defined on "%1$s" because it is a virtual class.'**

- This message is an error that appears when attempting to define a trigger on a virtual class (virtual table) in the CUBRID database; because a virtual table is a virtual table like a view and does not store actual data, you cannot define a trigger on it; it occurs during the process of validating a trigger definition.


.. _ERROR-507:

**ERROR CODE: -507, 'Inappropriate trigger target class "%1$s".'**

- This message is an error that appears when the target table of a trigger is inappropriate in the CUBRID database; it occurs when a trigger targets a specific table but that table does not exist or is not appropriate; it occurs during the process of validating a trigger definition.


.. _ERROR-508:

**ERROR CODE: -508, 'Inappropriate trigger target attribute "%1$s".'**

- This message is an error that appears when the target column of a trigger is inappropriate in the CUBRID database; it occurs when a trigger targets a specific column but that column does not exist or is not appropriate; it occurs during the process of validating a trigger definition.


.. _ERROR-511:

**ERROR CODE: -511, 'Not authorized to access trigger "%1$s".'**

- This message is an error that appears when the current user does not have permission to access (read/view) a specific trigger in the CUBRID database; it occurs when attempting to view or reference trigger information and there is insufficient privilege.


.. _ERROR-512:

**ERROR CODE: -512, 'Not authorized to delete trigger "%1$s".'**

- This message is an error that appears when the current user does not have permission to delete a specific trigger in the CUBRID database; it occurs when attempting to remove a trigger completely and there is insufficient privilege.


.. _ERROR-514:

**ERROR CODE: -514, 'Not authorized to alter trigger "%1$s".'**

- This message is an error that appears when the current user does not have permission to alter a specific trigger in the CUBRID database; it occurs when attempting operations such as renaming the trigger, changing its status, changing its priority, or changing its comment and there is insufficient privilege.


.. _ERROR-515:

**ERROR CODE: -515, 'Trigger action execution time of %2$s cannot be earlier than condition execution time of %1$s.'**

- This message is an error that appears when you violate the rule that a trigger action execution time cannot be earlier than the condition execution time in the CUBRID database; it occurs when the trigger condition must be executed before the action, but the action is configured to execute before the condition; this is a safety device to guarantee the logical execution order of triggers.


.. _ERROR-516:

**ERROR CODE: -516, 'Maximum trigger depth %1$d exceeded at trigger "%2$s".'**

- This message is an error that appears when the recursive call depth of a trigger exceeds the maximum allowed depth in the CUBRID database; it occurs when recursion repeats infinitely or exceeds the maximum depth in the process where a trigger calls another trigger and that trigger calls yet another trigger; this is a safety device to prevent an infinite loop or stack overflow due to recursive trigger calls.


.. _ERROR-517:

**ERROR CODE: -517, 'he operation has been rejected by trigger "%1$s".'**

- This message is an error that appears when a specific operation (e.g., INSERT, UPDATE, DELETE) is rejected by the `REJECT` action of a trigger in the CUBRID database; the `REJECT` action rolls back the current transaction and raises an error to stop the operation when the trigger is executed; it means that the trigger explicitly blocked the data modification operation according to the defined condition.


.. _ERROR-518:

**ERROR CODE: -518, 'Internal error: processing trigger "%1$s".'**

- This message is an error that appears when an internal error occurs while processing a trigger in the CUBRID database; it occurs when a parser error occurred during trigger compilation but detailed error information cannot be obtained; it indicates an unexpected internal state error in CUBRID's trigger system.


.. _ERROR-519:

**ERROR CODE: -519, 'Illegal trigger condition type.'**

- This message is an error that appears when the trigger condition type is invalid in the CUBRID database; it indicates the constraint that a CUBRID trigger condition must be an appropriate expression.


.. _ERROR-520:

**ERROR CODE: -520, 'The REJECT action cannot be used with an action time of AFTER or DEFERRED.'**

- This message is an error that appears when, when creating a trigger in the CUBRID database, you attempt to use the REJECT action together with an action time of AFTER or DEFERRED; the REJECT action is an action that rejects the transaction when the trigger is executed, and it is not compatible with AFTER or DEFERRED action times; it indicates the constraint that certain combinations of actions and action times are not allowed in CUBRID's trigger system.


.. _ERROR-521:

**ERROR CODE: -521, 'The REJECT action cannot be used with the ABORT or TIMEOUT events.'**

- This message is an error that appears when, when creating a trigger in the CUBRID database, you attempt to use the REJECT action together with the ABORT or TIMEOUT events; the REJECT action is an action that rejects the transaction when the trigger is executed, and it is not compatible with ABORT or TIMEOUT events; it indicates the constraint that certain combinations of actions and events are not allowed in CUBRID's trigger system.


.. _ERROR-523:

**ERROR CODE: -523, 'Trigger action expression was not given.'**

- This message is an error that appears when, when creating a trigger in the CUBRID database, the action type is set to `EXPRESSION` but the action source string (`action_source`) is not provided; a trigger action is the part that defines the work to be performed when the trigger is executed, and when the action type is an expression (`EXPRESSION`), an action source string must be provided; this message occurs when a required parameter is missing during trigger creation.


.. _ERROR-524:

**ERROR CODE: -524, 'The specified activity cannot be removed because it is owned by another user.'**

- This message is an error indicating that, when attempting to remove a trigger's deferred activity in the CUBRID database, the activity cannot be removed because it is owned by another user rather than the current user; trigger activity represents work performed when a trigger is executed, and deferred activity is an activity managed in a deferred trigger execution state; it occurs due to a privilege issue, and for security reasons removal of an activity owned by another user is restricted.


.. _ERROR-525:

**ERROR CODE: -525, 'Error compiling condition for "%1$s": %2$s'**

- This message is an error that appears when an error occurs while compiling a trigger's condition in the CUBRID database; it mainly appears when a syntax error or compilation error occurs while parsing and compiling the SQL statement or script defined in the condition part of the trigger.


.. _ERROR-526:

**ERROR CODE: -526, 'Error compiling action for "%1$s", %2$s'**

- This message is an error that appears when an error occurs while compiling a trigger's action in the CUBRID database; it mainly appears when a syntax error or compilation error occurs while parsing and compiling the SQL statement or script defined in the action part of the trigger.


.. _ERROR-527:

**ERROR CODE: -527, 'Error evaluating condition for "%1$s": %2$s'**

- This message is an error that appears when an error occurs while evaluating (executing) a trigger's condition in the CUBRID database; it mainly appears when a syntax error or execution error occurs while executing the SQL statement or script in the condition part of the trigger.


.. _ERROR-528:

**ERROR CODE: -528, 'Error evaluating action for "%1$s", %2$s'**

- This message is an error that appears when an error occurs while evaluating (executing) a trigger action in the CUBRID database, it usually appears when a syntax error or an execution error occurs while executing an SQL statement or a script in the action part of the trigger.


.. _ERROR-529:

**ERROR CODE: -529, 'Transaction cannot be committed, it was invalidated by trigger "%1$s".'**

- This message is an error that appears when you try to commit a transaction in the CUBRID database, but the transaction has been invalidated by a trigger and therefore cannot be committed, it usually occurs when the trigger's action is set to `INVALIDATE`.


.. _ERROR-539:

**ERROR CODE: -539, 'Attempt to divide by zero.'**

- This message is an error that appears when attempting a division operation by zero in the CUBRID database, it usually occurs when the divisor in a division operation is 0 during query processing, since it is a mathematically undefined operation, CUBRID treats it as an error.


.. _ERROR-552:

**ERROR CODE: -552, 'Date arithmetic underflow.'**

- This message is an error that appears when an underflow occurs while performing date arithmetic or functions in the CUBRID database, this usually occurs when subtracting or otherwise operating on a date value causes the result to go out of the date range, it is an error that occurs when the date arithmetic result becomes smaller than the minimum date value.


.. _ERROR-553:

**ERROR CODE: -553, 'Time arithmetic underflow.'**

- This message is an error that appears when an underflow occurs while performing time arithmetic or functions in the CUBRID database, this usually occurs when subtracting or otherwise operating on a time value causes the result to go out of the time range, it is an error that occurs when the time arithmetic result becomes smaller than the minimum time value.


.. _ERROR-618:

**ERROR CODE: -618, 'Attribute is not updatable.'**

- This message appears when you try to update a specific column in the CUBRID database but the column is of a type that is not updatable, it can occur when you directly update certain columns of a view table or columns managed by the system.


.. _ERROR-621:

**ERROR CODE: -621, 'Invalid data type. The arguments contain an invalid data type.'**

- This message appears when the data type of an argument passed to a function or an operation in the CUBRID database is invalid, for example, it can occur when you pass a numeric type to a string function, or when you pass a string type to a numeric operation.


.. _ERROR-622:

**ERROR CODE: -622, 'Incompatible code sets. The code sets of the arguments are incompatible.'**

- This message appears when the character sets (character set) of the strings passed as arguments are incompatible with each other during string comparison or operations in the CUBRID database, for example, it can occur when you try to directly compare or operate on a UTF-8 string and an EUC-KR string.


.. _ERROR-623:

**ERROR CODE: -623, 'Invalid escape sequence. An invalid escape sequence has caused an error in the regular expression compiler.'**

- This message appears when you use an invalid escape sequence in the ESCAPE clause of the LIKE operator in the CUBRID database, it occurs when the sequence used to escape special characters in the LIKE operator of an SQL statement is not valid.


.. _ERROR-624:

**ERROR CODE: -624, 'Invalid escape character. The escape character is non-NULL with a length greater than 1.'**

- This message appears when you use an invalid escape character in the ESCAPE clause of the LIKE operator in the CUBRID database.


.. _ERROR-628:

**ERROR CODE: -628, 'Invalid currency type: %1$d.'**

- This message is displayed when processing the monetary data type in the CUBRID database, it occurs when an unsupported currency type is specified.


.. _ERROR-631:

**ERROR CODE: -631, 'SQL statement violated NOT NULL constraint.'**

- This message is displayed when an SQL statement violates a NOT NULL constraint in the CUBRID database, it appears when a NOT NULL constraint is set on a table column but an INSERT or UPDATE statement attempts to input a NULL value into that column.



.. _ERROR-649:

**ERROR CODE: -649, 'Given precision of %1$d is invalid; it should be greater than %2$d and no greater than %3$d.'**

- This message is displayed when the precision value of a data type is invalid in the CUBRID database, it occurs when the specified precision value is outside the range allowed by the data type, it means a validation failure that occurs when defining a data type with a precision value that is too small or too large.


.. _ERROR-670:

**ERROR CODE: -670, 'Operation would have caused one or more unique constraint violations. INDEX %1$s%2$s ON CLASS %3$s%4$s. key: %5$s%6$s.'**

- This message occurs when an operation that violates a UNIQUE constraint is attempted in the CUBRID database, it indicates a situation where a duplicated key value is found in a B+tree index and thus violates the UNIQUE constraint, it means that a UNIQUE constraint that does not allow duplicate values in order to guarantee data integrity has been violated.


.. _ERROR-683:

**ERROR CODE: -683, 'Attempted to create string with illegal length %1$d'**

- This message is displayed when you try to create a string in the CUBRID database but the specified length is not valid, the string length cannot exceed the maximum length defined in CUBRID and cannot be negative, it mainly occurs in the process of converting LOB (Large Object) data to a string in the clob_to_char() function.


.. _ERROR-685:

**ERROR CODE: -685, 'Class "%1$s" is invalid.'**

- This message is displayed when the specified table is invalid while changing the owner in the CUBRID database.


.. _ERROR-704:

**ERROR CODE: -704, 'Evaluation of generic server function failed.'**

- This message occurs when evaluating (evaluation) a Java procedure fails in the CUBRID database,


.. _ERROR-709:

**ERROR CODE: -709, 'Constraint "%1$s" not found.'**

- This message is an error that appears when a constraint with a specific name cannot be found in the CUBRID database, CUBRID checks whether the constraint exists when dropping or modifying a constraint, this message occurs in the process of dropping or modifying a constraint in the CUBRID database


.. _ERROR-710:

**ERROR CODE: -710, 'Invalid constraint.'**

- This message is an error that appears when a constraint is invalid in the CUBRID database, CUBRID validates whether a constraint is valid when creating or managing constraints, this message can occur in the process of creating or managing constraints in the CUBRID database.


.. _ERROR-711:

**ERROR CODE: -711, 'An object with the supplied attribute value doesn't exist.'**

- This message is an error that appears when an object with a specific column value cannot be found in the CUBRID database, CUBRID raises this error when no object exists that satisfies the condition during object search or query execution.


.. _ERROR-712:

**ERROR CODE: -712, 'Constraint" %1$s" already exists.'**

- This message is an error that appears when a constraint with the same name already exists in the CUBRID database, CUBRID does not allow duplicate names when adding a constraint to a table or a table.


.. _ERROR-713:

**ERROR CODE: -713, 'An index does not exist on this attribute.'**

- This message is an error that appears when an index does not exist for a specific column in the CUBRID database, it can occur during object search or index-related operations in the CUBRID database.


.. _ERROR-716:

**ERROR CODE: -716, 'NOT NULL constraints are not allowed in virtual or proxy classes.'**

- This message is an error that appears when you try to add a NOT NULL constraint to a virtual class (virtual class) or a view table in the CUBRID database.


.. _ERROR-717:

**ERROR CODE: -717, 'UNIQUE constraints are not allowed in virtual or proxy classes.'**

- This message is an error that appears when you try to add a UNIQUE constraint to a virtual class (virtual class) or a view table in the CUBRID database.


.. _ERROR-721:

**ERROR CODE: -721, 'Insert query yields more than one row.'**

- This message is an error that appears when executing an INSERT statement in the CUBRID database and the result returns multiple rows rather than a single row, an INSERT statement generally must insert only one row, but this error occurs if a subquery or the VALUES clause returns multiple rows, this message mainly occurs when the VALUES clause of the INSERT statement has multiple rows, or when a subquery returns multiple rows.


.. _ERROR-722:

**ERROR CODE: -722, 'Wrong number of attributes for NOT NULL constraint.'**

- This message is an error that appears when the number of columns is incorrect when creating or dropping a NOT NULL constraint in the CUBRID database, a NOT NULL constraint is a constraint that can be applied to only one column, this message occurs when you try to apply a NOT NULL constraint to multiple columns at the same time, or when no column is specified.


.. _ERROR-723:

**ERROR CODE: -723, 'Cannot modify the domain of an existing attribute.'**

- This message is an error that appears when you attempt to change the data type of an existing column in the CUBRID database.


.. _ERROR-724:

**ERROR CODE: -724, 'Invalid parameter(s) for default constraint name generation.'**

- This message can occur when dropping an index in the CUBRID database and the index name is NULL.


.. _ERROR-727:

**ERROR CODE: -727, 'Invalid trigger event.'**

- This message is an error that appears when an invalid trigger event is used in the process of creating or managing a trigger in the CUBRID database, a trigger event is a condition that determines when a trigger is executed and is associated with database operations such as INSERT, UPDATE, and DELETE, this message occurs when an invalid event type is specified for a trigger.


.. _ERROR-729:

**ERROR CODE: -729, 'Overflow occurred in subtraction context.'**

- This message is an error that appears when an overflow occurs while performing subtraction in the CUBRID database, an overflow occurs when the operation result exceeds the maximum range that the data type can represent, this mainly occurs when the difference between two numbers exceeds the maximum value of integer (INT, BIGINT, SHORT) or floating-point (FLOAT, DOUBLE) types, it can also occur when the subtraction part in a compound expression causes an overflow.


.. _ERROR-730:

**ERROR CODE: -730, 'Overflow occurred in multiplication context.'**

- This message is an error that appears when an overflow occurs while performing multiplication in the CUBRID database, an overflow occurs when the operation result exceeds the maximum range that the data type can represent, this mainly occurs when the product of two numbers exceeds the maximum value of integer (INT, BIGINT, SHORT) or floating-point (FLOAT, DOUBLE) types, it can also occur when the result in multiplication of the NUMERIC type exceeds the representable range.


.. _ERROR-731:

**ERROR CODE: -731, 'Overflow occurred in division context.'**

- This message is an error that appears when an overflow occurs while performing division in the CUBRID database, an overflow occurs when the operation result exceeds the maximum range that the data type can represent, this can occur when a very large value is produced in floating-point division, or when the result exceeds the range under certain conditions in integer division (e.g., INT_MIN / -1), it can also occur when the result in division of the NUMERIC type exceeds the representable range.


.. _ERROR-732:

**ERROR CODE: -732, 'Overflow occurred in unary minus context.'**

- This message is an error that appears when an overflow occurs while performing a unary minus operation in the CUBRID database, unary minus is an operation that changes the sign of a numeric value, converting a positive number to a negative number and a negative number to a positive number, this message occurs when applying unary minus to the minimum value of a certain data type, for example, applying unary minus to the minimum value of the INT type (-2,147,483,648) yields a positive value (2,147,483,648) that exceeds the INT type range.


.. _ERROR-751:

**ERROR CODE: -751, 'The OID of a new object isn't available in a BEFORE INSERT trigger.'**

- This message occurs when you define a BEFORE INSERT trigger in the CUBRID database and try to directly reference the new object inside the trigger, since a BEFORE INSERT trigger runs before the data is actually inserted into the table, the OID of the new object has not yet been assigned, referencing the "new" object itself inside the trigger is not allowed, and you can reference only individual attributes in the form "new.attribute_name".


.. _ERROR-772:

**ERROR CODE: -772, 'db_serial class not found.'**

- This message indicates a situation where the db_serial catalog table does not exist when the CUBRID database tries to find the db_serial catalog table, this occurs in CUBRID's serial object management mechanism, and it can occur mainly when the db_serial table is not registered in the system.


.. _ERROR-773:

**ERROR CODE: -773, 'Serial "%1$s" not found.'**

- This message indicates a situation where the serial object with the specified name does not exist when you try to query a serial object in the CUBRID database.


.. _ERROR-774:

**ERROR CODE: -774, 'Serial "%1$s" already exist.'**

- This message indicates a situation where a serial object with the same name already exists when you try to create a serial object in the CUBRID database.


.. _ERROR-775:

**ERROR CODE: -775, 'Next value exceeds MAX(MIN) value.'**

- This message indicates a situation where the calculated next value (nextval) of a serial object exceeds the maximum value or the minimum value when calculating it in the CUBRID database.


.. _ERROR-776:

**ERROR CODE: -776, 'Cannot fetch serial object.'**

- This message indicates a situation where a serial object cannot be read when you try to fetch a serial object in the CUBRID database.


.. _ERROR-777:

**ERROR CODE: -777, 'Cannot update serial object.'**

- This message indicates a situation where a serial object cannot be modified when you try to update a serial object in the CUBRID database.


.. _ERROR-779:

**ERROR CODE: -779, 'Result exceeds limit: year of Date MUST be in 1 and 9999.'**

- This message occurs when using date-related functions in the CUBRID database and the year of the resulting date goes out of the range from year 1 to year 9999, this occurs in CUBRID's date processing mechanism, and it can occur mainly when a date arithmetic result exceeds the valid date range.


.. _ERROR-781:

**ERROR CODE: -781, 'Format string is too long. It must be less than 16K.'**

- This message occurs when using a string format conversion function in the CUBRID database and the length of the format string exceeds 16K (16,000 bytes), this occurs in CUBRID's string format conversion mechanism, and it can occur mainly when the format string length limit is exceeded.


.. _ERROR-782:

**ERROR CODE: -782, 'Empty string not allowed here.'**

- This message occurs when an empty string ('') is used in a string operation or function where an empty string is not allowed, it is a data validation error that occurs when meaningful input is required in certain string functions or operations.


.. _ERROR-783:

**ERROR CODE: -783, 'Invalid format.'**

- This message occurs when the string format or data format does not match the expected format. It is a format validation error that occurs when an invalid format is input in date/time formats, numeric formats, or string functions that require a specific pattern.


.. _ERROR-784:

**ERROR CODE: -784, 'Two arguments are mismatched.'**

- This message occurs when two arguments do not match in the string processing module of the CUBRID database, that is, it is an error message that occurs when the source string and the format string do not match while performing date conversion, this message mainly occurs when numeric parsing fails, which means that the format of the source string does not match the expected format.


.. _ERROR-785:

**ERROR CODE: -785, 'First argument string is too long. It must be less than 16K.'**

- This message occurs when calling a date/time conversion function in the string processing module of the CUBRID database and the length of the string passed as the first argument exceeds 16KB (16000 bytes), that is, if it exceeds this limit in the logic that checks the length of the original string, the error is set, this limit is set to prevent memory overflow or performance degradation that may occur when the database system processes strings.


.. _ERROR-786:

**ERROR CODE: -786, 'Format is duplicated.'**

- This message occurs when calling a date/time conversion function in the string processing module of the CUBRID database and the same format specifier is used redundantly in the format string, that is, during parsing of the date/time format string, if a format specifier (e.g., `YYYY`, `MM`, `DD`, etc.) that has already been used once appears again, the error is set, this is CUBRID's internal validation mechanism to prevent syntactic errors in date/time format strings.


.. _ERROR-787:

**ERROR CODE: -787, 'Conversion error in time format.'**

- This message is displayed in the time processing module of the CUBRID database during time format conversion, that is, the error is set when a problem occurs during converting time data to another format, converting time zones, or validating time values, this message can occur in multiple modules, and it mainly occurs during the validation process to ensure the integrity and consistency of time data.


.. _ERROR-788:

**ERROR CODE: -788, 'Conversion error in timestamp format.'**

- This message is displayed in the timestamp processing module of the CUBRID database during timestamp format conversion, that is, the error is set when a problem occurs during converting timestamp data to another format, parsing a date/time string into a timestamp, or validating timestamp values, this message can occur in multiple modules, and it mainly occurs during the validation process to ensure the integrity and consistency of timestamp data.


.. _ERROR-791:

**ERROR CODE: -791, 'Invalid serial value.'**

- This message is displayed when validation of a serial value fails in the serial object processing module of the CUBRID database, that is, the error is set when required attribute values are NULL or are not of the correct data type during validation of serial object property values, it occurs during the validation process to ensure the integrity of serial objects, and it mainly appears during schema export or SERIAL object management.


.. _ERROR-833:

**ERROR CODE: -833, 'The index name "%1$s" is ambiguous. Specify the class name of the index.'**

- This message occurs when the schema management module of the CUBRID database tries to find a table by index name, that is, when trying to find the table that owns an index by a specific index name and type, if indexes with the same name and type exist on multiple tables, it occurs when CUBRID cannot determine which table to return, it occurs when multiple tables use the same index name in database design or when the table name is not specified when referencing an index. Because CUBRID cannot distinguish which table's index it is by index name alone, it requires specifying the table name.


.. _ERROR-834:

**ERROR CODE: -834, 'There are some mismatches between source string "%1$s" and format string "%2$s" given in to_number().'**

- This message occurs in the query processing module of the CUBRID database during conversion of a string to a number using the `to_number()` function, that is, when attempting the conversion by comparing the source string and the format string, it occurs when the structures or contents of the two strings are not compatible and therefore conversion rules cannot be followed, it mainly occurs when the `to_number()` function is used incorrectly in an SQL query or when the input data format differs from what is expected. For example, it occurs when the string that must be converted to a numeric format contains non-numeric characters, or when the specified format string does not match the actual data format.


.. _ERROR-865:

**ERROR CODE: -865, 'Cannot execute statement due to unknown type input markers.'**

- This message occurs during execution of an SQL statement in the CUBRID database, it usually appears when the type of a host variable cannot be determined or when type conversion is not possible.


.. _ERROR-874:

**ERROR CODE: -874, 'Argument of power() is out of range.'**

- This message occurs during exponentiation (power) operations in the CUBRID database, specifically it occurs when using a non-integer exponent with a negative base. Since it is a mathematically undefined operation, CUBRID treats it as an error.


.. _ERROR-875:

**ERROR CODE: -875, 'Overflow happened in power context.'**

- This message occurs during exponentiation (power) operations in the CUBRID database, this situation generally occurs when performing exponentiation on very large numbers, and it appears when the operation is mathematically valid but exceeds the floating-point representation range of the computer.


.. _ERROR-886:

**ERROR CODE: -886, '"%1$s" caused unique constraint violation.'**

- This message occurs when you try to insert (INSERT) or update (UPDATE) data that violates a unique (Unique) constraint defined on a table in the CUBRID database, it occurs when you insert the same value as an already existing value, or when you update an existing value to create a duplicate value with another record.


.. _ERROR-887:

**ERROR CODE: -887, 'Stored procedure '%1$s' is already exist.'**

- This message occurs when you try to create a stored procedure in the CUBRID database and a stored procedure with the same name already exists, that is, when executing a `CREATE PROCEDURE` statement, the error is set if the stored procedure name is duplicated, this occurs in CUBRID's stored procedure management mechanism, and it can occur mainly due to a duplicate stored procedure name.


.. _ERROR-888:

**ERROR CODE: -888, 'Parameter count is invalid. expected: %1$d, actual: %2$d'**

- This message occurs when calling a stored procedure in the CUBRID database and the number of parameters passed does not match the number of parameters expected by the stored procedure definition, that is, the error is set when the parameter count does not match during a stored procedure call, this occurs in CUBRID's stored procedure call mechanism, and it can occur mainly due to a parameter count mismatch.


.. _ERROR-889:

**ERROR CODE: -889, 'Stored procedure execute error: %1$s'**

- This message is a general execution error that occurs during execution of a stored procedure (Stored Procedure) in the CUBRID database, that is, it indicates that an error occurred for various reasons during execution of the stored procedure, this situation generally occurs due to failure of SQL statement execution inside the stored procedure, data type mismatch, insufficient privileges, insufficient memory, or other runtime errors.


.. _ERROR-890:

**ERROR CODE: -890, 'Partition failed.'**

- This message occurs when a partitioning-related operation such as creating, altering, or dropping a table partition in the CUBRID database fails due to internal errors, parameter errors, data inconsistencies, lack of resources, constraint violations, and so on, it can appear due to various causes such as an incorrect partition definition, data not matching partition conditions, lack of system resources, internal bugs, or schema constraint violations.


.. _ERROR-891:

**ERROR CODE: -891, 'Appropriate partition does not exist.'**

- This message occurs when performing a table partition operation (e.g., data insertion, query, move, exploration during partition changes, etc.) in the CUBRID database and a partition corresponding to the specified condition or value is not defined, has already been dropped, or cannot be accessed due to an incorrect partition definition, it can appear due to causes such as a partition key value not being included in the partition range, an incorrect partition name/ID, a partition being dropped/changed, or internal metadata inconsistencies.


.. _ERROR-892:

**ERROR CODE: -892, 'Primary key "%2$s" already defined for class "%1$s".'**

- This message occurs when you try to add or define a primary key on a table in the CUBRID database and a primary key already exists on that table, that is, when you execute a DDL (Data Definition Language) statement such as `CREATE TABLE` or `ALTER TABLE ADD PRIMARY KEY` to define a primary key, the error is set if it already exists in the primary key duplication check, this occurs in CUBRID's schema management mechanism, and it can occur mainly due to duplicate primary key definitions.


.. _ERROR-893:

**ERROR CODE: -893, 'Attribute '%1$s', a member of the primary key, could not be dropped.'**

- This message occurs when you try to drop an attribute that is part of a primary key in the CUBRID database, that is, when executing a DDL (Data Definition Language) statement to drop an attribute, the error is set if that attribute is a member of the primary key. It occurs in CUBRID's schema management mechanism, and it can occur mainly due to primary key integrity protection.


.. _ERROR-894:

**ERROR CODE: -894, 'Stored procedure/function '%1$s' does not exist.'**

- This message occurs when you try to call a stored procedure or function that does not exist in the CUBRID database, that is, when you try to execute a stored procedure or function through a `CALL` statement or a function call and a stored procedure or function with the specified name does not exist in the database, the error is set, this occurs in CUBRID's stored procedure management mechanism, and it can occur mainly due to calling a non-existent stored procedure/function.


.. _ERROR-895:

**ERROR CODE: -895, 'Invalid stored procedure type: '%1$s' is the '%2$s'.'**

- This message occurs when the type of a stored procedure or function does not match in the CUBRID database, that is, when executing a DDL statement (for example, declaring it as a function and then doing drop procedure), the error is set if the actual stored procedure type differs from the requested type.


.. _ERROR-899:

**ERROR CODE: -899, 'Invalid partition requests.'**

- This message appears when an invalid request occurs while performing partition-related operations in the CUBRID database, that is, the error is set when attempting an inappropriate operation request on a partition table or an operation that violates partition-related constraints, it occurs in CUBRID's partition management mechanism, and it can occur mainly due to violations of partition-related constraints.


.. _ERROR-900:

**ERROR CODE: -900, 'Java VM library is not found: %1$s.'**

- This message occurs when the CUBRID database system fails to find or load the JVM (Java Virtual Machine) library required to start the JVM, that is, the CUBRID server process attempted to create a JVM process to execute Java-based stored procedures (Stored Procedure) or user-defined functions (User Defined Function), but could not load it because system environment variables (e.g., JAVA_HOME, JVM_PATH) are not correctly set for the JVM library path, the library file itself does not exist, or due to access permission issues. (From version 11.4, it occurs when the JVM library used by pl_server cannot be found)


.. _ERROR-901:

**ERROR CODE: -901, 'Java VM can not be started: %1$s.'**

- This message occurs when the CUBRID database system tried to start a JVM (Java Virtual Machine) process but failed, that is, the CUBRID server process attempted to create and initialize a JVM process to execute Java-based stored procedures (Stored Procedure) or user-defined functions (User Defined Function), but the JVM process could not be started normally due to various problems such as JVM library loading failure, JVM creation failure, failure to find required Java tables, memory allocation failure, or issues during JVM initialization. (From version 11.4, it occurs when pl_server for SP processing cannot be started)


.. _ERROR-902:

**ERROR CODE: -902, 'Java VM is not running.'**

- This message occurs when the CUBRID database system tries to use Java-based features while the JVM (Java Virtual Machine) process is not running., that is, the CUBRID server process attempted to connect to the JVM to execute Java-based stored procedures (Stored Procedure) or user-defined functions (User Defined Function), but it means that the JVM process itself has not started, has already terminated, or CUBRID's PL server manager is not initialized. (From version 11.4, it occurs when pl_server for SP processing is not running)


.. _ERROR-903:

**ERROR CODE: -903, 'Can't connect Java VM: %1$s'**

- This message occurs when the CUBRID database system tried to connect to the JVM (Java Virtual Machine) but failed, that is, the CUBRID server process attempted IPC (Inter-Process Communication) or socket communication with the JVM process to execute Java-based stored procedures (Stored Procedure) or user-defined functions (User Defined Function), but it means that the connection failed at the initial connection stage because the JVM process was not started, terminated abnormally, the CUBRID server could not find the JVM process, or there is a problem with communication settings. (From version 11.4, it occurs when it cannot connect to pl_server for SP processing)


.. _ERROR-904:

**ERROR CODE: -904, 'Invalid stored procedure name.'**

- This message occurs when the CUBRID database system determines that the name of a stored procedure (Stored Procedure) or a function (Function) is invalid, that is, when the CUBRID server tries to create (CREATE), alter (ALTER), drop (DROP), or call (CALL) a stored procedure, it means that the specified name does not satisfy CUBRID's naming rules or there was a problem during name processing, this is a protective error that occurs during validation of name validity in CUBRID's stored procedure management system.


.. _ERROR-905:

**ERROR CODE: -905, 'Networking with JVM failed: %1$d'**

- This message occurs when the CUBRID database system fails due to network-related problems while attempting to communicate with the JVM (Java Virtual Machine)., that is, the CUBRID server process attempted socket communication with the JVM process to execute Java-based stored procedures (Stored Procedure) or user-defined functions (User Defined Function), but it means that communication was interrupted due to network-level issues such as connection failure, data transmission errors, or timeouts, this situation generally occurs due to network configuration issues between the CUBRID server and the JVM process, firewalls, abnormal termination of the JVM process, lack of system resources, or errors in CUBRID's internal JVM integration module.  (From version 11.4, it occurs when an error occurs in pl_server for SP processing and it does not return results, etc.)


.. _ERROR-907:

**ERROR CODE: -907, 'The owner or a member of DBA group can drop stored procedure.'**

- This message occurs when the currently logged-in user tried to alter (ALTER) or drop (DROP) a specific stored procedure (Stored Procedure) but does not have sufficient privileges to drop that stored procedure, in CUBRID, only the owner of the stored procedure or a member of the DBA (Database Administrator) group can drop the stored procedure under a strict privilege policy.


.. _ERROR-908:

**ERROR CODE: -908, 'Too many arguments of stored procedure '%1$s'.'**

- This message occurs when defining or calling a stored procedure (Stored Procedure) or a function (Function) in the CUBRID database system and the number of arguments specified for the procedure exceeds the maximum number allowed by CUBRID, it means that, while processing a `CREATE PROCEDURE` or `CREATE FUNCTION` statement, the CUBRID server has exceeded the number of arguments of the user-defined stored procedure, this is a protective error that occurs during validation of argument count validity in CUBRID's stored procedure management system.


.. _ERROR-911:

**ERROR CODE: -911, 'Invalid call: it can not return ResultSet.'**

- This message occurs when calling a stored procedure (Stored Procedure) in the CUBRID database and the stored procedure is defined in a way that cannot return a ResultSet (query result set), or it is called in a context where returning a ResultSet is not allowed, that is, this error occurs when a specific stored procedure in CUBRID is not designed to return a ResultSet directly, or when the current execution environment (e.g., calling from inside another stored procedure, etc.) does not support returning a ResultSet, this situation generally occurs when calling a stored procedure using a `CALL` statement and the procedure definition or invocation method violates ResultSet return rules, this is a protective error to protect execution rules of stored procedures and the consistency of the database system.


.. _ERROR-912:

**ERROR CODE: -912, 'ResultSet can not be used input parameter.'**

- This message occurs when trying to use the 'RESULTSET' type as an input parameter while defining a stored procedure (Stored Procedure) or a function (Function) in the CUBRID database system, because in CUBRID's stored procedure system, the `RESULTSET` type is restricted to be used only as an output parameter. This is a constraint to protect system stability in terms of data flow and memory management.


.. _ERROR-913:

**ERROR CODE: -913, 'Too many nested stored procedure call.'**

- This message occurs when the nesting depth of calls where a stored procedure (Stored Procedure) calls another stored procedure exceeds the maximum allowed by the system in the CUBRID database system, this is a protective error to prevent system resource exhaustion or stack overflow caused by infinite recursive calls or excessive nested calls.


.. _ERROR-914:

**ERROR CODE: -914, 'A serial object already exists as an auto increment constraint.'**

- This message occurs when you try to create an auto increment (Auto Increment) constraint in the CUBRID database and a serial object with the same name already exists in the system.


.. _ERROR-915:

**ERROR CODE: -915, 'Increase value of an auto increment constraint should be greater than 0.'**

- This message occurs when creating or altering an auto increment (Auto Increment) constraint in the CUBRID database and the increment value is set to 0 or less, that is, the increment value of an auto increment sequence must be positive and 0 or negative values are not allowed. This is a constraint to ensure the logical meaning and mathematical consistency of auto increment, this situation generally occurs when defining an auto increment column using a `CREATE TABLE` or `ALTER TABLE` statement and specifying a value of 0 or less in the `INCREMENT BY` clause.


.. _ERROR-918:

**ERROR CODE: -918, 'The class '%1$s' referred by the foreign key does not exist.'**

- This message occurs when defining or altering a foreign key (Foreign Key) constraint in the CUBRID database and the parent table that the foreign key tries to reference does not exist in the database, this situation generally occurs when defining a foreign key using a `CREATE TABLE` or `ALTER TABLE` statement and the name of the parent table to be referenced is specified incorrectly or the table has already been dropped, this is a constraint validation error to protect the logical consistency of database design and referential integrity.


.. _ERROR-919:

**ERROR CODE: -919, 'The class '%1$s' referred by the foreign key does not have the primary key.'**

- This message occurs when defining or altering a foreign key (Foreign Key) constraint in the CUBRID database and the parent table that the foreign key tries to reference does not have a primary key (Primary Key) constraint defined, this situation generally occurs when defining a foreign key using a `CREATE TABLE` or `ALTER TABLE` statement and a primary key is not defined on the parent table or after the primary key has been dropped, this is a constraint validation error to protect data integrity and referential integrity.


.. _ERROR-920:

**ERROR CODE: -920, 'The foreign key '%1$s' does not include the primary key member '%2$s'.'**

- This message occurs when defining or altering a foreign key (Foreign Key) constraint in the CUBRID database and the list of columns that make up the foreign key does not include all columns of the referenced primary key (Primary Key), this situation generally occurs when defining a foreign key using a `CREATE TABLE` or `ALTER TABLE` statement and the referenced key's column list is specified incorrectly or some columns are omitted due to typos, this is a constraint validation error to protect data integrity and referential integrity.


.. _ERROR-921:

**ERROR CODE: -921, 'The domain of the foreign key member '%1$s' is different from that of the primary key member '%2$s'. (%3$s: %4$s vs. %5$s)'**

- This message occurs when defining or altering a foreign key (Foreign Key) constraint in the CUBRID database and the domain (data type and related properties) of a foreign key member column does not match the domain of the referenced primary key (Primary Key) member column, this situation generally occurs when defining a foreign key using a `CREATE TABLE` or `ALTER TABLE` statement and a non-compatible domain is specified due to misunderstanding or typos in the referenced key's column domain, this is a constraint validation error to ensure data integrity and referential integrity.


.. _ERROR-922:

**ERROR CODE: -922, 'The constraint of the foreign key '%1$s' is invalid, due to value '%2$s'.'**

- This message occurs when a foreign key (Foreign Key) constraint in the CUBRID database references an invalid value or violates data integrity due to a logical error in the constraint itself, that is, it means that a value inserted or updated in the foreign key column does not exist in the referenced parent table's primary key (Primary Key) or unique key (Unique Key), or that the foreign key constraint definition itself is incorrect and failed to pass validation, this situation generally occurs when adding or changing data in a child table using an `INSERT` or `UPDATE` statement and attempting to specify a value in the foreign key column that does not exist in the parent table. Also, it may occur due to data type mismatches in the referenced columns or other schema mismatches during the process of defining a foreign key, this is a protective error to protect data integrity and referential integrity.


.. _ERROR-923:

**ERROR CODE: -923, 'The primary key '%1$s' referred by a foreign key '%2$s' is not supposed to be dropped.'**

- This message occurs when you try to drop a primary key (Primary Key) in the CUBRID database and the primary key is referenced by a foreign key (Foreign Key) of another table, so the drop operation is not allowed, that is, to maintain referential integrity (Referential Integrity), the system restricts directly dropping a primary key of a parent table that is referenced by a child table. To drop the primary key, you must first remove all foreign key constraints that reference the primary key, this situation generally occurs when dropping a primary key using `ALTER TABLE ... DROP PRIMARY KEY` or when dropping a table with a primary key using `DROP TABLE` while foreign keys referencing it remain, this is a protective error to protect data integrity and maintain logical consistency of the database.


.. _ERROR-924:

**ERROR CODE: -924, 'Update/Delete operations are restricted by the foreign key '%1$s'.'**

- This message occurs when you try to update or delete a parent record in a table with a foreign key (Foreign Key) constraint set in the CUBRID database. In this case, since the foreign key's `ON UPDATE` or `ON DELETE` action is set to `RESTRICT`, the operation is not allowed if there are records in the child table that reference the parent record, that is, to maintain referential integrity (Referential Integrity), the system restricts modifying or deleting a parent record for which child records exist, this situation generally occurs when changing a parent table record using an `UPDATE` or `DELETE` statement while child table records that reference it remain, this is a protective error to protect data integrity and maintain logical consistency of the database.


.. _ERROR-926:

**ERROR CODE: -926, 'The instance having the foreign key '%1$s' cannot be dropped.'**

- This message occurs when you try to delete an instance (record) in the CUBRID database that is referenced by another table through a foreign key (Foreign Key) constraint, that is, it means that you are trying to delete an instance in the parent table but the delete operation is not allowed because a foreign key that references the parent instance exists in the child table and referential integrity could be broken, this situation generally occurs when deleting a parent table record using a `DELETE` statement while child table records that reference it remain, this is a protective error to protect data integrity and maintain logical consistency of the database.


.. _ERROR-927:

**ERROR CODE: -927, 'The number of keys of the foreign key '%1$s' is different from that of the primary key '%2$s'.'**

- This message occurs when defining or altering a foreign key (Foreign Key) constraint in the CUBRID database and the number of columns that make up the foreign key differs from the number of columns that make up the referenced primary key (Primary Key), that is, a foreign key must be composed of the same number of columns as the referenced parent table's primary key or unique key and the data types of each column must also be compatible. This message mainly occurs due to a mismatch in the number of columns, this situation generally occurs when defining a foreign key using a `CREATE TABLE` or `ALTER TABLE` statement and specifying a different number of columns due to misunderstanding the structure of the referenced key or due to typos, this is a constraint validation error to ensure data integrity and logical consistency of a relational database.


.. _ERROR-931:

**ERROR CODE: -931, 'Start value of an auto increment constraint should be less then max value.'**

- This message occurs when creating or altering an auto increment (Auto Increment) constraint in the CUBRID database and the start value is greater than or equal to the maximum value, that is, it is a constraint violation error that occurs when the start value of an auto increment sequence exceeds the maximum allowable range of the data type, or when the start value and the maximum value are the same, this situation generally occurs due to incorrect auto increment settings in table schema definitions, mismatches between the data type and the start value, or when a user explicitly sets a start value that exceeds the limits of the data type.


.. _ERROR-935:

**ERROR CODE: -935, 'Argument of %1$s function is out of range.'**

- This message occurs when the argument of a math function or a system function in the CUBRID database is outside the valid range that the function can handle, that is, it is an error that occurs when the function receives an input value that is outside the function's domain (domain) or is not mathematically valid, this situation generally occurs when attempting a mathematically undefined operation or attempting an out-of-range access of a system function.


.. _ERROR-936:

**ERROR CODE: -936, 'Overflow occurred in exp context.'**

- This message occurs when the CUBRID database query processor computes an exponential function for values of various data types and computes the exponential function for very large positive or negative values.


.. _ERROR-965:

**ERROR CODE: -965, 'Cannot use duplicate attribute names in index key list. Attribute name '%1$s' listed more than once.'**

- This message occurs when creating or altering an index in the CUBRID database and the same attribute (column) name is specified two or more times in the index key list, CUBRID does not allow including the same column redundantly in an index key for efficiency and consistency of the index. This is a protective error to prevent unnecessary duplication, optimize index performance, and maintain the integrity of the database schema, for example, this error occurs if you specify the same column twice like `CREATE INDEX idx_name ON table_name (col1, col2, col1)`.


.. _ERROR-979:

**ERROR CODE: -979, 'CONNECT BY loop in user data.'**

- This message occurs when an infinite loop happens due to a circular reference in a CONNECT BY clause.


.. _ERROR-996:

**ERROR CODE: -996, 'Incorrect number of values given. %1$d values were given but %2$d values are required.'**

- This message occurs in the CUBRID database when the number of host variables used in the PREPARE FROM statement and the number of values in the EXECUTE USING clause do not match.


.. _ERROR-997:

**ERROR CODE: -997, 'Cannot add the foreign key constraint %1$s to the partitioned %2$s. Referential actions that update attributes are not yet supported on partitioned classes.'**

- This message occurs when you try to add a foreign key constraint to a partitioned table in the CUBRID database. CUBRID currently does not support foreign key constraints on partitioned tables where the referential action updates attributes. Referential actions are features that automatically update the child table’s foreign key values when the parent table’s key value changes, such as `ON UPDATE CASCADE`, `ON UPDATE SET NULL`, and `ON UPDATE SET DEFAULT`. Because partitioned tables store data distributed across multiple partitions, performing such complex referential actions can cause performance issues or data consistency issues, so CUBRID restricts them.


.. _ERROR-998:

**ERROR CODE: -998, 'Cannot add the foreign key constraint %1$s on the attribute %2$s. The attribute cannot have a NOT NULL constraint because it has to match the SET NULL referential action.'**

- This message occurs when defining a foreign key that uses SET NULL as the referential action while the target column has a NOT NULL constraint. The SET NULL referential action must allow NULL values to maintain referential integrity, so it logically conflicts with a NOT NULL constraint.


.. _ERROR-999:

**ERROR CODE: -999, 'Cannot create table '%1$s' like table '%2$s' because the existing table has features that will not be duplicated (%3$s).'**

This error code occurs when using the CREATE TABLE LIKE statement and the source table has certain features or attributes that cannot be duplicated to the new table.


.. _ERROR-1000:

**ERROR CODE: -1000, '%1$s' is a CLASS or SHARED attribute but only normal attributes are allowed in this context.'**

- This message is an error that occurs with the `CREATE TABLE AS SELECT` statement, indicating that SHARED or CLASS attributes cannot be used in the column properties. For example, it can occur if you use a DDL (shared replication) statement format like: `CREATE TABLE x (name varchar);`  `CREATE TABLE y (name varchar shared '1111') as SELECT name FROM x;`.


.. _ERROR-1003:

**ERROR CODE: -1003, 'The constraint '%1$s' exists but has a different type.'**

- This message is a constraint type mismatch error that occurs during database schema management, and it occurs when trying to drop a primary key using `alter table .. drop index`.


.. _ERROR-1004:

**ERROR CODE: -1004, 'Please note that when using ALTER TABLE DROP FOREIGN KEY, CUBRID drops both the constraint and its associated index.'**

- This message is a notification provided to the user when executing the `ALTER TABLE DROP FOREIGN KEY` statement in the CUBRID database system. It is an informational message (not an error) that explains a characteristic of CUBRID’s foreign key management system and warns the user that both the constraint and its associated index are dropped.


.. _ERROR-1011:

**ERROR CODE: -1011, 'Cannot add the foreign key constraint %1$s on the shared attribute %2$s.'**

This error code indicates that a foreign key constraint cannot be added to an attribute with the SHARED property.


.. _ERROR-1012:

**ERROR CODE: -1012, 'Cannot create table with NULL column type.'**

This error code occurs when executing the `CREATE TABLE AS SELECT` statement and the result includes a column with a NULL type. For example, it can occur with a DDL copy statement format like `CREATE TABLE tbl AS SELECT NULL AS a`.


.. _ERROR-1013:

**ERROR CODE: -1013, 'Set DEFAULT or SHARED on %1$s typed column is not allowed.'**

This error code indicates that you cannot set a DEFAULT value or the SHARED attribute on a column of the BLOB or CLOB data type.


.. _ERROR-1014:

**ERROR CODE: -1014, 'Set NOT NULL on %1$s typed column is not allowed.'**

This error code indicates that you cannot set a NOT NULL constraint on a column of the BLOB or CLOB data type.


.. _ERROR-1041:

**ERROR CODE: -1041, 'A string was truncated in context of "%1$s".'**

- This message indicates that during query processing or data storage/retrieval in CUBRID, a string value was larger than the buffer or target column size, so the trailing part was truncated. It is a warning-type error message that may occur when reducing a column’s string length in `ALTER TABLE`, or during string concatenation such as the group_concat function.


.. _ERROR-1042:

**ERROR CODE: -1042, 'Trying to create a string requiring %1$d bytes of memory, while the maximum allowed is %2$d bytes.'**

- This message occurs when, during CUBRID query processing, the size of a string created as the result of a certain string operation (mainly string functions) exceeds the maximum size limit for a single string object allowed by the system. It is highly related to the string_max_size_bytes parameter setting in $CUBRID/conf/cubrid.conf.


.. _ERROR-1044:

**ERROR CODE: -1044, 'Cannot change attribute "%1$s". CUBRID cannot change attributes that are partitioning keys.'**

- This message means that the user attempted to change an attribute definition using the `ALTER TABLE ... CHANGE` statement, but the change was rejected because the attribute is used as a partitioning key. To maintain data distribution rules and integrity for a partitioned table, CUBRID does not allow changing the definition (e.g., data type, constraints, etc.) of a column used as a partitioning key. Since the partitioning key determines which partition data is stored in, changing the key column’s definition can invalidate the entire existing partition structure and break data consistency.


.. _ERROR-1045:

**ERROR CODE: -1045, 'Cannot change attribute "%1$s". CUBRID cannot change attributes used in foreign keys (either referenced or referencing).'**

- This message occurs when you try to change a specific column using `ALTER TABLE ... CHANGE` or `ALTER TABLE ... MODIFY` in the CUBRID database. If the column is used in a foreign key relationship, CUBRID does not allow the change to guarantee data integrity and referential consistency. A foreign key is an important constraint that maintains referential integrity, defining the relationship between a referenced column (the parent table’s primary key or unique key) and a referencing column (the child table’s foreign key). Since changing such columns can break referential integrity, CUBRID restricts it.


.. _ERROR-1046:

**ERROR CODE: -1046, 'Cannot change attribute "%1$s". CUBRID cannot change the attribute's domain. This change of domain is not supported.'**

- This message occurs when you try to change the data type of a specific column using `ALTER TABLE ... CHANGE` or `ALTER TABLE ... MODIFY` in the CUBRID database, and CUBRID currently does not support conversion between the involved data types. Not all data type conversions are always possible; in particular, changes between complex or incompatible data types may cause problems with data integrity or system stability, so CUBRID may explicitly not support them. This is a design limitation to keep database stability and consistency.


.. _ERROR-1047:

**ERROR CODE: -1047, 'Cannot change attribute "%1$s". CUBRID cannot change the attribute's domain. The new domain may not correctly represent all the existing attribute values.'**

- This message occurs when you try to change the data type of a specific column using `ALTER TABLE ... CHANGE` or `ALTER TABLE ... MODIFY` in the CUBRID database. After analyzing the data stored in the existing column, CUBRID raises this error when it determines that there are values that cannot be converted to the requested new data type, or that there is a high likelihood of data loss. This is CUBRID’s protection mechanism to ensure data integrity and prevent potential data loss. For example, it can occur when a `VARCHAR` column contains non-numeric strings and you attempt to change it to `INT`, or when an `INT` column contains values outside the `INT` range and you attempt to change it to `INT` from `BIGINT`.


.. _ERROR-1048:

**ERROR CODE: -1048, 'Cannot change attribute "%1$s". CUBRID cannot currently change an attribute's AUTO_INCREMENT if the attribute's domain also changes.'**

- This message occurs when you try to change both a column’s `AUTO_INCREMENT` attribute and its data type at the same time using `ALTER TABLE` in the CUBRID database. To maintain data integrity and reduce the complexity of schema changes, CUBRID does not allow such a simultaneous change. The `AUTO_INCREMENT` attribute relates to value generation logic, while a data type change relates to conversion and storage of existing data; changing both at once can cause potential data loss, consistency issues, or unpredictable behavior. Therefore, CUBRID requires these changes to be performed in separate steps.


.. _ERROR-1049:

**ERROR CODE: -1049, 'Cannot change attribute "%1$s". CUBRID cannot currently change the attribute's domain. The domain conversion is possible but this feature is not allowed by configuration.'**

- This message occurs when you try to change the data type of a specific column using `ALTER TABLE ... CHANGE` or `ALTER TABLE ... MODIFY` in the CUBRID database. Although CUBRID determines that the domain conversion is technically possible, it is restricted because the current system configuration does not allow this feature. It is highly likely that an administrator intentionally restricted the feature due to conversion complexity, potential data loss risk, or possible impact on system performance. This can occur when alter_table_change_type_strict is set to true in $CUBRID/conf/cubrid.conf.


.. _ERROR-1050:

**ERROR CODE: -1050, 'Cannot change attribute "%1$s". CUBRID cannot currently change the attribute's domain. The attribute has an index on it.'**

- This message occurs when you try to change the data type of a specific column using `ALTER TABLE ... CHANGE` or `ALTER TABLE ... MODIFY` in the CUBRID database. In particular, if an index already exists on the column you are trying to change, CUBRID does not allow the domain change due to data integrity and the complexity of index structures. Since an index is a structure that helps search quickly based on column values, changing a column’s data type can invalidate the index structure or require rebuilding. To prevent such complexity and potential data damage, CUBRID restricts domain changes on indexed columns.


.. _ERROR-1052:

**ERROR CODE: -1052, 'Attribute "%1$s" was not changed. CUBRID did not detect any effective changes in the attribute representation.'**

- This message is an informational warning that occurs when you try to change a column’s properties using `ALTER TABLE ... CHANGE` or `ALTER TABLE ... MODIFY`, but CUBRID determines that the requested changes are effectively the same as the current column definition. In other words, the data type, length, constraints (NOT NULL, DEFAULT, etc.) you attempted to change are the same as the existing definition, so no actual change occurred. This is not an error; it informs you that the system did not perform unnecessary work.


.. _ERROR-1054:

**ERROR CODE: -1054, 'The new value of an auto increment constraint should be greater than or equal to the current value.'**

This message occurs when you try to change the value of an auto increment (AUTO_INCREMENT) constraint using the `ALTER TABLE` statement in the CUBRID database system, and the new value is smaller than the current value. That is, during processing of `ALTER TABLE ... AUTO_INCREMENT = x`, the CUBRID server raises this message when the new value specified by the user is smaller than the current auto-increment value of the column. This is a constraint to protect data integrity, because decreasing the auto-increment value can cause consistency issues with existing data. This is a protective error to maintain data consistency in CUBRID’s auto-increment system.


.. _ERROR-1055:

**ERROR CODE: -1055, 'The new value of an auto increment constraint should be lower than the maximum value.'**

- This message occurs when you try to set an `AUTO_INCREMENT` value larger than the maximum value using the `ALTER TABLE` statement in the CUBRID database. In particular, it occurs when the new `AUTO_INCREMENT` start value (the N value in `ALTER TABLE table_name AUTO_INCREMENT = N;`) is larger than the maximum value allowed by the column’s data type. Since `AUTO_INCREMENT` must always increase and cannot exceed the data type’s range or conflict with the existing maximum value, CUBRID rejects such a setting to ensure data integrity.


.. _ERROR-1056:

**ERROR CODE: -1056, 'To avoid ambiguity, the AUTO_INCREMENT table option requires the table to have exactly one AUTO_INCREMENT column and no seed/increment specification.'**

- This message is displayed in the CUBRID database when, during table creation or alteration, a table-level `AUTO_INCREMENT = seed` option is specified together with a column-level seed/increment (as in `AUTO_INCREMENT(seed, increment)`), because it is difficult to determine which auto_increment value to apply. As of CUBRID 11.5, a table can have only one auto_increment column.


.. _ERROR-1057:

**ERROR CODE: -1057, 'Cannot add a NOT NULL '%1$s' typed attribute without a DEFAULT value.'**

- This message occurs when, in the CUBRID database, you try to add an attribute with a `NOT NULL` constraint using a `CREATE TABLE` or `ALTER TABLE` statement, or when the column’s type does not allow specifying a default value.


.. _ERROR-1058:

**ERROR CODE: -1058, 'ALTER TABLE .. CHANGE : changing to new domain : cast failed, current configuration doesn't allow truncation or overflow.'**

- This message occurs when you change a column’s data type using the `ALTER TABLE ... CHANGE` statement in the CUBRID database. During conversion (casting) of existing column data to the new data type, a situation was detected where some data could exceed the new domain’s range or be truncated. Under the current CUBRID configuration, such conversions are not allowed to prevent data loss. This is CUBRID’s protection mechanism to ensure data integrity and prevent unintended data loss.


.. _ERROR-1059:

**ERROR CODE: -1059, 'ALTER TABLE .. CHANGE : changing to new domain : cast failed, default value of new domain was set.'**

- This message is an informational notification that occurs when changing a column’s data type (when it is not numeric or string) using the `ALTER TABLE ... CHANGE` statement in the CUBRID database. During conversion (casting) of existing column data to the new data type, if some data does not fit the new domain’s range or cannot be converted, CUBRID automatically sets that data to the new domain’s default value. This is CUBRID’s automatic data conversion mechanism to complete schema changes while minimizing data loss. For example, when changing a `VARCHAR` column to `INT`, strings that cannot be converted to numbers may be set to 0, the default value of `INT`. This message reports the automatic conversion work successfully performed by the system for data consistency.


.. _ERROR-1060:

**ERROR CODE: -1060, 'ALTER TABLE .. CHANGE : changing to new domain : cast failed, minimum value of new domain was set.'**

- This message is an informational notification that occurs when changing a column’s data type using the `ALTER TABLE ... CHANGE` statement in the CUBRID database (when the domain value is negative). During conversion (casting) of existing column data to the new data type, if some data exceeds the new domain’s range or cannot be converted, CUBRID automatically sets that data to the new domain’s minimum value. This is CUBRID’s automatic data conversion mechanism to complete schema changes while minimizing data loss.


.. _ERROR-1061:

**ERROR CODE: -1061, 'ALTER TABLE .. CHANGE : changing to new domain : cast failed, maximum value of new domain was set.'**

- This message is an informational notification that occurs when changing a column’s data type using the `ALTER TABLE ... CHANGE` statement in the CUBRID database (when the domain value is positive). During conversion (casting) of existing column data to the new data type, if some data exceeds the new domain’s range or cannot be converted, CUBRID automatically sets that data to the new domain’s maximum value. This is CUBRID’s automatic data conversion mechanism to complete schema changes while minimizing data loss.


.. _ERROR-1062:

**ERROR CODE: -1062, 'Cannot change attribute "%1$s". The change would result in multiple primary keys being defined.'**

- This message occurs when you try to change a specific column using the `ALTER TABLE` statement in the CUBRID database. If the requested change would result in multiple primary keys being defined on the table, CUBRID rejects the change to guarantee data integrity and table structure consistency. In a relational database, only one primary key is allowed per table; having multiple primary keys can cause issues with uniqueness and referential integrity. This violates a fundamental database design principle.


.. _ERROR-1063:

**ERROR CODE: -1063, 'Cannot add NOT NULL constraint for attribute "%1$s": there are existing NULL values for this attribute.'**

- This message occurs when you try to add a `NOT NULL` constraint to a specific column using the `ALTER TABLE` statement in the CUBRID database. If one or more `NULL` values already exist in the column, CUBRID refuses to add the `NOT NULL` constraint to ensure data integrity. Because a `NOT NULL` constraint completely disallows storing `NULL` values in the column, you must handle or remove the existing `NULL` values first. This is CUBRID’s safety mechanism to protect database consistency and integrity.


.. _ERROR-1064:

**ERROR CODE: -1064, 'ALTER TABLE .. CHANGE : adding NOT NULL : all existing NULL values were changed to domain's hard default.'**

- This message is an informational notification that occurs when adding a `NOT NULL` constraint to a specific column using the `ALTER TABLE ... CHANGE` statement in the CUBRID database. If `NULL` values already exist in the column and the user did not specify an explicit `DEFAULT` value, CUBRID automatically converts all existing `NULL` values to the column data type’s hard default value to maintain data integrity. For example, numeric types are changed to 0, and string types are changed to an empty string (''). This is not an error; it reports the automatic conversion work successfully performed by the system for data consistency.


.. _ERROR-1067:

**ERROR CODE: -1067, 'Column '%1$s' cannot have a NOT NULL constraint and a NULL default value.'**

- This message occurs when creating or altering a table in the CUBRID database while attempting to apply a NOT NULL constraint to a specific column and simultaneously set NULL as the default value. Because a NOT NULL constraint does not allow storing NULL values in the column, a NULL default value logically conflicts with the constraint and is not allowed. This is the system’s protection mechanism to ensure data integrity and keep data definitions consistent.


.. _ERROR-1068:

**ERROR CODE: -1068, 'Maximum trigger depth cannot exceed %1$d.'**

- This message occurs when the maximum trigger depth limit allowed by CUBRID is exceeded. To prevent infinite trigger loops or excessive resource consumption, CUBRID limits the maximum depth of trigger invocations, and this message means that the setting itself has exceeded the system’s internal limit. This is an important protective error to safeguard database stability and performance.


.. _ERROR-1080:

**ERROR CODE: -1080, 'The maximum length of filter predicate string must be %1$d.'**

- This message is displayed when creating a function index in the CUBRID database, if the length of the argument (expression) used in the function is larger than the internally defined limit.


.. _ERROR-1085:

**ERROR CODE: -1085, 'Cannot compile regular expression: '%1$s''**

- This message appears when the CUBRID database cannot process a regular expression normally because a problem occurred while compiling it. This can mainly occur due to a syntax error in the regular expression itself, an invalid pattern, or an internal issue in the regular expression engine. If this error occurs when the database compiles the pattern before processing a regular expression function in a query, the query execution fails.


.. _ERROR-1086:

**ERROR CODE: -1086, 'Cannot execute regular expression: '%1$s''**

- This message appears when the CUBRID database cannot process a regular expression normally because a problem occurred while executing it. This can mainly occur due to a syntax error in the regular expression itself, an invalid pattern, or an internal issue in the regular expression engine. If this error occurs while the database processes a regular expression function in a query, the query execution fails.


.. _ERROR-1092:

**ERROR CODE: -1092, 'Cannot prepare statement of type '%1$s' with host variables.'**

- This message occurs when, in the CUBRID database, you try to prepare a certain type of SQL statement with host variables using a PREPARE statement. A PREPARE statement pre-compiles an SQL statement and builds an execution plan, allowing dynamic SQL construction using host variables. However, some statement types (e.g., DDL statements, certain system commands, etc.) are restricted and cannot be used with host variables. This is a constraint to protect database security and stability.


.. _ERROR-1096:

**ERROR CODE: -1096, 'Altering partitioning schema is not allowed when the partitioned class is referenced by a foreign key.'**

- This message occurs when you try to change the partitioning schema of a partitioned table in the CUBRID database while that table is referenced by a foreign key constraint from another table. It is a protection mechanism that restricts structural changes to a partition table when foreign key relationships exist, in order to protect data integrity.


.. _ERROR-1097:

**ERROR CODE: -1097, 'Column '%1$s' cannot have a PRIMARY KEY constraint and a NULL default value.'**

- This message occurs when creating or altering a table in the CUBRID database while attempting to apply a PRIMARY KEY constraint to a specific column and simultaneously set NULL as the default value. Because a PRIMARY KEY requires uniqueness and the NOT NULL property, a NULL default value conflicts with the constraint and is not allowed. This is the system’s protection mechanism to ensure data integrity.


.. _ERROR-1098:

**ERROR CODE: -1098, 'Locales for language '%1$s' are not available with charset '%2$s'.'**

- This message is displayed when using date/time formatting functions (e.g., DATE_FORMAT, TIME_FORMAT) in the CUBRID database. That is, it occurs when locale data for a specific language/charset combination is not installed on the system or is not supported. This is a multilingual support-related error that appears when trying to output a date or time in a specific language format but the locale information for that language is not compatible with the current charset.


.. _ERROR-1106:

**ERROR CODE: -1106, 'Multiple rows in source table match the same row in destination table.'**

- This message is displayed in the CUBRID database when, during GROUP BY processing, there are two or more identical records in the result, and it also occurs in the process of merging results after sorting.


.. _ERROR-1107:

**ERROR CODE: -1107, 'Invalid bucket number for NTILE. Should be greater than zero and no more than 2,147,483,647.'**

- This message occurs when using the SQL window function NTILE() in the CUBRID database and the number of buckets (groups) specified as the argument is outside the valid range. That is, in NTILE(n), this error occurs if n is less than or equal to 0, or if it exceeds the maximum value representable by CUBRID’s integer type (2,147,483,647). The `NTILE` function requires a positive integer bucket count to evenly divide rows, and values beyond the system’s integer limit cannot be processed. This is a protective error to prevent inputs that exceed the correct usage of the NTILE function and the limits of the data type.


.. _ERROR-1108:

**ERROR CODE: -1108, 'Invalid bucket number for WIDTH_BUCKET. Should be greater than zero and less than 2,147,483,647.'**

- This message occurs when the fourth argument (bucket count) of the WIDTH_BUCKET function in the CUBRID database is outside the valid range. That is, WIDTH_BUCKET divides data into a specified number of equal-width buckets, and this error occurs when the bucket count is less than 1 or is greater than or equal to the maximum 32-bit integer value (2,147,483,647). This is a protective error to guarantee normal function operation. WIDTH_BUCKET is mainly used in data analysis or statistical tasks, and if the bucket count is too large or too small, memory usage or performance problems can occur.


.. _ERROR-1109:

**ERROR CODE: -1109, 'Invalid value for partition definition.'**

- This message occurs when, in the CUBRID database, a partition table’s partition key value does not match the partition definition. That is, when trying to insert or update data into a specific partition, if the partition key value of the data does not belong to any partition range or does not match the partition definition, this error occurs. This is a protective error to ensure data integrity for partition tables. It mainly occurs when the partition key value is outside the ranges defined by the partition definition during INSERT or UPDATE operations, or when the partition definition is incorrect and the data cannot be placed into an appropriate partition.


.. _ERROR-1118:

**ERROR CODE: -1118, 'The argument of "%1$s" can not be coerced to desired domain "%2$s".'**

- This message occurs when, in the CUBRID database, the argument of a function or operation cannot be converted to the desired domain type. That is, it means that an incompatible data type conversion (type casting) was attempted. This mainly occurs when performing operations between different data types in aggregate functions (MEDIAN, AVG, SUM, etc.) or arithmetic operations. The system attempts automatic conversion to compatible types such as DOUBLE, DATETIME, TIME, etc., but if conversion is not possible, it raises this error.


.. _ERROR-1121:

**ERROR CODE: -1121, 'NOT NULL constraints do not allow NULL value.'**

- This message occurs when, in the CUBRID database, after inserting data and then creating an index, NULL data exists in a column with a NOT NULL constraint. This is a protective error due to a constraint violation. This message mainly occurs during operations such as index creation, data insertion, and updates, especially when trying to process data that includes NULL values in a column with a NOT NULL constraint during the B-tree index loading process.


.. _ERROR-1124:

**ERROR CODE: -1124, 'Query execution error. ERROR_CODE = %1$d, /* SQL_ID: %2$s */ %3$s'**

- This message indicates a general execution error that occurred during SQL query execution in the CUBRID database. That is, parsing or compilation succeeded, but a problem occurred during the actual execution phase. This message can occur in various situations such as internal errors in the query execution engine, data type conversion errors, insufficient memory, system resource issues, and more.


.. _ERROR-1137:

**ERROR CODE: -1137, 'The query has been rejected due to attempt to exceed the maximum allowed nesting level(%1$d).'**

- This message occurs when the nesting level (subqueries, recursive queries, etc.) of an SQL query exceeds the maximum depth allowed by the system (default: 400). It is a safety mechanism to protect system resources and prevent infinite recursion.


.. _ERROR-1150:

**ERROR CODE: -1150, 'Context requires compatible collations.'**

- This message occurs when, in the CUBRID database, incompatible collations are used or required while processing string data (e.g., comparison, sorting, joins, etc.). That is, it occurs when the database system detects that the collation of string data required for a certain operation does not match the expected collation in the current context. This situation can generally occur due to operations between tables or columns with different collation settings, or due to incorrect collation specification. This is a protective error to ensure consistency of data and accurate string processing.


.. _ERROR-1153:

**ERROR CODE: -1153, 'Attempt to use 'zero date'.'**

- This message occurs when, in the CUBRID database, you try to perform arithmetic operations or functions on a date/time type value that is a 'zero date' (a date value stored as 0). That is, it is a validation error that occurs when attempting operations on values stored internally as 0 (invalid dates) for date/time types such as TIMESTAMP, DATE, and DATETIME. This situation generally occurs when date/time data is not initialized, an incorrect value is stored, or 0 is stored instead of NULL.


.. _ERROR-1169:

**ERROR CODE: -1169, 'Partition key attributes must be present in the index key.'**

- This message occurs when creating or modifying a unique index on a partitioned table in the CUBRID database. CUBRID has special constraints for unique indexes on partitioned tables. That is, the columns (index keys) that make up the unique index must include all columns used as the table’s partition keys. This constraint exists to guarantee uniqueness across the entire table beyond partition boundaries. If you define a unique index without including the partition key columns in the index key, the database raises this error and rejects index creation.


.. _ERROR-1193:

**ERROR CODE: -1193, 'The length of the processed partition expression exceeds the maximum allowed(%1$d).'**

- This message occurs when, in the CUBRID database, the length of the partition expression used when partitioning a table (e.g., a column or function used as the partition key for `RANGE`, `LIST`, or `HASH`) exceeds the maximum length allowed by the system. Since partition expressions must be processed and stored internally, overly long expressions can cause system resource usage issues or violate internal constraints. This mainly occurs when executing `CREATE TABLE ... PARTITION...`, and it can appear when the partition key expression is complex or the string is long.


.. _ERROR-1195:

**ERROR CODE: -1195, 'The recursive part of a CTE should not contain aggregate functions without GROUP BY clause.'**

- This message occurs when, in the CUBRID database, you define a recursive CTE (Common Table Expression) and use aggregate functions (e.g., SUM, COUNT, AVG, MAX, MIN) without a GROUP BY clause in the recursive member part. A recursive CTE repeatedly references itself to generate results, and using aggregate functions without `GROUP BY` in the recursive part can lead to unexpected results or infinite loops, so CUBRID restricts it syntactically. This is a limitation based on SQL standards or specific database system rules for recursive query processing.


.. _ERROR-1196:

**ERROR CODE: -1196, 'Maximum recursions %1$d reached executing CTE.'**

- This message occurs when, in the CUBRID database, a recursive CTE (Common Table Expression) exceeds the maximum allowed recursion count during execution. Recursive CTEs are used to process hierarchical data or perform iterative computations, and to prevent infinite loops and protect system resources, the database system limits the maximum recursion count by default. When this limit is reached, query execution is stopped and this error message is returned.


.. _ERROR-1197:

**ERROR CODE: -1197, 'Invalid json: %1$s (%2$d)'**

- This message occurs when, in the CUBRID database, an input JSON string does not follow valid JSON syntax while parsing or processing JSON-formatted data. When the JSON parser detects a syntax error while analyzing the string, it returns this error along with detailed information including the error type and position. This can occur when calling functions that take JSON data as arguments, loading JSON-format configuration files, or inserting values into columns that contain JSON data.


.. _ERROR-1199:

**ERROR CODE: -1199, 'The provided JSON has been invalidated by the JSON schema (Invalid schema path: %1$s, Keyword: %2$s, Invalid provided JSON path: %3$s)'**

- This message occurs when, in the CUBRID database, the JSON format is not correct while handling JSON data.


.. _ERROR-1200:

**ERROR CODE: -1200, 'Invalid JSON path'**

- This message occurs when, in the CUBRID database, the syntax of a JSON Path Expression is invalid while handling JSON data. When trying to select or access specific elements within a JSON document using a JSON path, if the provided path string does not follow the JSON Path standard or the syntax rules supported by CUBRID, this error is returned. This can occur when using functions that take a JSON path as an argument, such as JSON_EXTRACT, JSON_SET, JSON_INSERT, and JSON_REPLACE.


.. _ERROR-1201:

**ERROR CODE: -1201, 'Json object name cannot be NULL.'**

- This message occurs when, in the CUBRID database, the member (property) name of a JSON object is specified as NULL while creating or manipulating JSON objects. According to the JSON standard, a member name must be a string and cannot be NULL. This message can appear when using the JSON_OBJECT function or the JSON_OBJECTAGG aggregate function if a NULL value is passed as the key argument or if the expression used as the key evaluates to NULL.


.. _ERROR-1202:

**ERROR CODE: -1202, 'Json PATH "%1$s" does not exist in %2$s'**

- This message occurs when, in the CUBRID database, you provide a JSON path that does not exist while using JSON functions.


.. _ERROR-1203:

**ERROR CODE: -1203, 'Invalid JSON type at path %1$s. Expected %2$s, but found %3$s.'**

- This message occurs when, in the CUBRID database, you provide a non-JSON-object value while using the JSON_INSERT or JSON_SET functions.


.. _ERROR-1204:

**ERROR CODE: -1204, 'Json KEY "%1$s" is duplicate'**

- This message occurs when, in the CUBRID database, the same key name is used two or more times while creating or manipulating a JSON object. According to the JSON standard, all keys in a JSON object must be unique. Therefore, when building JSON data using functions such as JSON_OBJECT or JSON_OBJECTAGG, if you attempt to include duplicate keys, the system detects this and returns this error to ensure the validity of the data.


.. _ERROR-1205:

**ERROR CODE: -1205, 'Invalid type %1$s. Expecting JSON document compatible data type (STRING or JSON).'**

- This message occurs when, in the CUBRID database, a data type that cannot be converted to a JSON document is provided while using JSON functions.


.. _ERROR-1206:

**ERROR CODE: -1206, 'Json PATH "%1$s" does not address a cell in an Json ARRAY.'**

- This message is an error that can occur when using the JSON_ARRAY_INSERT function in the CUBRID database while handling JSON data.


.. _ERROR-1207:

**ERROR CODE: -1207, 'Invalid JSON path. Index is larger than allowed by system parameter json_max_array_idx.'**

- This message occurs when, in the CUBRID database, while using the JSON_ARRAY_INSERT function, the index in the JSON path provided as the second argument is not a number or exceeds the value set in json_max_array_idx.


.. _ERROR-1208:

**ERROR CODE: -1208, 'Invalid one/all argument. The one/all argument may only take one of these two values: "one", "all".'**

- This message occurs when, in the CUBRID database, while using the `JSON_CONTAINS_PATH` function, the value passed as the second argument ("one" or "all") is invalid.


.. _ERROR-1284:

**ERROR CODE: -1284, 'Cannot truncate the table due to the primary key referred to on %1$s. Try again with CASCADE option.'**

- This message occurs when, in the CUBRID database, you execute `TRUNCATE TABLE` without the `CASCADE` option on a table whose primary key is referenced by a foreign key.


.. _ERROR-1285:

**ERROR CODE: -1285, 'Cannot cascade truncate because the ON DELETE action of the foreign key (%1$s) is not set to CASCADE.'**

- This message occurs when, in the CUBRID database, you execute `TRUNCATE TABLE CASCADE` on a table referenced by a foreign key, but the foreign key’s `ON DELETE` action is not set to `CASCADE`.


.. _ERROR-1339:

**ERROR CODE: -1339, 'Unsupported argument type of stored procedure: %1$s'**

- This message occurs when using an argument data type that is not supported by Java stored procedures in CUBRID.


.. _ERROR-1340:

**ERROR CODE: -1340, 'Unsupported return type of stored procedure: %1$s'**

- This message occurs when using a return data type that is not supported by Java stored procedures in CUBRID.


.. _ERROR-1343:

**ERROR CODE: -1343, 'RENAME does not allow change of owner.'**

- This message occurs when, in CUBRID, you use the RENAME command and attempt to change the owner along with the object name. Under CUBRID’s security and integrity policy, the RENAME command supports changing only the object name and does not support changing the owner. Owner changes are possible only through separate privilege management or DDL commands.


.. _ERROR-1344:

**ERROR CODE: -1344, 'DBA, members of DBA group, and owner can perform CREATE SERIAL.'**

- This message occurs when, in CUBRID, you try to create a serial (Serial, auto-increment sequence) object but the user executing the command is not a DBA, a member of the DBA group, or the owner of the target object for which the serial is being created. In other words, it is a privilege-related error that occurs when a user without permission attempts CREATE SERIAL.


.. _ERROR-1348:

**ERROR CODE: -1348, 'Synonym "%1$s" already exists.'**

- This message occurs when, in CUBRID, you execute a CREATE SYNONYM statement and the specified synonym name already exists in the database. That is, it is a constraint violation error that occurs when a synonym with the same name is already registered and duplicate creation is not possible.


.. _ERROR-1349:

**ERROR CODE: -1349, 'Synonym "%1$s" does not exist.'**

- This message occurs when, in CUBRID, the specified synonym name does not exist in the database in contexts such as DROP SYNONYM, ALTER SYNONYM, or queries that reference a synonym. That is, the synonym entered by the user is not currently registered in the database or has already been dropped.


.. _ERROR-1351:

**ERROR CODE: -1351, 'The maximum length of the function expression string must be %1$d.'**

- This message occurs when creating a function-based index in CUBRID and the expression string used as a function argument exceeds the maximum allowed length. That is, it is a constraint violation error that occurs when attempting to define an overly long function expression.


.. _ERROR-1360:

**ERROR CODE: -1360, 'In line %1$d, column %2$d\nStored procedure compile error: %3$s'**

- This message is a compilation error that occurs when creating CUBRID PL/CSQL.


.. _ERROR-1363:

**ERROR CODE: -1363, 'Dropping system generated stored procedure is not allowed.'**

- This message occurs when a user tries to drop a Java stored procedure/function generated by CUBRID.


.. _ERROR-1365:

**ERROR CODE: -1365, 'Only DBA and the owner can grant the %1$s privilege.'**

- This message occurs when setting (granting) privileges for a Java stored procedure/function in CUBRID, if the user is neither the owner nor a DBA (DBA group).


.. _ERROR-1366:

**ERROR CODE: -1366, 'String representation of a default value of a Stored procedure/function parameter may not exceed 255 characters.'**

- This message occurs when creating a Java stored procedure/function in CUBRID and setting a default value for a parameter, if the string length for a char type is 255 characters or more.
