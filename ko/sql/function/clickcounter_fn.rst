
:meta-keywords: cubrid incr, cubrid decr
:meta-description: The **INCR** and **DECR** functions are called "click counters" and can be effectively used to quickly update a counter.
      
:tocdepth: 3

****************
클릭 카운터 함수
****************

.. contents::

INCR, DECR
==========

.. function:: INCR (column)
.. function:: DECR (column)

    **INCR** 함수는 **SELECT** 절에 포함되어 인자로 주어진 칼럼의 값을 1 증가시켜 주는 기능을 한다. **DECR** 함수는 해당 칼럼의 값을 1 감소시킨다.

    :param column: SMALLINT, INT 또는 BIGINT 타입의 칼럼 이름
    :rtype: SMALLINT, INT 또는 BIGINT 

**INCR** 함수와 **DECR** 함수는 '클릭 카운터' 함수로 불리며, 게시판 유형의 웹 서비스에서 게시물의 조회수를 증가시키는데 유용하게 사용될 수 있다. 게시물의 내용을 **SELECT** 하고 곧바로 게시물의 조회수를 **UPDATE** 로 1 증가하는 유형의 시나리오에서 하나의 **SELECT** 문에 **INCR** 함수를 사용함으로써 한 번에 게시물 내용 조회와 조회수 증가 작업을 수행할 수 있다.

**INCR** 함수는 인자로 명시된 칼럼 값을 증가시킨다. 단, 인자로는 정수 타입의 숫자형만 올 수 있고, 값이 **NULL** 인 경우 **INCR** 함수를 수행하여도 값은 **NULL** 을 유지한다. 즉, 값이 설정되어야 **INCR** 함수를 써서 값을 증가시킬 수 있다. **DECR** 함수는 인자로 명시된 칼럼 값을 감소시킨다. 

**SELECT** 절에 **INCR** 함수를 명시한 경우, 카운터 값은 1이 증가하고 질의 결과는 증가하기 전의 값으로 출력한다. 그리고, **INCR** 함수는 질의 처리 과정에서 참여한 행(tuple)이 아니라 최종 결과에 참여한 행에 대해서만 값을 증가시킨다.

**SELECT** 리스트에 **INCR** 또는 **DECR** 함수를 명시하지 않고 클릭 카운터를 증가 또는 감소시키고자 하는 경우, WHERE 절 뒤에 **WITH INCREMENT FOR** *column* 또는 **WITH INCREMENT FOR** *column* 을 명시하면 된다. 

.. code-block:: sql

    CREATE TABLE board (id INT, cnt INT, content VARCHAR(8096));
    SELECT content FROM board WHERE id=1 WITH INCREMENT FOR cnt;

.. note::

    *   **INCR/DECR** 함수는 사용자 정의 트랜잭션과 별도로 시스템 내부에서 사용되는 top operation이 적용되어 트랜잭션의 **COMMIT/ROLLBACK** 과 상관없이 데이터베이스에 자동으로 적용된다.

    *   하나의 **SELECT** 문에 **INCR/DECR** 함수를 여러 개 사용할 경우, 해당 질의 내의 각각의 **INCR/DECR** 함수 중 하나라도 실패하면 모두 실패한다.

    *   **INCR/DECR** 함수는 최상위 **SELECT** 문에만 적용된다. **INSERT** ... **SELECT** ... 구문과 **UPDATE** table **SET** col = **SELECT** ... 등과 같은 **SUB SELECT**  문은 지원하지 않는다. 다음은 **INCR** 함수가 허용되지 않는 예이다.

        .. code-block:: sql
    
            SELECT b.content, INCR(b.read_count) FROM (SELECT * FROM board WHERE id = 1) AS b

    *   **INCR/DECR** 함수가 포함된 **SELECT** 문의 경우, 결과 행의 개수가 둘 이상이면 오류로 처리한다. 최종 결과가 하나인 경우에만 유효하다.
    
    *   **INCR/DECR** 함수는 숫자 타입에 대해서만 사용할 수 있다. 적용 가능한 타입은 **SMALLINT**, **INTEGER**, **BIGINT**\ 와 같은 정수형 데이터 타입으로 제한된다. 기타 타입에는 사용할 수 없다.
    
    *   **INCR** 함수 호출 시 결과 값은 현재 값이며, 저장 값은 현재 값 +1인 값이 저장된다. 결과를 저장값과 같은 값을 조회하고자 할 경우는 다음과 같이 수행한다.

        .. code-block:: sql
    
            SELECT content, INCR(read_count) + 1 FROM board WHERE id = 1;

    *   정의된 타입의 최대값을 초과할 경우 **INCR** 함수는 해당 칼럼을 0으로 초기화 한다. 반대로 최소값에 **DECR** 함수가 적용되어도 0으로 초기화된다.

    *   **INCR** / **DECR** 함수는 **UPDATE** 트리거와 무관하게 실행되므로 데이터 일관성이 보장되지 않을 수 있다. 다음은 **INCR** 함수가 **UPDATE** 트리거와 무관하게 실행되기 때문에 데이터베이스의 일관성이 위반되는 예이다.

        .. code-block:: sql

            CREATE TRIGGER event_tr BEFORE UPDATE ON event EXECUTE REJECT;
            SELECT INCR(players) FROM event WHERE gender='M';

    *   **INCR** / **DECR** 함수는 HA 구성의 슬레이브 노드나 read-only 모드의 CSQL 인터프리터(csql -r) 또는 Read Only, Standby Only 모드처럼 쓰기가 금지된 모드(cubrid_broker.conf의 ACCESS_MODE=RO 또는 SO)의 브로커에서 사용 시 오류를 반환한다.

**예제**

먼저, board 테이블에는 아래와 같이 3건의 데이터가 입력되었다고 가정한다.

.. code-block:: sql

    CREATE TABLE board (
      id  INT, 
      title  VARCHAR(100), 
      content  VARCHAR(4000), 
      read_count  INT 
    );
    INSERT INTO board VALUES (1, 'aaa', 'text...', 0);
    INSERT INTO board VALUES (2, 'bbb', 'text...', 0);
    INSERT INTO board VALUES (3, 'ccc', 'text...', 0);

다음은 id 값이 1인 데이터의 read_count 칼럼의 값을 **INCR** 함수로 증가시키는 예이다.

.. code-block:: sql

    SELECT content, INCR(read_count) FROM board WHERE id = 1;

::

      content                read_count
    ===================================
      'text...'                       0

예와 같이 **SELECT** 문에 **INCR** 함수를 사용함으로써 해당 칼럼 값은 read_count + 1이 된다. 결과는 다음과 같은 **SELECT** 문을 통해 확인해 볼 수 있다.

.. code-block:: sql

    SELECT content, read_count FROM board WHERE id = 1;
    
::
    
      content                read_count
    ===================================
      'text...'                       1
