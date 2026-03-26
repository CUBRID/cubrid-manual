
:meta-keywords: cubrid number, cubrid date time, cubrid string, cubrid character, cubrid collection, cubrid null, database literal
:meta-description: This section describes how to write a literal value in CUBRID.


******
리터럴
******

CUBRID에서 리터럴(literal) 값을 작성하는 방법을 기술한다.

숫자
====

숫자 값에는 정확한 수치(exact value)를 표기하는 방법과 근사치(approximate value)를 표기하는 방법이 있다.

*   정확한 수치는 일련의 숫자와 .으로 표현하며, 값의 범위에 따라 INT, BIGINT 또는 NUMERIC 타입으로 해석된다.

    ::
    
        10, 123456789012, 1234234324234.23

*   근사치는 일련의 숫자와 . 그리고 E(공학용 표기. 10의 승수)로 표현하며, DOUBLE 타입으로 해석된다.

    ::
    
        1.2345E15, 12345E5

*   +, -를 숫자 앞에 표기할 수 있으며, 승수를 표현하는 E 뒤에 나오는 숫자 앞에도 +, -를 표기할 수 있다.

    ::
    
        +10.2345, -1.2345E-15

.. _date-time-literal:

날짜/시간
=========

날짜와 시간을 나타내는 타입에는 DATE, TIME, DATETIME, TIMESTAMP 타입이 있으며, 문자열 앞에 date, time, datetime, timestamp 리터럴(대소문자 구분 없음)을 추가하여 이 값들을 표기한다.

날짜/시간 리터럴을 사용하면 문자열을 날짜/시간으로 변환하는 :func:`TO_DATE`, :func:`TO_TIME`, :func:`TO_DATETIME`, :func:`TO_TIMESTAMP`\와 같은 함수를 사용하지 않아도 된다.
단, 날짜와 시간을 표현하는 문자열의 순서는 반드시 지켜야 한다.

*   날짜 리터럴은 'YYYY-MM-DD' 또는 'MM/DD/YYYY'만 허용한다.

    ::
    
        date'1974-12-31', date'12-31-1974'


*   시간 리터럴은 'HH:MI:SS', 'HH:MI:SS AM', 'HH:MI:SS PM'만 허용한다.

    ::
        
        time'12:13:25', time'12:13:25 AM', time'12:13:25 PM'

*   DATETIME 타입에서 사용하는 날짜/시간 리터럴은 'YYYY-MM-DD HH:MI:SS[.msec AM|PM]' 또는 'MM/DD/YYYY HH:MI:SS[.msec AM|PM]' 형식을 허용한다. msec은 밀리 초로, 3자리 숫자까지 표기한다.

    ::
    
        datetime'1974-12-31 12:13:25.123 AM', datetime'12/31/1974 12:13:25.123 AM'

*   TIMESTAMP 타입에서 사용하는 날짜/시간 리터럴은 'YYYY-MM-DD HH:MI:SS[ AM|PM]' 또는 'MM/DD/YYYY HH:MI:SS[ AM|PM]' 형식을 허용한다.

    ::
    
        timestamp'1974-12-31 12:13:25 AM', timestamp'12/31/1974 12:13:25 AM'
        
*   타임존을 포함하는 날짜/시간 타입의 리터럴은 위에서 설명한 것과 동일한 형식을 가지며, 뒤에 타임존 정보를 나타내는 오프셋 또는 지역 이름을 추가한다. 

    *   각 타입의 값을 나타내기 위해 문자열 앞에 datetimetz, datetimeltz, timestamptz 또는 timestampltz 문자를 추가한다.

        ::
        
            datetimetz'10/15/1986 5:45:15.135 am +02:30:20';
            datetimetz'10/15/1986 5:45:15.135 am +02:30';
            datetimetz'10/15/1986 5:45:15.135 am +02';
            datetimeltz'10/15/1986 5:45:15.135 am Europe/Bucharest'
            datetimetz'2001-10-11 02:03:04 AM Europe/Bucharest EEST';
            timestampltz'10/15/1986 5:45:15 am Europe/Bucharest'
            timestamptz'10/15/1986 5:45:15 am Europe/Bucharest'
 
    *   문자열 앞에 오는 리터럴은 <날짜/시간 타입> WITH TIMEZONE 또는 <날짜/시간 타입> WITH LOCAL TIME ZONE으로 대체할 수 있다.

        ::

            DATETIME WITH TIMEZONE = datetimetz
            DATETIME WITH LOCAL TIMEZONE = datetimeltz
            TIMESTAMP WITH TIMEZONE = timestamptz
            TIMESTAMP WITH LOCAL TIMEZONE = timestampltz
    
        ::
        
            DATETIME WITH TIME ZONE'10/15/1986 5:45:15.135 am +02';
            DATETIME WITH LOCAL TIME ZONE'10/15/1986 5:45:15.135 am +02';

    .. note::
    
        *   <date/time type> WITH LOCAL TIME ZONE: 내부적으로 UTC 시간을 저장하며, 출력 시 로컬(현재의 세션) 타임존으로 변환된다.
        *   <date/time type> WITH TIME ZONE: 내부적으로 UTC 시간과 생성 시 타임존 정보(사용자가 명시하거나 세션 타임존에 의해 결정됨)를 저장한다.

비트열
======

비트열은 2진수 형식과 16진수 형식의 2가지를 사용한다.

2진수 형식은 숫자 앞에 B 또는 0b를 붙여 표기한다. B 뒤에는 0과 1로 된 문자열을, 0b 뒤에는 0과 1로 된 숫자를 입력한다. 

::

    B'10100000'
    0b10100000
    
2진수는 8자리씩 표기하며, 입력 숫자가 8자리로 나뉘지 않으면 뒤에 0이 8자리를 채울 때까지 덧붙은 값으로 저장된다. 즉, B'1'은 B'10000000'으로 저장된다.

16진수 형식은 숫자 앞에 X 또는 0x를 붙여 표기한다. X 뒤에는 16진수 문자열을, 0x 뒤에는 16진수 숫자를 입력한다.

::

    X'a0'
    0xA0

16진수는 2자리씩 표기하며, 입력 숫자가 2자리로 나뉘지 않으면 뒤에 0이 2자리를 채울 때까지 덧붙은 값으로 저장된다. 즉, X'a'는 X'a0'으로 저장된다.


문자열
======

문자열은 작은 따옴표로 감싸서 표현한다. 

*   작은 따옴표를 문자열에 포함시키고 싶으면 연속해서 두 번 입력한다.

    .. code-block:: sql
    
        SELECT 'You''re welcome.';

*   백슬래시를 이용한 이스케이프는 **cubrid.conf**\의 **no_backslash_escapes** 파라미터 값을 no로 설정했을 때만 사용할 수 있다. 기본값은 yes이다.

    보다 자세한 설명은 :ref:`escape-characters`\를 참고한다.

*   문자열 앞에 문자셋 소개자를 두고 문자열 뒤에는 COLLATE 수정자가 올 수 있다.

    보다 자세한 설명은 :ref:`charset-introducer`\를 참고한다.

컬렉션
======

컬렉션 타입에는 SET, MULTISET, LIST가 있으며, 쉼표로 구분되는 원소들을 중괄호({, })로 감싸서 표현한다.

::

    {'c','c','c','b','b','a'}

보다 자세한 설명은 :ref:`collection-data-type`\을 참고한다.

NULL
====

NULL 값은 데이터가 없다는 것을 의미한다. NULL은 대소문자를 구분하지 않아 null로도 쓰일 수 있다.
NULL 값은 숫자 0 또는 빈 문자열('')이 아니라는 점에 주의한다.
