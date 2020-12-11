
:meta-keywords: security, cubrid security, authorization, acl, access control list, ssl, tls, secure socket layer, packet encryption, tde, transparent data encryption, cubrid tde, data-at-rest encryption, kernel encryption, engine encryption, key management
:meta-description: CUBRID Security includes authorization for specific objects, access control to a database, packet encryption, data encryption, etc. Users can manage user data more safely by using CUBRID Security features.

***************
CUBRID Security
***************
This chapter describes the CUBRID security features. CUBRID provides Packet Encryption, ACL(Access Control List), authorization, and TDE(Transparent Data Encryption) features to protect the user database.

Packet Encryption
=================

.. _access-control:

ACL (Access Control List)
=========================

CUBRID supports the ACL function that can be accessed only for users or applications authorized in two layers: the broker layer and the database server layer.
The ACL of the broker layer is mainly used to control the access of application programs such as WEB and WAS, and the ACL of the database server layer is mainly used to control the access of the brokers and csqls.

For more details, see :ref:`limiting-broker-access` and :ref:`limiting-server-access`. 

.. _authorization:

Authorization
=============

CUBRID can create users(or groups), and provides a function to control the access of the other users(or groups) to tables created by a user.

If you want to allow other users(or groups) to access your tables, you could provide access privileges to the users(or groups) by :ref:`granting-authorization`. Also, to revoke access previleges of other users, you can use :ref:`revoking-authorization`. The access to the (virtual) table created by a PUBLIC user is allowed to all users.

For more details, see :doc:`/sql/authorization`.


TDE (Transparent Data Encryption)
=================================
