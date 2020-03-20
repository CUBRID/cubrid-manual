
:meta-keywords: call statement, cubrid method type
:meta-description: The methods are written in C with built-in functions of CUBRID database system, and are called by the CALL statement.

******
메서드
******

메서드(method)는 CUBRID 데이터베이스 시스템의 내장 함수로 C로 작성된 프로그램이고, **CALL** 문에 의해 호출된다. 메서드 프로그램은 메서드가 호출되었을 때 동적 로더에 의해 실행 중인 응용과 함께 로드(load)되고 연결(link)된다. 메서드 실행 결과 생성된 리턴 값(return value)은 호출자(caller)에게 전달된다.

메서드 타입
===========

CSQL 언어는 클래스 메서드와 인스턴스 메서드 두 가지 타입의 메서드를 지원한다.

*   **클래스 메서드** 는 클래스 객체에서 호출되는 메서드이다. 일반적으로 클래스의 새로운 인스턴스를 생성하거나 초기화하기 위하여 사용된다. 또한 클래스 속성에 접근하거나 갱신하기 위해서도 사용될 수 있다.

*   **인스턴스 메서드** 는 클래스의 인스턴스에서 호출되는 메서드이다. 대부분의 연산들이 인스턴스에서 수행되기 때문에 클래스 메서드보다 더 자주 사용된다. 예를 들어 인스턴스 메서드는 인스턴스의 속성을 계산하거나 갱신하기 위해 작성될 수 있다. 이 메서드는 메서드가 정의된 클래스의 어떤 인스턴스에서도 호출될 수 있고, 메서드를 상속받은 어떠한 서브클래스의 인스턴스에서도 호출할 수 있다.

메서드에 대한 상속 법칙은 속성에 대한 상속 법칙과 비슷하다. 서브클래스는 수퍼클래스로부터 클래스와 인스턴스 메서드를 상속받는다. 서브클래스는 수퍼클래스로부터 클래스의 정의나 인스턴스 메서드의 정의를 따를 수 있다.

메서드 이름에 대한 충돌 해결 규칙은 속성 이름에 대한 충돌 해결 규칙과 같다. 속성과 메서드 상속 충돌에 대한 추가적인 정보는 :ref:`class-conflict-resolution` 을 참조한다.

메서드 호출
===========

**CALL** 문은 데이터베이스에 정의된 메서드를 호출하기 위해 사용된다. 클래스 메서드, 인스턴스 메서드 모두 **CALL** 문으로 호출이 가능하다. **CALL** 문으로 시스템에 정의된 메서드를 호출하는 예는 :ref:`authorization-method` 를 참고한다. ::

    CALL <method_call> ;

    <method_call> ::=
        method_name ([<arg_value> [{, <arg_value> } ...]]) ON <call_target> [<to_variable>] |
        method_name (<call_target> [, <arg_value> [{, <arg_value>} ...]] ) [<to_variable>]

        <arg_value> ::=
            any CSQL expression

        <call_target> ::=
            an object-valued expression

        <to_variable> ::=
            INTO variable |
            TO variable

*   *method_name*\ 은 클래스에 정의된 메서드의 이름이거나, 시스템에 정의된 메서드의 이름이다. 메서드는 하나 혹은 그 이상의 인수 값을 필요로 한다. 메서드에 인수가 없으면 빈 괄호를 사용해야 한다.

*   <*call_target*>\ 은 클래스 이름, 변수, 혹은 또 다른 메서드 호출(객체를 반환하는)을 포함하는 객체 값의 표현식(object-valued expression)이다. 만약 클래스 객체에서 동작하는 클래스 메서드를 호출하려면, <*call_target*> 앞에 반드시 **CLASS** 키워드가 있어야 한다. 이 경우 클래스 이름은 클래스 메서드가 정의된 클래스의 이름이어야 한다. 만약 인스턴스 메서드를 호출하려면, 인스턴스 객체를 나타내는 식을 지정해야 한다. 클래스 메서드나 인스턴스 메서드에 의해 반환되는 값은 선택적으로 <*to_variable*>\ 에 저장할 수 있다. 이 반환 변수의 값은 <*call_target*>\ 이나 <*arg_value*> 파라미터처럼 **CALL** 문 내에 사용될 수 있다.

*   중첩된 메서드 호출은 다른 *method_call*\ 이 메서드의 <*call_target*> 이거나 <*arg_value*> 인수의 하나로 주어질 때 성립된다.
