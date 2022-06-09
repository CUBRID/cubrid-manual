

*****************************
SYNONYM DEFINITION STATEMENTS
*****************************

CREATE SYONYM
=============

Create another new name to use for an object that already exists in the database. The user can use the object with the name specified when creating the database object, and can also use the object as a synonym.

    * The user can only create private synonym, and private synonym can only be used in the schema for the user.
    * The information of the created synonym can be found in the **db_synonym** system virtual class.
    * **GRANT** and **REVOKE** cannot be done for synonyms.

::

    CREATE [OR REPLACE] [PRIVATE] SYNONYM [schema_name_of_synonym.]synonym_name
    FOR [schema_name_of_target.]object_name
    [COMMENT 'synonym_comment_string'] ;

*   **OR REPLACE**: The existing synonym is replaced by a new one without error, even if *synonym_name* already exists in the schema.

    *   The existing synonym are dropped before creating a new one.

*   **PRIVATE**: Specifies to create a private synonym. Even if omitted, a private synonym is created by default.
*   *schema_name_of_synonym*: Specifies the schema name of the synonym. If omitted, the schema name of the current session is used.
*   *synonym_name*: Specifies the name of the synonym.
*   *schema_name_of_target*: Specifies the schema name of the target object. If omitted, the schema name of the current session is used.
*   *object_name*: Specifies the name of the target object.
*   *comment_string*: Specifies a comment of a synonym.

.. warning::
    
    It does not support public synonym yet.

SYONYM NAME
-----------

Synonym names cannot be duplicated with table or view names. If a table or view already exists with the same name, a synonym cannot be created with that name.

.. code-block:: sql

    CREATE TABLE t1 (c1 INT);
    CREATE TABLE t2 (c1 VARCHAR);

    CREATE SYNONYM t1 FOR t2;

.. code-block::

    ERROR: before ' ; '
    Class public.t1 already exists.

.. code-block:: sql

    CREATE SYNONYM s1 FOR t2;

.. code-block::

    Execute OK.

Synonym for synonym
-------------------

The user can create new synonyms by specifying existing synonyms as target objects, but cannot use them.

.. code-block:: sql

    CREATE TABLE t1 (c1 INT);
    CREATE SYNONYM s1 FOR t1;
    CREATE SYNONYM s2 FOR s1;

.. code-block::

    Execute OK.

.. code-block:: sql

    SELECT * FROM s2;

.. code-block::

    ERROR: before ' ; '
    Unknown class "public.s1".

ALTER SYONYM
============

::

    ALTER [PRIVATE] SYNONYM [schema_name_of_synonym.]synonym_name
    FOR [schema_name_of_target.]object_name
    [COMMENT 'synonym_comment_string'] ;

*   **PRIVATE**: Specifies to alter a private synonym. Even if omitted, a private synonym is created by default.
*   *schema_name_of_synonym*: Specifies the schema name of the synonym. If omitted, the schema name of the current session is used.
*   *synonym_name*: Specifies the name of the synonym.
*   *schema_name_of_target*: Specifies the schema name of the target object. If omitted, the schema name of the current session is used.
*   *object_name*: Specifies the name of the target object.
*   *comment_string*: Specifies a comment of a synonym.

Change target object
--------------------

.. code-block:: sql

    CREATE TABLE t1 (c1 INT);
    CREATE TABLE t2 (c1 VARCHAR);

    INSERT INTO t1 VALUES (1);
    INSERT INTO t2 VALUES ('A');

    CREATE SYNONYM s1 FOR t1;
    SELECT * FROM db_synonym;
    SELECT * FROM s1;

.. code-block::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's1'                  'PUBLIC'              'NO'                  't1'                  'PUBLIC'              NULL

               c1
    =============
                1

.. code-block:: sql

    ALTER SYNONYM s1 FOR t2;

    SELECT * FROM db_synonym;
    SELECT * FROM s1;

.. code-block::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's1'                  'PUBLIC'              'NO'                  't2'                  'PUBLIC'              NULL

      c1
    ======================
      'A'

Change comment
--------------

.. code-block:: sql

    CREATE TABLE t1 (c1 INT);

    CREATE SYNONYM s1 FOR t1 COMMENT 'It is a synonym for the t1 table.';
    SELECT synonym_name, synonym_owner_name, is_public_synonym, comment FROM db_synonym;

.. code-block::

      synonym_name          synonym_owner_name    is_public_synonym     comment
    ========================================================================================
      's1'                  'PUBLIC'              'NO'                  'It is a synonym for the t1 table.'

.. code-block:: sql

    ALTER SYNONYM s1 COMMENT 'The comment was changed.';

.. code-block::

    ERROR: Invalid alter synonym.
      ALTER [PRIVATE] SYNONYM [<user_name>.]<synonym_name> FOR [<user_name>.]<target_name> [COMMENT 'comment_string']

It is not possible to change the comment without specifying the target object yet.

.. code-block:: sql

    ALTER SYNONYM s1 FOR t1 COMMENT 'The comment was changed.';

.. code-block::

      synonym_name          synonym_owner_name    is_public_synonym     comment
    ========================================================================================
      's1'                  'PUBLIC'              'NO'                  'The comment was changed.'

.. warning::
    
    When the **ALTER**, **DROP**, **RENAME** statements for a synonym are executed, be careful because all query plans using the target object are deleted from the query plan cache.

DROP SYONYM
===========

::

    DROP [PRIVATE] SYNONYM [IF EXISTS] [schema_name.]synonym_name ;

*   **PRIVATE**: Specifies to drop a private synonym. Even if omitted, a private synonym is created by default.
*   **IF EXISTS**: No error occurs, Even if *synonym_name* does not exist in the schema.
*   *schema_name*: Specifies the schema name of the synonym. If omitted, the schema name of the current session is used.
*   *synonym_name*: Specifies the name of the synonym.

.. code-block:: sql

    CREATE TABLE t1 (c1 INT);

    CREATE SYNONYM s1 FOR t1;
    SELECT synonym_name, synonym_owner_name, is_public_synonym FROM db_synonym;

.. code-block::

      synonym_name          synonym_owner_name    is_public_synonym
    ==================================================================
      's1'                  'PUBLIC'              'NO'

.. code-block:: sql

    DROP SYNONYM s1;
    SELECT synonym_name, synonym_owner_name, is_public_synonym FROM db_synonym;

.. code-block::

    There are no results.
    0 row selected.

RENAME SYONYM
=============

::

    RENAME [PRIVATE] SYNONYM [schema_name_of_old_synonym.]old_synonym_name
    [AS | TO] [schema_name_of_new_synonym.]new_synonym_name ;

*   **PRIVATE**: Specifies to rename a private synonym. Even if omitted, a private synonym is created by default.
*   *schema_name_of_old_synonym*: Specifies the schema name of the to-be-renamed synonym. If omitted, the schema name of the current session is used.
*   *old_synonym_name*: Specifies the name of the to-be-renamed synonym.
*   *schema_name_of_new_synonym*: Specifies the schema name of synonym for new name. If omitted, the schema name of the current session is used.
*   *new_synonym_name*: Specifies the new name of the new-named synonym.

.. code-block:: sql

    CREATE TABLE t1 (c1 INT);

    CREATE SYNONYM s1 FOR t1;
    SELECT * FROM db_synonym;

.. code-block::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's1'                  'PUBLIC'              'NO'                  't1'                  'PUBLIC'              NULL

.. code-block:: sql

    RENAME SYNONYM s1 AS s2;
    SELECT * FROM db_synonym;

.. code-block::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's2'                  'PUBLIC'              'NO'                  't1'                  'PUBLIC'              NULL

The schema name for the to-be-renamed synonym and the schema name for the new-named synonym must be the same.

.. code-block:: sql

    /* CURRENT_USER: PUBLIC */
    CREATE TABLE t1 (c1 INT);

    CREATE SYNONYM s1 FOR t1;
    SELECT * FROM db_synonym;

.. code-block::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's1'                  'PUBLIC'              'NO'                  't1'                  'PUBLIC'              NULL

.. code-block:: sql

    /* CURRENT_USER: DBA */
    CREATE USER u1;
    RENAME SYNONYM public.s1 AS u1s2;

.. code-block::

    ERROR: before ' ; '
    Rename cannot change owner.
