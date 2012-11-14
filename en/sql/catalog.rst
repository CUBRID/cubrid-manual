*********************
CUBRID System Catalog
*********************

You can easily get various schema information from the SQL statement by using the system catalog virtual class. For example, you can get the following schema information by using the catalog virtual class.

.. code-block:: sql

	-- Classes that refer to the 'b_user' class
	SELECT class_name
	FROM db_attribute
	WHERE domain_class_name = 'db_user';
	 
	-- The number of classes that the current user can access
	SELECT COUNT(*)
	FROM db_class;
	 
	-- Attribute of the 'db_user' class
	SELECT attr_name, data_type
	FROM db_attribute
	WHERE class_name = 'db_user';
	
System Catalog Classes
======================

To define a catalog virtual class, define a catalog class first. The figure below shows catalog classes to be added and their relationships. The arrows represent the reference relationship between classes, and the classes that start with an underline (_) are catalog classes.

.. image:: /images/image9.png

Added catalog classes represent information about all classes, attributes and methods in the database. Catalog classes are made up of class composition hierarchy and designed to have OIDs of catalog class instances for cross reference.

_db_class
---------

Represents class information. An index for class_name is created.

+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type**             | **Description**                                                                          |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| class_of           | object                    | A class object. Represents a meta information object for the class stored in the system. |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| class_name         | VARCHAR(255)              | Class name                                                                               |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| class_type         | INTEGER                   | 0 for a class, and 1 for a virtual class                                                 |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| is_system_class    | INTEGER                   | 0 for a user-defined class, and 1 for a system class                                     |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| owner              | db_user                   | Class owner                                                                              |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| inst_attr_count    | INTEGER                   | The number of instance attributes                                                        |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| class_attr_count   | INTEGER                   | The number of class attributes                                                           |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| shard_attr_count   | INTEGER                   | The number of shared attributes                                                          |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| inst_meth_count    | INTEGER                   | The number of instance methods                                                           |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| calss_meth_count   | INTEGER                   | The number of class methods                                                              |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| sub_classes        | SEQUENCE OF _db_class     | Class one level down                                                                     |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| super_classes      | SEQUENCE OF _db_class     | Class one level up                                                                       |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| inst_attrs         | SEQUENCE OF _db_attribute | Instance attribute                                                                       |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| class_attrs        | SEQUENCE OF _db_attribute | Class attribute                                                                          |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| shared_attrs       | SEQUENCE OF _db_attribute | Shared attribute                                                                         |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| inst_meths         | SEQUENCE OF _db_method    | Instance method                                                                          |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| class_meths        | SEQUENCE OF _db_method    | Class method                                                                             |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| meth_files         | SEQUENCE OF _db_methfile  | File path in which the function for the method is located                                |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| query_specs        | SEQUENCE OF _db_queryspec | SQL definition statement for a virtual class                                             |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+
| indexes            | SEQUENCE OF _db_index     | Index created in the class                                                               |
|                    |                           |                                                                                          |
+--------------------+---------------------------+------------------------------------------------------------------------------------------+

**Example**

The following example shows how to retrieve all sub classes under the class owned by user '**PUBLIC**' (for the child class *female_event* in the result, see the example in `Adding a super class <#syntax_syntax_table_inherit_add__5365>`_).

.. code-block:: sql

	SELECT class_name, SEQUENCE(SELECT class_name FROM _db_class s WHERE s IN c.sub_classes)
	 FROM _db_class c
	 WHERE c.owner.name = 'PUBLIC' AND c.sub_classes IS NOT NULL;
	  class_name            sequence((select class_name from _db_class s where s in c.sub_classes))
	============================================
	  'event'               {'female_event'}

.. note::

	All examples of system catalog classes have been written in the csql utility. In this example, **--no-auto-commit** (inactive mode of auto-commit) and **-u** (specifying user DBA) options are used. ::
	
		% csql --no-auto-commit -u dba demodb

_db_attribute
-------------

Represents attribute information. Indexes for class_of and attr_name are created.

+--------------------+------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type**          | **Description**                                                                                                                                             |
|                    |                        |                                                                                                                                                             |
+--------------------+------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+
| class_of           | _db_class              | Class to which the attribute belongs                                                                                                                        |
|                    |                        |                                                                                                                                                             |
+--------------------+------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+
| attr_name          | VARCHAR(255)           | Attribute name                                                                                                                                              |
|                    |                        |                                                                                                                                                             |
+--------------------+------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+
| attr_type          | INTEGER                | Type defined for the attribute. 0 for an instance attribute, 1 for a class attribute, and 2 for a shared attribute.                                         |
|                    |                        |                                                                                                                                                             |
+--------------------+------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+
| from_class_of      | _db_class              | If the attribute is inherited, the super class in which the attribute is defined is specified. Otherwise,                                                   |
|                    |                        | **NULL**                                                                                                                                                    |
|                    |                        | is specified.                                                                                                                                               |
|                    |                        |                                                                                                                                                             |
+--------------------+------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+
| from_attr_name     | VARCHAR(255)           | Inherited attribute. If an attribute name has changed to resolve a name conflict, the original name define in the super class is specified. Otherwise,      |
|                    |                        | **NULL**                                                                                                                                                    |
|                    |                        | is specified.                                                                                                                                               |
|                    |                        |                                                                                                                                                             |
+--------------------+------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+
| def_order          | INTEGER                | Order of attributes in the class. Begins with 0. If the attribute is inherited, the order is the one defined in the super class. For example,               |
|                    |                        | if class y inherits attribute a from class x and a was first defined in x, def_order becomes 0.                                                             |
+--------------------+------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+
| data_type          | INTEGER                | Data type of the attribute. One of the values specified in the "Data Types Supported by CUBRID" table below.                                                |
|                    |                        |                                                                                                                                                             |
+--------------------+------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+
| default_value      | VARCHAR(255)           | Default value. Stores as a character string regardless of data types. If there is no default value, NULL. If the default value is                           |
|                    |                        | **NULL**                                                                                                                                                    |
|                    |                        | ,                                                                                                                                                           |
|                    |                        | **NULL**                                                                                                                                                    |
|                    |                        | is used. If the data type is an object, 'volume id | page id | slot id' is used. If the data type is a collection, '{element 1, element 2, ... is used.     |
|                    |                        |                                                                                                                                                             |
+--------------------+------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+
| domains            | SEQUENCE OF _db_domain | Domain information of the data type                                                                                                                         |
|                    |                        |                                                                                                                                                             |
+--------------------+------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+
| is_nullable        | INTEGER                | 0 if a not null constraint is configured, and 1 otherwise.                                                                                                  |
|                    |                        |                                                                                                                                                             |
+--------------------+------------------------+-------------------------------------------------------------------------------------------------------------------------------------------------------------+

**Data Types Supported by CUBRID**

+-----------+-------------+-----------+-------------+
| **Value** | **Meaning** | **Value** | **Meaning** |
|           |             |           |             |
+-----------+-------------+-----------+-------------+
| 1         | INTEGER     | 18        | SHORT       |
|           |             |           |             |
+-----------+-------------+-----------+-------------+
| 2         | FLOAT       | 20        | OID         |
|           |             |           |             |
+-----------+-------------+-----------+-------------+
| 3         | DOUBLE      | 22        | NUMERIC     |
|           |             |           |             |
+-----------+-------------+-----------+-------------+
| 4         | STRING      | 23        | BIT         |
|           |             |           |             |
+-----------+-------------+-----------+-------------+
| 5         | OBJECT      | 24        | VARBIT      |
|           |             |           |             |
+-----------+-------------+-----------+-------------+
| 6         | SET         | 25        | CHAR        |
|           |             |           |             |
+-----------+-------------+-----------+-------------+
| 7         | MULTISET    | 26        | CHAR        |
|           |             |           |             |
+-----------+-------------+-----------+-------------+
| 8         | SEQUENCE    | 27        | VARNCHAR    |
|           |             |           |             |
+-----------+-------------+-----------+-------------+
| 9         | ELO         | 31        | VARNCHAR    |
|           |             |           |             |
+-----------+-------------+-----------+-------------+
| 10        | TIME        | 32        | DATETIME    |
|           |             |           |             |
+-----------+-------------+-----------+-------------+
| 11        | TIMESTAMP   | 33        | BLOB        |
|           |             |           |             |
+-----------+-------------+-----------+-------------+
| 12        | DATE        | 34        | CLOB        |
|           |             |           |             |
+-----------+-------------+-----------+-------------+
| 13        | MONETARY    | 35        | ENUM        |
|           |             |           |             |
+-----------+-------------+-----------+-------------+

**Character Sets Supported by CUBRID**

+-----------+------------------------------+
| **Value** | **Meaning**                  |
|           |                              |
+-----------+------------------------------+
| 0         | US English - ASCII encoding  |
|           |                              |
+-----------+------------------------------+
| 3         | Latin 1 - ISO 8859 encoding  |
|           |                              |
+-----------+------------------------------+
| 4         | KSC 5601 1990 - EUC encoding |
|           |                              |
+-----------+------------------------------+

**Example**

The following example shows how to retrieve user classes (from_class_of.is_system_class = 0) among the ones owned by user '**PUBLIC**'.'

.. code-block:: sql

	SELECT class_of.class_name, attr_name
	FROM _db_attribute
	WHERE class_of.owner.name = 'PUBLIC' AND FROM _class_of.is_system_class = 0
	ORDER BY 1, def_order;
	
	class_of.class_name   attr_name
	============================================
	  'female_event'        'code'
	  'female_event'        'sports'
	  'female_event'        'name'
	  'female_event'        'gender'
	  'female_event'        'players'

_db_domain
----------

Represents domain information. An index for object_of is created.

+--------------------+------------------------+---------------------------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type**          | **Description**                                                                                         |
|                    |                        |                                                                                                         |
+--------------------+------------------------+---------------------------------------------------------------------------------------------------------+
| object_of          | object                 | Attribute that refers to the domain, which can be a method parameter or domain                          |
|                    |                        |                                                                                                         |
+--------------------+------------------------+---------------------------------------------------------------------------------------------------------+
| data_type          | INTEGER                | Data type of the domain (a value in the "Value" column of the "Data Types Supported by CUBRID" table in |
|                    |                        | `_db_attribute <#syntax_syntax_catalog_class_dbat_4222>`_                                               |
|                    |                        | )                                                                                                       |
|                    |                        |                                                                                                         |
+--------------------+------------------------+---------------------------------------------------------------------------------------------------------+
| prec               | INTEGER                | Precision of the data type. 0 is used if the precision is not specified.                                |
|                    |                        |                                                                                                         |
+--------------------+------------------------+---------------------------------------------------------------------------------------------------------+
| scale              | INTEGER                | Scale of the data type. 0 is used if the scale is not specified.                                        |
|                    |                        |                                                                                                         |
+--------------------+------------------------+---------------------------------------------------------------------------------------------------------+
| class_of           | _db_class              | Domain class if the data type is an object,                                                             |
|                    |                        | **NULL**                                                                                                |
|                    |                        | otherwise.                                                                                              |
|                    |                        |                                                                                                         |
+--------------------+------------------------+---------------------------------------------------------------------------------------------------------+
| code_set           | INTEGER                | Character set (value of table "character sets supported by CUBRID" in                                   |
|                    |                        | `_db_attribute <#syntax_syntax_catalog_class_dbat_4222>`_                                               |
|                    |                        | ) if it is character data type. 0 otherwise.                                                            |
|                    |                        |                                                                                                         |
+--------------------+------------------------+---------------------------------------------------------------------------------------------------------+
| set_domains        | SEQUENCE OF _db_domain | Domain information about the data type of collection element if it is collection data type.             |
|                    |                        | **NULL**                                                                                                |
|                    |                        | otherwise.                                                                                              |
|                    |                        |                                                                                                         |
+--------------------+------------------------+---------------------------------------------------------------------------------------------------------+

_db_method
----------

Represents method information. Indexes for class_of and meth_name are created.

+--------------------+--------------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type**            | **Description**                                                                                                                               |
|                    |                          |                                                                                                                                               |
+--------------------+--------------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| class_of           | _db_class                | Class to which the method belongs                                                                                                             |
|                    |                          |                                                                                                                                               |
+--------------------+--------------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| meth_type          | INTEGER                  | Type of the method defined in the class. 0 for an instance method, and 1 for a class method.                                                  |
|                    |                          |                                                                                                                                               |
+--------------------+--------------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| from_class_of      | _db_class                | If the method is inherited, the super class in which it is defined is used otherwise                                                          |
|                    |                          | **NULL**                                                                                                                                      |
|                    |                          |                                                                                                                                               |
+--------------------+--------------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| from_meth_name     | VARCHAR(255)             | If the method is inherited and its name is changed to resolve a name conflict, the original name defined in the super class is used otherwise |
|                    |                          | **NULL**                                                                                                                                      |
|                    |                          |                                                                                                                                               |
+--------------------+--------------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| meth_name          | VARCHAR(255)             | Method name                                                                                                                                   |
|                    |                          |                                                                                                                                               |
+--------------------+--------------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| signatures         | SEQUENCE OF _db_meth_sig | C function executed when the method is called                                                                                                 |
|                    |                          |                                                                                                                                               |
+--------------------+--------------------------+-----------------------------------------------------------------------------------------------------------------------------------------------+

**Example**

The following example shows how to retrieve class methods of the class with a class method (c.class_meth_count > 0), among classes owned by user 'DBA.'

.. code-block:: sql

	SELECT class_name, SEQUENCE(SELECT meth_name
								FROM _db_method m
								WHERE m in c.class_meths)
	FROM _db_class c
	WHERE c.owner.name = 'DBA' AND c.class_meth_count > 0
	ORDER BY 1;
	
	  class_name            sequence((select meth_name from _db_method m where m in c.class_meths))
	============================================
	  'db_serial'           {'change_serial_owner'}
	  'db_authorizations'   {'add_user', 'drop_user', 'find_user', 'print_authorizations', 'info', 'change_owner', 'change_trigg
	r_owner', 'get_owner'}
	  'db_authorization'    {'check_authorization'}
	  'db_user'             {'add_user', 'drop_user', 'find_user', 'login'}
	  'db_root'             {'add_user', 'drop_user', 'find_user', 'print_authorizations', 'info', 'change_owner', 'change_trigg
	r_owner', 'get_owner', 'change_sp_owner'}

_db_meth_sig
------------

Represents configuration information of C functions on the method. An index for meth_of is created.

+--------------------+--------------------------+-----------------------------------------------+
| **Attribute Name** | **Data Type**            | **Description**                               |
|                    |                          |                                               |
+--------------------+--------------------------+-----------------------------------------------+
| meth_of            | _db_method               | Method for the function information           |
|                    |                          |                                               |
+--------------------+--------------------------+-----------------------------------------------+
| arg_count          | INTEGER                  | The number of input arguments of the function |
|                    |                          |                                               |
+--------------------+--------------------------+-----------------------------------------------+
| func_name          | VARCHAR(255)             | Function name                                 |
|                    |                          |                                               |
+--------------------+--------------------------+-----------------------------------------------+
| return_value       | SEQUENCE OF _db_meth_arg | Return value of the function                  |
|                    |                          |                                               |
+--------------------+--------------------------+-----------------------------------------------+
| arguments          | SEQUENCE OF _db_meth_arg | Input arguments of the function               |
|                    |                          |                                               |
+--------------------+--------------------------+-----------------------------------------------+

_db_meth_arg
------------

Represents method argument information. An index for meth_sig_of is created.

+--------------------+------------------------+-----------------------------------------------------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type**          | **Description**                                                                                                                   |
|                    |                        |                                                                                                                                   |
+--------------------+------------------------+-----------------------------------------------------------------------------------------------------------------------------------+
| meth_sig_of        | _db_meth_sig           | Information of the function to which the argument belongs                                                                         |
|                    |                        |                                                                                                                                   |
+--------------------+------------------------+-----------------------------------------------------------------------------------------------------------------------------------+
| data_type          | INTEGER                | Data type of the argument (a value in the "Value" column of the "Data Types Supported by CUBRID" in                               |
|                    |                        | `_db_attribute <#syntax_syntax_catalog_class_dbat_4222>`_                                                                         |
|                    |                        | )                                                                                                                                 |
|                    |                        |                                                                                                                                   |
+--------------------+------------------------+-----------------------------------------------------------------------------------------------------------------------------------+
| index_of           | INTEGER                | Order of the argument listed in the function definition. Begins with 0 if it is a return value, and 1 if it is an input argument. |
|                    |                        |                                                                                                                                   |
+--------------------+------------------------+-----------------------------------------------------------------------------------------------------------------------------------+
| domains            | SEQUENCE OF _db_domain | Domain of the argument                                                                                                            |
|                    |                        |                                                                                                                                   |
+--------------------+------------------------+-----------------------------------------------------------------------------------------------------------------------------------+

_db_meth_file
-------------

Represents information of a file in which a function is defined. An index for class_of is created.

+--------------------+---------------+-------------------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                                                                 |
|                    |               |                                                                                                 |
+--------------------+---------------+-------------------------------------------------------------------------------------------------+
| class_of           | _db_class     | Class to which the method file information belongs                                              |
|                    |               |                                                                                                 |
+--------------------+---------------+-------------------------------------------------------------------------------------------------+
| from_class_of      | _db_class     | If the file information is inherited, the super class in which it is defined is used otherwise, |
|                    |               | **NULL**                                                                                        |
|                    |               |                                                                                                 |
+--------------------+---------------+-------------------------------------------------------------------------------------------------+
| path_name          | VARCHAR(255)  | File path in which the method is located                                                        |
|                    |               |                                                                                                 |
+--------------------+---------------+-------------------------------------------------------------------------------------------------+

_db_query_spec
--------------

Represents the SQL statement of a virtual class. An index for class_of is created.

+--------------------+---------------+-----------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                               |
|                    |               |                                               |
+--------------------+---------------+-----------------------------------------------+
| class_of           | _db_class     | Class information of the virtual class        |
|                    |               |                                               |
+--------------------+---------------+-----------------------------------------------+
| spec               | VARCHAR(4096) | SQL definition statement of the virtual class |
|                    |               |                                               |
+--------------------+---------------+-----------------------------------------------+

_db_index
---------

Represents index information. An index for class_of is created.

+--------------------+---------------------------+------------------------------------------------+
| **Attribute Name** | **Data Type**             | **Description**                                |
|                    |                           |                                                |
+--------------------+---------------------------+------------------------------------------------+
| class_of           | _db_class                 | Class to which to index belongs                |
|                    |                           |                                                |
+--------------------+---------------------------+------------------------------------------------+
| index_name         | varchar(255)              | Index name                                     |
|                    |                           |                                                |
+--------------------+---------------------------+------------------------------------------------+
| is_unique          | INTEGER                   | 1 if the index is unique, and 0 otherwise.     |
|                    |                           |                                                |
+--------------------+---------------------------+------------------------------------------------+
| key_count          | INTEGER                   | The number of attributes that comprise the key |
|                    |                           |                                                |
+--------------------+---------------------------+------------------------------------------------+
| key_attrs          | SEQUENCE OF _db_index_key | Attributes that comprise the key               |
|                    |                           |                                                |
+--------------------+---------------------------+------------------------------------------------+
| is_reverse         | INTEGER                   | 1 for a reverse index, and 0 otherwise.        |
|                    |                           |                                                |
+--------------------+---------------------------+------------------------------------------------+
| is_primary_key     | INTEGER                   | 1 for a primary key, and 0 otherwise.          |
|                    |                           |                                                |
+--------------------+---------------------------+------------------------------------------------+
| is_foreign_key     | INTEGER                   | 1 for a foreign key, and 0 otherwise.          |
|                    |                           |                                                |
+--------------------+---------------------------+------------------------------------------------+
| filter_expression  | VARCHAR(255)              | The conditions of filtered indexes             |
|                    |                           |                                                |
+--------------------+---------------------------+------------------------------------------------+
| have_function      | INTEGER                   | 1 for a foreign key, and 0 otherwise.          |
|                    |                           |                                                |
+--------------------+---------------------------+------------------------------------------------+

**Example**

The following example shows how to retrieve names of indexes that belong to the class.

.. code-block:: sql

	SELECT class_of.class_name, index_name
	FROM _db_index
	ORDER BY 1;
	
	  class_of.class_name   index_name
	============================================
	  '_db_attribute'       'i__db_attribute_class_of_attr_name'
	  '_db_auth'            'i__db_auth_grantee'
	  '_db_class'           'i__db_class_class_name'
	  '_db_domain'          'i__db_domain_object_of'
	  '_db_index'           'i__db_index_class_of'
	  '_db_index_key'       'i__db_index_key_index_of'
	  '_db_meth_arg'        'i__db_meth_arg_meth_sig_of'
	  '_db_meth_file'       'i__db_meth_file_class_of'
	  '_db_meth_sig'        'i__db_meth_sig_meth_of'
	  '_db_method'          'i__db_method_class_of_meth_name'
	  '_db_partition'       'i__db_partition_class_of_pname'
	  '_db_query_spec'      'i__db_query_spec_class_of'
	  '_db_stored_procedure'  'u__db_stored_procedure_sp_name'
	  '_db_stored_procedure_args'  'i__db_stored_procedure_args_sp_name'
	  'athlete'             'pk_athlete_code'
	  'db_serial'           'pk_db_serial_name'
	  'db_user'             'i_db_user_name'
	  'event'               'pk_event_code'
	  'game'                'pk_game_host_year_event_code_athlete_code'
	  'game'                'fk_game_event_code'
	  'game'                'fk_game_athlete_code'
	  'history'             'pk_history_event_code_athlete'
	  'nation'              'pk_nation_code'
	  'olympic'             'pk_olympic_host_year'
	  'participant'         'pk_participant_host_year_nation_code'
	  'participant'         'fk_participant_host_year'
	  'participant'         'fk_participant_nation_code'
	  'record'              'pk_record_host_year_event_code_athlete_code_medal'
	  'stadium'             'pk_stadium_code'

_db_index_key
-------------

Represents key information on an index. An index for index_of is created.

+--------------------+---------------+--------------------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                                    |
|                    |               |                                                                    |
+--------------------+---------------+--------------------------------------------------------------------+
| index_of           | _db_index     | Index to which the key attribute belongs                           |
|                    |               |                                                                    |
+--------------------+---------------+--------------------------------------------------------------------+
| key_attr_name      | VARCHAR(255)  | Name of the attribute that comprises the key                       |
|                    |               |                                                                    |
+--------------------+---------------+--------------------------------------------------------------------+
| key_order          | INTEGER       | Order of the attribute in the key. Begins with 0.                  |
|                    |               |                                                                    |
+--------------------+---------------+--------------------------------------------------------------------+
| asc_desc           | INTEGER       | 1 if the order of attribute values is descending, and 0 otherwise. |
|                    |               |                                                                    |
+--------------------+---------------+--------------------------------------------------------------------+
| key_prefix_length  | INTEGER       | Length of prefix to be used as a key                               |
|                    |               |                                                                    |
+--------------------+---------------+--------------------------------------------------------------------+
| func               | VARCHAR(255)  | Functional expression of function based index                      |
|                    |               |                                                                    |
+--------------------+---------------+--------------------------------------------------------------------+

**Example**

The following example shows how to retrieve the names of index that belongs to the class.

.. code-block:: sql

	SELECT class_of.class_name, SEQUENCE(SELECT key_attr_name
										 FROM _db_index_key k
										 WHERE k in i.key_attrs)
	FROM _db_index i
	WHERE key_count >= 2;
	
	  class_of.class_name   sequence((select key_attr_name from _db_index_key k where k in
	i.key_attrs))
	============================================
	  '_db_partition'       {'class_of', 'pname'}
	  '_db_method'          {'class_of', 'meth_name'}
	  '_db_attribute'       {'class_of', 'attr_name'}
	  'participant'         {'host_year', 'nation_code'}
	  'game'                {'host_year', 'event_code', 'athlete_code'}
	  'record'              {'host_year', 'event_code', 'athlete_code', 'medal'}
	  'history'             {'event_code', 'athlete'}

_db_auth
--------

Represents user authorization information of the class. An index for the grantee is created.

+--------------------+---------------+----------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                                                  |
|                    |               |                                                                                  |
+--------------------+---------------+----------------------------------------------------------------------------------+
| grantor            | db_user       | Authorization grantor                                                            |
|                    |               |                                                                                  |
+--------------------+---------------+----------------------------------------------------------------------------------+
| grantee            | db_user       | Authorization grantee                                                            |
|                    |               |                                                                                  |
+--------------------+---------------+----------------------------------------------------------------------------------+
| class_of           | _db_class     | Class object to which authorization is to be granted                             |
|                    |               |                                                                                  |
+--------------------+---------------+----------------------------------------------------------------------------------+
| auth_type          | VARCHAR(7)    | Type name of the authorization granted                                           |
|                    |               |                                                                                  |
+--------------------+---------------+----------------------------------------------------------------------------------+
| is_grantable       | INTEGER       | 1 if authorization for the class can be granted to other users, and 0 otherwise. |
|                    |               |                                                                                  |
+--------------------+---------------+----------------------------------------------------------------------------------+

Authorization types supported by CUBRID are as follows:

*   **SELECT**
*   **INSERT**
*   **UPDATE**
*   **DELETE**
*   **ALTER**
*   **INDEX**
*   **EXECUTE**

**Example**

The following example shows how to retrieve authorization information defined in the class *db_trig*.

.. code-block:: sql

	SELECT grantor.name, grantee.name, auth_type
	FROM _db_auth
	WHERE class_of.class_name = 'db_trig';

	  grantor.name          grantee.name          auth_type
	==================================================================
	  'DBA'                 'PUBLIC'              'SELECT'

_db_data_type
-------------

Represents the data type supported by CUBRID (see the "Data Types Supported by CUBRID" table in `_db_attribute <#syntax_syntax_catalog_class_dbat_4222>`_).

+--------------------+---------------+--------------------------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                                                                        |
|                    |               |                                                                                                        |
+--------------------+---------------+--------------------------------------------------------------------------------------------------------+
| type_id            | INTEGER       | Data type identifier. Corresponds to the "Value" column in the "Data Types Supported by CUBRID" table. |
|                    |               |                                                                                                        |
+--------------------+---------------+--------------------------------------------------------------------------------------------------------+
| type_name          | VARCHAR(9)    | Data type name. Corresponds to the "Meaning" column in the "Data Types Supported by CUBRID" table.     |
|                    |               |                                                                                                        |
+--------------------+---------------+--------------------------------------------------------------------------------------------------------+

**Example**

The following example shows how to retrieve attributes and type names of the *event* class.

.. code-block:: sql

	SELECT a.attr_name, t.type_name
	FROM _db_attribute a join _db_data_type t ON a.data_type = t.type_id
	WHERE class_of.class_name = 'event'
	ORDER BY a.def_order;

	  attr_name             type_name
	============================================
	  'code'                'INTEGER'
	  'sports'              'STRING'
	  'name'                'STRING'
	  'gender'              'CHAR'
	  'players'             'INTEGER'

_db_partition
-------------

Represents partition information. Indexes for class_of and pname are created.

+--------------------+---------------+---------------------------------+
| **Attribute Name** | **Data Type** | **Description**                 |
|                    |               |                                 |
+--------------------+---------------+---------------------------------+
| class_of           | _db_class     | OID of the parent class         |
|                    |               |                                 |
+--------------------+---------------+---------------------------------+
| pname              | VARCHAR(255)  | Parent -                        |
|                    |               | **NULL**                        |
|                    |               |                                 |
+--------------------+---------------+---------------------------------+
| ptype              | INTEGER       | 0 - HASH                        |
|                    |               | 1 - RANGE                       |
|                    |               | 2 - LIST                        |
|                    |               |                                 |
+--------------------+---------------+---------------------------------+
| pexpr              | VARCHAR(255)  | Parent only                     |
|                    |               |                                 |
+--------------------+---------------+---------------------------------+
| pvalues            | SEQUENCE OF   | Parent - Column name, Hash size |
|                    |               | RANGE - MIN/MAX value :         |
|                    |               | - Infinite MIN/MAX is stored as |
|                    |               | **NULL**                        |
|                    |               | LIST - value list               |
|                    |               |                                 |
+--------------------+---------------+---------------------------------+

_db_stored_procedure
--------------------

Represents Java stored procedure information. An index for sp_name is created.

+--------------------+---------------------------------------+-------------------------------------------+
| **Attribute Name** | **Data Type**                         | **Description**                           |
|                    |                                       |                                           |
+--------------------+---------------------------------------+-------------------------------------------+
| sp_name            | VARCHAR(255)                          | Stored procedure name                     |
|                    |                                       |                                           |
+--------------------+---------------------------------------+-------------------------------------------+
| sp_type            | INTEGER                               | Stored procedure type                     |
|                    |                                       | (function or procedure)                   |
|                    |                                       |                                           |
+--------------------+---------------------------------------+-------------------------------------------+
| return_type        | INTEGER                               | Return value type                         |
|                    |                                       |                                           |
+--------------------+---------------------------------------+-------------------------------------------+
| arg_count          | INTEGER                               | The number of arguments                   |
|                    |                                       |                                           |
+--------------------+---------------------------------------+-------------------------------------------+
| args               | SEQUENCE OF _db_stored_procedure_args | Argument list                             |
|                    |                                       |                                           |
+--------------------+---------------------------------------+-------------------------------------------+
| lang               | INTEGER                               | Implementation language (currently, Java) |
|                    |                                       |                                           |
+--------------------+---------------------------------------+-------------------------------------------+
| target             | VARCHAR(4096)                         | Name of the Java method to be executed    |
|                    |                                       |                                           |
+--------------------+---------------------------------------+-------------------------------------------+
| owner              | db_user                               | Owner                                     |
|                    |                                       |                                           |
+--------------------+---------------------------------------+-------------------------------------------+

_db_stored_procedure_args
-------------------------

Represents Java stored procedure argument information. An index for sp_name is created.

+--------------------+---------------+---------------------------+
| **Attribute Name** | **Data Type** | **Description**           |
|                    |               |                           |
+--------------------+---------------+---------------------------+
| sp_name            | VARCHAR(255)  | Stored procedure name     |
|                    |               |                           |
+--------------------+---------------+---------------------------+
| index_of           | INTEGER       | Order of the arguments    |
|                    |               |                           |
+--------------------+---------------+---------------------------+
| arg_name           | VARCHAR(255)  | Argument name             |
|                    |               |                           |
+--------------------+---------------+---------------------------+
| data_type          | INTEGER       | Data type of the argument |
|                    |               |                           |
+--------------------+---------------+---------------------------+
| mode               | INTEGER       | Mode (IN, OUT, INOUT)     |
|                    |               |                           |
+--------------------+---------------+---------------------------+

_db_collation
-------------

The information on collation.

+--------------------+---------------+-----------------------------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                                             |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+
| coll_id            | INTEGER       | Collation ID                                                                |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+
| coll_name          | VARCHAR(32)   | Collation name                                                              |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+
| charset_id         | INTEGER       | Charset ID                                                                  |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+
| built_in           | INTEGER       | Built-in or not while installing the product (0: Not built-in, 1: Built-in) |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+
| expansions         | INTEGER       | Expansion support (0: Not supported, 1: Supported)                          |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+
| contractions       | INTEGER       | Contraction support (0: Not supported, 1: Supported)                        |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+
| checksum           | VARCHAR(32)   | Checksum of a collation file                                                |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+
| uca_strength       | INTEGER       | Weight strength                                                             |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+

db_user
-------

+--------------------+---------------------+---------------------------------------------------------+
| **Attribute Name** | **Data Type**       | **Description**                                         |
|                    |                     |                                                         |
+--------------------+---------------------+---------------------------------------------------------+
| name               | VARCHAR(1073741823) | User name                                               |
|                    |                     |                                                         |
+--------------------+---------------------+---------------------------------------------------------+
| id                 | INTEGER             | User identifier                                         |
|                    |                     |                                                         |
+--------------------+---------------------+---------------------------------------------------------+
| password           | db_password         | User password. Not displayed to the user.               |
|                    |                     |                                                         |
+--------------------+---------------------+---------------------------------------------------------+
| direct_groups      | SET OF db_user      | Groups to which the user belongs directly               |
|                    |                     |                                                         |
+--------------------+---------------------+---------------------------------------------------------+
| groups             | SET OF db_user      | Groups to which the user belongs directly or indirectly |
|                    |                     |                                                         |
+--------------------+---------------------+---------------------------------------------------------+
| authorization      | db_authorization    | Information of the authorization owned by the user      |
|                    |                     |                                                         |
+--------------------+---------------------+---------------------------------------------------------+
| triggers           | SEQUENCE OF object  | Triggers that occur due to user actions                 |
|                    |                     |                                                         |
+--------------------+---------------------+---------------------------------------------------------+

**Function Names**

*   **set_password** ()
*   **set_password_encoded** ()
*   **add_member** ()
*   **drop_member** ()
*   **print_authorizations** ()
*   **add_user** ()
*   **drop_user** ()
*   **find_user** ()
*   **login** ()

db_authorization
----------------

+--------------------+--------------------+--------------------------------------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type**      | **Description**                                                                                                    |
|                    |                    |                                                                                                                    |
+--------------------+--------------------+--------------------------------------------------------------------------------------------------------------------+
| owner              | db_user            | User information                                                                                                   |
|                    |                    |                                                                                                                    |
+--------------------+--------------------+--------------------------------------------------------------------------------------------------------------------+
| grants             | SEQUENCE OF object | Sequence of {object for which the user has authorization, authorization grantor of the object, authorization type} |
|                    |                    |                                                                                                                    |
+--------------------+--------------------+--------------------------------------------------------------------------------------------------------------------+

**Method Name**

*   **check_authorization** (varchar(255), integer)

db_trigger
----------

+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **Attribute Name**     | **Data Type**       | **Description**                                                                                                                                            |
|                        |                     |                                                                                                                                                            |
+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| owner                  | db_user             | Trigger owner                                                                                                                                              |
|                        |                     |                                                                                                                                                            |
+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| name                   | VARCHAR(1073741823) | Trigger name                                                                                                                                               |
|                        |                     |                                                                                                                                                            |
+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| status                 | INTEGER             | 1 for INACTIVE, and 2 for ACTIVE. The default value is 2.                                                                                                  |
|                        |                     |                                                                                                                                                            |
+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| priority               | DOUBLE              | Execution priority between triggers. The default value is 0.                                                                                               |
|                        |                     |                                                                                                                                                            |
+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| event                  | INTEGER             | 0 is set for UPDATE, 1 for UPDATE STATEMENT, 2 for DELETE, 3 for DELETE STATEMENT, 4 for INSERT, 5 for INSERT STATEMENT, 8 for COMMIT, and 9 for ROLLBACK. |
|                        |                     |                                                                                                                                                            |
+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| target_class           | object              | Class object for the trigger target class                                                                                                                  |
|                        |                     |                                                                                                                                                            |
+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| target_attribute       | VARCHAR(1073741823) | Trigger target attribute name. If the target attribute is not specified,                                                                                   |
|                        |                     | **NULL**                                                                                                                                                   |
|                        |                     | is used.                                                                                                                                                   |
|                        |                     |                                                                                                                                                            |
+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| target_class_attribute | INTEGER             | If the target attribute is an instance attribute, 0 is used. If it is a class attribute, 1 is used. The default value is 0.                                |
|                        |                     |                                                                                                                                                            |
+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| condition_type         | INTEGER             | If a condition exist, 1; otherwise                                                                                                                         |
|                        |                     | **NULL**                                                                                                                                                   |
|                        |                     | .                                                                                                                                                          |
|                        |                     |                                                                                                                                                            |
+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| condition              | VARCHAR(1073741823) | Action condition specified in the IF statement                                                                                                             |
|                        |                     |                                                                                                                                                            |
+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| condition_time         | INTEGER             | 1 for BEFORE, 2 for AFTER, and 3 for DEFERRED if a condition exists;                                                                                       |
|                        |                     | **NULL**                                                                                                                                                   |
|                        |                     | , otherwise.                                                                                                                                               |
|                        |                     |                                                                                                                                                            |
+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| action_type            | INTEGER             | 1 for one of INSERT, UPDATE, DELETE, and CALL, 2 for REJECT, 3 for INVALIDATE_TRANSACTION, and 4 for PRINT.                                                |
|                        |                     |                                                                                                                                                            |
+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| action_definition      | VARCHAR(1073741823) | Execution statement to be triggered                                                                                                                        |
|                        |                     |                                                                                                                                                            |
+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+
| action_time            | INTEGER             | 1 for BEFORE, 2 for AFTER, and 3 for DEFERRED.                                                                                                             |
|                        |                     |                                                                                                                                                            |
+------------------------+---------------------+------------------------------------------------------------------------------------------------------------------------------------------------------------+

db_ha_apply_info
----------------

A table that stores the progress status every time the **applylogdb** utility applies replication logs. This table is updated at every point the **applylogdb** utility commits, and the acculmative count of operations are stored in the \*_counter column. The meaning of each column is as follows:

+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| **Column Name**      | **Column Type** | **Description**                                                                                                                                    |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| db_name              | VARCHAR(255)    | Name of the database stored in the log                                                                                                             |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| db_creation_time     | DATETIME        | Creation time of the source database for the log to be applied                                                                                     |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| copied_log_path      | VARCHAR(4096)   | Path to the log file to be applied                                                                                                                 |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| committed_lsa_pageid | BIGINT          | The page id of commit log lsa reflected last.                                                                                                      |
|                      |                 | Although applylogdb is restarted, the logs before last_committed_lsa are not reflected again.                                                      |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| committed_lsa_offset | INTEGER         | The offset of commit log lsa reflected last.                                                                                                       |
|                      |                 | Although applylogdb is restarted, the logs before last_committed_lsa are not reflected again.                                                      |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| committed_rep_pageid | BIGINT          | The page id of the replication log lsa reflected last.                                                                                             |
|                      |                 | Check whether the reflection of replication has been delayed or not.                                                                               |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| committed_rep_offset | INTEGER         | The offset of the replication log lsa reflected last.                                                                                              |
|                      |                 | Check whether the reflection of replication has been delayed or not.                                                                               |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| append_lsa_page_id   | BIGINT          | The page id of the last replication log lsa at the last reflection.                                                                                |
|                      |                 | Saves append_lsa of the replication log header that is being processed by applylogdb at the time of reflecting the replication.                    |
|                      |                 | Checks whether the reflection of replication has been delayed or not at the time of reflecting the replication log.                                |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| append_lsa_offset    | INTEGER         | The offset of the last replication log lsa at the last refelction.                                                                                 |
|                      |                 | Saves append_lsa of the replication log header that is being processed by applylogdb at the time of reflecting the replication.                    |
|                      |                 | Checks whether the reflection of replication has been delayed or not at the time of reflecting the replication log.                                |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| eof_lsa_page_id      | BIGINT          | The page id of the replication log eof lsa at the last reflection.                                                                                 |
|                      |                 | Saves eof_lsa of the replication log header that is being processed by applylogdb at the time of reflecting the replication.                       |
|                      |                 | Checks whether the reflection of replication has been delayed or not at the time of reflecting the replication log.                                |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| eof_lsa_offset       | INTEGER         | The offset of the replication log eof lsa at the last reflection.                                                                                  |
|                      |                 | Saves eof_lsa of the replication log header that is being processed by applylogdb at the time of reflecting the replication.                       |
|                      |                 | Checks whether the reflection of replication has been delayed or not at the time of reflecting the replication log.                                |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| final_lsa_pageid     | BIGINT          | The pageid of replication log lsa processed last by applylogdb.                                                                                    |
|                      |                 | Checks whether the reflection of replication has been delayed or not.                                                                              |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| final_lsa_offset     | INTEGER         | The offset of replication log lsa processed last by applylogdb.                                                                                    |
|                      |                 | Checks whether the reflection of replication has been delayed or not.                                                                              |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| required_page_id     | BIGINT          | The smallest page which should not be deleted by the log_max_archives parameter. The log page number from which the replication will be reflected. |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| required_page_offset | INTEGER         | The offset of the log page from which the replication will be reflected.                                                                           |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| log_record_time      | DATETIME        | Timestamp included in replication log committed in the slave database, i.e. the creation time of the log                                           |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| log_commit_time      | DATETIME        | The time of reflecting the last commit log                                                                                                         |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| last_access_time     | DATETIME        | The final update time of the db_ha_apply_info catalog                                                                                              |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| status               | INTEGER         | Progress status (0: IDLE, 1: BUSY)                                                                                                                 |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| insert_counter       | BIGINT          | Number of times that applylogdb was inserted                                                                                                       |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| update_counter       | BIGINT          | Number of times that applylogdb was updated                                                                                                        |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| delete_counter       | BIGINT          | Number of times that applylogdb was deleted                                                                                                        |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| schema_counter       | BIGINT          | Number of times that applylogdb changed the schema                                                                                                 |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| commit_counter       | BIGINT          | Number of times that applylogdb was committed                                                                                                      |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| fail_counter         | BIGINT          | Number of times that applylogdb failed to be inserted/updated/deleted/committed and to change the schema                                           |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+
| start_time           | DATETIME        | Time when the applylogdb process accessed the slave database                                                                                       |
|                      |                 |                                                                                                                                                    |
+----------------------+-----------------+----------------------------------------------------------------------------------------------------------------------------------------------------+

System Catalog Virtual Class
============================

General users can only see information of classes for which they have authorization through system catalog virtual classes. This section explains which information each system catalog virtual class represents, and virtual class definition statements.

DB_CLASS
--------

Represents information of classes for which the current user has access authorization to a database.

+--------------------+---------------+----------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                          |
|                    |               |                                                          |
+--------------------+---------------+----------------------------------------------------------+
| class_name         | VARCHAR(255)  | Class name                                               |
|                    |               |                                                          |
+--------------------+---------------+----------------------------------------------------------+
| owner_name         | VARCHAR(255)  | Name of class owner                                      |
|                    |               |                                                          |
+--------------------+---------------+----------------------------------------------------------+
| class_type         | VARCHAR(6)    | 'CLASS' for a class, and 'VCLASS' for a virtual class    |
|                    |               |                                                          |
+--------------------+---------------+----------------------------------------------------------+
| is_system_class    | VARCHAR(3)    | 'YES' for a system class, and 'NO' otherwise.            |
|                    |               |                                                          |
+--------------------+---------------+----------------------------------------------------------+
| partitioned        | VARCHAR(3)    | 'YES' for a partitioned group class, and 'NO' otherwise. |
|                    |               |                                                          |
+--------------------+---------------+----------------------------------------------------------+
| is_reuse_oid_class | VARCHAR(3)    | 'YES' for a REUSE_OID class, and 'NO' otherwise.         |
|                    |               |                                                          |
+--------------------+---------------+----------------------------------------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_class (class_name, owner_name, class_type, is_system_class, partitioned, is_reuse_oid_class)
	AS
	 
	SELECT c.class_name, CAST(c.owner.name AS VARCHAR(255)),
		CASE c.class_type WHEN 0 THEN 'CLASS' WHEN 1 THEN 'VCLASS' ELSE 'UNKNOW' END,
		CASE WHEN MOD(c.is_system_class, 2) = 1 THEN 'YES' ELSE 'NO' END,
		CASE WHEN c.sub_classes IS NULL THEN 'NO' ELSE NVL((SELECT 'YES' FROM _db_partition p WHERE p.class_of = c and p.pname IS NULL), 'NO') END,
		CASE WHEN MOD(c.is_system_class / 8, 2) = 1 THEN 'YES' ELSE 'NO' END
	FROM _db_class c
	WHERE CURRENT_USER = 'DBA' OR
		{c.owner.name} SUBSETEQ (  
			SELECT SET{CURRENT_USER} + COALESCE(SUM(SET{t.g.name}), SET{})  
			FROM db_user u, TABLE(groups) AS t(g)  
			WHERE u.name = CURRENT_USER) OR
		{c} SUBSETEQ (
			SELECT SUM(SET{au.class_of})  
			FROM _db_auth au  
			WHERE {au.grantee.name} SUBSETEQ(  
				SELECT SET{CURRENT_USER} + COALESCE(SUM(SET{t.g.name}), SET{})
				FROM db_user u, TABLE(groups) AS t(g)  
				WHERE u.name = CURRENT_USER) AND  au.auth_type = 'SELECT');

The following example shows how to retrieve classes owned by the current user.

.. code-block:: sql

	SELECT class_name
	FROM db_class
	WHERE owner_name = CURRENT_USER;

	  class_name
	======================
	  'stadium'
	  'code'
	  'nation'
	  'event'
	  'athlete'
	  'participant'
	  'olympic'
	  'game'
	  'record'
	  'history'
	'female_event'

The following example shows how to retrieve virtual classes that can be accessed by the current user.

.. code-block:: sql

	SELECT class_name
	FROM db_class
	WHERE class_type = 'VCLASS';

	  class_name
	======================
	  'db_stored_procedure_args'
	  'db_stored_procedure'
	  'db_partition'
	  'db_trig'
	  'db_auth'
	  'db_index_key'
	  'db_index'
	  'db_meth_file'
	  'db_meth_arg_setdomain_elm'
	  'db_meth_arg'
	  'db_method'
	  'db_attr_setdomain_elm'
	  'db_attribute'
	  'db_vclass'
	  'db_direct_super_class'
	  'db_class'

The following example shows how to retrieve system classes that can be accessed by the current user (**PUBLIC** user).

.. code-block:: sql

	SELECT class_name
	FROM db_class
	WHERE is_system_class = 'YES' AND class_type = 'CLASS'
	ORDER BY 1;
	
	  class_name
	======================
	  'db_authorization'
	  'db_authorizations'
	  'db_root'
	  'db_serial'
	  'db_user'

DB_DIRECT_SUPER_CLASS
---------------------

Represents the names of super classes (if any) of the class for which the current user has access authorization to a database.

+--------------------+---------------+------------------+
| **Attribute Name** | **Data Type** | **Description**  |
|                    |               |                  |
+--------------------+---------------+------------------+
| class_name         | VARCHAR(255)  | Class name       |
|                    |               |                  |
+--------------------+---------------+------------------+
| super_class_name   | VARCHAR(255)  | super class name |
|                    |               |                  |
+--------------------+---------------+------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_direct_super_class (class_name, super_class_name)
	AS
	SELECT c.class_name, s.class_name
	FROM _db_class c, TABLE(c.super_classes) AS t(s)
	WHERE (CURRENT_USER = 'DBA' OR
			{c.owner.name} subseteq (
					SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
					from db_user u, table(groups) as t(g)
					where u.name = CURRENT_USER ) OR
			{c} subseteq (
	SELECT sum(set{au.class_of})
					FROM _db_auth au
					WHERE {au.grantee.name} subseteq (
								SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
								from db_user u, table(groups) as t(g)
								where u.name = CURRENT_USER ) AND
										au.auth_type = 'SELECT'));

The following example shows how to retrieve super classes of the *female_event* class (see `ADD SUPERCLASS Clause <#syntax_syntax_table_inherit_add__5365>`_).

.. code-block:: sql

	SELECT super_class_name
	FROM db_direct_super_class
	WHERE class_name = 'female_event';
	
	  super_class_name
	======================
	  'event'

The following example shows how to retrieve super classes of the class owned by the current user (**PUBLIC** user).

.. code-block:: sql

	SELECT c.class_name, s.super_class_name
	FROM db_class c, db_direct_super_class s
	WHERE c.class_name = s.class_name AND c.owner_name = user
	ORDER BY 1;
	
	  class_name            super_class_name
	============================================
	  'female_event'        'event'

DB_VCLASS
---------

Represents SQL definition statements of virtual classes for which the current user has access authorization to a database.

+--------------------+---------------+-----------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                               |
|                    |               |                                               |
+--------------------+---------------+-----------------------------------------------+
| vclass_name        | VARCHAR(255)  | Virtual class name                            |
|                    |               |                                               |
+--------------------+---------------+-----------------------------------------------+
| vclass_def         | VARCHAR(4096) | SQL definition statement of the virtual class |
|                    |               |                                               |
+--------------------+---------------+-----------------------------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_vclass (vclass_name, vclass_def)
	AS
	SELECT q.class_of.class_name, q.spec
	FROM _db_query_spec q
	WHERE CURRENT_USER = 'DBA' OR
			{q.class_of.owner.name} subseteq (
					SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
					from db_user u, table(groups) as t(g)
					where u.name = CURRENT_USER ) OR
			{q.class_of} subseteq (
	SELECT sum(set{au.class_of})
					FROM _db_auth au
					WHERE {au.grantee.name} subseteq (
								SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
								from db_user u, table(groups) as t(g)
								where u.name = CURRENT_USER ) AND
										au.auth_type = 'SELECT');

The following example shows how to retrieve SQL definition statements of the *db_class* virtual class.

.. code-block:: sql

	SELECT vclass_def
	FROM db_vclass
	WHERE vclass_name = 'db_class';
	
	'SELECT c.class_name, CAST(c.owner.name AS VARCHAR(255)), CASE c.class_type WHEN 0 THEN 'CLASS' WHEN 1 THEN 'VCLASS' WHEN 2 THEN 'PROXY' ELSE 'UNKNOW' END, CASE WHEN MOD(c.is_system_class, 2) = 1 THEN 'YES' ELSE 'NO' END, CASE WHEN c.sub_classes IS NULL THEN 'NO' ELSE NVL((SELECT 'YES' FROM _db_partition p WHERE p.class_of = c and p.pname IS NULL), 'NO') END FROM _db_class c WHERE CURRENT_USER = 'DBA' OR {c.owner.name} SUBSETEQ (  SELECT SET{CURRENT_USER} + COALESCE(SUM(SET{t.g.name}), SET{})  FROM db_user u, TABLE(groups) AS t(g)  WHERE u.name = CURRENT_USER) OR {c} SUBSETEQ (  SELECT SUM(SET{au.class_of})  FROM _db_auth au  WHERE {au.grantee.name} SUBSETEQ (  SELECT SET{CURRENT_USER} + COALESCE(SUM(SET{t.g.name}), SET{})  FROM db_user u, TABLE(groups) AS t(g)  WHERE u.name = CURRENT_USER) AND  au.auth_type = 'SELECT')'

DB_ATTRIBUTE
------------

Represents the attribute information of a class for which the current user has access authorization in the database.

+--------------------+---------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                                                                                                                                           |
|                    |               |                                                                                                                                                                           |
+--------------------+---------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| attr_name          | VARCHAR(255)  | Attribute name                                                                                                                                                            |
|                    |               |                                                                                                                                                                           |
+--------------------+---------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| class_name         | VARCHAR(255)  | Name of the class to which the attribute belongs                                                                                                                          |
|                    |               |                                                                                                                                                                           |
+--------------------+---------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| attr_type          | VARCHAR(8)    | 'INSTANCE' for an instance attribute, 'CLASS' for a class attribute, and 'SHARED' for a shared attribute.                                                                 |
|                    |               |                                                                                                                                                                           |
+--------------------+---------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| def_order          | INTEGER       | Order of attributes in the class. Begins with 0. If the attribute is inherited, the order is the one defined in the super class.                                          |
|                    |               |                                                                                                                                                                           |
+--------------------+---------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| from_class_name    | VARCHAR(255)  | If the attribute is inherited, the super class in which it is defined is used. Otherwise,                                                                                 |
|                    |               | **NULL**                                                                                                                                                                  |
|                    |               |                                                                                                                                                                           |
+--------------------+---------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| from_attr_name     | VARCHAR(255)  | If the attribute is inherited and its name is changed to resolve a name conflict, the original name defined in the super class is used. Otherwise,                        |
|                    |               | **NULL**                                                                                                                                                                  |
|                    |               |                                                                                                                                                                           |
+--------------------+---------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| data_type          | VARCHAR(9)    | Data type of the attribute (one in the "Meaning" column of the "Data Types Supported by CUBRID" table in                                                                  |
|                    |               | `_db_attribute <#syntax_syntax_catalog_class_dbat_4222>`_                                                                                                                 |
|                    |               | )                                                                                                                                                                         |
|                    |               |                                                                                                                                                                           |
+--------------------+---------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| prec               | INTEGER       | Precision of the data type. 0 is used if the precision is not specified.                                                                                                  |
|                    |               |                                                                                                                                                                           |
+--------------------+---------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| scale              | INTEGER       | Scale of the data type. 0 is used if the scale is not specified.                                                                                                          |
|                    |               |                                                                                                                                                                           |
+--------------------+---------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| code_set           | INTEGER       | Character set (value of table "character sets supported by CUBRID" in                                                                                                     |
|                    |               | `_db_attribute <#syntax_syntax_catalog_class_dbat_4222>`_                                                                                                                 |
|                    |               | ) if it is string type. 0 otherwise.                                                                                                                                      |
|                    |               |                                                                                                                                                                           |
+--------------------+---------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| domain_class_name  | VARCHAR(255)  | Domain class name if the data type is an object.                                                                                                                          |
|                    |               | **NULL**                                                                                                                                                                  |
|                    |               | otherwise.                                                                                                                                                                |
|                    |               |                                                                                                                                                                           |
+--------------------+---------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| default_value      | VARCHAR(255)  | Saved as a character string by default, regardless of data types. If no default value is specified,                                                                       |
|                    |               | **NULL**                                                                                                                                                                  |
|                    |               | is stored if a default value is                                                                                                                                           |
|                    |               | **NULL**                                                                                                                                                                  |
|                    |               | , it is displayed as 'NULL'. An object data type is represented as 'volume id | page id | slot id' while a set data type is represented as '{element 1, element 2, ... '. |
|                    |               |                                                                                                                                                                           |
+--------------------+---------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+
| is_nullable        | VARCHAR(3)    | 'NO' if a not null constraint is set, and 'YES' otherwise.                                                                                                                |
|                    |               |                                                                                                                                                                           |
+--------------------+---------------+---------------------------------------------------------------------------------------------------------------------------------------------------------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_attribute (
	attr_name, class_name, attr_type, def_order, from_class_name, from_attr_name, data_type, prec, scale, code_set, domain_class_name, default_value, is_nullable)
	AS
	SELECT a.attr_name, c.class_name,
		   CASE WHEN a.attr_type = 0 THEN 'INSTANCE'
				WHEN a.attr_type = 1 THEN 'CLASS'
				ELSE 'SHARED' END,
		   a.def_order, a.from_class_of.class_name, a.from_attr_name, t.type_name,
		   d.prec, d.scale, d.code_set, d.class_of.class_name, a.default_value,
		   CASE WHEN a.is_nullable = 0 THEN 'YES' ELSE 'NO' END
	FROM _db_class c, _db_attribute a, _db_domain d, _db_data_type t
	WHERE a.class_of = c AND d.object_of = a AND d.data_type = t.type_id AND
			(CURRENT_USER = 'DBA' OR
			{c.owner.name} subseteq (
					SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
					from db_user u, table(groups) as t(g)
					where u.name = CURRENT_USER ) OR
			{c} subseteq (
	SELECT sum(set{au.class_of})
					FROM _db_auth au
					WHERE {au.grantee.name} subseteq (
								SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
								from db_user u, table(groups) as t(g)
								where u.name = CURRENT_USER ) AND
										au.auth_type = 'SELECT'));

The following example shows how to retrieve attributes and data types of the *event* class.

.. code-block:: sql

	SELECT attr_name, data_type, domain_class_name
	FROM db_attribute
	WHERE class_name = 'event'
	ORDER BY def_order;
	
	  attr_name             data_type             domain_class_name
	==================================================================
	  'code'                'INTEGER'             NULL
	  'sports'              'STRING'              NULL
	  'name'                'STRING'              NULL
	  'gender'              'CHAR'                NULL
	  'players'             'INTEGER'             NULL

The following example shows how to retrieve attributes of the *female_event* class and its super class.

.. code-block:: sql

	SELECT attr_name, from_class_name
	FROM db_attribute
	WHERE class_name = 'female_event'
	ORDER BY def_order;
	
	  attr_name             from_class_name
	============================================
	  'code'                'event'
	  'sports'              'event'
	  'name'                'event'
	  'gender'              'event'
	  'players'             'event'

The following example shows how to retrieve classes whose attribute names are similar to *name*, among the ones owned by the current user. (The user is **PUBLIC**.)

.. code-block:: sql

	SELECT a.class_name, a.attr_name
	FROM db_class c join db_attribute a ON c.class_name = a.class_name
	WHERE c.owner_name = CURRENT_USER AND attr_name like '%name%'
	ORDER BY 1;
	
	  class_name            attr_name
	============================================
	  'athlete'             'name'
	  'code'                'f_name'
	  'code'                's_name'
	  'event'               'name'
	  'female_event'        'name'
	  'nation'              'name'
	  'stadium'             'name'

DB_ATTR_SETDOMAIN_ELM
---------------------

Among attributes of the class to which the current user has access authorization in the database, if an attribute's data type is a collection (set, multiset, sequence), this macro represents the data type of the element of the collection.

+--------------------+---------------+-----------------------------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                                                                           |
|                    |               |                                                                                                           |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------+
| attr_name          | VARCHAR(255)  | Attribute name                                                                                            |
|                    |               |                                                                                                           |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------+
| class_name         | VARCHAR(255)  | Name of the class to which the attribute belongs                                                          |
|                    |               |                                                                                                           |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------+
| attr_type          | VARCHAR(8)    | 'INSTANCE' for an instance attribute, 'CLASS' for a class attribute, and 'SHARED' for a shared attribute. |
|                    |               |                                                                                                           |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------+
| data_type          | VARCHAR(9)    | Data type of the element                                                                                  |
|                    |               |                                                                                                           |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------+
| prec               | INTEGER       | Precision of the data type of the element                                                                 |
|                    |               |                                                                                                           |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------+
| scale              | INTEGER       | Scale of the data type of the element                                                                     |
|                    |               |                                                                                                           |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------+
| code_set           | INTEGER       | Character set if the data type of the element is a character                                              |
|                    |               |                                                                                                           |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------+
| domain_class_name  | VARCHAR(255)  | Domain class name if the data type of the element is an object                                            |
|                    |               |                                                                                                           |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_attr_setdomain_elm (
	attr_name, class_name, attr_type,data_type, prec, scale, code_set, domain_class_name)
	AS
	SELECT a.attr_name, c.class_name,
		   CASE WHEN a.attr_type = 0 THEN 'INSTANCE'
				WHEN a.attr_type = 1 THEN 'CLASS'
				ELSE 'SHARED' END,
		   et.type_name, e.prec, e.scale, e.code_set, e.class_of.class_name
	FROM _db_class c, _db_attribute a, _db_domain d,
		  TABLE(d.set_domains) AS t(e), _db_data_type et
	WHERE a.class_of = c AND d.object_of = a AND e.data_type = et.type_id AND
			(CURRENT_USER = 'DBA' OR
			{c.owner.name} subseteq (
					SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
					from db_user u, table(groups) as t(g)
					where u.name = CURRENT_USER ) OR
			{c} subseteq (
	SELECT sum(set{au.class_of})
					FROM _db_auth au
					WHERE {au.grantee.name} subseteq (
								SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
								from db_user u, table(groups) as t(g)
								where u.name = CURRENT_USER ) AND
										au.auth_type = 'SELECT')); 

If the set_attr attribute of class D is of a SET (A, B, C) type, the following three records exist.

+---------------+----------------+---------------+---------------+----------+-----------+--------------+-----------------------+
| **Attr_name** | **Class_name** | **Attr_type** | **Data_type** | **Prec** | **Scale** | **Code_set** | **Domain_class_name** |
|               |                |               |               |          |           |              |                       |
+---------------+----------------+---------------+---------------+----------+-----------+--------------+-----------------------+
| 'set_attr'    | 'D'            | 'INSTANCE'    | 'SET'         | 0        | 0         | 0            | 'A'                   |
|               |                |               |               |          |           |              |                       |
+---------------+----------------+---------------+---------------+----------+-----------+--------------+-----------------------+
| 'set_attr'    | 'D'            | 'INSTANCE'    | 'SET'         | 0        | 0         | 0            | 'B'                   |
|               |                |               |               |          |           |              |                       |
+---------------+----------------+---------------+---------------+----------+-----------+--------------+-----------------------+
| 'set_attr'    | 'D'            | 'INSTANCE'    | 'SET'         | 0        | 0         | 0            | 'C'                   |
|               |                |               |               |          |           |              |                       |
+---------------+----------------+---------------+---------------+----------+-----------+--------------+-----------------------+

The following example shows how to retrieve collection type attributes and data types of the *city* class (the *city* table defined in `Containment Operators <#syntax_syntax_operator_contain_c_5562>`_ is created).

.. code-block:: sql

	SELECT attr_name, attr_type, data_type, domain_class_name
	FROM db_attr_setdomain_elm
	WHERE class_name = 'city';
	
	  attr_name             attr_type             data_type             domain_class_name
	==============================================================================
	 
	'sports'              'INSTANCE'            'STRING'              NULL

DB_METHOD
---------

Represents method information of a class for which the current user has access authorization to a database.

+--------------------+---------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                                                                                                               |
|                    |               |                                                                                                                                               |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| meth_name          | VARCHAR(255)  | Method name                                                                                                                                   |
|                    |               |                                                                                                                                               |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| class_name         | VARCHAR(255)  | Name of the class to which the method belongs                                                                                                 |
|                    |               |                                                                                                                                               |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| meth_type          | VARCHAR(8)    | 'INSTANCE' for an instance method, and 'CLASS' for a class method.                                                                            |
|                    |               |                                                                                                                                               |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| from_class_name    | VARCHAR(255)  | If the method is inherited, the super class in which it is defined is used otherwise                                                          |
|                    |               | **NULL**                                                                                                                                      |
|                    |               |                                                                                                                                               |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| from_meth_name     | VARCHAR(255)  | If the method is inherited and its name is changed to resolve a name conflict, the original name defined in the super class is used otherwise |
|                    |               | **NULL**                                                                                                                                      |
|                    |               |                                                                                                                                               |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------------------------------------------+
| func_name          | VARCHAR(255)  | Name of the C function for the method                                                                                                         |
|                    |               |                                                                                                                                               |
+--------------------+---------------+-----------------------------------------------------------------------------------------------------------------------------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_method (
	meth_name, class_name, meth_type, from_class_name, from_meth_name, func_name)
	AS
	 
	SELECT m.meth_name, m.class_of.class_name,
		   CASE WHEN m.meth_type = 0 THEN 'INSTANCE' ELSE 'CLASS' END,
		   m.from_class_of.class_name, m.from_meth_name, s.func_name
	FROM _db_method m, _db_meth_sig s
	WHERE s.meth_of = m AND
			(CURRENT_USER = 'DBA' OR
			{m.class_of.owner.name} subseteq (
					SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
					from db_user u, table(groups) as t(g)
					where u.name = CURRENT_USER ) OR
			{m.class_of} subseteq (
	SELECT sum(set{au.class_of})
					FROM _db_auth au
					WHERE {au.grantee.name} subseteq (
								SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
								from db_user u, table(groups) as t(g)
								where u.name = CURRENT_USER ) AND
										au.auth_type = 'SELECT'));

The following example shows how to retrieve methods of the *db_user* class.

.. code-block:: sql

	SELECT meth_name, meth_type, func_name
	FROM db_method
	WHERE class_name = 'db_user'
	ORDER BY meth_type, meth_name;
	
	  meth_name             meth_type             func_name
	==================================================================
	  'add_user'            'CLASS'               'au_add_user_method'
	  'drop_user'           'CLASS'               'au_drop_user_method'
	  'find_user'           'CLASS'               'au_find_user_method'
	  'login'               'CLASS'               'au_login_method'
	  'add_member'          'INSTANCE'            'au_add_member_method'
	  'drop_member'         'INSTANCE'            'au_drop_member_method'
	  'print_authorizations'  'INSTANCE'            'au_describe_user_method'
	  'set_password'        'INSTANCE'            'au_set_password_method'
	  'set_password_encoded'  'INSTANCE'            'au_set_password_encoded_method'
	  'set_password_encoded_sha1'  'INSTANCE'            'au_set_password_encoded_sha1_method'

DB_METH_ARG
-----------

Represents the input/output argument information of the method of the class for which the current user has access authorization to a database.

+--------------------+---------------+------------------------------------------------------------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                                                                                                          |
|                    |               |                                                                                                                                          |
+--------------------+---------------+------------------------------------------------------------------------------------------------------------------------------------------+
| meth_name          | VARCHAR(255)  | Method name                                                                                                                              |
|                    |               |                                                                                                                                          |
+--------------------+---------------+------------------------------------------------------------------------------------------------------------------------------------------+
| class_name         | VARCHAR(255)  | Name of the class to which the method belongs                                                                                            |
|                    |               |                                                                                                                                          |
+--------------------+---------------+------------------------------------------------------------------------------------------------------------------------------------------+
| meth_type          | VARCHAR(8)    | 'INSTANCE' for an instance method, and 'CLASS' for a class method.                                                                       |
|                    |               |                                                                                                                                          |
+--------------------+---------------+------------------------------------------------------------------------------------------------------------------------------------------+
| index_of           | INTEGER       | Order in which arguments are listed in the function definition. Begins with 0 if it is a return value, and 1 if it is an input argument. |
|                    |               |                                                                                                                                          |
+--------------------+---------------+------------------------------------------------------------------------------------------------------------------------------------------+
| data_type          | VARCHAR(9)    | Data type of the argument                                                                                                                |
|                    |               |                                                                                                                                          |
+--------------------+---------------+------------------------------------------------------------------------------------------------------------------------------------------+
| prec               | INTEGER       | Precision of the argument                                                                                                                |
|                    |               |                                                                                                                                          |
+--------------------+---------------+------------------------------------------------------------------------------------------------------------------------------------------+
| scale              | INTEGER       | Scale of the argument                                                                                                                    |
|                    |               |                                                                                                                                          |
+--------------------+---------------+------------------------------------------------------------------------------------------------------------------------------------------+
| code_set           | INTEGER       | Character set if the data type of the argument is a character.                                                                           |
|                    |               |                                                                                                                                          |
+--------------------+---------------+------------------------------------------------------------------------------------------------------------------------------------------+
| domain_class_name  | VARCHAR(255)  | Domain class name if the data type of the argument is an object.                                                                         |
|                    |               |                                                                                                                                          |
+--------------------+---------------+------------------------------------------------------------------------------------------------------------------------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_meth_arg (
	meth_name, class_name, meth_type,
	index_of, data_type, prec, scale, code_set, domain_class_name)
	AS
	SELECT s.meth_of.meth_name, s.meth_of.class_of.class_name,
		   CASE WHEN s.meth_of.meth_type = 0 THEN 'INSTANCE' ELSE 'CLASS' END,
		   a.index_of, t.type_name, d.prec, d.scale, d.code_set,
		   d.class_of.class_name
	FROM _db_meth_sig s, _db_meth_arg a, _db_domain d, _db_data_type t
	WHERE a.meth_sig_of = s AND d.object_of = a AND d.data_type = t.type_id AND
			(CURRENT_USER = 'DBA' OR
			{s.meth_of.class_of.owner.name} subseteq (
					SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
					from db_user u, table(groups) as t(g)
					where u.name = CURRENT_USER ) OR
			{s.meth_of.class_of} subseteq (
	SELECT sum(set{au.class_of})
					FROM _db_auth au
					WHERE {au.grantee.name} subseteq (
								SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
								from db_user u, table(groups) as t(g)
								where u.name = CURRENT_USER ) AND
										au.auth_type = 'SELECT'));

**Example**

The following example shows how to retrieve input arguments of the method of the *db_user* class.

.. code-block:: sql

	SELECT meth_name, data_type, prec
	FROM db_meth_arg
	WHERE class_name = 'db_user';
	
	  meth_name             data_type                    prec
	=========================================================
	  'append_data'         'STRING'               1073741823

DB_METH_ARG_SETDOMAIN_ELM
-------------------------

If the data type of the input/output argument of the method of the class is a set, for which the current user has access authorization in the database, this macro represents the data type of the element of the set.

+--------------------+---------------+--------------------------------------------------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                                                                                                |
|                    |               |                                                                                                                                |
+--------------------+---------------+--------------------------------------------------------------------------------------------------------------------------------+
| meth_name          | VARCHAR(255)  | Method name                                                                                                                    |
|                    |               |                                                                                                                                |
+--------------------+---------------+--------------------------------------------------------------------------------------------------------------------------------+
| class_name         | VARCHAR(255)  | Name of the class to which the method belongs                                                                                  |
|                    |               |                                                                                                                                |
+--------------------+---------------+--------------------------------------------------------------------------------------------------------------------------------+
| meth_type          | VARCHAR(8)    | 'INSTANCE' for an instance method, and 'CLASS' for a class method.                                                             |
|                    |               |                                                                                                                                |
+--------------------+---------------+--------------------------------------------------------------------------------------------------------------------------------+
| index_of           | INTEGER       | Order of arguments listed in the function definition. Begins with 0 if it is a return value, and 1 if it is an input argument. |
|                    |               |                                                                                                                                |
+--------------------+---------------+--------------------------------------------------------------------------------------------------------------------------------+
| data_type          | VARCHAR(9)    | Data type of the element                                                                                                       |
|                    |               |                                                                                                                                |
+--------------------+---------------+--------------------------------------------------------------------------------------------------------------------------------+
| prec               | INTEGER       | Precision of the element                                                                                                       |
|                    |               |                                                                                                                                |
+--------------------+---------------+--------------------------------------------------------------------------------------------------------------------------------+
| scale              | INTEGER       | Scale of the element                                                                                                           |
|                    |               |                                                                                                                                |
+--------------------+---------------+--------------------------------------------------------------------------------------------------------------------------------+
| code_set           | INTEGER       | Character set if the data type of the element is a character                                                                   |
|                    |               |                                                                                                                                |
+--------------------+---------------+--------------------------------------------------------------------------------------------------------------------------------+
| domain_class_name  | VARCHAR(255)  | Domain class name if the data type of the element is an object                                                                 |
|                    |               |                                                                                                                                |
+--------------------+---------------+--------------------------------------------------------------------------------------------------------------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_meth_arg_setdomain_elm(
	meth_name, class_name, meth_type,
	index_of, data_type, prec, scale, code_set, domain_class_name)
	AS
	SELECT s.meth_of.meth_name, s.meth_of.class_of.class_name,
		   CASE WHEN s.meth_of.meth_type = 0 THEN 'INSTANCE' ELSE 'CLASS' END,
		   a.index_of, et.type_name, e.prec, e.scale, e.code_set,
		   e.class_of.class_name
	FROM _db_meth_sig s, _db_meth_arg a, _db_domain d,
		  TABLE(d.set_domains) AS t(e), _db_data_type et
	WHERE a.meth_sig_of = s AND d.object_of = a AND e.data_type = et.type_id AND
			(CURRENT_USER = 'DBA' OR
			{s.meth_of.class_of.owner.name} subseteq (
					SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
					from db_user u, table(groups) as t(g)
					where u.name = CURRENT_USER ) OR
			{s.meth_of.class_of} subseteq (
	SELECT sum(set{au.class_of})
					FROM _db_auth au
					WHERE {au.grantee.name} subseteq (
								SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
								from db_user u, table(groups) as t(g)
								where u.name = CURRENT_USER ) AND
										au.auth_type = 'SELECT'));

DB_METH_FILE
------------

Represents information of a file in which the method of the class for which the current user has access authorization in the database is defined.

+--------------------+---------------+-------------------------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                                                                       |
|                    |               |                                                                                                       |
+--------------------+---------------+-------------------------------------------------------------------------------------------------------+
| class_name         | VARCHAR(255)  | Name of the class to which the method file belongs                                                    |
|                    |               |                                                                                                       |
+--------------------+---------------+-------------------------------------------------------------------------------------------------------+
| path_name          | VARCHAR(255)  | File path in which the C function is defined                                                          |
|                    |               |                                                                                                       |
+--------------------+---------------+-------------------------------------------------------------------------------------------------------+
| from_class_name    | VARCHAR(255)  | Name of the super class in which the method file is defined if the method is inherited, and otherwise |
|                    |               | **NULL**                                                                                              |
|                    |               |                                                                                                       |
+--------------------+---------------+-------------------------------------------------------------------------------------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_meth_file (class_name, path_name, from_class_name)
	AS
	SELECT f.class_of.class_name, f.path_name, f.from_class_of.class_name
	FROM _db_meth_file f
	WHERE (CURRENT_USER = 'DBA' OR
			{f.class_of.owner.name} subseteq (
					SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
					from db_user u, table(groups) as t(g)
					where u.name = CURRENT_USER ) OR
			{f.class_of} subseteq (
	SELECT sum(set{au.class_of})
					FROM _db_auth au
					WHERE {au.grantee.name} subseteq (
								SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
								from db_user u, table(groups) as t(g)
								where u.name = CURRENT_USER ) AND
										au.auth_type = 'SELECT'));

DB_INDEX
--------

Represents information of indexes created for the class for which the current user has access authorization to a database.

+--------------------+---------------+-------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                 |
|                    |               |                                                 |
+--------------------+---------------+-------------------------------------------------+
| index_name         | VARCHAR(255)  | Index name                                      |
|                    |               |                                                 |
+--------------------+---------------+-------------------------------------------------+
| is_unique          | VARCHAR(3)    | 'YES' for a unique index, and 'NO' otherwise.   |
|                    |               |                                                 |
+--------------------+---------------+-------------------------------------------------+
| is_reverse         | VARCHAR(3)    | 'YES' for a reversed index, and 'NO' otherwise. |
|                    |               |                                                 |
+--------------------+---------------+-------------------------------------------------+
| class_name         | VARCHAR(255)  | Name of the class to which the index belongs    |
|                    |               |                                                 |
+--------------------+---------------+-------------------------------------------------+
| key_count          | INTEGER       | The number of attributes that comprise the key  |
|                    |               |                                                 |
+--------------------+---------------+-------------------------------------------------+
| is_primary_key     | VARCHAR(3)    | 'YES' for a primary key, and 'NO' otherwise.    |
|                    |               |                                                 |
+--------------------+---------------+-------------------------------------------------+
| is_foreign_key     | VARCHAR(3)    | 'YES' for a foreign key, and 'NO' otherwise.    |
|                    |               |                                                 |
+--------------------+---------------+-------------------------------------------------+
| filter_expression  | VARCHAR(255)  | Conditions of filtered indexes                  |
|                    |               |                                                 |
+--------------------+---------------+-------------------------------------------------+
| have_function      | VARCHAR(3)    | 'YES' for function based and 'NO' otherwise.    |
|                    |               |                                                 |
+--------------------+---------------+-------------------------------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_index (index_name, is_unique, is_reverse, class_name, key_count, is_primary_key, is_foreign_key, filter_expression, have_function)
	AS
	SELECT i.index_name, CASE WHEN i.is_unique = 0 THEN 'NO' ELSE 'YES' END,
	CASE WHEN i.is_reverse = 0 THEN 'NO' ELSE 'YES' END, i.class_of.class_name,
	i.key_count,
	CASE WHEN i.is_primary_key = 0 THEN 'NO' ELSE 'YES' END, CASE WHEN i.is_foreign_key = 0 THEN 'NO' ELSE 'YES' END, i.filter_expression,
	CASE WHEN i.have_function = 0 THEN 'NO' ELSE 'YES' END
	FROM _db_index i
	WHERE (CURRENT_USER = 'DBA' OR
			{i.class_of.owner.name} subseteq (
					SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
					from db_user u, table(groups) as t(g)
					where u.name = CURRENT_USER ) OR
			{i.class_of} subseteq (
	SELECT sum(set{au.class_of})
					FROM _db_auth au
					WHERE {au.grantee.name} subseteq (
								SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
								from db_user u, table(groups) as t(g)
								where u.name = CURRENT_USER ) AND
										au.auth_type = 'SELECT'));


**Example**

The following example shows how to retrieve index information of the class.

.. code-block:: sql

	SELECT class_name, index_name, is_unique
	FROM db_index
	ORDER BY 1;
	
	  class_name            index_name            is_unique
	==================================================================
	  'athlete'             'pk_athlete_code'     'YES'
	  'city'                'pk_city_city_name'   'YES'
	  'db_serial'           'pk_db_serial_name'   'YES'
	  'db_user'             'i_db_user_name'      'NO'
	  'event'               'pk_event_code'       'YES'
	  'female_event'        'pk_event_code'       'YES'
	  'game'                'pk_game_host_year_event_code_athlete_code'  'YES'
	  'game'                'fk_game_event_code'  'NO'
	  'game'                'fk_game_athlete_code'  'NO'
	  'history'             'pk_history_event_code_athlete'  'YES'
	  'nation'              'pk_nation_code'      'YES'
	  'olympic'             'pk_olympic_host_year'  'YES'
	  'participant'         'pk_participant_host_year_nation_code'  'YES'
	  'participant'         'fk_participant_host_year'  'NO'
	  'participant'         'fk_participant_nation_code'  'NO'
	  'record'              'pk_record_host_year_event_code_athlete_code_medal'  'YES'
	  'stadium'             'pk_stadium_code'     'YES'
	…

DB_INDEX_KEY
------------

Represents the key information of indexes created for the class for which the current user has access authorization to a database.

+--------------------+---------------+-----------------------------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                                             |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+
| index_name         | VARCHAR(255)  | Index name                                                                  |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+
| class_name         | VARCHAR(255)  | Name of the class to which the index belongs                                |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+
| key_attr_name      | VARCHAR(255)  | Name of attributes that comprise the key                                    |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+
| key_order          | INTEGER       | Order of attributes in the key. Begins with 0.                              |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+
| asc_desc           | VARCHAR(4)    | 'DESC' if the order of attribute values is descending, and 'ASC' otherwise. |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+
| key_prefix_length  | INTEGER       | The length of prefix to be used as a key                                    |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+
| func               | VARCHAR(255)  | Functional expression of function based index                               |
|                    |               |                                                                             |
+--------------------+---------------+-----------------------------------------------------------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_index_key (index_name, class_name, key_attr_name, key_order, key_prefix_length, func)
	AS
	SELECT k.index_of.index_name, k.index_of.class_of.class_name, k.key_attr_name, k.key_order
	CASE k.asc_desc
	WHEN 0 THEN 'ASC'
	WHEN 1 THEN 'DESC' ELSE 'UNKN' END,
	k.key_prefix_length, k.func
	FROM _db_index_key k
	WHERE (CURRENT_USER = 'DBA' OR
		{k.index_of.class_of.owner.name}
		subseteq (
			SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
			from db_user u, table(groups) as t(g)
			where u.name = CURRENT_USER ) OR {k.index_of.class_of}
			subseteq (
				SELECT sum(set{au.class_of})
				FROM _db_auth au
				WHERE {au.grantee.name} subseteq (
					SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
					from db_user u, table(groups) as t(g)
					where u.name = CURRENT_USER ) AND
				au.auth_type = 'SELECT'));

The following example shows how to retrieve index key information of the class.

.. code-block:: sql

	SELECT class_name, key_attr_name, index_name
	FROM db_index_key
	ORDER BY class_name, key_order;
	
	  'athlete'             'code'                'pk_athlete_code'
	  'city'                'city_name'           'pk_city_city_name'
	  'db_serial'           'name'                'pk_db_serial_name'
	  'db_user'             'name'                'i_db_user_name'
	  'event'               'code'                'pk_event_code'
	  'female_event'        'code'                'pk_event_code'
	  'game'                'host_year'           'pk_game_host_year_event_code_athlete_code'
	  'game'                'event_code'          'fk_game_event_code'
	  'game'                'athlete_code'        'fk_game_athlete_code'
	 …

DB_AUTH
-------

Represents authorization information of classes for which the current user has access authorization to a database.

+--------------------+---------------+-----------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                                                         |
|                    |               |                                                                                         |
+--------------------+---------------+-----------------------------------------------------------------------------------------+
| grantor_name       | VARCHAR(255)  | Name of the user who grants authorization                                               |
|                    |               |                                                                                         |
+--------------------+---------------+-----------------------------------------------------------------------------------------+
| grantee_name       | VARCHAR(255)  | Name of the user who is granted authorization                                           |
|                    |               |                                                                                         |
+--------------------+---------------+-----------------------------------------------------------------------------------------+
| class_name         | VARCHAR(255)  | Name of the class for which authorization is to be granted                              |
|                    |               |                                                                                         |
+--------------------+---------------+-----------------------------------------------------------------------------------------+
| auth_type          | VARCHAR(7)    | Name of the authorization type granted                                                  |
|                    |               |                                                                                         |
+--------------------+---------------+-----------------------------------------------------------------------------------------+
| is_grantable       | VARCHAR(3)    | 'YES' if authorization for the class can be granted to other users, and 'NO' otherwise. |
|                    |               |                                                                                         |
+--------------------+---------------+-----------------------------------------------------------------------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_auth (grantor_name, grantee_name, class_name, auth_type, is_grantable )
	AS
	SELECT CAST(a.grantor.name AS VARCHAR(255)),
			CAST(a.grantee.name AS VARCHAR(255)),
			a.class_of.class_name, a.auth_type,
			CASE WHEN a.is_grantable = 0 THEN 'NO' ELSE 'YES' END
	FROM _db_auth a
	WHERE (CURRENT_USER = 'DBA' OR
			{a.class_of.owner.name} subseteq (
					SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
					from db_user u, table(groups) as t(g)
					where u.name = CURRENT_USER ) OR
			{a.class_of} subseteq (
	SELECT sum(set{au.class_of})
					FROM _db_auth au
					WHERE {au.grantee.name} subseteq (
								SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
								from db_user u, table(groups) as t(g)
								where u.name = CURRENT_USER ) AND
										au.auth_type = 'SELECT'));

The following example how to retrieve authorization information of the classes whose names begin with *db_a*.

.. code-block:: sql

	SELECT class_name, auth_type, grantor_name
	FROM db_auth
	WHERE class_name like 'db_a%'
	ORDER BY 1;
	
	  class_name            auth_type             grantor_name
	==================================================================
	  'db_attr_setdomain_elm'  'SELECT'             'DBA'
	  'db_attribute'           'SELECT'             'DBA'
	  'db_auth'                'SELECT'             'DBA'
	  'db_authorization'       'EXECUTE'            'DBA'
	  'db_authorization'       'SELECT'             'DBA'
	  'db_authorizations'      'EXECUTE'            'DBA'
	  'db_authorizations'      'SELECT'             'DBA'

DB_TRIG
-------

Represents information of a trigger that has the class for which the current user has access authorization to a database, or its attribute as the target.

+--------------------+---------------+-------------------------------------------------------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                                                                                               |
|                    |               |                                                                                                                               |
+--------------------+---------------+-------------------------------------------------------------------------------------------------------------------------------+
| trigger_name       | VARCHAR(255)  | Trigger name                                                                                                                  |
|                    |               |                                                                                                                               |
+--------------------+---------------+-------------------------------------------------------------------------------------------------------------------------------+
| target_class_name  | VARCHAR(255)  | Target class                                                                                                                  |
|                    |               |                                                                                                                               |
+--------------------+---------------+-------------------------------------------------------------------------------------------------------------------------------+
| target_attr_name   | VARCHAR(255)  | Target attribute. If not specified in the trigger,                                                                            |
|                    |               | **NULL**                                                                                                                      |
|                    |               |                                                                                                                               |
+--------------------+---------------+-------------------------------------------------------------------------------------------------------------------------------+
| target_attr_type   | VARCHAR(8)    | Target attribute type. If specified, 'INSTANCE' is used for an instance attribute, and 'CLASS' is used for a class attribute. |
|                    |               |                                                                                                                               |
+--------------------+---------------+-------------------------------------------------------------------------------------------------------------------------------+
| action_type        | INTEGER       | 1 for one of INSERT, UPDATE, DELETE, and CALL, 2 for REJECT, 3 for INVALIDATE_TRANSACTION, and 4 for PRINT.                   |
|                    |               |                                                                                                                               |
+--------------------+---------------+-------------------------------------------------------------------------------------------------------------------------------+
| action_time        | INTEGER       | 1 for BEFORE, 2 for AFTER, and 3 for DEFERRED.                                                                                |
|                    |               |                                                                                                                               |
+--------------------+---------------+-------------------------------------------------------------------------------------------------------------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_trig (
	trigger_name, target_class_name, target_attr_name, target_attr_type, action_type, action_time)
	AS
	SELECT CAST(t.name AS VARCHAR(255)), c.class_name,
			CAST(t.target_attribute AS VARCHAR(255)),
			CASE WHEN t.target_class_attribute = 0 THEN 'INSTANCE' ELSE 'CLASS' END,
			t.action_type, t.action_time
	FROM _db_class c, db_trigger t
	WHERE t.target_class = c.class_of AND
			(CURRENT_USER = 'DBA' OR
			{c.owner.name} subseteq (
					SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
					from db_user u, table(groups) as t(g)
					where u.name = CURRENT_USER ) OR
			{c} subseteq (
	SELECT sum(set{au.class_of})
					FROM _db_auth au
					WHERE {au.grantee.name} subseteq (
								SELECT set{CURRENT_USER} + coalesce(sum(set{t.g.name}), set{})
								from db_user u, table(groups) as t(g)
								where u.name = CURRENT_USER ) AND
										au.auth_type = 'SELECT'));

DB_PARTITION
------------

Represents information of partitioned classes for which the current user has access authorization to a database.

+----------------------+---------------+-------------------------+
| **Attribute Name**   | **Data Type** | **Description**         |
|                      |               |                         |
+----------------------+---------------+-------------------------+
| class_name           | VARCHAR(255)  | Class name              |
|                      |               |                         |
+----------------------+---------------+-------------------------+
| partition_name       | VARCHAR(255)  | Partition name          |
|                      |               |                         |
+----------------------+---------------+-------------------------+
| partition_class_name | VARCHAR(255)  | Partitioned class name  |
|                      |               |                         |
+----------------------+---------------+-------------------------+
| partition_type       | VARCHAR(32)   | Partition type          |
|                      |               | (HASH, RANGE, LIST)     |
|                      |               |                         |
+----------------------+---------------+-------------------------+
| partition_expr       | VARCHAR(255)  | Partition expression    |
|                      |               |                         |
+----------------------+---------------+-------------------------+
| partition_values     | SEQUENCE OF   | RANGE - MIN/MAX value   |
|                      |               | - For infinite MIN/MAX, |
|                      |               | **NULL**                |
|                      |               | LIST - value list       |
|                      |               |                         |
+----------------------+---------------+-------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_partition
	(sp_name, sp_type, return_type, arg_count, lang, target, owner)
	AS
	SELECT p.class_of.class_name AS class_name, p.pname AS partition_name,
				p.class_of.class_name || '__p__' || p.pname AS partition_class_name,
				CASE WHEN p.ptype = 0 THEN 'HASH'
					 WHEN p.ptype = 1 THEN 'RANGE'
				ELSE 'LIST' ENDASpartition_type,
				TRIM(SUBSTRING( pi.pexpr FROM 8 FOR (POSITION(' FROM ' IN pi.pexpr)-8))) AS
					partition_expression,
				p.pvalues AS partition_values
	FROM _db_partition p,
		 ( select * from _db_partition sp
	where sp.class_of =  p.class_of AND sp.pname is null) pi
	WHERE p.pname is not null AND
		  ( CURRENT_USER = 'DBA'
			OR
			{p.class_of.owner.name} SUBSETEQ
			 ( SELECT SET{CURRENT_USER} + COALESCE(SUM(SET{t.g.name}), SET{}) 
			   FROM db_user u, TABLE(groups) AS t(g) 
			   WHERE u.name = CURRENT_USER
			 )
			OR
			{p.class_of} SUBSETEQ
			 ( SELECT SUM(SET{au.class_of}) 
			   FROM _db_auth au 
			   WHERE {au.grantee.name} SUBSETEQ
					 ( SELECT SET{CURRENT_USER} + COALESCE(SUM(SET{t.g.name}), SET{}) 
					   FROM db_user u, TABLE(groups) AS t(g) 
					   WHERE u.name = CURRENT_USER) AND 
					   au.auth_type = 'SELECT'
			 )
		  )

The following example shows how to retrieve the partition information currently configured for the participant2 class (see examples in `Defining Range Partitions <#syntax_syntax_partition_range_de_4841>`_).

.. code-block:: sql

	SELECT * from db_partition where class_name = 'participant2';

	  class_name            partition_name        partition_class_name         partition_type   partition_expr        partition_values
	====================================================================================================================================
	  'participant2'        'before_2000'         'participant2__p__before_2000'  'RANGE'       'host_year'           {NULL, 2000}
	  'participant2'        'before_2008'         'participant2__p__before_2008'  'RANGE'       'host_year'           {2000, 2008}

DB_STORED_PROCEDURE
-------------------

Represents information of Java stored procedure for which the current user has access authorization to a database.

+--------------------+---------------+-----------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                               |
|                    |               |                                               |
+--------------------+---------------+-----------------------------------------------+
| sp_name            | VARCHAR(255)  | Stored procedure name                         |
|                    |               |                                               |
+--------------------+---------------+-----------------------------------------------+
| sp_type            | VARCHAR(16)   | Stored procedure type (function or procedure) |
|                    |               |                                               |
+--------------------+---------------+-----------------------------------------------+
| return_type        | VARCHAR(16)   | Return value type                             |
|                    |               |                                               |
+--------------------+---------------+-----------------------------------------------+
| arg_count          | INTEGER       | The number of arguments                       |
|                    |               |                                               |
+--------------------+---------------+-----------------------------------------------+
| lang               | VARCHAR(16)   | Implementing language (currently, Java)       |
|                    |               |                                               |
+--------------------+---------------+-----------------------------------------------+
| target             | VARCHAR(4096) | Name of the Java method to be executed        |
|                    |               |                                               |
+--------------------+---------------+-----------------------------------------------+
| owner              | VARCHAR(256)  | Owner                                         |
|                    |               |                                               |
+--------------------+---------------+-----------------------------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_stored_procedure
	(sp_name, sp_type, return_type, arg_count, lang, target, owner)
	AS
	SELECT sp.sp_name,
				CASE sp.sp_type   WHEN 1 THEN 'PROCEDURE'  
				ELSE 'FUNCTION' END,
				CASE WHEN sp.return_type = 0 THEN 'void'  
					 WHEN sp.return_type = 28 THEN 'CURSOR'  
				ELSE ( SELECT dt.type_name
					   FROM _db_data_type dt
					   WHERE sp.return_type = dt.type_id) END,
			   sp.arg_count,
			   CASE sp.lang   WHEN 1 THEN 'JAVA'  
			   ELSE '' END, sp.target, sp.owner.name
	FROM _db_stored_procedure sp

The following example shows how to retrieve Java stored procedures owned by the current user.

.. code-block:: sql

	SELECT sp_name, target from db_stored_procedure
	WHERE sp_type = 'FUNCTION' AND owner = CURRENT_USER 

	  sp_name               target             
	============================================
	  'hello'               'SpCubrid.HelloCubrid() return java.lang.String'
	  'sp_int'              'SpCubrid.SpInt(int) return int'

DB_STORED_PROCEDURE_ARGS
------------------------

Represents argument information of Java stored procedure for which the current user has access authorization to a database.

+--------------------+---------------+---------------------------+
| **Attribute Name** | **Data Type** | **Description**           |
|                    |               |                           |
+--------------------+---------------+---------------------------+
|  sp_name           | VARCHAR(255)  | Stored procedure name     |
|                    |               |                           |
+--------------------+---------------+---------------------------+
|  index_of          | INTEGER       | Order of the arguments    |
|                    |               |                           |
+--------------------+---------------+---------------------------+
|  arg_name          | VARCHAR(256)  | Argument name             |
|                    |               |                           |
+--------------------+---------------+---------------------------+
|  data_type         | VARCHAR(16)   | Data type of the argument |
|                    |               |                           |
+--------------------+---------------+---------------------------+
|  mode              | VARCHAR(6)    | Mode (IN, OUT, INOUT)     |
|                    |               |                           |
+--------------------+---------------+---------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_stored_procedure_args (sp_name, index_of, arg_name, data_type, mode)
	AS
	SELECT sp.sp_name, sp.index_of, sp.arg_name,
				CASE sp.data_type   WHEN 28 THEN 'CURSOR'  
				ELSE ( SELECT dt.type_name FROM _db_data_type dt
					   WHERE sp.data_type = dt.type_id) END,
				CASE WHEN sp.mode = 1 THEN 'IN' WHEN sp.mode = 2 THEN 'OUT'  
				ELSE 'INOUT' END
	FROM _db_stored_procedure_args sp
	ORDER BY sp.sp_name, sp.index_of ;

The following example shows how to retrieve arguments the 'phone_info' Java stored procedure in the order of the arguments.

.. code-block:: sql

	SELECT index_of, arg_name, data_type, mode 
	FROM db_stored_procedure_args
	WHERE sp_name = 'phone_info'
	ORDER BY index_of

		 index_of  arg_name              data_type             mode
	===============================================================
				0  'name'                'STRING'              'IN'
				1  'phoneno'             'STRING'              'IN'

DB_COLLATION
------------

The information on collation.

+--------------------+---------------+-------------------------------------------------------------------------------+
| **Attribute Name** | **Data Type** | **Description**                                                               |
|                    |               |                                                                               |
+--------------------+---------------+-------------------------------------------------------------------------------+
| coll_id            | INTEGER       | Collation ID                                                                  |
|                    |               |                                                                               |
+--------------------+---------------+-------------------------------------------------------------------------------+
| coll_name          | VARCHAR(255)  | Collation name                                                                |
|                    |               |                                                                               |
+--------------------+---------------+-------------------------------------------------------------------------------+
| charset_name       | VARCHAR(256)  | Charset name                                                                  |
|                    |               |                                                                               |
+--------------------+---------------+-------------------------------------------------------------------------------+
| is_builtin         | VARCHAR(3)    | Built-in or not while installing the product                                  |
|                    |               |                                                                               |
+--------------------+---------------+-------------------------------------------------------------------------------+
| has_expansions     | VARCHAR(3)    | Having expansion or not                                                       |
|                    |               |                                                                               |
+--------------------+---------------+-------------------------------------------------------------------------------+
| contractions       | INTEGER       | Whether to include abbreviation                                               |
|                    |               |                                                                               |
+--------------------+---------------+-------------------------------------------------------------------------------+
| uca_strength       | VARCHAR(255)  | Weight strength                                                               |
|                    |               | (NOT APPLICABLE, PRIMARY, SECONDARY, TERTIARY, QUATERNARY, IDENTITY, UNKNOWN) |
|                    |               |                                                                               |
+--------------------+---------------+-------------------------------------------------------------------------------+

**Definition**

.. code-block:: sql

	CREATE VCLASS db_collation (coll_id, coll_name, charset_name, is_builtin, has_expansions, contractions)
	AS
	SELECT c.coll_id, c.coll_name,
	CASE c.charset_id
		WHEN 3 THEN 'ISO8859-1'
		WHEN 5 THEN 'UTF-8'
		WHEN 4 THEN 'KSC-EUC'  
		WHEN 0 THEN 'ASCII'  
		WHEN 1 THEN 'RAW-BITS'  
		WHEN 2 THEN 'RAW-BYTES'  
		WHEN -1 THEN 'NONE'  
	ELSE 'OTHER' END,
	CASE c.built_in  
		WHEN 0 THEN 'NO'  
		WHEN 1 THEN 'YES'  
	ELSE 'ERROR' END,
	CASE c.expansions  
		WHEN 0 THEN 'NO'  
		WHEN 1 THEN 'YES'  
	ELSE 'ERROR' END, c.contractions,
	CASE c.uca_strength  
		WHEN 0 THEN 'NOT APPLICABLE'  
		WHEN 1 THEN 'PRIMARY'  
		WHEN 2 THEN 'SECONDARY'  
		WHEN 3 THEN 'TERTIARY'
		WHEN 4 THEN 'QUATERNARY'  
		WHEN 5 THEN 'IDENTITY'  
	ELSE 'UNKNOWN' END
	FROM _db_collation c ORDER BY c.coll_id;

Catalog Class/Virtual Class Authorization
=========================================

Catalog classes are created to be owned by **dba**. However, **dba** can only execute **SELECT** operations. If **dba** executes operations such as **UPDATE** / **DELETE**, an authorization failure error occurs. General users cannot execute queries on system catalog classes.

Although catalog virtual classes are created to be owned by **dba**, all users can perform the **SELECT** statement on catalog virtual classes. Of course, **UPDATE** / **DELETE** operations on catalog virtual classes are not allowed.

Updating catalog classes/virtual classes is automatically performed by the system when users execute a DDL statement that creates/modifies/deletes a class/attribute/index/user/authorization.

Consistency of Catalog Information
==================================

Catalog information is represented by the instance of a catalog class/virtual class. If such information is accessed at the **READ UNCOMMITTED INSTANCES** (**TRAN_REP_CLASS_UNCOMMIT_INSTANCE** or **TRAN_COMMIT_CLASS_UNCOMMIT_INSTANCE**) isolation level, incorrect values (values being changed) can be read. Therefore, to get correct catalog information, you must use the **SELECT** query on the catalog class/virtual class at the **READ COMMITTED INSTANCES** isolation level or higher.

Querying on Catalog
===================

To query on catalog classes, you must convert identifiers such as class, virtual class, attribute, trigger, method and index names to lowercases, and create them. Therefore, you must use lowercases when querying on catalog classes.

.. code-block:: sql

	CREATE TABLE Foo(name varchar(255));
	SELECT class_name, partitioned FROM db_class WHERE class_name = 'Foo';
	 
	There are no results.
	 
	SELECT class_name, partitioned FROM db_class WHERE class_name = 'foo';
	  class_name   partitioned
	============================
	  'foo'       'NO'    
