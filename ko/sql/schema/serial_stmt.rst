
:meta-keywords: serial definition, create serial, alter serial, drop serial
:meta-description: Define serials in CUBRID database using create serial, alter serial and drop serial statements.


*************
시리얼 정의문
*************

CREATE SERIAL
=============

시리얼(**SERIAL**)은 고유한 순번을 생성하는 객체이다. 시리얼은 다음과 같은 특성을 갖는다.

*   시리얼은 다중 사용자 환경에서 고유한 순번을 생성하는데 용이하다.
*   시리얼 번호는 테이블과 독립적으로 생성된다. 따라서 하나 이상의 테이블에 동일한 시리얼을 사용할 수 있다.
*   **PUBLIC** 을 포함하여 모든 사용자가 시리얼 객체를 생성할 수 있다. 일단 생성되면 모든 사용자들이 **CURRENT_VALUE**, **NEXT_VALUE** 를 통해 시리얼 숫자를 가져갈 수 있다.
*   시리얼 객체의 소유자와 **DBA** 만 시리얼 객체를 갱신하고 삭제할 수 있다. 소유자가 **PUBLIC** 이면 모든 사용자가 갱신하거나 삭제할 수 있다.

**CREATE SERIAL** 문을 이용하여 데이터베이스에 시리얼 객체를 생성한다. 시리얼 이름 작성 원칙은 :doc:`/sql/identifier`\ 를 참고한다.

::

    CREATE SERIAL serial_name
    [START WITH initial]
    [INCREMENT BY interval]
    [MINVALUE min | NOMINVALUE]
    [MAXVALUE max | NOMAXVALUE]
    [CACHE cached_num | NOCACHE]
    [COMMENT 'comment_string'];

*   *serial_identifier*: 생성할 시리얼의 이름을 지정한다(최대 254 바이트).

*   **START WITH** *initial*: 처음 생성되는 시리얼 숫자를 지정한다. 이 값의 범위는 -1,000,000,000,000,000,000,000,000,000,000,000,000(-10^36)와   9,999,999,999,999,999,999,999,999,999,999,999,999(10^37-1) 사이이다. 오름차순 시리얼의 경우 기본값은 1이며 내림차순 시리얼의 경우 기본값은 -1이다.

*   **INCREMENT BY** *interval*: 시리얼 숫자 간의 간격을 지정한다. *interval* 값으로 0을 제외하고 -9,999,999,999,999,999,999,999,999,999,999,999,999(-10^37+1)와 9,999,999,999,999,999,999,999,999,999,999,999,999(10^37-1) 사이의 어느 정수라도 올 수 있다.  *interval* 의 절대값은 **MAXVALUE** 와 **MINVALUE** 의 차이와 같거나 작아야 한다. 음수가 설정되면 시리얼은 내림차순이 되고 양수가 설정되면 오름차순이 된다. 기본값은 **1** 이다.

*   **MINVALUE**: 시리얼의 최소값을 지정한다. 이 값의 범위는 -1,000,000,000,000,000,000,000,000,000,000,000,000(-10^36)와 9,999,999,999,999,999,999,999,999,999,999,999,999(10^37-1) 사이이다. **MINVALUE**\ 는 초기값보다 작거나 같아야 하고 최대값보다 작아야 한다.

*   **NOMINVALUE**: 오름차순 시리얼에 대해서는 1, 내림차순 시리얼에 대해서는 -1,000,000,000,000,000,000,000,000,000,000,000,000(-10^36) 이 최소값으로 자동 지정된다.

*   **MAXVALUE**: 시리얼의 최대값을 지정한다. 이 값의 범위는 -999,999,999,999,999,999,999,999,999,999,999,999(-10^36+1)와 10,000,000,000,000,000,000,000,000,000,000,000,000(10^37) 사이이다. **MAXVALUE**\ 는 초기값보다 크거나 같아야 하고 최소값보다 커야 한다.

*   **NOMAXVALUE**: 오름차순 시리얼에 대해서는 10,000,000,000,000,000,000,000,000,000,000,000,000(10^37), 내림차순 시리얼에 대해서는 -1이 최대값으로 자동 지정된다.

*   **CYCLE**: 시리얼 값이 최대 또는 최소값에 도달한 후에 연속적으로 값을 생성하도록 지정한다. 오름차순 시리얼은 최대값에 도달한 후에 다음 값으로 최소값이 생성된다. 내림차순 시리얼은 최소값에 도달한 후에 다음 값으로 최대값이 생성된다.

*   **NOCYCLE**: 시리얼이 최대 또는 최소값에 도달한 후에 시리얼 값이 더 이상 생성되지 않도록 지정한다. 기본값은 **NOCYCLE** 이다.

*   **CACHE**: 시리얼 성능을 향상시키기 위하여 *cached_num* 에 지정된 개수만큼의 시리얼을 캐시에 저장하고, 시리얼 값을 요청받으면 캐시된 시리얼 값을 가져온다. 또한, 메모리에 캐시된 시리얼을 전부 사용하게 되면, *cached_num* 개수 만큼의 시리얼을 디스크로부터 메모리로 다시 가져오며, 데이터베이스 서버가 중간에 종료되면 캐시된 시리얼 값들은 삭제된다. 따라서 데이터베이스 서버가 재시작되기 이전과 이후의 시리얼 값은 비연속적일 수 있다. 캐시된 시리얼은 트랜잭션 롤백되지 않으므로, 롤백을 수행하더라도 다음에 요청하는 시리얼은 이전에 최종 요청한 시리얼 값의 다음 값이 된다. **CACHE** 키워드 뒤에 *cached_num* 는 생략할 수 없으며, 1 이하의 숫자가 지정되면 시리얼 캐시가 적용되지 않는다.

*   **NOCACHE**: 시리얼 캐시 기능을 사용하지 않으며, 매번 시리얼 값을 업데이트한다.

*   *comment_string*: 시리얼의 커멘트를 지정한다.

.. code-block:: sql

    --creating serial with default values
    CREATE SERIAL order_no;
     
    --creating serial within a specific range
    CREATE SERIAL order_no START WITH 10000 INCREMENT BY 2 MAXVALUE 20000;
    
    --creating serial with specifying the number of cached serial values
    CREATE SERIAL order_no START WITH 10000 INCREMENT BY 2 MAXVALUE 20000 CACHE 3;
     
    --selecting serial information from the db_serial class
    SELECT * FROM db_serial;

::

      name            current_val      increment_val         max_val         min_val         cyclic      started       cached_num        att_name
    ====================================================================================================================================================
    'order_no'      10006            2                     20000           10000                0            1                3            NULL

다음은 선수 번호와 이름을 저장하는 *athlete_idx* 테이블을 생성하고 *order_no* 시리얼을 이용하여 인스턴스를 생성하는 예제이다. *order_no*.CURRENT_VALUE는 시리얼의 현재 값을 반환하고, *order_no*.NEXT_VALUE는 시리얼 값을 증가시킨 후 값을 반환한다.

.. code-block:: sql

    CREATE TABLE athlete_idx( code INT, name VARCHAR(40) );
    CREATE SERIAL order_no START WITH 10000 INCREMENT BY 2 MAXVALUE 20000;
    INSERT INTO athlete_idx VALUES (order_no.NEXT_VALUE, 'Park');
    INSERT INTO athlete_idx VALUES (order_no.NEXT_VALUE, 'Kim');
    INSERT INTO athlete_idx VALUES (order_no.NEXT_VALUE, 'Choo');
    INSERT INTO athlete_idx VALUES (order_no.CURRENT_VALUE, 'Lee');
    
    SELECT * FROM athlete_idx;

::

             code  name
    ===================================
            10000  'Park'
            10002  'Kim'
            10004  'Choo'
            10004  'Lee'

시리얼의 커멘트
---------------

다음은 시리얼 생성 시 커멘트를 추가한다.

.. code-block:: sql

    CREATE SERIAL order_no 
    START WITH 100 INCREMENT BY 2 MAXVALUE 200 
    COMMENT 'from 100 to 200 by 2';

시리얼의 커멘트를 확인하려면 다음의 구문을 실행한다.

.. code-block:: sql

    SELECT name, comment FROM db_serial;

시리얼 커멘트의 변경은 ALTER SERIAL 문의 설명을 참고한다.

ALTER SERIAL
============

**ALTER SERIAL** 문을 이용하면 시리얼 값의 증가치를 갱신하고 시작 값, 최소 값, 최대 값을 설정하거나 제거할 수 있으며, 순환 속성을 설정할 수 있다.

::

    ALTER SERIAL serial_identifier
    [INCREMENT BY interval]
    [START WITH initial_value]
    [MINVALUE min | NOMINVALUE]
    [MAXVALUE max | NOMAXVALUE]
    [CACHE cached_num | NOCACHE]
    [COMMENT 'comment_string'];

*   *serial_identifier*: 생성할 시리얼의 이름을 지정한다(최대 254 바이트).

*   **INCREMENT BY** *interval*: 시리얼 숫자간의 간격을 지정한다. *interval* 값으로 0을 제외한 38자리 이하의 어떤 정수도 지정할 수 있다. *interval* 의 절대값은 **MAXVALUE** 와 **MINVALUE** 의 차이보다 작아야 한다. 음수가 설정되면 시리얼은 내림차순이 되고 양수가 설정되면 오름차순이 된다. 기본값은 **1** 이다.

*   **START WITH** *initial_value*: 시리얼의 시작 값을 변경한다.

*   **MINVALUE**: 시리얼의 최소값을 지정한다. 이 값은 38자리 이하의 숫자이다. **MINVALUE** 는 초기값보다 작거나 같아야 하고 최대값보다 작아야 한다.

*   **NOMINVALUE**: 오름차순 시리얼에 대해서는 1, 내림차순 시리얼에 대해서는 -(10)\*\*38이 최소값으로 자동 지정된다.

*   **MAXVALUE**: 시리얼의 최대값을 지정한다. 이 값은 38자리 이하의 숫자이다. **MAXVALUE** 는 초기값보다 크거나 같아야 하고 최소값보다 커야 한다.

*   **NOMAXVALUE**: 오름차순 시리얼에 대해서는 (10)\*\*37, 내림차순 시리얼에 대해서는 -1이 최대값으로 자동 지정된다.

*   **CYCLE**: 시리얼 값이 최대 또는 최소값에 도달한 후에 연속적으로 값을 생성하도록 지정한다. 오름차순 시리얼은 최대값에 도달한 후에 다음 값으로 최소값이 생성된다. 내림차순 시리얼은 최소값에 도달한 후에 다음 값으로 최대값이 생성된다.

*   **NOCYCLE**: 시리얼이 최대 또는 최소값에 도달한 후에 시리얼 값이 더 이상 생성되지 않도록 지정한다. 기본값은 **NOCYCLE** 이다.

*   **CACHE**: 시리얼 성능을 향상시키기 위하여 *cached_num* 에 지정된 개수만큼의 시리얼을 캐시에 저장하고, 시리얼 값을 요청받으면 캐시된 시리얼 값을 가져온다. **CACHE** 키워드 뒤에 *cached_num* 는 생략할 수 없으며, 1 이하의 숫자가 지정되면 시리얼 캐시가 적용되지 않는다.

*   **NOCACHE**: 시리얼 캐시 기능을 사용하지 않으며, 매번 시리얼 값이 업데이트된다. 

*   *comment_string*: 시리얼의 커멘트를 지정한다.

.. code-block:: sql

    --altering serial by changing start and incremental values
    ALTER SERIAL order_no START WITH 100 MINVALUE 100 INCREMENT BY 2;
     
    --altering serial to operate in cache mode
    ALTER SERIAL order_no CACHE 5;
     
    --altering serial to operate in common mode
    ALTER SERIAL order_no NOCACHE;
    
.. warning::

    CUBRID 2008 R1.x 버전에서는 시스템 카탈로그인 **db_serial** 테이블을 업데이트하는 방식으로 시리얼 값을 변경할 수 있었으나, CUBRID 2008 R2.0 이상 버전부터는 **db_serial** 테이블의 수정은 허용되지 않고 **ALTER SERIAL** 구문을 이용하는 방식만 허용된다. 따라서 CUBRID 2008 R2.0 이상 버전에서 내보내기(unloaddb)한 데이터에 **ALTER SERIAL** 구문이 포함된 경우에는 이를 CUBRID 2008 R1.x 이하 버전에서 가져오기(loaddb)할 수 없다.

.. warning::

    **ALTER SERIAL** 이후 첫번째 **NEXT_VALUE** 값을 구하면 CUBRID 9.0 미만 버전에서는 **ALTER SERIAL** 로 설정한 초기값의 다음 값을 반환했으나, CUBRID 9.0 이상 버전에서는 **ALTER_SERIAL** 의 설정값을 반환한다.

    ::
    
        CREATE SERIAL s1;
        SELECT s1.NEXTVAL;

        ALTER SERIAL s1 START WITH 10;
        
        SELECT s1.NEXTVAL;
        -- From 9.0, above query returns 10
        -- In the version less than 9.0, above query returns 11

다음은 시리얼의 커멘트를 변경한다.

.. code-block:: sql

    ALTER SERIAL order_no COMMENT 'new comment';

DROP SERIAL
===========

**DROP SERIAL** 문으로 시리얼 객체를 데이터베이스에서 삭제할 수 있다. 
**IF EXISTS** 절을 함께 지정하는 경우, 대상 시리얼이 없어도 에러가 발생하지 않는다.

::

    DROP SERIAL [ IF EXISTS ] serial_identifier ;

*   *serial_identifier*: 삭제할 시리얼의 이름을 지정한다.

다음 예는 *order_no* 시리얼을 삭제하는 예제이다.

.. code-block:: sql

    DROP SERIAL order_no;
    DROP SERIAL IF EXISTS order_no;

시리얼 사용
===========

의사 칼럼
---------

시리얼 이름과 의사 칼럼(pseudo column)을 통해서 해당 시리얼을 읽고 갱신할 수 있다. ::

    serial_identifier.CURRENT_VALUE
    serial_identifier.NEXT_VALUE

*   *serial_identifier*.\ **CURRENT_VALUE**: 시리얼의 현재 값을 반환한다.
*   *serial_identifier*.\ **NEXT_VALUE**: 시리얼 값을 증가시키고 그 값을 반환한다.

다음은 선수 번호와 이름을 저장하는 *athlete_idx* 테이블을 생성하고 *order_no* 시리얼을 이용하여 인스턴스를 생성하는 예제이다.

.. code-block:: sql

    CREATE TABLE athlete_idx (code INT, name VARCHAR (40));
    CREATE SERIAL order_no START WITH 10000 INCREMENT BY 2 MAXVALUE 20000;
    INSERT INTO athlete_idx VALUES (order_no.NEXT_VALUE, 'Park');
    INSERT INTO athlete_idx VALUES (order_no.NEXT_VALUE, 'Kim');
    INSERT INTO athlete_idx VALUES (order_no.NEXT_VALUE, 'Choo');
    INSERT INTO athlete_idx VALUES (order_no.NEXT_VALUE, 'Lee');
    SELECT * FROM athlete_idx;
    
::
     
             code  name
    ===================================
            10000  'Park'
            10002  'Kim'
            10004  'Choo'
            10006  'Lee'

.. note:: 

    시리얼을 생성하고 처음 사용할 때 **NEXT_VALUE** 를 이용하면 초기 값을 반환한다. 그 이후에는 현재 값에 증가 값이 추가되어 반환된다.

함수
----

.. function:: SERIAL_CURRENT_VALUE (serial_name)
.. function:: SERIAL_NEXT_VALUE (serial_name, number)

    시리얼 함수에는 **SERIAL_CURRENT_VALUE** 함수와 **SERIAL_NEXT_VALUE** 함수가 있다.
    
    :param serial_name: 시리얼 이름
    :param number: 얻고자 하는 시리얼 개수
    :rtype:  NUMERIC(38,0)

**SERIAL_CURRENT_VALUE** 함수는 현재의 시리얼 값을 반환하며, *serial_name*.\ **current_value** 와 동일한 값을 반환한다.

**SERIAL_NEXT_VALUE** 함수는 현재의 시리얼 값에서 지정한 개수의 시리얼 간격만큼 증가시킨 값을 반환한다. 시리얼 간격은 **CREATE SERIAL ... INCREMENT BY** 절로 지정한 값을 따른다. **SERIAL_NEXT_VALUE** (*serial_name*, 1)은 *serial_name*.\ **next_value** 와 동일한 값을 반환한다.

한꺼번에 많은 개수의 시리얼을 얻고자 할 때에는, *serial_name*.\ **next_value** 를 반복하여 호출하는 것보다 원하는 개수를 인자로 하여 **SERIAL_NEXT_VALUE** 함수를 한 번만 호출하는 것이 성능상 유리하다.

즉, 어떤 응용 프로세스가 한꺼번에 *N* 개의 시리얼을 얻고자 한다면 N번 *serial_name*.\ **next_value** 를 호출하여 값들을 구하는 것보다는, 한 번 **SERIAL_NEXT_VALUE** (*serial_name*, *N*)을 호출하여 반환하는 값을 가지고 (함수를 호출한 시점의 시리얼 시작값)과 (반환 값) 사이의 시리얼 값들을 계산하는 것이 낫다. (함수를 호출한 시점의 시리얼 시작값)은 (반환 값) - (얻고자 하는 시리얼 개수-1) * (시리얼 간격)이다.

예를 들어, 101로 시작하며 1씩 증가하는 시리얼을 처음에 생성하였을 경우, 처음 **SERIAL_NEXT_VALUE** (*serial_name*, 10)을 호출하면 110이 반환된다. 이 시점의 시작값을 구하면 110-(10-1)*1 = 101이므로 101, 102, 103, ... 110까지 10개의 시리얼 값을 해당 응용 프로세스에서 사용할 수 있다. 한 번 더 **SERIAL_NEXT_VALUE** (*serial_name*, 10)을 호출하면 120이 반환되며, 이 시점의 시작값은 120-(10-1)*1 = 111이다.

.. code-block:: sql

    CREATE SERIAL order_no START WITH 101 INCREMENT BY 1 MAXVALUE 20000;
    SELECT SERIAL_CURRENT_VALUE(order_no);
    
::

    101
     
.. code-block:: sql

    -- At first, the first serial value starts with the initial serial value, 10000. So the l0'th serial value will be 10009.
    SELECT SERIAL_NEXT_VALUE(order_no, 10);
    
::

    110
     
.. code-block:: sql

    SELECT SERIAL_NEXT_VALUE(order_no, 10);
    
::

    120

.. note:: \

    시리얼을 생성하고 **SERIAL_NEXT_VALUE** 함수를 처음 호출하면, 첫 번째 값은 초기값을 반환하므로 한 개의 값이 빠져 현재의 시리얼 값에 (시리얼 간격) * (얻고자 하는 시리얼 개수-1)만큼 증가한 값이 반환된다. 이후 **SERIAL_NEXT_VALUE** 함수를 호출하면 현재 값에 (시리얼 간격) * (얻고자 하는 시리얼 개수)만큼 증가한 값이 반환된다. 위의 예제를 참고한다.
