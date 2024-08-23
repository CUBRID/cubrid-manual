
:meta-keywords: server definition, create server, drop server, alter server, rename server
:meta-description: Define servers in CUBRID database using create server, alter server, drop server and rename server statements.


**************
서버 정의문
**************

CREATE SERVER
=============

서버 정의
-----------

**CREATE SERVER** 문을 사용하여 원격 접속 정보를 생성한다. 
생성된 서버는 **DBLINK**\를 이용하는 :doc:`/sql/query/select` 질의를 수행 할 때 원격 서버를 지정하는데 사용한다.
준비된 서버의 사용 방법은 :ref:`dblink-clause`\를 참고한다.

::

    CREATE SERVER <dblink_server_name> (<connect_info>) ;
   
        <dblink_server_name> ::= [owner_name.] server_name
		
        <connect_info> ::=
                <connect_info>, <connect_item>
                | <connect_item>						   
        <connect_item> ::= 
                HOST = host_string
                | PORT = port_number 
                | DBNAME = db_name 
                | USER = user_name
                | PASSWORD = [password_string]
                | PROPERTIES = [properties_string] 
                | COMMENT = [server_comment_string]
      
*   *owner_name*: 생성할 서버의 소유자 이름을 지정한다.
*   *server_name*: 생성할 서버의 이름을 지정한다(최대 254바이트).
*   <*connect_info*>: 접속 정보 리스트로 <connect_item> 항목 중 **HOST**, **PORT**, **DBNAME**, **USER**\는 필수 항목이다.
*   <*connect_item*>: HOST, PORT, DBNAME, USER, PASSWOED, PROPERTIES, COMMENT 항목으로 구성되며, 동일한 항목이 중복될 수 없다.
	
    *   *host_string*: 원격 접속할 DBMS 정보를 가지고 있는 Broker 서버의 호스명 또는 IP 주소이다.
    *   *port_number*: 원격 접속할 DBMS 정보를 가지고 있는 Broker 서버의 포트 번호이다.
    *   *db_name*: 원격 접속할 데이터베이스 이름.
    *   *user_name*: 원격 접속할 데이터베이스에 접속할 때 사용할 사용자 이름.
    *   *password_string*: 원격 접속할 데이터베이스에 접속할 때 사용하는 *user_name*\에 대한 패스워드 문자열.
    *   *properties_string*: 원격 데이터베이스 사용을 위해 broker(또는 gateway)에 접속시 사용하는 property 정보 문자열 (최대 2047 바이트).  상세한 정보는 :ref:`cci-connect-with-url` 를 참고한다.	
    *   *server_comment_string*: 서버 정보에 대한 커멘트를 지정한다.(최대 1023바이트)

.. note::

    db_name과 user_name은 식별자 형식과 문자열 리터럴 형식으로 모두 기술할 수 있다.
    
    * 식별자 형식의 예  
             t123db,  "123db",  `123db`,  [124db]
    * 문자열 형식의 예  
             't123db', '123db'

**예제 1**
다음은 필수항목(HOST,PORT,DBNAME,USER) 및 PROPERTIES, COMMENT를 사용한 예제이다.
  
.. code-block:: sql

    CREATE SERVER dblink_srv1 (
	   HOST='192.168.1.8',
	   PORT=3300,
	   DBNAME=demodb,
	   USER=dba,
	   PASSWORD='password1234',
	   PROPERTIES='?rcTime=600',
	   COMMENT='this is dblink_srv1'	   
    );

**예제 2**
다음은 서버 생성 시 최소한의 정보만 포함하는 예제이다.
원격지의 demodb에 비밀번호가 없는 dev1 계정으로 접속할 것임을 나타내고 있다. 
srv1, srv2, srv3는 동일한 의미이다.

.. code-block:: sql

    CREATE SERVER srv1 (
	   HOST='192.168.1.8',
	   PORT=3300,
	   DBNAME=demodb,
	   USER=dev1	 
    );
    
   CREATE SERVER srv2 (
	   HOST='192.168.1.8',
	   PORT=3300,
	   DBNAME=demodb,
	   USER=dev1,
	   PASSWORD=       	 
    );
    
    CREATE SERVER srv3 (
	   HOST='192.168.1.8',
	   PORT=3300,
	   DBNAME=demodb,
	   USER=dev1,
	   PASSWORD=''       	 
    );

**예제 3**
다음은 서버 생성 시 소유자를 지정하는 예제이다.
소유자 지정 없이 CREATE하는 경우에는 현재 사용자가 소유자가 된다.
추후 ALTER SERVER 구문을 이용해서 소유자를 변경할 수 있다.
아래 예제에서 두 서버의 이름이 동일하게 *srv2*\로 같지만 소유자는 각각 *dba*\와 *cub*\로 다르다.

.. code-block:: sql

    -- When the current account is dba
    CREATE SERVER srv2 (
	   HOST='192.168.1.8',
	   PORT=3300,
	   DBNAME=demodb,
	   USER=dev1,
	   PASSWORD='dev1-password',
	   COMMENT='The owner of this server is dba'
    );
    
   CREATE SERVER cub.srv2 (
	   HOST='192.168.1.8',
	   PORT=3300,
	   DBNAME=demodb,
	   USER=dev2,
	   PASSWORD='dev2-password',
	   COMMENT='The owner of this server is cub.'
    );

**예제 4**
다음은 HA 환경으로 구성된  원격 데이터베이스 사용시 PROPERTIES에 altHosts 속성을 사용하여 원격 데이터베이스의 fail-over에 대비한 설정 예제이다. 자세한 내용은 :ref:`cci-connect-with-url` 을 참고한다.

.. code-block:: sql

    CREATE SERVER dblink_srv1 (
       HOST='192.168.1.8',
       PORT=3300,
       DBNAME=demodb,
       USER=dba,
       PASSWORD='password1234',
       PROPERTIES='?altHosts=192.168.1.9:33000',
       COMMENT='this is dblink_srv1'
    );

**예제 5**
다음은 서버명(<dblink_server_name>)를 잘 못 사용한 예제이다. 

.. code-block:: sql
    
    CREATE SERVER srv1 ( HOST='localhost', PORT=3300, DBNAME=demodb, USER=dev1 );
    CREATE SERVER "srv 1" ( HOST='localhost', PORT=3300, DBNAME=demodb, USER=dev1 );
    CREATE SERVER "srv.1" ( HOST='localhost', PORT=3300, DBNAME=demodb, USER=dev1 );
    CREATE SERVER cub.srv1 ( HOST='localhost', PORT=3300, DBNAME=demodb, USER=dev1 );
    
    CREATE SERVER "cub"."srv 2" ( HOST='localhost', PORT=3300, DBNAME=demodb, USER=dev1 );
    CREATE SERVER [cub].[srv.2] ( HOST='localhost', PORT=3300, DBNAME=demodb, USER=dev1 );
        

위의 예시에서 *"srv.1"* 과 *[cub].[srv.2]*  이름으로 생성은 실패한다.
*cub.srv1* 의 경우 *cub* 는 사용자 이름으로 인식되고 서버명은 *srv1* 으로 인식 될 것이다.
        
.. note::

    서버명에는 점('.')을 사용 할 수 없다. 따옴표나 []로 묶은 경우라고 해도 서버명에 점('.')을 사용할 수는 없다.



RENAME SERVER
=============

**RENAME SERVER** 구문을 사용하여 서버 이름을 변경할 수 있다.

::

    RENAME SERVER [owner_name.] old_server_name {AS | TO} new_server_name ;
            
        
*   *owner_name*: 이름을 변경할 대상 서버의 소유자 이름을 지정한다.
*   *old_server_name*: 이름을 변경할 대상 서버의 이름을 지정한다.
*   *new_server_name*: 새롭게 부여될 서버의 이름을 지정한다(최대 254바이트).

.. note::

    서버의 소유자 또는 소유 그룹의 멤버만 정보를 변경 할 수 있다. 
    특별히 **DBA** 또는 **DBA** 의 멤버는 모든 서버 정보를 변경할 수 있다.
    
    변경 후에도 소유자는 동일하게 유지된다. 소유자를 변경하기 위해서는 :ref:`ALTER SERVER 구문<owner_to>`\을 참고한다.


.. code-block:: sql

    -- When the current account is dba
    RENAME SERVER srv1 AS srv2;
    RENAME SERVER dev1.srv1 AS srv3;

..

위 예제를 *dba* 계정에서 수행하더라도, *srv2* 의 소유자는 변경되지 않고 *srv1* 서버의 소유자인 *cub* 로 유지한다.
또한, *srv3* 서버의 소유자는 *dev1* 으로 계속 유지 된다. 


DROP SERVER
===========

**DROP SERVER** 구문을 이용하여 기존의 서버를 제거할 수 있다. **IF EXISTS** 절을 함께 사용하면 해당 서버가 존재하지 않더라도 에러가 발생하지 않는다.

::

    DROP SERVER [IF EXISTS] [owner_name.] server_name  ;
    
*   *owner_name*: 제거할 서버의 소유자 이름을 지정한다.
*   *server_name*: 제거할 서버의 이름을 지정한다.


.. code-block:: sql

    DROP SERVER srv1;
    DROP SERVER cub.srv1;
    DROP SERVER IF EXISTS srv2;
    
    

.. Warning::

    DROP USER 구문으로 사용자를 삭제 할 때 해당 사용자가 소유하고 있는 서버가 있다면 에러 처리되고 사용자는 삭제되지 않는다.
    먼저 사용자가 소유하고 있는 서버를 DROP SERVER 구문으로 제거한 후에 사용자 계정을 삭제해야 한다.
        
::
   
    -- When the current account is dba   
    csql> create user cub;
    Execute OK. (0.000371 sec) Committed.

    1 command(s) successfully processed.
    csql> create server cub.tsrv (HOST='localhost', PORT=3300, DBNAME=demdb, USER=dev1);
    Execute OK. (0.000761 sec) Committed.

    1 command(s) successfully processed.
    csql> drop user cub;

    In the command from line 1,

    ERROR: Cannot drop the user who owns database objects(class/trigger/serial/server etc).

    0 command(s) successfully processed.
    csql> drop server cub.tsrv;
    Execute OK. (0.000761 sec) Committed.

    1 command(s) successfully processed.
    csql> drop user cub;
    Execute OK. (0.001650 sec) Committed.

    1 command(s) successfully processed.
    csql>


..

위 예시를 보면 *cub* 계정 소유의 *tsrv* 서버를 생성한 상태에서 drop user *cub* 구문이 실패하고 있음을 볼 수 있다.
이후 *cub.tsrv* 서버를 제거한 후에는 정상적으로 *cub* 계정을 삭제할 수 있었음을 볼 수 있다.     



.. _alter-server:

ALTER SERVER
=============

**ALTER** 구문을 이용하여 서버의 정보를 변경할 수 있다. 대상 서버의 소유자를 변경하거나, HOST, PORT, DBNAME, USER, PASSWOED, PROPERTIES, COMMENT에 대한 정보를 갱신 할 수 있다.  

::

    ALTER SERVER <dblink_server_name> <alter_server_list> ;
     
        <dblink_server_name> ::=  [owner_name.] server_name 
        
        <alter_server_list> ::=
                <alter_server_list>, <alter_server_item>
                | <alter_server_item>						   
        <alter_server_item> ::= 
                OWNER TO owner_name
                | CHANGE <connect_item>
        <connect_item> ::= 
                HOST = host_string
                | PORT = port_number 
                | DBNAME = db_name 
                | USER = user_name
                | PASSWORD = [password_string]
                | PROPERTIES = [properties_string] 
                | COMMENT = [server_comment_string]
                

.. note::

    서버의 소유자 또는 소유 그룹의 멤버만 정보를 변경 할 수 있다. 
    특별히 **DBA** 또는 **DBA** 의 멤버는 모든 서버 정보를 변경할 수 있다.

.. warning::

    HOST, PORT, DBNAME, USER에 대한 값을 제거하는 갱신은 할 수 없다.


.. _owner_to:

OWNER TO 절
----------------

**OWNER TO** 절을 사용하여 서버의 소유자를 변경할 수 있다. 

::

    ALTER SERVER [owner_name.] server_name  OWNER TO new_owner_name ;
    
*   *owner_name*: 소유자를 변경할 대상 서버의 소유자 이름을 지정한다.
*   *server_name*: 소유자를 변경할 대상 서버의 이름을 지정한다.
*   *new_owner_name*: 새로운 소유자 이름을 지정한다.

.. warning::
    
    *   하나의 ALTER SERVER 구문에 OWNER TO 절은 오직 한번만  지정되어야 한다.


.. code-block:: sql
    
    CREATE SERVER srv1 (HOST='broker-server-name', PORT=3300, DBNAME=demodb, USER=dev1);
    ALTER SERVER srv1 OWNER TO usr1;    
    ALTER SERVER usr1.srv1 OWNER TO usr2;    


.. _change-server:

CHANGE 절
----------------

**CHANGE** 절은HOST, PORT, DBNAME, USER, PASSWOED, PROPERTIES, COMMENT 항목의 값을 변경 하는데 사용한다.

.. warning::
    
    *   하나의 ALTER SERVER 구문에 CHANGE 절은 콤마(,)로 구분하여 여러개가 나열 될 수 있다. 다만 이때 동일한 항목에 대한 CHANGE절은 오직 하나만 지정되어야 한다.
    *   ALTER SERVER 구문 수행시 언급이 없었던 항목은 그 값이 초기화 되거나 삭제되는 것이 아니고 기존의 값을 그대로 유지하게 된다. 

::

     ALTER SERVER  [owner_name.] server_name CHANGE <connect_item> [, CHANGE <connect_item>] ... ;

        <connect_item> ::= 
                HOST = host_string
                | PORT = port_number 
                | DBNAME = db_name 
                | USER = user_name
                | PASSWORD = [password_string]
                | PROPERTIES = [properties_string] 
                | COMMENT = [server_comment_string]

*   *owner_name*: 생성할 서버의 소유자 이름을(user name)지정한다.
*   *server_name*: 생성할 서버의 이름을 지정한다(최대 254바이트).
*   *host_string*: 원격 접속할DBMS 정보를 가지고 있는 Broker 서버의 호스트명 또는 IP 주소이다.
*   *port_number*: 원격 접속할 DBMS 정보를 가지고 있는 Broker 서버의 포트 번호이다.
*   *db_name*: 원격 접속할 데이터베이스 이름.
*   *user_name*: 원격 접속할 데이터베이스에 접속할 때 사용할 사용자 이름.
*   *password_string*: 원격 접속할 데이터베이스에 접속할 때 사용하는 *user_name* 에 대한 패스워드 문자열.
*   *properties_string*: 원격 접속할 데이터베이스에 접속할 때 사용하는 property 정보 문자열.	
*   *server_comment_string*: 서버 정보에 대한 커멘트를 지정한다.



.. code-block:: sql

    CREATE SERVER srv1 ( HOST='localhost', PORT=3300, DBNAME=demodb, USER=dev1 );
    
    ALTER SERVER srv1 CHANGE HOST='127.0.0.1';
    ALTER SERVER srv1 CHANGE HOST='127.0.0.1', OWNER TO usr1;
    ALTER SERVER srv1 CHANGE USER=dev2, CHANGE PASSWORD='dev2-pawword', CHANGE PORT=3500;

..

위 예시는 **CHANGE** 절은 여러개를 한번에 나열해서 사용할 수도 있고 **OWNER TO** 절과 함께 사용 될 수도 있음을 보여 준다. 


.. code-block:: sql
     
    ALTER SERVER srv1 CHANGE PORT=;    
    ALTER SERVER srv1 CHANGE DBNAME=;    
    ALTER SERVER srv1 CHANGE USER=;
    ALTER SERVER srv1 CHANGE HOST=;
    ALTER SERVER srv1 CHANGE HOST='';
    
..

위 예시는 모두 지원 되지 않는 경우에 대한 예시이다. 서버 정보의 구성에서 필수요소인 HOST, PORT, DBNAME, USER는 반드시 값을 가지고 있어야 하기 때문에 값을 삭제하는 설정 변경은 지원되지 않는다. 특히, HOST의 경우 빈 문자열로 설정하는 것도 허용 되지 않는다. 
    
    
.. code-block:: sql
    
    ALTER SERVER srv1 CHANGE PASSWORD=;
    ALTER SERVER srv1 CHANGE PASSWORD='';
    
    ALTER SERVER srv1 CHANGE PROPERTIES=;
    ALTER SERVER srv1 CHANGE PROPERTIES='';
    
    ALTER SERVER srv1 CHANGE COMMENT=;
    ALTER SERVER srv1 CHANGE COMMENT='';
    
..

위 예시는 모두 지원되는 예시이다. 서버 정보의 구성에서 필수요소가 아닌 PASSWORD, PROPERTIES, COMMENT는 반드시 값을 가지고 있어야할 필요가 없기 때문에 값을 삭제하는 설정 변경이 가능하다. 
    
  
