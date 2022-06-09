

**************
동의어 정의문
**************

CREATE SYONYM
=============

이미 데이터베이스에 존재하는 객체를 사용할 수 있는 또 다른 새로운 이름을 만든다. 사용자는 데이터베이스 객체를 생성할 때 지정한 이름으로 객체를 사용할 수 있고, 동의어로도 객체를 사용할 수 있다.

    * 사용자는 전용(Private) 동의어만 생성할 수 있으며, 전용(Private) 동의어는 사용자의 스키마에서만 사용할 수 있다.
    * 생성된 동의어의 정보는 :ref:`db_synonym <db_synonym>` 시스템 가상 클래스에서 조회할 수 있다.
    * 동의어를 대상으로 **GRANT** 및 **REVOKE**\를 할 수 없다.

::

    CREATE [OR REPLACE] [PRIVATE] SYNONYM [schema_name_of_synonym.]synonym_name
    FOR [schema_name_of_target.]object_name
    [COMMENT 'synonym_comment_string'] ;

*   **OR REPLACE**: 스키마에 *synonym_name*\이 이미 존재하더라도 에러를 출력하지 않고 기존의 동의어가 새로운 동의어로 대체된다.

    *  새 동의어를 생성하기 전에 기존 동의어는 삭제된다.

*   **PRIVATE**: 전용(Private) 동의어를 생성하도록 지정한다. 생략해도 기본값으로 전용(Private) 동의어를 생성한다.
*   *schema_name_of_synonym*: 동의어의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *synonym_name*: 동의어의 이름을 지정한다.
*   *schema_name_of_target*: 대상 객체의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *object_name*: 대상 객체의 이름을 지정한다.
*   *synonym_comment_string*: 동의어의 커멘트를 지정한다.

.. warning::
    
    아직은 공용(Public) 동의어를 지원하지 않는다.

동의어 이름
-----------

동일한 이름의 테이블이나 뷰가 이미 존재하는 경우 해당 이름으로 동의어를 생성할 수 없다.

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

동의어에 대한 동의어
--------------------

사용자가 기존 동의어를 대상 객체로 지정하여 새로운 동의어를 만들 수 있지만 사용할 수는 없다.

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

*   **PRIVATE**: 전용(Private) 동의어를 변경하도록 지정한다. 생략해도 기본값으로 전용(Private) 동의어를 생성한다.
*   *schema_name_of_synonym*: 동의어의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *synonym_name*: 동의어의 이름을 지정한다.
*   *schema_name_of_target*: 대상 객체의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *object_name*: 대상 객체의 이름을 지정한다.
*   *synonym_comment_string*: 동의어의 커멘트를 지정한다.

대상 객체 변경
--------------

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

커멘트 변경
------------

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

아직은 대상 객체를 지정하지 않고 커멘트를 변경할 수 없다.

.. code-block:: sql

    ALTER SYNONYM s1 FOR t1 COMMENT 'The comment was changed.';

.. code-block::

      synonym_name          synonym_owner_name    is_public_synonym     comment
    ========================================================================================
      's1'                  'PUBLIC'              'NO'                  'The comment was changed.'

.. warning::
    
    동의어에 대한 **ALTER**, **DROP**, **RENAME** 문이 실행되면 쿼리 계획 캐시에서 대상 객체를 사용하는 쿼리 계획을 모두 삭제하므로 주의해야 한다.

DROP SYONYM
===========

::

    DROP [PRIVATE] SYNONYM [IF EXISTS] [schema_name.]synonym_name ;

*   **PRIVATE**: 전용(Private) 동의어를 삭제하도록 지정한다. 생략해도 기본값으로 전용(Private) 동의어를 생성한다.
*   **IF EXISTS**: 스키마에 *synonym_name*\이 존재하지 않더라도 에러가 발생하지 않는다.
*   *schema_name*: 동의어의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *synonym_name*: 동의어의 이름을 지정한다.

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

*   **PRIVATE**: 전용(Private) 동의어를 변경하도록 지정한다. 생략해도 기본값으로 전용(Private) 동의어를 생성한다.
*   *schema_name_of_old_synonym*: 이름을 바꿀 동의어의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *old_synonym_name*: 이름을 바꿀 동의어의 이름을 지정한다.
*   *schema_name_of_new_synonym*: 새로운 이름의 동의어에 대한 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *new_synonym_name*: 동의어의 새로운 이름을 지정한다.

.. code-block:: sql

    CREATE TABLE t1 (c1 INT);

    CREATE SYNONYM s1 FOR t1;
    SELECT * FROM db_synonym;

.. code-block::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's1'                  'PUBLIC'              'NO'                  't1'                  'PUBLIC'              NULL

.. code-block:: sql

   /* CURRENT_USER: DBA */
    RENAME SYNONYM s1 AS s2;
    SELECT * FROM db_synonym;

.. code-block::

      synonym_name          synonym_owner_name    is_public_synonym     target_name           target_owner_name     comment
    ====================================================================================================================================
      's2'                  'PUBLIC'              'NO'                  't1'                  'PUBLIC'              NULL

이름 변경할 동의어의 스키마 이름과 새로운 이름의 동의어에 대한 스키마 이름은 같아야 한다.

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
