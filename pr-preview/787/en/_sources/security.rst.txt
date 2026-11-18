
:meta-keywords: security, cubrid security, authorization, acl, access control list, ssl, tls, secure socket layer, packet encryption, tde, transparent data encryption, cubrid tde, data-at-rest encryption, kernel encryption, engine encryption, key management
:meta-description: CUBRID Security includes authorization for specific objects, access control to a database, packet encryption, data encryption, etc. Users can manage user data more safely by using CUBRID Security features.

***************
CUBRID Security
***************
This chapter describes the CUBRID security features. CUBRID provides Packet Encryption, ACL(Access Control List), authorization, and TDE(Transparent Data Encryption) features to protect the user database.

Packet Encryption
=================

Requirements of secure communication
--------------------------------------
Without an encrypted connection between the client and the server, a hacker with access to the network can monitor all traffic and steal and exploit data exchanged between the client and the server. In this way, a third party (middle-man) steals and makes bad use of information, it is called a man in the middle (MITM) attack. This MITM attack can be prevented by adding a secure authentication procedure to the connection between the client and the server.

Packet encryption method
------------------------------
CUBRID uses SSL/TLS (Secure Socket Layer/Transport Layer Security) protocol to encrypt data transmitted between the client and the server. The CUBRID server uses OpenSSL for encryption, and the client can make an encrypted connection using JDBC or CCI Driver. The encryption protocols supported between the client and server are SSLv3, TLSv1, TLSv1.1, and TLSv1.2.

.. note:: **SSL/TLS**

	SSL was first developed by Netscape as an encryption protocol that provides authentication and data encryption between a client and server working over a network. Netscape released version 3.0, which improved security flaws in 1996; Since then, the SSL 3.0 version became the basis for TLS 1.0; and it was defined by the IETF in January 1999 as the `RFC2246 <https://tools.ietf.org/html/RFC2246>`_ standard. The last update is `RFC5246 <https://tools.ietf.org/html/RFC5246>`_ . TLS is defined based on SSL 3.0, and it is almost similar to SSL 3.0.

	SSL/TLS provides Server Authentication, Client Authentication, and Data Encryption functions. Authentication refers to a procedure to verify that the other party is correct, and encryption refers to preventing access to contents even if data is stolen.


Server setup for packet encryption
------------------------------------
**Setting encryption mode and non-encryption mode**

CUBRID can set either an encryption mode or a non-encryption mode, and the default is non-encryption mode. To change to the encryption mode, you can set the encryption mode by changing the SSL parameter value in cubrid_broker.conf. If you change the SSL parameter value of cubrid_broker.conf, you must restart the broker. For detailed configuration method, refer to :ref:`broker-configuration`\ .


**Certificate and Private Key**

In order to exchange an encrypted symmetric session key which will be used in a secure communication session, a public key and a private key are required in the server.

The public key used by the server is included in the certificate 'cas_ssl_cert.crt', and the private key is included in 'cas_ssl_cert.key'. The certificate and private key are located in the $CUBRID/conf directory.

This certificate, 'self-signed' certificate, was created with the OpenSSL command tool utility and can be replaced with another certificate issued by a public CA (Certificate Authorities, for example, IdenTrust or DigiCert) if desired. Or, the existing certificate/private key can be replaced by generating a new one using the OpenSSL command utility as shown below.

.. code-block:: bash

	$ openssl genrsa -out my_cert.key 2048                                               # create 2048 bit size RSA private key
	$ openssl req -new -key my_cert.key -out my_cert.csr                                 # create CSR (Certificate Signing Request)
	$ openssl x509 -req -days 365 -in my_cert.csr -signkey my_cert.key -out my_cert.crt  # create a certificate valid for 1 year.

And replace my_cert.key and my_cert.crt with $CUBRID/conf/cas_ssl_cert.key and $CUBRID/conf/cas_ssl_cert.crt respectively.


Supported driver
------------------------------
CUBRID provides various drivers. Currently, the drivers that support packet encryption feature are JDBC and CCI.

**Encrypted connection method**

The client can make an encrypted connection with the server by using the useSSL property of db-url during driver connection setup. For more information on how to use it, refer to :ref:`jdbc-connection-conf`\  of the JDBC driver or :ref:`cci_connect_with_url`\  of the CCI driver.

.. _access-control:

ACL (Access Control List)
=========================

CUBRID supports the ACL function that can be accessed only for users or applications authorized in two layers: the broker layer and the database server layer.
The ACL of the broker layer is mainly used to control the access of application programs such as WEB and WAS, and the ACL of the database server layer is mainly used to control the access of the brokers and csqls.

For more details, see :ref:`limiting-broker-access` and :ref:`limiting-server-access`. 

.. _authorization:

Authorization
=============

CUBRID can create users(or groups) and provide a function to control the access of the other users(or groups) to tables created by a user.

If you want to allow other users(or groups) to access your tables, you could provide access privileges to the users(or groups) by :ref:`granting-authorization`. Also, to revoke access privileges of other users, you can use :ref:`revoking-authorization`. Access to the (virtual) table created by a PUBLIC user is allowed to all users.

For more details, see :doc:`/sql/authorization`.

.. _tde:

TDE (Transparent Data Encryption)
=================================

.. _tde-overview:

CUBRID TDE Concept
------------------

CUBRID supports **Transparent Data Encryption (henceforth, TDE)**. TDE means transparently encrypting data from the user's point of view. This allows users to encrypt data stored on disk with little to no application change.

CUBRID TDE provides encryption and decryption at the engine level to minimize performance degradation due to encryption. When a user creates an encrypted table, all relevant user data stored on disk (data at rest) is automatically encrypted. By providing TDE, CUBRID helps users to comply with security regulations and guidelines required in various sites.

**Table Encryption**

In CUBRID, a **table** is the unit for TDE-encryption. To use the TDE feature, create a table using the **ENCRYPT** option as follows. For more information, see :ref:`create-tde-table`.

.. code-block:: sql

	CREATE TABLE tde_tbl (att1 INT, att2 VARCHAR(20)) ENCRYPT=AES;

When an encrypted table is created, all data related to the table is automatically encrypted when written to disk; and decrypted when read into memory. Related data includes not only tables but also indexes created on the table, temporary data created while executing queries related to the table, logs created when data is changed, DWB, and backups. For more details, see :ref:`tde-enc-target` and :ref:`tde-restriction`.

.. _tde-key:

Key Management
--------------

CUBRID uses symmetric key algorithms to encrypt the data. Keys used for encryption are managed in two levels consisting of master keys and data keys for efficiency. Master keys managed by the user are stored in a separate file, and CUBRID provides a utility to manage it.

.. _tde-2level-key:

2-Level Key Management
^^^^^^^^^^^^^^^^^^^^^^

CUBRID TDE manages keys in two levels as follows:

.. image:: /images/tde_2_level_key.png
  :width: 469
  :align: center
  :alt: 2 Level Key Management Image

*    **Master key**: A key used when encrypting and decrypting data keys, and it is managed by DBA user.
*    **Data Key**: A key used when encrypting user data such as table and log, and it is managed by CUBRID Engine.

Data keys are stored within the data volume and are always securely encrypted using a master key when written to disk. The master key is stored in a separate file, and it must be managed safely according to the security policy users comply with.

Managing keys in two levels makes it possible to perform the key change operation efficiently. If there is only a key that encrypts the user data, it takes a long time to work when you change the key. All the data that has been encrypted has to be read, decrypted, and re-encrypted. Also, the overall performance of the database may be degraded during this process.

.. warning:: **Loss of Master Key**
    
    If the master key is lost, data encrypted by TDE cannot be read or changed.

.. _tde-file-based-key: 

File-based Master Key Management
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

Master keys are separately stored and managed as a separate key file so that the user can manage master keys in various ways according to individual security requirements. This key file contains all the information of master keys, so if it is leaked, there may be a security problem, and if it is lost, the encrypted data cannot be read (:ref:`tde-load-failure`). So, be careful to manage this key file.

By default, the key file is created with the name of **<database-name>_keys** at the location where the data volume is created when creating a database using **cubrid createdb** utility. Without additional configuration for the key file, this key file is automatically used. The location of the key file te be used can be changed by a system parameter. For more information, see :ref:`disk-parameters`.

The key file can contain several master keys (up to 128). A master key among those keys is set on the database to encrypt the database, data keys technically. One master key is created and set by default when the key file is created, and DBA can add, delete, change, and search keys using the TDE utility (:ref:`TDE utility<tde-utility>`). When deleting a key, the key to delete must exist in the key file, and the key set on the database currently cannot be removed. When changing a key to set to encrypt a database, both the previously key set on the database and the key to be set must exist in the key file. Through key inquiry, you can check the number of keys and creation time of them, and you can check the current key set on the database and setting time.

.. code-block:: bash

	$ cubrid tde --show-keys testdb
	Key File: /home/usr/CUBRID/databases/testdb/testdb_keys

	The current key set on testdb:
	Key Index: 2
	Created on Fri Nov 27 11:14:54 2020
	Set     on Fri Nov 27 11:15:30 2020

	Keys Information: 
	Key Index: 0 created on Fri Nov 27 11:11:27 2020
	Key Index: 1 created on Fri Nov 27 11:14:47 2020
	Key Index: 2 created on Fri Nov 27 11:14:54 2020
	Key Index: 3 created on Fri Nov 27 11:14:55 2020

	The number of keys: 4

.. note:: **Creating a database using an existing key file**
    
    If you want to create a new database using a key file that was previously managed for reasons such as security policy, copy or move the key file to the directory where the database will be created before creating it. The name of the key file must be changed to **<database_name>_keys**. If you're using the **tde_keys_file_path** system parameter, you have to copy the key file to the path.

.. _tde-enc-target:

Encryption Target
-----------------

.. _tde-enc-perm:

Permanent Data Encryption
^^^^^^^^^^^^^^^^^^^^^^^^^

The encrypted table data and all index data created on the table are encrypted. For more information on the encrypted table, see :ref:`create-tde-table`.

.. _tde-enc-temp:

Temporary Data Encryption
^^^^^^^^^^^^^^^^^^^^^^^^^

In addition to persistent data such as tables, temporary data created during queries related to encrypted tables are also encrypted. For example, all temporary data created in executing a query such as `SELECT * FROM tde_tbl ORDER BY att1` or creating an index on `tde_tbl` are encrypted when it is written to disk. For more information on temporary data, see :ref:`temporary-volumes`.

.. _tde-enc-log:

Log Data Encryption
^^^^^^^^^^^^^^^^^^^

Since the data which has to be encrypted may be included in the REDO and UNDO log records generated when the encrypted table is manipulated, all log data related to the encrypted table is encrypted. Encryption is applied to both the active log and the archive log. For more information on log volumes, see :ref:`database-volume`.

.. _tde-enc-dwb:

DWB Encryption
^^^^^^^^^^^^^^

Persistent data is temporarily written to the Double Write Buffer (DWB) before being written to the data volume. It may be encrypted even at this time because the data for the encrypted table can be included. For more information on DWB, see :ref:`database-volume`.

.. _tde-enc-backup:

Backup Encryption
^^^^^^^^^^^^^^^^^

If there are encrypted data in data volumes and log volumes, they are also stored as encrypted in backup volumes. For more information on backup, see :ref:`backupdb`.

**Backup Key File**

The backup volume contains the key file by default. If the backup volume, including the key file, is leaked, meaning the master key is also leaked. There may be a security problem even though the data in the volume is encrypted. To prevent this, you can backup the key file separately by using the **\-\-separate-keys** option. However, in the case of separating the key file, it must be managed carefully to prevent losing the key file for database restore. The separated backup key file is created in the same directory path as the backup volume and has the name **<database_name>_bk<backup_level>_keys**.

.. code-block:: bash

	$ cubrid backupdb -S --separate-keys testdb 
	Backup Volume Label: Level: 0, Unit: 0, Database testdb, Backup Time: Mon Nov 30 14:34:49 2020
	$ ls
	lob  testdb  testdb_bk0_keys  testdb_bk0v000  testdb_bkvinf  testdb_keys
	testdb_lgar_t  testdb_lgat  testdb_lginf  testdb_vinf

**The key file used to restore**

The key file separated during backup can be given as the key file for restoration by using the **\-\-keys-file-path** option (restoredb). If the valid key file does not exist in the specified path, restore fails.

If the **\-\-keys-file-path** option is not given, the key file to be used is searched according to the following priority. If the valid key file cannot be found, restore fails.

*Key file classification*

- Server key file: A key file that is generally used when running the server. It can be set with the tde_keys_file_path system parameter or in the default path same as the data volume.
- Backup key file: A key file created during backup included in the backup volume or separated by **\-\-separate-keys** option.

*The priority of the key file to use for restore*

#.  The backup key file that the backup volume contains.
#.  The backup key file created with the **\-\-separate-keys** option during backup (e.g. testdb_bk0_keys). This key file must exist in the same path as the backup volume.
#.  The server key file in the path specified by the **tde-keys-file-path** system parameter.
#.  The server key file in the same path as the data volume (e.g., testdb_keys).

 .. note::

  In the case of \(1\), If the backup volume contains a backup key file, the backup key file is copied with the same name as the one created by **\-\-separate-keys** during restore.

  Even if the valid key file is not found, restore could be successful if there is no encrypted data in the backup volume. However, since the key file does not exist, you cannot use TDE functions later.

.. note:: **Incremental Backup**

  When performing restoration using multiple level backup volumes by incremental backup, the backup key file of the level specified by the **\-\-level** option is used. If the **\-\-level** option is not specified, the highest level backup key file is used. If only the key file to be used exists, restore can succeed.

.. note:: **Loss of the backup key file**

  If the backup key file is lost, the restore would fail. However, if the key is not changed, the backup key file of the previous volume can be used by using the **\-\-keys-file-path** option. Also, if the key at the backup time exists in the server key, it can be used for backup recovery. Generally, restore can succeed if any key file that has the key intactly at the backup time is given.

.. note:: **The case in which the key is changed automatically after restore**

  Suppose the key set on the database does not exist in the server key file at the end of the restoration process. In that case, the backup key file is copied to the server key file, and the first key in the key file is arbitrarily set on the database for encrypting the database. This is because the key set on the database may not exist in any key file after the restore is complete.

.. _tde-algorithm:

Encryption Algorithm
--------------------

CUBRID supports the following encryption algorithms for TDE.

**TDE Encryption Algorithm**

=================================  =============  =============
 Algorithm                          Key Size       Option Name   
=================================  =============  =============
 Advanced Encryption Standard      256 bits       AES         
---------------------------------  -------------  -------------
 ARIA                               256 bits       ARIA        
=================================  =============  =============

Advanced Encryption Algorithm (AES) is a specification established by the National Institute of Standards and Technology (NIST) and is widely used worldwide. It has high stability and many optimizations are supported by many platforms such as hardware acceleration, so there is little performance degradation during encryption/decryption. ARIA is one of Korea's national standard encryption algorithms and is optimized for lightweight environments and hardware implementation.

.. note:: **Default Encryption Algorithm for TDE**

  If the algorithm is not specified when creating the TDE encryption table, AES is used by default. If you want to change the default encryption algorithm, you can specify it by the system parameter **tde_default_algorithm**. This default encryption algorithm is used to encrypt logs or temporary data in addition to tables. For details on specifying the encryption algorithm when creating a table, see :ref:`create-tde-table`.

.. _tde-check-enc:

Table Encryption Checking
-------------------------

You can check whether the table is encrypted by following three ways.

SHOW CREATE TABLE
^^^^^^^^^^^^^^^^^

.. code-block:: sql
    
    csql> show create table tde_tbl1;

    === <Result of SELECT Command in Line 1> ===

      TABLE                 CREATE TABLE        
    ============================================
      'tde_tbl1'          'CREATE TABLE [tde_tbl1] ([a] INTEGER) REUSE_OID, COLLATE iso88591_bin ENCRYPT=AES'

    1 row selected. (0.144627 sec) Committed.

    1 command(s) successfully processed.

Inquiry to db_class
^^^^^^^^^^^^^^^^^^^

Encryption of each table and encryption algorithm can be checked by the **tde_algorithm** column of the system catalog **db_class** or **_db_class**. For more information on the system catalog, see :ref:`catalog`.

.. code-block:: sql

    csql> select class_name, tde_algorithm from db_class where class_name like '%tde%';

    === <Result of SELECT Command in Line 1> ===

      class_name            tde_algorithm
    ============================================
      'tde_tbl1'          'AES'               
      'tde_tbl2'          'ARIA'              
      'not_tde_tbl'       'NONE'              

    3 rows selected. (0.057243 sec) Committed.

    1 command(s) successfully processed.

Using cubrid diagdb utility
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

You can check by referring to **tde_algorithm** among the file header information of encrypted tables and index files from the result by **cubrid diagdb** utility with  the -d1 (dump file tables) option. For more details, see :ref:`diagdb`.

.. code-block:: bash
    
    $ cubrid diagdb -d1 testdb
    ...
    Dumping file 0|3520 
            file header: 
                    vfid = 0|3520 
                    permanent 
                    regular 
                    tde_algorithm: AES
                    page: total = 64, user = 1, table = 1, free = 62 
                    sector: total = 1, partial = 1, full = 0, empty = 0  
    ...

.. _tde-ha:

TDE on HA
---------

In a HA environment, TDE is applied independently to each node. This means that for each node, the key file and TDE-related system parameters can be managed independently.

However, the TDE information of the replicated table is shared and the same. So, if the TDE module of the slave node is not loaded, the replication will stop when attempting to manipulate an encrypted table from the master node. In this case, not only the changes to a TDE-enctyped table, but also any subsequent changes cannot be replicated. Afterward, if the slave node's TDE configuration is correct and restarted, replication resumes from the stopped point.

.. _tde-load-failure:

When TDE is unavailable
-----------------------

In the following cases, the TDE feature cannot be used, and an error occurs because the TDE module cannot be loaded correctly.

* When the valid key file cannot be found
* When the key set on the database cannot be found in the key file

Even if the TDE module is not loaded, the server can start normally, and users can access unencrypted tables. This means that all DML and DDL such as SELECT and INSERT only for TDE-encrypted tables cannot be executed.

However, the case log data has been encrypted is different. If the log data is encrypted when the TDE module is not loaded and the log is accessed by recovery, HA, VACUUM, etc., the system cannot be properly executed, and the entire server has no option but to stop running the server.

.. _tde-restriction:

TDE Restriction
---------------

In addition to the restrictions described above, there are the following.

#. The replication log is not encrypted in HA.
#. CUBRID does not support the **ALTER TABLE** statement to change the TDE table option, which means you cannot set TDE to existing tables. If you want to do that, you need to move the data to the new table created with the TDE table option.
#. SQL log is not encrypted. For more information on the SQL log, see :ref:`sql-log-manage`.
