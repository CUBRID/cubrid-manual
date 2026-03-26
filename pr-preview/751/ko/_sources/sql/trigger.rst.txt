
:meta-keywords: cubrid trigger, database trigger, trigger condition, trigger action, trigger debugging, trigger example
:meta-description: CUBRID trigger definition, manipulation and mechanics.

******
트리거
******

.. _create-trigger:

CREATE TRIGGER
==============

트리거 정의를 위한 가이드라인
-----------------------------

트리거 정의로 다양하고 강력한 기능을 만들 수 있다. 트리거를 생성하기 전에 다음과 같은 정의 사항을 고려해야 한다.

*   **트리거의 조건 영역 표현식이 데이터베이스에 예측할 수 없는 결과(side effect)를 가져오지는 않는가?**

    SQL 문을 예측이 가능한 범위 내에서 사용해야 한다.

*   **트리거의 실행 영역이 트리거의 이벤트 대상으로 주어진 테이블을 변경하지는 않는가?**

    이러한 유형의 설계가 트리거의 정의에서 사용이 금지되어 있지는 않지만, 무한 루프로 빠지는 트리거가 만들어질 수 있어 주의해서 사용해야 한다. 트리거 실행 영역이 이벤트 대상 테이블을 수정할 때, 같은 트리거가 다시 불려질 수 있다. 또한 **WHERE** 절을 포함하는 문장에 의해 트리거가 발생하면, 해당 트리거는 **WHERE** 절에 의해 수행되는 테이블에는 일반적으로 부작용이 없다.

*   **트리거가 불필요한 오버헤드를 만들어 내고 있지는 않는가?**

    원하는 동작이 소스 내에서 조금 더 효과적으로 표현될 수 있다면 직접 소스 내에서 구현하도록 한다.

*   **트리거가 재귀적으로 실행되고 있지는 않는가?**

    트리거의 실행 영역이 트리거를 부르고 이 트리거가 다시 처음 트리거를 부르면 재귀 루프(recursive loop)가 데이터베이스에 만들어진다. 재귀 루프가 만들어지면, 트리거가 정확히 수행되지 않거나 진행 중인 루프를 단절하기 위해 현재 세션을 강제로 종료해야 할 수도 있다.

*   **트리거의 정의는 유일한가?**

    동일한 테이블에서 정의된 트리거나, 동일한 실행 영역에서 시작된 트리거는 복구할 수 없는 에러의 원인이 된다. 동일한 테이블에 있는 트리거는 다른 트리거 이벤트를 가져야 한다. 또한, 트리거의 우선순위는 명시적으로 정의되어 있거나 모호하지 않아야 한다.

트리거 정의 구문
----------------

**CREATE TRIGGER** 문을 사용하여 새로운 트리거를 생성하고, 트리거 대상, 실행 조건과 수행할 내용을 정의할 수 있다. 트리거는 데이터베이스 객체로서, 특정 이벤트가 대상 테이블에 대해 발생하면 정의된 동작을 수행한다. ::

    CREATE TRIGGER trigger_name
    [ STATUS { ACTIVE | INACTIVE } ]
    [ PRIORITY key ]
    <event_time> <event_type> [<event_target>]
    [ IF condition ]
    EXECUTE [ AFTER | DEFERRED ] action 
    [COMMENT 'trigger_comment'];
     
    <event_time> ::=
        BEFORE |
        AFTER  |
        DEFERRED
     
    <event_type> ::=
        INSERT |
        STATEMENT INSERT |
        UPDATE |
        STATEMENT UPDATE |
        DELETE |
        STATEMENT DELETE |
        ROLLBACK |
        COMMIT
     
    <event_target> ::=
        ON table_name |
        ON table_name [ (column_name) ]
     
    <condition> ::=
        expression
     
    <action> ::=
        REJECT |
        INVALIDATE TRANSACTION |
        PRINT message_string |
        INSERT statement |
        UPDATE statement |
        DELETE statement

*   *trigger_name*: 정의하려는 트리거의 이름을 지정한다.
*   [ **STATUS** { **ACTIVE** | **INACTIVE** } ]: 트리거의 상태를 정의한다(정의하지 않을 경우 기본값은 **ACTIVE** ).

    *   **ACTIVE** 상태인 경우 관련 이벤트가 발생할 때마다 트리거를 실행한다.
    *   **INACTIVE** 상태인 경우 관련 이벤트가 발생하여도 트리거를 실행하지 않는다. 트리거의 활성 여부는 변경할 수 있다. 자세한 내용은 :ref:`alter-trigger` 을 참조한다.

*   [ **PRIORITY** *key* ]: 하나의 이벤트에 대해서 다수의 트리거가 불려질 경우 실행되는 우선순위를 부여한다. *key* 값은 반드시 음수가 아닌 부동 소수점 값이어야 한다. 우선순위를 정의하지 않을 경우 가장 낮은 우선순위인 0을 할당한다. 같은 우선순위를 가지는 트리거는 임의의 순서로 실행된다. 트리거의 우선순위는 변경할 수 있다. 자세한 내용은 :ref:`alter-trigger` 을 참조한다.

*   <*event_time*>: 트리거의 조건 영역과 실행 영역이 실행되는 시점을 지정하며 **BEFORE**, **AFTER**, **DEFERRED** 가 있다. 자세한 내용은 :ref:`trigger-event-time` 을 참조한다.
*   <*event_type*>: 트리거 타입은 사용자 트리거와 테이블 트리거로 나뉜다. 자세한 내용은 :ref:`trigger-event-type` 을 참조한다.
*   <*event_target*>: 이벤트 대상은 트리거가 호출되기 위한 대상을 지정할 때 쓰인다. 자세한 내용은 :ref:`trigger-event-target` 을 참조한다.

*   <*condition*>: 트리거의 조건영역을 지정한다. 자세한 내용은 :ref:`trigger-condition` 을 참조한다.
*   <*action*>: 트리거의 실행영역을 지정한다. 자세한 내용은 :ref:`trigger-action` 을 참조한다.
*   *trigger_comment*: 트리거의 커멘트를 지정한다.

다음은 *participant* 테이블의 레코드를 갱신할 때 획득 메달의 개수가 0보다 작을 경우 갱신을 거절하는 트리거를 생성하는 예제이다.
2004년도 올림픽에 한국이 획득한 금메달의 개수를 음수로 갱신할 경우 갱신이 거절되는 것을 알 수 있다.

.. code-block:: sql

    CREATE TRIGGER medal_trigger
    BEFORE UPDATE ON participant
    IF new.gold < 0 OR new.silver < 0 OR new.bronze < 0
    EXECUTE REJECT;
     
    UPDATE participant SET gold = -5 WHERE nation_code = 'KOR'
    AND host_year = 2004;

::

    ERROR: The operation has been rejected by trigger "medal_trigger".

.. _trigger-event-time:

이벤트 시점
-----------

트리거의 조건 영역과 실행 영역이 실행되는 시점을 지정한다. 이벤트 시점의 종류에는 **BEFORE**, **AFTER**, **DEFERRED** 가 있다.

*   **BEFORE**: 이벤트가 처리되기 전에 조건을 검사한다.
*   **AFTER**: 이벤트가 처리된 후에 조건을 검사한다.
*   **DEFERRED**: 이벤트에 대한 트랜잭션의 끝에서 조건을 검사한다. **DEFERRED** 로 지정할 경우 이벤트 타입에 **COMMIT** 이나 **ROLLBACK** 을 사용할 수 없다.

트리거 타입
-----------

**사용자 트리거(User Trigger)**

*   데이터베이스의 특정 사용자와 관련된 트리거를 사용자 트리거(user trigger)라고 한다.
*   사용자 트리거는 이벤트 대상이 없으며 트리거의 소유자(트리거를 생성한 사용자)에 의해서만 실행된다.
*   사용자 트리거를 정의하는 이벤트 타입은 **COMMIT** 과 **ROLLBACK** 이 있다.

**테이블 트리거(Table Trigger)**

*   특정 테이블을 이벤트 대상으로 가지는 트리거를 테이블 트리거(클래스 트리거)라 한다.
*   테이블 트리거는 대상 테이블에 **SELECT** 권한을 가지는 모든 사용자가 볼 수 있다.
*   테이블 트리거를 정의하는 이벤트 타입은 인스턴스 이벤트와 문장 이벤트가 있다.

.. _trigger-event-type:

트리거 이벤트 타입
------------------

*   인스턴스 이벤트(instance event) : 인스턴스 이벤트는 이벤트 연산의 단위가 인스턴스(레코드)인 이벤트 타입을 말한다. 인스턴스 이벤트의 종류는 다음과 같다.

    *   **INSERT**
    *   **UPDATE**
    *   **DELETE**

*   문장 이벤트(statement event): 이벤트 타입을 문장 이벤트로 정의하면 주어진 문장(이벤트)에 의해 영향을 받는 객체(인스턴스)가 많더라도, 트리거는 문장이 시작할 때 한 번만 불려지게 된다. 문장 이벤트의 종류는 다음과 같다.

    *   **STATEMENT INSERT**
    *   **STATEMENT UPDATE**
    *   **STATEMENT DELETE**

*   기타 이벤트: **COMMIT** 과 **ROLLBACK** 은 개별적인 인스턴스에는 적용할 수 없다.

    *   **COMMIT**
    *   **ROLLBACK**

다음은 인스턴스 이벤트를 사용하는 예제이다. *example* 트리거는 데이터베이스 갱신에 의해 영향을 받는 각각의 인스턴스에 대해서 한번씩 불려진다. 예를 들어, *history* 테이블의 다섯 개 인스턴스의 *score* 를 변경했다면, 이 트리거는 다섯 번 불려진다.

.. code-block:: sql

    CREATE TABLE update_logs(event_code INTEGER, score VARCHAR(10), dt DATETIME);
    
    CREATE TRIGGER example
    BEFORE UPDATE ON history(score)
    EXECUTE INSERT INTO update_logs VALUES (obj.event_code, obj.score, SYSDATETIME);

만약 *score* 칼럼의 첫 번째 인스턴스가 갱신되기 전에 트리거가 한 번만 호출되게 하려면, 아래의 예와 같이 **STATEMENT UPDATE** 형식을 사용한다.

다음은 문장 이벤트를 사용하는 예제이다. 문장 이벤트를 지정하면 갱신의 영향을 받는 인스턴스가 많더라도, 첫 번째 인스턴스가 갱신되기 전에 트리거가 한 번만 불려지게 된다.

.. code-block:: sql

    CREATE TRIGGER example
    BEFORE STATEMENT UPDATE ON history(score)
    EXECUTE PRINT 'There was an update on history table';

.. note::

    *   이벤트 타입으로 인스턴스 이벤트와 문장 이벤트를 지정할 경우에는 반드시 이벤트 대상을 명시해야 한다.
    *   **COMMIT** 과 **ROLLBACK** 은 이벤트 대상을 가질 수 없다.

.. _trigger-event-target:

트리거 이벤트 대상
------------------

이벤트 대상은 트리거가 호출되기 위한 대상을 지정할 때 쓰인다. 트리거 이벤트의 대상은 테이블명 혹은 테이블명과 칼럼명으로 지정할 수 있으며 칼럼명을 지정하면 해당 칼럼이 이벤트의 영향을 받을 때에만 트리거가 불려진다. 만약 칼럼을 지정하지 않으면 지정된 테이블 내에 어떤 칼럼이 영향을 받더라도 트리거가 호출된다. 오직 **UPDATE**, **STATEMENT UPDATE** 이벤트만이 이벤트 대상에 칼럼을 지정할 수 있다.

다음은 *example* 트리거의 이벤트 대상을 *history* 테이블의 *score* 칼럼으로 지정한 예제이다.

.. code-block:: sql

    CREATE TABLE update_logs(event_code INTEGER, score VARCHAR(10), dt DATETIME);
    
    CREATE TRIGGER example
    BEFORE UPDATE ON history(score)
    EXECUTE INSERT INTO update_logs VALUES (obj.event_code, obj.score, SYSDATETIME);

이벤트 타입과 대상 조합
-----------------------

트리거를 호출하는 데이터베이스 이벤트는 트리거 이벤트 타입과 트리거 정의 내의 이벤트 대상에 의해 식별된다. 다음은 트리거 이벤트 타입과 대상 조합, 트리거 이벤트가 나타내는 CUBRID 데이터베이스 이벤트의 활동을 표로 정리한 것이다.

+--------------+--------------+-----------------------------------------------------------------+
| 이벤트 타입  | 이벤트 대상  | 대응되는 데이터베이스 활동                                      |
+==============+==============+=================================================================+
| **UPDATE**   | 테이블       | 테이블에 **UPDATE** 문이 실행되었을 때 트리거가 호출된다.       |
+--------------+--------------+-----------------------------------------------------------------+
| **INSERT**   | 테이블       | 테이블에 **INSERT** 문이 실행되었을 때 트리거가 호출된다.       |
+--------------+--------------+-----------------------------------------------------------------+
| **DELETE**   | 테이블       | 테이블에 **DELETE** 문이 실행되었을 때 트리거가 호출된다.       |
+--------------+--------------+-----------------------------------------------------------------+
| **COMMIT**   | 없음         | 데이터베이스 트랜잭션이 커밋되었을 때 트리거가 호출된다.        |
+--------------+--------------+-----------------------------------------------------------------+
| **ROLLBACK** | 없음         | 데이터베이스의 트랜잭션이 롤백되었을 때 트리거가 호출된다.      |
+--------------+--------------+-----------------------------------------------------------------+

.. _trigger-condition:

트리거 조건 영역
----------------

트리거를 정의할 때 조건 영역을 정의하여 트리거의 수행 영역에 대한 수행 여부를 결정한다.

*   트리거 조건 영역이 기술된다면, 참 또는 거짓을 평가할 수 있는 단독적인 복합 표현식으로 쓰여질 수 있다. 이 경우에 표현식은 **SELECT** 문의 **WHERE** 절에 허용되는 산술 연산자와 논리 연산자를 포함할 수 있다. 조건 영역이 참이면, 트리거 실행 영역이 수행되고, 거짓이면 실행되지 않는다.

*   트리거의 조건 영역을 생략하면 조건 없는 트리거(unconditional trigger)가 되며 트리거가 호출될 때 항상 트리거의 실행 영역이 수행된다.

다음은 조건 영역 내의 표현식에 상관명을 이용한 예제이다. 이벤트 타입이 **INSERT**, **UPDATE**, **DELETE** 인 경우에, 조건 영역 내의 표현식은 특정 칼럼 값에 접근하기 위하여 상관명 **obj**, **new**, **old** 를 사용할 수 있다. 예제에서 *example* 트리거는 칼럼의 새로운 값을 이용해서 조건 영역을 검사하기 위해 트리거 조건 영역에 **new** 를 칼럼 이름 앞에 사용하였다.

.. code-block:: sql

    CREATE TRIGGER example
    BEFORE UPDATE ON participant
    IF new.gold < 0 OR new.silver < 0 OR new.bronze < 0
    EXECUTE REJECT;

다음은 조건 영역 내의 표현식에 **SELECT** 문을 사용한 예제이다. 예제의 트리거는 집계함수 **COUNT** (\*)를 사용하는 **SELECT** 문을 사용하여 그 값과 상수를 비교한다. **SELECT** 문은 반드시 괄호로 싸여 있어야 하고, 표현식의 마지막에 위치해야 한다.

.. code-block:: sql

    CREATE TRIGGER example
    BEFORE INSERT ON participant
    IF 1000 >  (SELECT COUNT(*) FROM participant)
    EXECUTE REJECT;

.. note::

    트리거 조건 영역에 주어진 표현식은 조건 영역이 수행되는 동안에 메서드가 호출되면 데이터베이스에 부작용을 초래할 수 있다. 트리거 조건 영역은 데이터베이스에 생각지 못한 부작용이 발생하지 않도록 구성해야 한다.

상관명(correlation name)
------------------------

트리거를 정의할 때 상관명을 사용하여 대상 테이블의 칼럼 값에 접근할 수 있다. 상관명은 실제적으로 트리거를 부르는 데이터베이스 연산에 의해 영향을 받는 인스턴스를 나타낸다. 상관명은 트리거의 조건 영역이나 실행 영역에도 기술할 수 있다.

상관명의 종류에는 **new**, **old**, **obj** 가 있으며 이러한 상관명은 인스턴스 트리거에서 **INSERT**, **UPDATE**, **DELETE** 의 이벤트 타입을 가지고 있는 트리거에서만 사용할 수 있다.

상관명의 사용은 아래 표와 같이 트리거 조건 영역에 정의된 이벤트 시점에 의해 더욱 제한된다.

+------------+------------+-----------------------+
|            | BEFORE     | AFTER or DERERRED     |
+============+============+=======================+
| **INSERT** | **new**    | **obj**               |
+------------+------------+-----------------------+
| **UPDATE** | **obj**    | **obj**               |
|            |            |                       |
|            | **new**    | **old** (AFTER)       |
+------------+------------+-----------------------+
| **DELETE** | **obj**    | N/A                   |
+------------+------------+-----------------------+

+---------+-------------------------------------------------------------------------------------------------------------+
| 상관명  | 대표 속성 값                                                                                                |
+=========+=============================================================================================================+
| **obj** | 인스턴스의 현재 속성 값을 나타낸다. 인스턴스가 갱신되거나 삭제되기 전에 속성값에 접근하기 위해서 사용한다.  |
|         | 그리고 인스턴스가 갱신되거나 삽입된 후에 속성 값에 접근하기 위해 사용한다.                                  |
+---------+-------------------------------------------------------------------------------------------------------------+
| **new** | 삽입이나 갱신 연산에 의해 제시되는 속성값을 나타낸다.                                                       |
|         | 새로운 값은 인스턴스가 실제적으로 삽입되거나 갱신되기 전에만 접근할 수 있다.                                |
+---------+-------------------------------------------------------------------------------------------------------------+
| **old** | 갱신 연산의 완료 전에 존재하던 속성값을 나타낸다. 이 값은 트리거가 수행되는 동안만 유지된다.                |
|         | 트리거가 종료되면 **old** 값은 잃어버리게 된다.                                                             |
+---------+-------------------------------------------------------------------------------------------------------------+

.. _trigger-action:

트리거 실행 영역
----------------

트리거 실행 영역은 트리거의 조건 영역이 참이거나 조건 영역이 생략된 경우 수행될 내용을 기술하는 영역이다. 실행 영역 절에 특정 시점(**AFTER** 나 **DEFERRED**)이 주어지지 않으면, 실행 영역은 트리거 이벤트와 같은 시점에서 수행된다.

아래 목록은 트리거를 정의할 때 사용할 수 있는 실행 영역의 목록이다.

*   **REJECT**: 트리거에서 조건 영역이 참이 아닌 경우 트리거를 발동시킨 연산은 거절되고 데이터베이스의 예전 상태를 그대로 유지한다. 연산이 수행된 후에는 거절할 수 없기 때문에 **REJECT** 는 실행 시점이 **BEFORE** 일 때만 허용된다. 따라서 실행 시점이 **AFTER** 나 **DERERRED** 인 경우 **REJECT** 를 사용해서는 안 된다.

*   **INVALIDATE TRANSACTION**: 트리거를 부른 이벤트 연산은 수행되지만, 커밋을 포함하고 있는 트랜잭션은 수행되지 않도록 한다. 트랜잭션이 유효하지 않으면 반드시 **ROLLBACK** 문으로 취소시켜야 한다. 이러한 실행은 데이터를 변경하는 이벤트가 발생한 후에 유효하지 않은 데이터를 가지는 것으로부터 데이터베이스를 보호하기 위해 사용된다.

*   **PRINT**: 터미널 화면에 텍스트 메시지로 트리거 활동을 가시적으로 보여주기 때문에 트리거의 개발이나 시험하는 도중에 사용될 수 있다. 이벤트 연산의 결과를 거절하거나 무효화시키지는 않는다.
*   **INSERT**: 테이블에 하나 혹은 그 이상의 새로운 인스턴스를 추가한다.
*   **UPDATE**: 테이블에 있는 하나 혹은 그 이상의 칼럼 값을 변경한다.
*   **DELETE**: 테이블로부터 하나 혹은 그 이상의 인스턴스를 제거한다.

다음은 트리거 생성 시에 실행영역의 정의 방법을 보여주는 예제이다. *medal_trig* 트리거는 실행 영역에 **REJECT** 를 지정하였다. **REJECT** 는 실행 시점이 **BEFORE** 일 때만 지정 가능하다.

.. code-block:: sql

    CREATE TRIGGER medal_trig
    BEFORE UPDATE ON participant
    IF new.gold < 0 OR new.silver < 0 OR new.bronze < 0
    EXECUTE REJECT;

.. note::

    *   **INSERT** 이벤트가 정의된 트리거의 실행 영역에 **INSERT** 를 사용할 때는 트리거가 무한 루프에 빠질 수 있으므로 주의해야 한다.
    *   **UPDATE** 이벤트가 정의된 트리거가 분할된 테이블에서 동작하는 경우, 정의된 분할이 깨지거나 의도하지 않은 오동작이 발생할 수 있으므로 주의해야 한다. 이를 방지하기 위해 CUBRID는 트리거가 동작중인 경우 분할 변경을 야기하는 **UPDATE** 가 실행되지 않도록 오류 처리한다. **UPDATE** 이벤트가 정의된 트리거의 실행 영역에 **UPDATE** 를 사용할 때는 무한 루프에 빠질 수 있으므로 주의해야 한다.

트리거의 커멘트
---------------

트리의 커멘트를 다음과 같이 명시할 수 있다. 

.. code-block:: sql

    CREATE TRIGGER trg_ab BEFORE UPDATE on abc(c) EXECUTE UPDATE cube_ab SET sumc = sumc + 1
    COMMENT 'test trigger comment';

트리거의 커멘트는 다음 구문에서 확인할 수 있다.

.. code-block:: sql

	SELECT name, comment FROM db_trigger;
	SELECT trigger_name, comment FROM db_trig;

또는 CSQL 인터프리터에서 스키마를 출력하는 ;sc 명령으로 트리거의 커멘트를 확인할 수 있다.

.. code-block:: sql

    $ csql -u dba demodb
    
    csql> ;sc tbl

트리거 커멘트의 변경은 아래의 **ALTER TRIGGER** 문을 참고한다.

.. _alter-trigger:

ALTER TRIGGER
=============

트리거 정의에서 **STATUS** 와 **PRIORITY** 옵션에 대해 **ALTER** 구문을 이용하여 변경할 수 있다. 만약 트리거의 다른 부분에 대해 변경(이벤트 대상 또는 조건 표현식)이 필요하면, 트리거를 삭제한 후 재생성해야 한다. 

::

    ALTER TRIGGER trigger_name <trigger_option> ;

    <trigger_option> ::=
        STATUS { ACTIVE | INACTIVE } |
        PRIORITY key

*   *trigger_name*: 변경할 트리거의 이름을 지정한다.
*   **STATUS** { **ACTIVE** | **INACTIVE** }: 트리거의 상태를 변경한다.
*   **PRIORITY** *key*: 우선순위를 변경한다.

다음은 medal_trig 트리거를 생성하고 트리거의 상태를 **INACTIVE** 로, 우선순위를 0.7로 변경하는 예제이다.

.. code-block:: sql

    CREATE TRIGGER medal_trig
    STATUS ACTIVE
    BEFORE UPDATE ON participant
    IF new.gold < 0 OR new.silver < 0 OR new.bronze < 0
    EXECUTE REJECT;

    ALTER TRIGGER medal_trig STATUS INACTIVE;
    ALTER TRIGGER medal_trig PRIORITY 0.7;

.. note::

    *   같은 **ALTER TRIGGER** 문 내에서는 한 개의 *trigger_option* 만 기술할 수 있다.
    *   만약 테이블 트리거를 변경하려면, 해당 트리거의 소유자이거나, 해당 트리거가 있는 테이블에 대해 **ALTER** 권한이 부여되어 있어야 한다.
    *   사용자 트리거를 변경하기 위해서는 반드시 해당 트리거의 소유자여야 한다. *trigger_option* 에 대한 자세한 내용은 :ref:`create-trigger` 을 참조한다. **PRIORITY** 옵션과 같이 기술하는 key는 반드시 음이 아닌 부동 소수점 값(non-negative floating point value)이어야 한다.

트리거 커멘트
-------------

트리거 커멘트는 **ALTER TRIGGER** 문을 실행하여 다음과 같이 변경할 수 있다.

::

    ALTER TRIGGER trigger_name [trigger_option] 
    [COMMENT ‘comment_string’];

*   *comment_string*: 트리거의 커멘트를 지정한다.

트리거의 커멘트만 변경하는 경우 트리거 옵션(trigger_option)을 생략할 수 있다.

*trigger_option*\은 위의 :ref:`alter-trigger` 구문을 참고한다.

.. code-block:: sql

    ALTER TRIGGER trg_ab COMMENT 'new trigger comment';

DROP TRIGGER
============

**DROP TRIGGER** 구문을 이용하여 트리거를 삭제한다. ::

    DROP TRIGGER trigger_name ; 

*   *trigger_name*: 삭제할 트리거의 이름을 지정한다.

다음은 medal_trig 트리거를 삭제하는 예제이다.

.. code-block:: sql

    DROP TRIGGER medal_trig;

.. note::

    *   트리거가 사용자 트리거(즉 트리거 이벤트가 **COMMIT** 이거나 **ROLLBACK**)이면, 트리거의 소유자만 볼 수 있고 소유자만 제거할 수 있다.
    *   한 개의 **DROP TRIGGER** 문에서는 한 개의 트리거만 제거할 수 있다.테이블 트리거는 트리거가 속해 있는 테이블에 대해 **ALTER** 권한이 있는 사용자에 의해 제거될 수 있다.

RENAME TRIGGER
==============

트리거의 이름은 **RENAME** 구문의 **TRIGGER** 예약어를 이용해서 변경한다. ::

    RENAME TRIGGER old_trigger_name AS new_trigger_name ;

*   *old_trigger_name*: 트리거의 현재 이름을 입력한다.
*   *new_trigger_name*: 변경할 트리거의 이름을 지정한다.

.. code-block:: sql

    RENAME TRIGGER medal_trigger AS medal_trig;

.. note::

    *   트리거 이름은 모든 트리거 사이에서 유일해야 한다. 하지만 데이터베이스 내의 테이블 이름과 같은 이름을 가질 수는 있다.
    *   만약 테이블 트리거의 이름을 변경하려면, 트리거의 소유자이거나, 해당 트리거가 있는 테이블에 대해 **ALTER** 권한이 부여되어 있어야 한다. 사용자 트리거는 트리거의 소유자만 이름을 변경할 수 있다.

지연된 트리거
=============

지연된 트리거 실행영역과 조건 영역은 나중에 실행되거나 취소될 수 있다. 이러한 트리거들은 이벤트 시점(event time)이나 실행 영역(action) 절에 **DEFERRED** 시간 옵션을 포함하고 있다. **DEFERRED** 옵션이 이벤트 시점에 기술되고, 실행 영역 앞에 시간이 생략되었다면, 실행 영역은 자동으로 지연된다.

지연된 영역 실행
----------------

지연된 트리거의 조건 영역이나 실행 영역을 즉시 실행시킨다. ::

    EXECUTE DEFERRED TRIGGER <trigger_identifier> ;

    <trigger_identifier> ::=
        trigger_name |
        ALL TRIGGERS

*   *trigger_name*: 트리거의 이름을 지정하면 지정된 트리거의 지연된 활동이 실행된다.
*   **ALL TRIGGERS**: 현재 모든 지연된 활동이 실행된다.

지연된 영역 취소
----------------

지연된 트리거의 조건 영역과 실행 영역을 취소한다. ::

    DROP DEFERRED TRIGGER <trigger_identifier> ;

    <trigger_identifier> ::=
        trigger_name |
        ALL TRIGGERS

*   *trigger_name*: 트리거의 이름을 지정하면 지정된 트리거의 지연된 활동이 취소된다.
*   **ALL TRIGGERS**: 현재 모든 지연된 활동이 취소된다.

트리거 권한 부여
----------------

트리거에 대한 권한은 명시적으로 부여되지 않는다. 트리거의 정의에 기술된 이벤트 대상 테이블에 권한이 부여되었을 때 사용자는 테이블 트리거에 대한 권한을 자동적으로 획득한다. 다시 말하자면, 테이블 대상(**INSERT**, **UPDATE** 등)을 가지는 트리거는 해당 테이블에 적절한 권한을 가지는 모든 사용자에게 보인다. 사용자 트리거(**COMMIT** 과 **ROLLBACK**)는 트리거를 정의한 사용자만 볼 수 있다. 트리거의 소유자이면 모든 권한은 자동적으로 부여된다.

.. note::

    *   테이블 트리거를 정의하기 위해서는 관련된 테이블에 **ALTER** 권한이 반드시 있어야 한다.
    *   사용자 트리거를 정의하기 위해서는 유효한 사용자를 이용하여 데이터베이스에 접근하는 것이 필요하다.

REPLACE와 INSERT ... ON DUPLICATE KEY UPDATE에서의 트리거
=========================================================

CUBRID에서는 **REPLACE** 문과 **INSERT ... ON DUPLICATE KEY UPDATE** 문 실행 시 내부적으로 **DELETE**, **UPDATE**, **INSERT** 작업이 발생하면서 해당 트리거가 실행된다. 다음 표는 **REPLACE** 혹은 **INSERT ... ON DUPLICATE KEY UPDATE** 문이 수행될 때 발생하는 이벤트에 따라 CUBRID에서 트리거가 어떤 순서로 동작하는지를 나타낸다. **REPLACE** 문과 **INSERT ... ON DUPLICATE KEY UPDATE** 문 모두 상속받은 클래스(테이블)에서는 트리거가 동작하지 않는다.

**REPLACE와 INSERT ... ON DUPLICATE KEY UPDATE 문에서 트리거의 동작 순서**

+--------------------------------------------+------------------+
| 이벤트                                     | 트리거 동작 순서 |
+============================================+==================+
| REPLACE                                    | BEFORE DELETE >  |
| 레코드가 삭제되고 삽입될 때                | AFTER DELETE >   |
|                                            | BEFORE INSERT >  |
|                                            | AFTER INSERT     |
+--------------------------------------------+------------------+
| INSERT ... ON DUPLICATE KEY UPDATE         | BEFORE UPDATE >  |
| 레코드가 업데이트될 때                     | AFTER UPDATE     |
+--------------------------------------------+------------------+
| REPLACE, INSERT ... ON DUPLCATE KEY UPDATE | BEFORE INSERT >  |
| 레코드가 삽입만 될 때                      | AFTER INSERT     |
+--------------------------------------------+------------------+

다음은 *with_trigger* 테이블에 **INSERT ... ON DUPLICATE KEY UPDATE** 와 **RELPACE** 를 수행하면 트리거가 동작하여 *trigger_actions* 테이블에 레코드를 삽입하는 예제이다.

.. code-block:: sql

    CREATE TABLE with_trigger (id INT UNIQUE);
    INSERT INTO with_trigger VALUES (11);
     
    CREATE TABLE trigger_actions (val INT);
     
    CREATE TRIGGER trig_1 BEFORE INSERT ON with_trigger EXECUTE INSERT INTO trigger_actions VALUES (1);
    CREATE TRIGGER trig_2 BEFORE UPDATE ON with_trigger EXECUTE INSERT INTO trigger_actions VALUES (2);
    CREATE TRIGGER trig_3 BEFORE DELETE ON with_trigger EXECUTE INSERT INTO trigger_actions VALUES (3);
     
    INSERT INTO with_trigger VALUES (11) ON DUPLICATE KEY UPDATE id=22;
     
    SELECT * FROM trigger_actions;

::
    
              va
    ==============
                2
     
.. code-block:: sql

    REPLACE INTO with_trigger VALUES (22);
     
    SELECT * FROM trigger_actions;
    
::
    
              va
    ==============
                2
                3
                1

트리거 디버깅
=============

트리거를 정의한 후에는 트리거가 의도한 대로 동작하는지 검사하는 것이 좋다. 종종 트리거가 기대했던 것보다 처리하는데 오랜 시간이 걸리는 경우가 있다. 이는 시스템에 너무 많은 오버헤드를 주거나, 재귀적 루프에 빠졌다는 뜻이다. 이 절에서는 트리거를 디버그하는 몇 가지 방법을 설명한다.

다음은 호출되면 재귀적으로 루프에 빠지도록 정의한 트리거이다. *loop_trg* 트리거는 목적이 다소 인위적이지만 트리거를 디버그하기 위한 예제로 사용될 수 있다.

.. code-block:: sql

    CREATE TRIGGER loop_tgr
    BEFORE UPDATE ON participant(gold)
    IF new.gold > 0
    EXECUTE UPDATE participant
            SET gold = new.gold - 1
            WHERE nation_code = obj.nation_code AND host_year = obj.host_year;

트리거 실행 로그 보기
---------------------

**SET TRIGGER TRACE** 문을 이용하여 터미널에서 트리거의 실행 로그를 볼 수 있다. ::

    SET TRIGGER TRACE <switch> ;

    <switch> ::=
        ON |
        OFF

*   **ON**: **TRACE** 가 작동되며 **OFF** 하거나 현재 데이터베이스 세션을 종료할 때까지 계속 유지된다.
*   **OFF**: **TRACE** 의 작동을 멈춘다.

다음 예제는 트리거의 실행 로그를 보기 위해 **TRACE** 를 작동시키고 *loop_trg* 트리거를 작동시키는 예제이다. 트리거가 호출될 때 수행된 각각의 조건 영역과 실행 영역에 대한 추적을 식별하기 위한 메시지가 터미널에 나타난다. *loop_trg* 트리거는 *gold* 값이 0이 될 때까지 실행되므로 예제에서는 아래의 메세지가 15번 나타난다.

.. code-block:: sql

    SET TRIGGER TRACE ON;
    UPDATE participant SET gold = 15 WHERE nation_code = 'KOR' AND host_year = 1988;

::

    TRACE: Evaluating condition for trigger "loop".
    TRACE: Executing action for trigger "loop".

중첩된 트리거 제한
------------------

**SET TRIGGER** 문의 **MAXIMUM DEPTH** 키워드를 이용하여 단계적으로 발동되는 트리거 수를 제한할 수 있다. 이를 이용하면 재귀적으로 호출되는 트리거가 무한루프에 빠지는 것을 막을 수 있다. ::

    SET TRIGGER [ MAXIMUM ] DEPTH count ;

*   *count*: 양의 정수값으로 트리거가 다른 트리거나 자신을 재귀적으로 발동할 수 있는 횟수를 지정한다. 트리거의 수가 최대 깊이에 도달하면 데이터베이스 요청은 중단되고 트랜잭션은 유효하지 않은 것처럼 표시된다. 설정된 **DEPTH** 는 현재 세션을 제외한 나머지 모든 트리거에 적용된다. 최대값은 32이다.

다음은 재귀적 트리거 호출의 최대 값을 10으로 설정하는 예제이다. 이는 이후에 발동하는 모든 트리거에 적용된다. 이 예제에서 *gold* 칼럼에 대한 값은 15로 갱신되어 트리거는 총 16번 불려지게 된다. 이는 현재 설정된 최대 깊이를 초과하게 되고 아래와 같은 에러 메시지가 발생한다.

.. code-block:: sql

    SET TRIGGER MAXIMUM DEPTH 10;
    UPDATE participant SET gold = 15 WHERE nation_code = 'KOR' AND host_year = 1988;
     
::

    ERROR: Maximum trigger depth 10 exceeded at trigger "loop_tgr".

트리거를 이용한 응용
====================

여기에서는 데모 데이터베이스에 있는 트리거 정의에 대해 알아본다. *demodb* 데이터베이스에 생성되어 있는 트리거는 그리 복잡하지는 않지만 CUBRID에서 사용할 수 있는 대부분의 기능을 활용한다. 이러한 트리거를 테스트할 때, *demodb* 데이터베이스의 원형을 유지하고 싶다면 데이터에 변경이 발생한 후 롤백을 수행해야 한다.

사용자 데이터베이스에 직접 생성한 트리거는 사용자가 만든 응용 프로그램만큼이나 강력할 수 있다.

*participant* 테이블에 만들어진 아래 트리거는 제시된 값이 0보다 작을 때 메달 칼럼(*gold*, *silver*, *bronze*)에 대한 업데이트를 거절한다. 트리거의 조건에 상관명 new가 사용되었기 때문에 시작 시점(evaluation time)은 반드시 **BEFORE** 가 되어야 한다. 비록 기술하지는 않았지만, 이 트리거에서 실행 시점(action time) 또한 **BEFORE** 이다.

.. code-block:: sql

    CREATE TRIGGER medal_trigger
    BEFORE UPDATE ON participant
    IF new.gold < 0 OR new.silver < 0 OR new.bronze < 0
    EXECUTE REJECT;

국가 코드가 'BLA'인 나라의 금메달(*gold*) 수를 업데이트 할 때, *medal_trigger* 트리거가 발동한다. 금메달 수가 음수인 경우를 허용하지 않도록 트리거를 생성하였으므로, 업데이트를 허용하지 않는다.

.. code-block:: sql

    UPDATE participant
    SET gold = -10
    WHERE nation_code = 'BLA';

아래 트리거는 위의 예제와 같은 조건인데, **STATUS ACTIVE** 가 추가된 경우이다. **STATUS** 문이 생략될 경우 기본값은 **ACTIVE** 이며, **ALTER TRIGGER** 문에 의해 **STATUS** 를 **INACTIVE** 로 변경할 수 있다. 

**STATUS** 의 값에 따라 트리거의 실행 여부를 지정할 수 있다.

.. code-block:: sql

    CREATE TRIGGER medal_trig
    STATUS ACTIVE
    BEFORE UPDATE ON participant
    IF new.gold < 0 OR new.silver < 0 OR new.bronze < 0
    EXECUTE REJECT;
     
    ALTER TRIGGER medal_trig
    STATUS INACTIVE;

다음 트리거는 트랜잭션이 커밋되었을 때 어떻게 무결성 제약 조건을 강제적으로 수행하는지 보여 준다. 하나의 트리거가 여러 테이블에 대해 지정 조건을 넣을 수 있다는 점이 앞의 예제와 다르다.

.. code-block:: sql

    CREATE TRIGGER check_null_first
    BEFORE COMMIT
    IF 0 < (SELECT count(*) FROM athlete WHERE gender IS NULL)
    OR 0 < (SELECT count(*) FROM game WHERE nation_code IS NULL)
    EXECUTE REJECT;

다음 트리거는 *record* 테이블에 대해서 트랜잭션이 커밋될 때까지 업데이트 무결성 제약조건 검사를 지연시킨다. **DEFERRED** 키워드가 이벤트 시점으로 주어졌기 때문에 업데이트 실행 시점에 즉시 트리거가 실행되지는 않는다.

.. code-block:: sql

    CREATE TRIGGER deferred_check_on_record
    DEFERRED UPDATE ON record
    IF obj.score = '100'
    EXECUTE INVALIDATE TRANSACTION;

*record* 테이블에서 업데이트가 완료되었을 때, 해당 업데이트는 현재 트랜잭션의 마지막(커밋이나 롤백할 때)에 확인하게 된다. 상관명 **old** 는 **DEFERRED UPDATE** 를 사용하는 트리거의 조건 절에 사용할 수 없다. 따라서 아래와 같은 트리거는 생성할 수 없다.

.. code-block:: sql

    CREATE TABLE foo (n int);
    CREATE TRIGGER foo_trigger
        DEFERRED UPDATE ON foo
        IF old.n = 100
        EXECUTE PRINT 'foo_trigger';

위와 같이 트리거를 생성하려고 하면 다음과 같은 에러 메시지를 보여주고, 실패한다. 

::

    ERROR: Error compiling condition for 'foo_trigger' : old.n is not defined.

상관명 **old** 는 트리거의 조건 시간이 **AFTER** 일 때에만 사용될 수 있다.
