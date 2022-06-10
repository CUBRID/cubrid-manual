
:meta-keywords: call statement
:meta-description: The CALL statement invokes a stored procedure

****
CALL
****

**CALL** 문은 데이터 베이스에 정의된 메서드 또는 저장 함수/프로시저와 같은 저장 루틴을 호출하기 위해 사용한다.
CUBRID에서 지원하는 저장 루틴은 다음과 같다.

  * :ref:`create-procedure` 구문을 통해 등록한 저장 프로시저 
  * :ref:`create-function` 구문을 통해 등록한 저장 함수
  * C로 작성한 :doc:`/sql/method`

::

    CALL <routine_clause> ;
        
        <routine_clause> ::=
            routine_name ([<arg_value> [{, <arg_value> } ...]]) [<method_call_target>] [<to_variable>]

        <arg_value>
            literal |
            :host_variable |
            any CSQL expression

        <method_call_target> ::=
            ON [CLASS] object-valued-expression

        <to_variable> ::= 
            INTO :host_variable |
            TO :host_variable

*   *routine_name*: 호출할 메서드 또는 저장 함수/프로시저의 이름을 지정한다. 대소문자를 구분하지 않는다.
*   <*arg_value*>: 호출할 저장 루틴의 인수를 지정한다. 리터럴 값, 호스트 변수와 표현식을 지정할 수 있다.
*   <*method_call_target*>: C로 작성한 메서드를 사용할 때만 지정한다.
    클래스 이름, 변수, 혹은 또 다른 메서드 호출(객체를 반환하는)을 포함하는 객체 값의 표현식(object-valued-expression)을 지정한다.
    만약 클래스 객체에서 동작하는 클래스 메서드를 호출하려면, <*call_target*> 앞에 반드시 **CLASS** 키워드가 있어야 한다. 
    이 경우 클래스 이름은 클래스 메서드가 정의된 클래스의 이름이어야 한다. 만약 인스턴스 메서드를 호출하려면, 인스턴스 객체를 나타내는 식을 지정해야 한다. 
*   <*to_variable*>: 저장 루틴이 반환하는 값을 호스트 변수에 저장할 수 있다.

**CALL** 문에서 지정한 이름에 대하여 CUBRID에 등록된 저장 루틴을 다음과 같이 검색한다.

1.   **CALL** 문에 대상 클래스가 있는 경우 메서드로 처리한다.
2.   **CALL** 문에 대상 클래스가 없는 경우 먼저 Java 저장 함수/프로시저 수행 여부를 검사하고 같은 이름이 존재하면 수행한다.
3.   2에서 Java 저장 함수/프로시저가 존재하지 않으면 메서드 수행 여부를 검사하여 같은 이름이 존재하면 수행한다.

.. code-block:: sql

    CALL Hello() INTO :HELLO;
    CALL Sp_int(3) INTO :i;
    CALL phone_info('Tom','016-111-1111');

만약 존재하지 않는 메소드 또는 저장 함수/프로시저를 호출하는 경우에는 다음과 같은 에러가 나타난다.

*    **CALL** 문에 인자가 없는 경우는 메서드와 구분되므로 "ERROR: Stored procedure/function 'deposit' does not exist."라는 오류 메시지가 나타난다. 
*    **CALL** 문에 인자가 있는 경우에는 메서드와 구분할 수 없기 때문에 "ERROR: Methods require an object as their target."이라는 메시지가 나타난다.

.. code-block:: sql

    // CALL 문에 인자가 없는 경우
    CALL deposit();
    
::

    ERROR: Stored procedure/function 'deposit' does not exist.

.. code-block:: sql

    // CALL 문에 인자가 있는 경우
    CALL deposit('Tom', 3000000);
    
::

    ERROR: Methods require an object as their target.

그리고, 아래와 같이 저장 함수/프로시저를 호출하는 **CALL** 문 안에 **CALL** 문이 중첩되는 경우와 **CALL** 문을 사용하여 저장 함수/프로시저 호출 시 인자로 서브 질의를 사용할 경우 **CALL** 문은 수행이 되지 않는다.

.. code-block:: sql

    CALL phone_info('Tom', CALL sp_int(999));
    CALL phone_info((SELECT * FROM Phone WHERE id='Tom'));
