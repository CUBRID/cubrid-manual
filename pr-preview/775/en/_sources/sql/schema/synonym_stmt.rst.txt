
:meta-keywords: synonym definition, create synonym, alter synonym, drop synonym, rename synonym, use synonym
:meta-description: Define synonym in CUBRID database using create synonym, alter synonym, drop synonym and rename synonym statements.

*****************************
SYNONYM DEFINITION STATEMENTS
*****************************

CREATE SYNONYM
==============

Create another new name to use for an object that already exists in the database. The user can use the object with the name specified when creating the database object, and can also use the object as a synonym.

    #. The user can only create private synonym, and private synonym can only be used in the schema for the user.
    #. The information of the created synonym can be found in the :ref:`db-synonym` system virtual class.
    #. If a table or view already exists with the same name, a synonym cannot be created with that name.
    #. When creating a synonym, it checks whether the schema of the target object exists, but it does not check whether the target object exists. If the target object does not exist, an error occurs when using a synonym.
    #. The user can create a synonym only in the schema of the user, but **DBA** and members of **DBA** can create synonyms by specifying schema.

::

    CREATE [OR REPLACE] [PRIVATE] SYNONYM [schema_name_of_synonym.]synonym_name
    FOR [schema_name_of_target.]target_name
    [COMMENT 'synonym_comment_string'] ;

*   **OR REPLACE**: The existing synonym is replaced by a new one without error, even if *synonym_name* already exists in the schema.

    *   The existing synonym are dropped before creating a new one.

*   **PRIVATE**: Specifies to create a private synonym. Even if omitted, a private synonym is created by default.
*   *schema_name_of_synonym*: Specifies the schema name of the synonym. If omitted, the schema name of the current session is used.
*   *synonym_name*: Specifies the name of the synonym.
*   *schema_name_of_target*: Specifies the schema name of the target object. If omitted, the schema name of the current session is used.
*   *target_name*: Specifies the name of the target object.
*   *synonym_comment_string*: Specifies a comment of a synonym.

.. warning::

    The target object of a synonym supports only tables and views. It is possible to use another object as the target object, but an error occurs when using a synonym.

.. warning::
    
    It does not support public synonym yet.

1. Private synonym
------------------

In the example below, user u1 and user u2 have created a synonym with the same name but use different target objects.

.. code-block:: sql

    call login ('dba') on class db_user;

    /* current_user: dba */
    create user u1;
    create user u2;

    create table t1 (c1 varchar(100));
    insert into t1 values ('private synonym for user u1.');
    grant select on t1 to u1;
    grant select on t1 to u2;

    create table t2 (c1 varchar(100));
    insert into t2 values ('private synonym for user u2.');
    grant select on t2 to u1;
    grant select on t2 to u2;

.. code-block:: sql

    call login ('u1') on class db_user;

    /* current_user: u1 */
    create synonym s1 for dba.t1;
    select * from s1;

::

      c1
    ======================
      'private synonym for user u1.'

.. code-block:: sql

    call login ('u2') on class db_user;

    /* current_user: u2 */
    create synonym s1 for dba.t2;
    select * from s1;

::

      c1
    ======================
      'private synonym for user u2.'

2. Synonym information
----------------------

In the example below, the user can find the information of a synonym in the :ref:`db-synonym` system virtual class.

.. code-block:: sql

    /* There should be the result of example 1. */

    call login ('dba') on class db_user;

    /* current_user: dba */
    select * from db_synonym;

::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's1'                  'U1'                  'NO'                  't1'                  'DBA'                 NULL
      's1'                  'U2'                  'NO'                  't2'                  'DBA'                 NULL

.. code-block:: sql

    call login ('u1') on class db_user;

    /* current_user: u1 */
    select * from db_synonym;

::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's1'                  'U1'                  'NO'                  't1'                  'DBA'                 NULL

.. code-block:: sql

    call login ('u2') on class db_user;

    /* current_user: u2 */
    select * from db_synonym;

::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's1'                  'U2'                  'NO'                  't2'                  'DBA'                 NULL

.. code-block:: sql

    /* clean */

    call login ('dba') on class db_user;

    /* current_user: dba */
    drop synonym if exists u1.s1;
    drop synonym if exists u2.s1;
    drop user u1;
    drop user u2;

    drop table if exists t1;
    drop table if exists t2;

3. Synonym name
---------------

If a table or view already exists with the same name, a synonym cannot be created with that name.

.. code-block:: sql

    call login ('public') on class db_user;

    /* current_user: public */
    create table t1 (c1 varchar(100));
    insert into t1 values ('table for user public.');

    create table s1 (c1 varchar(100));
    create view s2 as select * from t1;

    /* Already used as a table name. */
    create synonym s1 for t1;

::

    ERROR: before ' ; '
    Class public.s1 already exists.

.. code-block:: sql

    /* Already used as a view name. */
    create synonym s2 for t1;

::

    ERROR: before ' ; '
    Class public.s2 already exists.

.. code-block:: sql

    create synonym s3 for t1;
    select * from s3;

::

      c1
    ======================
      'table for user public.'

.. code-block:: sql

    /* clean */

    call login ('public') on class db_user;

    /* current_user: public */
    drop synonym if exists s1;
    drop synonym if exists s2;
    drop synonym if exists s3;
    drop view if exists s2;
    drop table if exists t1;
    drop table if exists s1;

4. Synonym for synonym
----------------------

When creating a synonym, it does not check whether the target object exists, so the user can create a new synonym by specifying an existing synonym as the target object. However, if a synonym is used, the target object of the synonym, which is the target object, is not found again.

.. code-block:: sql

    call login ('public') on class db_user;

    /* current_user: public */
    create table t1 (c1 varchar(100));
    insert into t1 values ('synonym for synonym.');

    create synonym s1 for t1;
    create synonym s2 for s1;
    select * from db_synonym;

::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's1'                  'PUBLIC'              'NO'                  't1'                  'PUBLIC'              NULL
      's2'                  'PUBLIC'              'NO'                  's1'                  'PUBLIC'              NULL

.. code-block:: sql

    select * from s2;

::

    ERROR: before ' ; '
    Unknown class "public.s1".

.. code-block:: sql

    select * from s1;

::

      c1
    ======================
      'synonym for synonym.'

.. code-block:: sql

    /* clean */

    call login ('public') on class db_user;

    /* current_user: public */
    drop synonym if exists s1;
    drop synonym if exists s2;
    drop table if exists t1;

5. Creating schema-specified synonyms
-------------------------------------

When **DBA** and members of **DBA** create a synonym by specifying a schema, the synonym is created in the specified schema.

.. code-block:: sql

    call login ('dba') on class db_user;

    /* current_user: dba */
    create user u1;
    create user u2;

    create table t1 (c1 varchar(100));
    insert into t1 values ('private synonym for user u1.');
    grant select on t1 to u1;
    grant select on t1 to u2;

    create table t2 (c1 varchar(100));
    insert into t2 values ('private synonym for user u2.');
    grant select on t2 to u1;
    grant select on t2 to u2;

    create synonym u1.s1 for dba.t1;
    create synonym u2.s1 for dba.t2;

    select * from db_synonym;

::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's1'                  'U1'                  'NO'                  't1'                  'DBA'                 NULL
      's1'                  'U2'                  'NO'                  't2'                  'DBA'                 NULL


.. code-block:: sql

    call login ('u1') on class db_user;

    /* current_user: u1 */
    select * from s1;

::

      c1
    ======================
      'private synonym for user u1.'

.. code-block:: sql

    call login ('u2') on class db_user;

    /* current_user: u2 */
    select * from s1;

::

      c1
    ======================
      'private synonym for user u2.'

The user can create a synonym only in the schema of the user.

.. code-block:: sql

    call login ('u1') on class db_user;

    /* current_user: u1 */
    create synonym u2.s2 for dba.t1;

::

    ERROR: before ' ; '
    DBA, members of DBA group, and owner can perform CREATE SYNONYM.

.. code-block:: sql

    /* clean */

    call login ('dba') on class db_user;

    /* current_user: dba */
    drop synonym if exists u1.s1;
    drop synonym if exists u2.s1;
    drop user u1;
    drop user u2;

    drop table if exists t1;
    drop table if exists t2;

ALTER SYNONYM
=============

Change the target object or comment of a synonym. The synonym in use cannot be changed.

::

    ALTER [PRIVATE] SYNONYM [schema_name_of_synonym.]synonym_name
    {
	FOR [<schema_name_of_target>.]<target_name> [COMMENT 'comment_string'] |
	COMMENT 'synonym_comment_string'
    } ;

*   **PRIVATE**: Specifies to alter a private synonym. Even if omitted, a private synonym is created by default.
*   *schema_name_of_synonym*: Specifies the schema name of the synonym. If omitted, the schema name of the current session is used.
*   *synonym_name*: Specifies the name of the synonym.
*   *schema_name_of_target*: Specifies the schema name of the target object. If omitted, the schema name of the current session is used.
*   *target_name*: Specifies the name of the target object.
*   *synonym_comment_string*: Specifies a comment of a synonym.

.. warning::
    
    When the **ALTER**, **DROP**, **RENAME** statements for a synonym are executed, be careful because all query plans using the target object are deleted from the query plan cache.

    | However, when the **ALTER** statement is executed, the query plan is not deleted when changing to the same target object or only changing comments.

Change target object
--------------------

In the example below, the target object is changed.

.. code-block:: sql

    call login ('public') on class db_user;

    /* current_user: public */
    create table t1 (c1 varchar(100));
    insert into t1 values ('target table before change.');

    create table t2 (c1 varchar(100));
    insert into t2 values ('target table after change.');

    create synonym s1 for t1;
    select * from db_synonym;
    select * from s1;

::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's1'                  'PUBLIC'              'NO'                  't1'                  'PUBLIC'              NULL

      c1
    ======================
      'target table before change.'

.. code-block:: sql

    alter synonym s1 for t2;
    select * from db_synonym;
    select * from s1;

::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's1'                  'PUBLIC'              'NO'                  't2'                  'PUBLIC'              NULL

      c1
    ======================
      'target table after change.'

.. code-block:: sql

    /* clean */

    call login ('public') on class db_user;

    /* current_user: public */
    drop synonym if exists s1;
    drop table if exists t1;
    drop table if exists t2;

Change comment
--------------

In the example below, the user changes the comment of a synonym.

.. code-block:: sql

    call login ('public') on class db_user;

    /* current_user: public */
    create table t1 (c1 varchar(100));
    insert into t1 values ('change comment.');

    create synonym s1 for t1 comment 'It is a synonym for the t1 table.';
    select synonym_name, synonym_owner_name, is_public_synonym, comment from db_synonym;

::

      synonym_name          synonym_owner_name    is_public_synonym     comment
    ========================================================================================
      's1'                  'PUBLIC'              'NO'                  'It is a synonym for the t1 table.'

It is possible to change the comment without specifying the target object.

.. code-block:: sql

    alter synonym s1 comment 'the comment was changed.';
    select synonym_name, synonym_owner_name, is_public_synonym, comment from db_synonym;

::

      synonym_name          synonym_owner_name    is_public_synonym     comment
    ========================================================================================
      's1'                  'PUBLIC'              'NO'                  'the comment was changed.'

If both the target object and comment are not specified, an error occurs.

.. code-block:: sql

    alter synonym s1;

::

    ERROR: No options specified for ALTER SYNONYM.

To change a comment to **NULL**, change the comment to an empty string.

.. code-block:: sql

    alter synonym s1 comment '';
    select synonym_name, synonym_owner_name, is_public_synonym, comment from db_synonym;

::

      synonym_name          synonym_owner_name    is_public_synonym     comment
    ========================================================================================
      's1'                  'PUBLIC'              'NO'                  NULL

.. code-block:: sql

    /* clean */

    call login ('public') on class db_user;

    /* current_user: public */
    drop synonym if exists s1;
    drop table if exists t1;

DROP SYNONYM
============

Drop the synonym. The synonym in use cannot be dropped. Even if a synonym is dropped, the target object is not dropped.

::

    DROP [PRIVATE] SYNONYM [IF EXISTS] [schema_name.]synonym_name ;

*   **PRIVATE**: Specifies to drop a private synonym. Even if omitted, a private synonym is created by default.
*   **IF EXISTS**: No error occurs, Even if *synonym_name* does not exist in the schema.
*   *schema_name*: Specifies the schema name of the synonym. If omitted, the schema name of the current session is used.
*   *synonym_name*: Specifies the name of the synonym.

.. warning::
    
    When the **ALTER**, **DROP**, **RENAME** statements for a synonym are executed, be careful because all query plans using the target object are deleted from the query plan cache.

.. code-block:: sql

    call login ('public') on class db_user;

    /* current_user: public */
    create table t1 (c1 varchar(100));
    insert into t1 values ('The target object of the to-be-deleted synonym.');

    create synonym s1 for t1;
    select synonym_name, synonym_owner_name, is_public_synonym, comment from db_synonym;
    select * from s1;

::

      synonym_name          synonym_owner_name    is_public_synonym     comment
    ========================================================================================
      's1'                  'PUBLIC'              'NO'                  NULL

      c1
    ======================
      'The target object of the to-be-deleted synonym.'

.. code-block:: sql

    drop synonym s1;
    select synonym_name, synonym_owner_name, is_public_synonym, comment from db_synonym;

::

    There are no results.
    0 row selected.

.. code-block:: sql

    select * from s1;

::

    ERROR: before ' ; '
    Unknown class "public.s1".

.. code-block:: sql

    select * from t1;

::

      c1
    ======================
      'The target object of the to-be-deleted synonym.'

.. code-block:: sql

    /* clean */

    call login ('public') on class db_user;

    /* current_user: public */
    drop synonym if exists s1;
    drop table if exists t1;

RENAME SYNONYM
==============

Change the name of the synonym. The name of the synonym in use cannot be changed.

    #. The user cannot change the schema of the synonym when renaming a synonym.
    #. If a table, view, or synonym already exists with the name to be changed, the name cannot be changed.

::

    RENAME [PRIVATE] SYNONYM [schema_name_of_old_synonym.]old_synonym_name
    {AS | TO} [schema_name_of_new_synonym.]new_synonym_name ;

*   **PRIVATE**: Specifies to rename a private synonym. Even if omitted, a private synonym is created by default.
*   *schema_name_of_old_synonym*: Specifies the schema name of the to-be-renamed synonym. If omitted, the schema name of the current session is used.
*   *old_synonym_name*: Specifies the name of the to-be-renamed synonym.
*   *schema_name_of_new_synonym*: Specifies the schema name of synonym for new name. If omitted, the schema name of the current session is used.
*   *new_synonym_name*: Specifies the new name of the new-named synonym.

.. warning::
    
    When the **ALTER**, **DROP**, **RENAME** statements for a synonym are executed, be careful because all query plans using the target object are deleted from the query plan cache.

1. Cannot change schema
-----------------------

In the example below, an error occurs when a schema name is specified differently when renamed.

.. code-block:: sql

    call login ('dba') on class db_user;

    /* current_user: dba */
    create user u1;
    create user u2;

.. code-block:: sql

    call login ('u1') on class db_user;

    /* current_user: u1 */
    create table t1 (c1 varchar(100));
    insert into t1 values ('private synonym for user u1.');

    create synonym s1 for t1;
    select synonym_name, synonym_owner_name, is_public_synonym, comment from db_synonym;
    select * from s1;

::

      synonym_name          synonym_owner_name    is_public_synonym     comment
    ========================================================================================
      's1'                  'U1'                  'NO'                  NULL

      c1
    ======================
      'private synonym for user u1.'

.. code-block:: sql

    rename synonym s1 as u2.s2;
    rename synonym u1.s1 as u2.s2;

::

    ERROR: before ' ; '
    Rename cannot change owner.

.. code-block:: sql

    call login ('dba') on class db_user;

    /* current_user: dba */
    rename synonym u1.s1 as u2.s2;

::

    ERROR: before ' ; '
    Rename cannot change owner.

.. code-block:: sql

    call login ('u1') on class db_user;

    /* current_user: u1 */
    rename synonym s1 as s2;
    select synonym_name, synonym_owner_name, is_public_synonym, comment from db_synonym;
    select * from s2;

::

      synonym_name          synonym_owner_name    is_public_synonym     comment
    ========================================================================================
      's2'                  'U1'                  'NO'                  NULL

      c1
    ======================
      'private synonym for user u1.'

.. code-block:: sql

    /* clean */

    call login ('dba') on class db_user;

    /* current_user: dba */
    drop synonym if exists u1.s1;
    drop synonym if exists u1.s2;
    drop synonym if exists u2.s2;
    drop table if exists u1.t1;
    drop user u1;
    drop user u2;

2. Name already in use
----------------------

In the example below, an error occurs because the name to be changed is already in use.

.. code-block:: sql

    call login ('public') on class db_user;

    /* current_user: public */
    create table t1 (c1 varchar(100));
    insert into t1 values ('first table for user u1.');

    create table t2 (c1 varchar(100));
    insert into t2 values ('second table for user u1.');

    create table s_t1 (c1 varchar(100));
    create table s_v1 as select * from t1;
    create synonym s_s1 for t2;

    create synonym s1 for t1;
    select * from db_synonym;
    select * from s1;

::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's_s1'                'PUBLIC'              'NO'                  't2'                  'PUBLIC'              NULL
      's1'                  'PUBLIC'              'NO'                  't1'                  'PUBLIC'              NULL

      c1
    ======================
      'first table for user u1.'

.. code-block:: sql

    rename synonym s1 as s_t1;

::

    ERROR: before ' ; '
    Class dba.s_t1 already exists.

.. code-block:: sql

    rename synonym s1 as s_v1;

::

    ERROR: before ' ; '
    Class dba.s_v1 already exists.

.. code-block:: sql

    rename synonym s1 as s_s1;

::

    ERROR: before ' ; '
    Synonym "dba.s_s1" already exists.

.. code-block:: sql

    rename synonym s1 as s2;
    select * from db_synonym;
    select * from s2;

::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's_s1'                'PUBLIC'              'NO'                  't2'                  'PUBLIC'              NULL
      's2'                  'PUBLIC'              'NO'                  't1'                  'PUBLIC'              NULL

      c1
    ======================
      'first table for user u1.'

.. code-block:: sql

    /* clean */

    call login ('public') on class db_user;

    /* current_user: public */
    drop synonym if exists s1;
    drop synonym if exists s2;
    drop synonym if exists s_s1;
    drop table if exists t1;
    drop table if exists t2;
    drop table if exists s_t1;
    drop table if exists s_v1;

USE SYNONYM
=============

The synonym can be used only if table names and view names are available. When using synonyms, the user must have access authorization to the target object.

    #. **GRANT** and **REVOKE** cannot be done for synonyms. If the schema name of a synonym is specified, a synonym existing in another schema can be used.
    #. The synonym cannot be used in **ALTER**, **DROP**, **RENAME** statements and **TRUNCATE** statements that change the target object.

1. Use synonyms from other schemas
----------------------------------

.. code-block:: sql

    call login ('dba') on class db_user;

    /* current_user: dba */
    create user u1;
    create user u2;

.. code-block:: sql

    call login ('u1') on class db_user;

    /* current_user: u1 */
    create table t1 (c1 varchar(100));
    insert into t1 values ('first table for user u1.');
    grant select on t1 to u2;

    create synonym s1 for t1;
    select * from s1;

::

      c1
    ======================
      'first table for user u1.'

.. code-block:: sql

    call login ('u2') on class db_user;

    /* current_user: u2 */
    select * from s1;
    select * from u1.s1;

::

    ERROR: before ' ; '
    Unknown class "u2.s1".

      c1
    ======================
      'first table for user u1.'

.. code-block:: sql

    /* clean */

    call login ('dba') on class db_user;

    /* current_user: dba */
    drop synonym if exists u1.s1;
    drop table if exists u1.t1;
    drop user u1;
    drop user u2;

2. Statements where synonyms cannot be used
-------------------------------------------

.. code-block:: sql

    call login ('public') on class db_user;

    /* current_user: public */
    create table t1 (c1 varchar(100));
    insert into t1 values ('first table for user public.');

    create synonym s1 for t1;
    select * from db_synonym;
    select * from s1;

::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's1'                  'PUBLIC'              'NO'                  't1'                  'PUBLIC'              NULL

      c1
    ======================
      'first table for user public.'

.. code-block:: sql

   alter table s1 add column c2 int;

::

    ERROR: before '  add column c2 int; '
    Class public.s1 does not exist.

.. code-block:: sql

   drop table s1;

::

    ERROR: before ' ; '
    Class public.s1 does not exist.

.. code-block:: sql

   rename table s1 to s2;

::

    ERROR: before ' ; '
    Class public.s1 does not exist.

.. code-block:: sql

   truncate s1;

::

    ERROR: before ' ; '
    Class public.s1 does not exist.

.. code-block:: sql

   select * from s1;

::

      c1
    ======================
      'first table for user public.'

.. code-block:: sql

    /* clean */

    call login ('public') on class db_user;

    /* current_user: public */
    drop synonym if exists s1;
    drop table if exists t1;
    drop table if exists s2;
