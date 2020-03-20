
:meta-keywords: cubrid authorization, cubrid dba, cubrid user
:meta-description: CUBRID has two types of users by default: DBA and PUBLIC.  DBA and DBA members can create, drop and alter users by using SQL statements.

***********
사용자 관리
***********

데이터베이스 사용자
===================

사용자 이름 작성 원칙은 :doc:`identifier` 절을 참고한다.

CUBRID는 기본적으로 **DBA** 와 **PUBLIC** 두 종류의 사용자를 제공한다. 처음 제품을 설치했을 때에는 비밀번호가 설정되어 있지 않다.

*   모든 사용자는 **PUBLIC** 사용자에게 부여된 권한을 소유한다. 데이터베이스의 모든 사용자는 **PUBLIC** 의 멤버가 된다. 사용자에게 권한을 부여하는 방법은 **PUBLIC** 사용자에 대한 권한을 부여하는 것이다.

*   **DBA** 는 데이터베이스 관리자를 위한 권한을 소유한다. **DBA** 는 자동으로 모든 사용자와 그룹의 멤버가 된다. 즉, **DBA** 는 모든 테이블에 대한 접근 권한을 갖는다. 따라서 **DBA** 와 **DBA** 의 멤버에게 명시적으로 권한을 부여할 필요는 없다. 데이터베이스 사용자는 고유한 이름을 갖는다. 데이터베이스 관리자는 **cubrid createdb** 유틸리티를 이용하여 일괄적으로 사용자를 생성할 수 있다(자세한 내용은 :ref:`cubrid-utilities` 참조). 데이터베이스 사용자는 동일한 권한을 갖는 멤버를 소유할 수 없다. 사용자에게 권한이 부여되면 해당 사용자의 모든 멤버는 자동으로 동일한 권한을 소유한다.

.. _create-user:

CREATE/ALTER/DROP USER
======================

**DBA** 와 **DBA** 의 멤버는 SQL 문을 사용하여 사용자를 생성, 변경, 삭제할 수 있다. ::

    CREATE USER user_name
    [PASSWORD password]
    [GROUPS user_name [{, user_name } ... ]]
    [MEMBERS user_name [{, user_name } ... ]] 
    [COMMENT 'comment_string'];
    
    ALTER USER user_name PASSWORD password;
    
    DROP USER user_name;

*   *user_name*: 생성, 삭제, 변경할 사용자 이름을 지정한다.
*   *password*: 생성 혹은 변경할 사용자의 비밀번호를 지정한다.
*   *comment_string*: 사용자에 대한 커멘트를 지정한다.

다음은 사용자 *Fred* 를 생성하고 비밀번호를 변경한 후에 *Fred* 를 삭제하는 예제이다.

.. code-block:: sql

    CREATE USER Fred;
    ALTER USER Fred PASSWORD '1234';
    DROP USER Fred;

다음은 사용자를 생성하고 생성된 사용자에 멤버를 추가하는 예제이다. 다음 문장을 통해 *company* 는 *engineering*, *marketing*, *design* 을 멤버로 가지는 그룹이 된다. *marketing* 은 *smith*, *jones* 를, *design* 은 *smith* 를, *engineering* 은 *brown* 을 멤버로 가지는 그룹이 된다.

.. code-block:: sql

    CREATE USER company;
    CREATE USER engineering GROUPS company;
    CREATE USER marketing GROUPS company;
    CREATE USER design GROUPS company;
    CREATE USER smith GROUPS design, marketing;
    CREATE USER jones GROUPS marketing;  
    CREATE USER brown GROUPS engineering;

다음은 위와 동일한 그룹을 생성하는 예이지만 **GROUPS** 대신 **MEMBERS** 문을 사용하는 예제이다.

.. code-block:: sql

    CREATE USER smith;
    CREATE USER brown;
    CREATE USER jones;
    CREATE USER engineering MEMBERS brown;
    CREATE USER marketing MEMBERS smith, jones;
    CREATE USER design MEMBERS smith;
    CREATE USER company MEMBERS engineering, marketing, design;

사용자의 커멘트
---------------

사용자에 대한 커멘트는 다음과 같이 지정한다.

.. code-block:: sql

    CREATE USER designer GROUPS dbms, qa COMMENT 'user comment';

사용자에 대한 커멘트는 ALTER USER 문을 사용하여 다음과 같이 변경이 가능하다.

.. code-block:: sql
    
    ALTER USER DESIGNER COMMENT 'new comment';
    
다음 구문으로 사용자에 대한 커멘트를 확인할 수 있다.

.. code-block:: sql

    SELECT name, comment FROM db_user;

.. _granting-authorization:

GRANT
=====

CUBRID에서 권한 부여의 최소 단위는 테이블이다. 자신이 만든 테이블에 다른 사용자(그룹)의 접근을 허용하려면 해당 사용자(그룹)에게 적절한 권한을 부여해야 한다.

권한이 부여된 그룹에 속한 모든 멤버는 같은 권한을 소유하므로 모든 멤버에게 개별적으로 권한을 부여할 필요는 없다. **PUBLIC** 사용자가 생성한 (가상) 테이블은 모든 사용자에게 접근이 허용된다. **GRANT** 문을 사용하여 사용자에게 접근 권한을 부여할 수 있다. ::

    GRANT operation [ { ,operation } ... ] ON table_name [ { ,table_name } ... ]
    TO user [ { ,user } ... ] [ WITH GRANT OPTION ] ; 

*   *operation*: 권한을 부여할 때 사용 가능한 연산을 나타낸다.

    *   **SELECT**: 테이블 정의 내용을 읽을 수 있고 인스턴스 조회가 가능. 가장 일반적인 유형의 권한.
    *   **INSERT**: 테이블의 인스턴스를 생성할 수 있는 권한.
    *   **UPDATE**: 테이블에 이미 존재하는 인스턴스를 수정할 수 있는 권한.
    *   **DELETE**: 테이블의 인스턴스를 삭제할 수 있는 권한.
    *   **ALTER**: 테이블의 정의를 수정할 수 있고, 테이블의 이름을 변경하거나 삭제할 수 있는 권한.
    *   **INDEX**: 검색 속도의 향상을 위해 칼럼에 인덱스를 생성할 수 있는 권한.
    *   **EXECUTE**: 테이블 메서드 혹은 인스턴스 메서드를 호출할 수 있는 권한.
    *   **ALL PRIVILEGES**: 앞서 설명한 7가지 권한을 모두 포함.

* *table_name*: 권한을 부여할 테이블 혹은 뷰의 이름을 지정한다.
* *user*: 권한을 부여할 사용자나 그룹의 이름을 지정한다. 데이터베이스 사용자의 로그인 이름을 입력하거나 시스템 정의 사용자인 **PUBLIC** 을 입력할 수 있다. **PUBLIC** 이 명시되면 데이터베이스의 모든 사용자는 부여한 권한을 가진다.
* **WITH GRANT OPTION**: **WITH GRANT OPTION** 을 이용하면 권한을 부여받은 사용자가 부여받은 권한을 다른 사용자에게 부여할 수 있다.

다음은 *smith* (*smith* 의 모든 멤버 포함)에게 *olympic* 테이블의 검색 권한을 부여한 예제이다.

.. code-block:: sql

    GRANT SELECT ON olympic TO smith;

다음은 *brown* 와 *jones* (두 사용자에 속한 모든 멤버)에게 *nation* 과 *athlete* 테이블에 대해 **SELECT**, **INSERT**, **UPDATE**, **DELETE** 권한을 부여한 예제이다.

.. code-block:: sql

    GRANT SELECT, INSERT, UPDATE, DELETE ON nation, athlete TO brown, jones;

다음은 모든 사용자(public)에게 *tbl1*, *tbl2* 테이블에 대해 모든 권한을 부여하는 예제이다.

.. code-block:: sql

    CREATE TABLE tbl1 (a INT);
    CREATE TABLE tbl2 (a INT);
    GRANT ALL PRIVILEGES ON tbl1, tbl2 TO public;

다음 **GRANT** 문은 *brown* 에게 *record*, *history* 테이블에 대한 검색 권한을 부여하고 *brown* 이 다른 사용자에게 검색 권한을 부여하는 것을 허용하도록 **WITH GRANT OPTION** 절을 사용한 예제이다. 이후 *brown* 은 다른 사용자에게 자신이 받은 권한 내에서 권한을 부여할 수 있다.

.. code-block:: sql

    GRANT SELECT ON record, history TO brown WITH GRANT OPTION;

.. note:: \

    *   권한을 부여하는 사용자는 권한 부여 전에 나열된 모든 테이블의 소유자이거나, **WITH GRANT OPTION** 을 가지고 있어야 한다.
    *   뷰에 대한 **SELECT**, **UPDATE**, **DELETE**, **INSERT** 권한을 부여하기 전에 뷰의 소유자는 뷰의 질의 명세부에 포함되어 있는 모든 테이블에 대해서 **SELECT** 권한과 **GRANT** 권한을 가져야 한다. **DBA** 사용자와 **DBA** 그룹에 속한 멤버는 자동적으로 모든 테이블에 대한 모든 권한을 가진다.
    *   **TRUNCATE** 문을 수행하려면 **ALTER**, **INDEX**, **DELETE** 권한이 필요하다.

REVOKE
======

**REVOKE** 문을 사용하여 권한을 해지할 수 있다. 사용자에게 부여된 권한은 언제든지 해지가 가능하다. 한 사용자에게 두 종류 이상의 권한을 부여했다면 권한 중 일부 또는 전부를 해지할 수 있다. 또한 하나의 **GRANT** 문으로 여러 사용자에게 여러 테이블에 대한 권한을 부여한 경우라도 일부 사용자와 일부 테이블에 대해 선택적인 권한 해지가 가능하다.

권한을 부여한 사용자에게서 권한(**WITH GRANT OPTION**)을 해지하면, 권한을 해지당한 사용자로부터 권한을 받은 사용자도 권한을 해지당한다. ::

    REVOKE operation [ { , operation } ... ] ON table_name [ { , class_name } ... ]
    FROM user [ { , user } ... ] ;

*   *operation*: 권한을 부여할 때 부여할 수 있는 연산의 종류이다(자세한 내용은 :ref:`granting-authorization` 참조).
*   *table_name*: 권한을 부여할 테이블 혹은 뷰의 이름을 지정한다.
*   *user*: 권한을 부여할 사용자나 그룹의 이름을 지정한다.

다음은 *smith*, *jones* 사용자에게 *nation*, *athlete* 두 테이블에 대해 **SELECT**, **INSERT**, **UPDATE**, **DELETE** 권한을 부여하는 예제이다.

.. code-block:: sql

    GRANT SELECT, INSERT, UPDATE, DELETE ON nation, athlete TO smith, jones;

다음은 *jones* 에게 조회 권한만을 부여하기 위해 **REVOKE** 문장을 수행하는 예제이다. 만약 *jones* 가 다른 사용자에게 권한을 부여했다면 권한받은 사용자 또한 조회만 가능하다.

.. code-block:: sql

    REVOKE INSERT, UPDATE, DELETE ON nation, athlete FROM jones;

다음은 *smith* 에게 부여한 모든 권한을 해지하기 위해 **REVOKE** 문을 수행하는 예제이다. 이 문장이 수행되면 *smith* 는 *nation*, *athlete* 테이블에 대한 어떠한 연산도 허용되지 않는다.

.. code-block:: sql

    REVOKE ALL PRIVILEGES ON nation, athlete FROM smith;

.. _change-owner:

ALTER ... OWNER
===============

데이터베이스 관리자(**DBA**) 또는 **DBA** 그룹의 멤버는 다음의 질의를 통해 테이블, 뷰, 트리거, Java 저장 함수/프로시저의 소유자를 변경할 수 있다. ::

    ALTER [TABLE | CLASS | VIEW | VCLASS | TRIGGER | PROCEDURE | FUNCTION] name OWNER TO user_id;

*   *name*: 소유자를 변경할 스키마 객체의 이름
*   *user_id*: 사용자 ID

.. code-block:: sql

    ALTER TABLE test_tbl OWNER TO public;
    ALTER VIEW test_view OWNER TO public;
    ALTER TRIGGER test_trigger OWNER TO public;
    ALTER FUNCTION test_function OWNER TO public;
    ALTER PROCEDURE test_procedure OWNER TO public;

.. _authorization-method:

사용자 권한 관리 메서드
=======================

데이터베이스 관리자(**DBA**)는 데이터베이스 사용자에 대한 정보를 저장하는 **db_user** 또는 시스템 권한 클래스인 **db_authorizations** 에 정의된 권한 관련 메서드들을 호출하여 사용자 권한을 조회 및 수정할 수 있다. 호출하고자 하는 메서드에 따라 **db_user** 또는 **db_authorizations** 클래스를 명시할 수 있으며, 메서드의 리턴 값을 변수에 저장할 수 있다. 또한, 일부 메서드는 **DBA** 와 **DBA** 그룹의 멤버에 의해서만 호출될 수 있음을 유의한다.

.. note:: HA 환경에서 마스터 노드에서의 메서드 호출은 슬레이브 노드에 반영되지 않으므로 이에 주의한다.

::

    CALL method_definition ON CLASS auth_class [ TO variable ] [ ; ]
    CALL method_definition ON variable [ ; ]

**login() 메서드**

**login** () 메서드는 **db_user** 클래스의 클래스 메서드로서, 현재 데이터베이스에 접속한 사용자를 변경하고자 할 때 사용된다. 새로 접속할 사용자 이름과 비밀번호가 인자로 주어지며, 문자열 타입이어야 한다. 비밀번호가 없는 경우 인자에 공백 문자('')을 입력할 수 있다. **DBA** 나 **DBA** 그룹의 멤버는 비밀번호를 입력하지 않고 **login** () 메서드를 호출할 수 있다.

.. code-block:: sql

    -- 비밀번호가 없는 DBA 사용자로 접속하기
    CALL login ('dba', '') ON CLASS db_user;
    
    -- 비밀번호가 cubrid인 user_1 사용자로 접속하기
    CALL login ('user_1', 'cubrid') ON CLASS db_user;

**add_user() 메서드**

**add_user** () 메서드는 **db_user** 클래스의 클래스 메서드로서, 새로운 사용자를 추가할 때 사용된다. 새로 추가할 사용자 이름과 비밀번호가 인자로 주어지며, 문자열 타입이어야 한다. 이때, 추가할 사용자 이름은 이미 등록된 데이터베이스 사용자 이름과 중복되어서는 안 된다. 한편, **add_user** () 메서드는 **DBA** 사용자와 **DBA** 그룹에 속한 멤버만 호출할 수 있다.

.. code-block:: sql

    -- 비밀번호가 없는 user_2 추가하기
    CALL add_user ('user_2', '') ON CLASS db_user;
    
    -- 비밀번호가 없는 user_3 추가하고, 메서드 리턴 값을 admin 변수에 저장하기
    CALL add_user ('user_3', '') ON CLASS db_user to admin;

**drop_user() 메서드**

**drop_user** () 메서드는 **db_user** 클래스의 클래스 메서드로서, 기존 사용자를 삭제할 때 사용된다. 삭제하고자 하는 사용자 이름만 인자로 주어지며, 문자열 타입이어야 한다. 이때, 클래스의 소유자는 삭제할 수 없으므로, **DBA** 는 관련 클래스의 소유자를 변경한 후, 해당 사용자를 삭제할 수 있다. **drop_user** () 메서드 역시 **DBA** 사용자와 **DBA** 그룹에 속한 멤버만 호출할 수 있다.

.. code-block:: sql

    -- user_2 삭제하기
    CALL drop_user ('user_2') ON CLASS db_user;

**find_user() 메서드**

**find_user** () 메서드는 **db_user** 클래스의 클래스 메서드로서, 인자로 주어진 사용자를 검색할 때 사용된다. 찾고자 하는 사용자 이름이 인자로 주어지며, **TO** 뒤에 지정된 변수에 메서드의 리턴 값을 저장하여 다음 질의 수행 시 변수에 저장된 값을 이용할 수 있다.

.. code-block:: sql

    -- user_2를 찾아서 admin이라는 변수에 저장하기
    CALL find_user ('user_2') ON CLASS db_user TO admin;

**set_password() 메서드**

**set_password** () 메서드는 사용자 인스턴스 각각에 대해 호출할 수 있는 인스턴스 메서드로서, 사용자의 비밀번호를 변경할 때 사용된다. 지정된 사용자의 새로운 비밀번호가 인자로 주어진다. **DBA** 와 **DBA** 그룹의 멤버를 제외한 일반 사용자는 자신의 비밀번호만 변경할 수 있다.

.. code-block:: sql

    -- user_4 를 추가하고 user_common 변수에 저장하기
    CALL add_user ('user_4', '') ON CLASS db_user to user_common;
    
    -- user_4의 비밀번호를 'abcdef'로 변경하기
    CALL set_password('abcdef') on user_common;

**change_owner() 메서드**

**change_owner** () 메서드는 **db_authorizations** 클래스의 클래스 메서드로서, 클래스 소유자를 변경할 때 사용된다. 소유자를 변경하고자 하는 클래스 이름과 새로운 소유자의 이름이 각각 인자로 주어진다. 이때, 데이터베이스에 존재하는 클래스와 소유자가 인자로 지정되어야 하며, 그렇지 않은 경우 에러가 발생한다. **change_owner** () 메서드는 **DBA** 와 **DBA** 그룹의 멤버만 호출할 수 있다. 이 메서드와 같은 역할을 하는 질의로 **ALTER ... OWNER** 가 있다. 이에 대한 내용은 :ref:`change-owner` 절을 참고한다.

.. code-block:: sql

    -- table_1의 소유자를 user_4로 변경하기
    CALL change_owner ('table_1', 'user_4') ON CLASS db_authorizations;

다음 예제는 특정 데이터베이스 사용자의 존재 여부를 판단하기 위해 시스템 클래스인 **db_user** 에 등록된 메서드인 **find_user** 를 호출하는 **CALL** 문의 수행을 보여준다. 첫 번째 문장은 **db_user** 클래스에 정의된 클래스 메서드를 호출한다. 찾고자 하는 대상 사용자가 데이터베이스에 등록되어 있을 경우 x에는 해당 클래스 이름(여기에서는 **db_user**)이 저장되고, 없을 경우엔 **NULL** 이 저장된다.

두 번째 문장은 변수 x에 저장된 값을 출력하는 방법이다. 이 질의문에서 **DB_ROOT** 는 시스템 클래스로서, 하나의 인스턴스만이 존재하여 sys_date나 등록된 변수의 값을 출력하는 데 사용할 수 있다. 이러한 용도로 쓰일 경우 **DB_ROOT** 는 인스턴스가 하나인 다른 테이블로 대체할 수 있다.

.. code-block:: sql

    CALL find_user('dba') ON CLASS db_user to x;
    
::

    Result
    ======================
    db_user
     
.. code-block:: sql

    SELECT x FROM db_root;
    
::

    x
    ======================
    db_user

**find_user** 메서드를 이용하면 결과값이 **NULL** 인지 아닌지에 따라 해당 사용자가 데이터베이스에 존재하는지 여부를 판단할 수 있다.
