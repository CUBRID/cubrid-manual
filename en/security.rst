
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

CUBRID supports the ACL function that can be accessed only for users or applications authorized in two layers, broker layer and database server layer.
The ACL of the broker layer is mainly used to control the access of application programs such as WEB and WAS, and the ACL of database server layer is mainly used to control the access of the brokers and csqls.

For more details, see :ref:`limiting-broker-access` and :ref:`limiting-server-access`. 

.. _authorization:

Authorization
=============

큐브리드는 사용자(그룹)를 생성할 수 있고, 사용자가 생성한 테이블에 대해 다른 사용자(그룹)의 접근 여부를 제어할 수 있는 기능을 제공한다.

자신이 만든 테이블에 다른 사용자(그룹)의 접근을 허용하려면 :ref:`granting-authorization` 구문을 사용하여 해당 사용자(그룹)에게 적절한 권한을 제공해야 한다. 접근 권한을 해지하기 위해서는 :ref:`revoking-authorization` 구문을 사용하여 해당 사용자(그룹)으로부터 권한을 회수할 수 있다. PUBLIC 사용자가 생성한 (가상) 테이블은 권한 제공 절차 없이 모든 사용자에게 접근이 허용된다.

For more details, see :doc:`/sql/authorization`.


TDE (Transparent Data Encryption)
=================================


