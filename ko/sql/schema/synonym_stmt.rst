
:meta-keywords: synonym definition, create synonym, alter synonym, drop synonym, rename synonym, use synonym
:meta-description: Define synonym in CUBRID database using create synonym, alter synonym, drop synonym and rename synonym statements.

**************
동의어 정의문
**************

CREATE SYNONYM
==============

이미 데이터베이스에 존재하는 객체를 사용할 수 있는 또 다른 새로운 이름을 만든다. 사용자는 데이터베이스 객체를 생성할 때 지정한 이름으로 객체를 사용할 수 있고, 동의어로도 객체를 사용할 수 있다.

    #. 사용자는 전용(Private) 동의어만 생성할 수 있으며, 전용(Private) 동의어는 사용자의 스키마에서만 사용할 수 있다.
    #. 생성된 동의어의 정보는 :ref:`db-synonym` 시스템 가상 클래스에서 조회할 수 있다.
    #. 같은 이름의 테이블이나 뷰가 이미 존재하는 경우 해당 이름으로 동의어를 생성할 수 없다.
    #. 동의어를 생성할 때 대상 객체의 스키마가 존재하는지는 확인하지만 대상 객체가 존재하는지는 확인하지 않는다. 대상 객체가 존재하지 않으면 동의어를 사용할 때 에러가 발생한다.
    #. 사용자는 해당 사용자의 스키마에서만 동의어를 생성할 수 있지만 DBA와 DBA의 멤버는 스키마를 지정해서 동의어를 생성할 수 있다.

::

    CREATE [OR REPLACE] [PRIVATE] SYNONYM [schema_name_of_synonym.]synonym_name
    FOR [schema_name_of_target.]target_name
    [COMMENT 'synonym_comment_string'] ;

*   **OR REPLACE**: 스키마에 *synonym_name*\이 이미 존재하더라도 에러를 출력하지 않고 기존의 동의어가 새로운 동의어로 대체된다.

    *  새로운 동의어를 생성하기 전에 기존 동의어는 삭제된다.

*   **PRIVATE**: 전용(Private) 동의어를 생성하도록 지정한다. 생략해도 기본값으로 전용(Private) 동의어를 생성한다.
*   *schema_name_of_synonym*: 동의어의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *synonym_name*: 동의어의 이름을 지정한다.
*   *schema_name_of_target*: 대상 객체의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *target_name*: 대상 객체의 이름을 지정한다.
*   *synonym_comment_string*: 동의어의 커멘트를 지정한다.

.. warning::

    동의어의 대상 객체는 테이블과 뷰만 지원한다. 다른 객체를 대상 객체로 사용할 수는 있지만 동의어를 사용할 때 에러가 발생한다.

.. warning::
    
    아직은 공용(Public) 동의어를 지원하지 않는다.

1. 전용(Private) 동의어
-----------------------

아래 예제에서 u1 사용자와 u2 사용자는 같은 이름으로 동의어를 생성했지만 다른 대상 객체를 사용한다.

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

2. 동의어 정보
--------------

아래 예제에서 사용자는 :ref:`db-synonym` 시스템 가상 클래스에서 동의어의 정보를 확인할 수 있다.

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

3. 동의어 이름
--------------

같은 이름의 테이블이나 뷰가 이미 존재하는 경우 해당 이름으로 동의어를 생성할 수 없다.

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

4. 동의어에 대한 동의어
-----------------------

동의어를 생성할 때 대상 객체가 존재하는지는 확인하지 않으므로, 사용자가 기존 동의어를 대상 객체로 지정하여 새로운 동의어를 만들 수 있다. 하지만 동의어를 사용할 때 대상 객체인 동의어의 대상 객체를 다시 찾지 않는다.

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

5. 스키마 지정 동의어 생성
--------------------------

**DBA**\와 **DBA**\의 멤버가 스키마를 지정하여 동의어를 생성하면, 동의어는 지정한 스키마에 만들어진다.

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

사용자는 해당 사용자의 스키마에서만 동의어를 생성할 수 있다.

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

동의어의 대상 객체나 커멘트를 변경한다. 사용 중인 동의어는 변경할 수 없다.

::

    ALTER [PRIVATE] SYNONYM [schema_name_of_synonym.]synonym_name
    {
	FOR [<schema_name_of_target>.]<target_name> [COMMENT 'comment_string'] |
	COMMENT 'synonym_comment_string'
    } ;

*   **PRIVATE**: 전용(Private) 동의어를 변경하도록 지정한다. 생략해도 기본값으로 전용(Private) 동의어를 생성한다.
*   *schema_name_of_synonym*: 동의어의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *synonym_name*: 동의어의 이름을 지정한다.
*   *schema_name_of_target*: 대상 객체의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *target_name*: 대상 객체의 이름을 지정한다.
*   *synonym_comment_string*: 동의어의 커멘트를 지정한다.

.. warning::
    
    동의어에 대한 **ALTER**, **DROP**, **RENAME** 문이 실행되면 쿼리 계획 캐시에서 대상 객체를 사용하는 쿼리 계획을 모두 삭제하므로 주의해야 한다.

    | 하지만 **ALTER** 문을 실행할 때 같은 대상 객체로 변경하거나 커멘트만 변경하는 경우에는 쿼리 계획을 삭제하지 않는다.

대상 객체 변경
--------------

아래 예제에서 대상 객체를 변경한다.

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

커멘트 변경
------------

아래 예제에서 사용자는 동의어의 커멘트를 변경한다.

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

대상 객체를 지정하지 않고 커멘트를 변경할 수 있다.

.. code-block:: sql

    alter synonym s1 comment 'the comment was changed.';
    select synonym_name, synonym_owner_name, is_public_synonym, comment from db_synonym;

::

      synonym_name          synonym_owner_name    is_public_synonym     comment
    ========================================================================================
      's1'                  'PUBLIC'              'NO'                  'the comment was changed.'

대상 객체와 커멘트를 모두 지정하지 않으면 에러가 발생한다.

.. code-block:: sql

    alter synonym s1;

::

    ERROR: No options specified for ALTER SYNONYM.

커멘트를 **NULL**\로 변경하려면, 커멘트를 빈 문자열로 변경하면 된다.

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

동의어를 삭제한다. 사용 중인 동의어는 삭제할 수 없다. 동의어를 삭제해도 대상 객체는 삭제되지 않는다.

::

    DROP [PRIVATE] SYNONYM [IF EXISTS] [schema_name.]synonym_name ;

*   **PRIVATE**: 전용(Private) 동의어를 삭제하도록 지정한다. 생략해도 기본값으로 전용(Private) 동의어를 생성한다.
*   **IF EXISTS**: 스키마에 *synonym_name*\이 존재하지 않더라도 에러가 발생하지 않는다.
*   *schema_name*: 동의어의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *synonym_name*: 동의어의 이름을 지정한다.

.. warning::
    
    동의어에 대한 **ALTER**, **DROP**, **RENAME** 문이 실행되면 쿼리 계획 캐시에서 대상 객체를 사용하는 쿼리 계획을 모두 삭제하므로 주의해야 한다.

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

동의어의 이름을 변경한다. 사용 중인 동의어의 이름은 변경할 수 없다.

    #. 사용자는 동의어의 이름을 변경하면서 동의어의 스키마를 변경할 수 없다.
    #. 변경하는 이름의 테이블이나 뷰, 동의어가 이미 존재하는 경우 이름을 변경할 수 없다.

::

    RENAME [PRIVATE] SYNONYM [schema_name_of_old_synonym.]old_synonym_name
    {AS | TO} [schema_name_of_new_synonym.]new_synonym_name ;

*   **PRIVATE**: 전용(Private) 동의어를 변경하도록 지정한다. 생략해도 기본값으로 전용(Private) 동의어를 생성한다.
*   *schema_name_of_old_synonym*: 이름을 바꿀 동의어의 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *old_synonym_name*: 이름을 바꿀 동의어의 이름을 지정한다.
*   *schema_name_of_new_synonym*: 새로운 이름의 동의어에 대한 스키마 이름을 지정한다. 생략하면 현재 세션의 스키마 이름을 사용한다.
*   *new_synonym_name*: 동의어의 새로운 이름을 지정한다.

.. warning::
    
    동의어에 대한 **ALTER**, **DROP**, **RENAME** 문이 실행되면 쿼리 계획 캐시에서 대상 객체를 사용하는 쿼리 계획을 모두 삭제하므로 주의해야 한다.

1. 스키마를 변경할 수 없음
--------------------------

아래 예제에서 rename 시 스키마 이름을 다르게 지정할 때 에러가 발생한다.

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

2. 이미 사용 중인 이름
----------------------

아래 예제에서 변경할 이름이 이미 사용중이기 때문에 에러가 발생한다.

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

동의어 사용
===========

동의어는 테이블 이름과 뷰 이름을 사용할 수 있는 경우에만 사용할 수 있다.

    #. 동의어를 대상으로 **GRANT** 및 **REVOKE**\를 할 수 없다. 동의어의 스키마 이름이 지정되면 다른 스키마에 존재하는 동의어도 사용할 수 있다.
    #. 대상 객체를 변경하는 **ALTER**, **DROP**, **RENAME** 문과 **TRUNCATE** 문에서는 동의어를 사용할 수 없다.

1. 다른 스키마의 동의어 사용
------------------------------

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

2. 동의어를 사용할 수 없는 구문
-------------------------------

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
