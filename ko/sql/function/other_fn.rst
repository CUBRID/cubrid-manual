
:meta-keywords: cubrid sleep, cubrid sys_guid, cubrid uuid, cubrid uuid_format

:tocdepth: 3

*********
기타 함수
*********

.. contents::

SLEEP
=====

.. function:: SLEEP ( sec )

    명시한 시간동안 멈춰있다가 수행한다.

    :param sec: sleep 시간. 단위는 초이며, double 타입 값을 입력한다.
    :rtype: INT

    .. code-block:: sql

        SELECT SLEEP(3);

    3초간 멈춰있다가 수행한다.
    
    
SYS_GUID
========

.. function:: SYS_GUID () 

    무작위로 고유한 32개 문자의 헥사 문자열(hexadecimal)을 반환한다. 
     
     
    .. code-block:: sql 
     
        SELECT SYS_GUID();

    ::

        sys_guid()
        ==================================
        '938210043A7B4455927A8697FB2571FF'


UUID
====

.. function:: UUID ( [version] )

    무작위로 생성한 128비트 UUID(Universally Unique Identifier)를 **BIT(128)** 타입으로 반환한다. *version* 인자로 생성할 UUID의 버전을 지정할 수 있으며, 생략하면 버전 4를 사용한다.

    :param version: 생성할 UUID의 버전으로 **0**, **4**, **7** 중 하나를 지정할 수 있으며, 생략하면 **4** 이다.
    :rtype: BIT(128)

    *   **UUID()**, **UUID(0)**, **UUID(4)** 는 모두 무작위(random) 기반의 버전 4 UUID를 생성한다. **UUID(0)**는 ORACLE 호환 목적으로 지원하며 DEFAULT 버전을 의미한다. :func:`SYS_GUID` 함수도 동일하게 버전 4 UUID를 생성하며, **UUID(4)** 와는 반환 타입만 다르다. 즉, **SYS_GUID** 는 32자리 16진수 **문자열(STRING)** 을 반환하고, **UUID(4)** 는 **BIT(128)** 값을 반환한다.
    *   **UUID(7)** 은 시간순으로 정렬 가능한(time-ordered) 버전 7 UUID를 생성한다. `RFC 9562 <https://www.rfc-editor.org/rfc/rfc9562>`_ 표준 형식을 따르되, 한 개의 실행기(스레드) 내에서 생성 순서에 따른 단조 증가(monotonicity)를 보장하기 위해 ``rand_a`` 영역의 앞 8비트를 카운터(counter)로 사용한다.

    .. code-block:: sql

        SELECT UUID();

    ::

        uuid()
        ======================
        X'8f2a1c4b5d6e47f8a9b0c1d2e3f4a5b6'

    .. note::

        *   버전 인자로 **NULL** 을 지정하면 에러가 발생한다. (``UUID(NULL)``)
        *   **UUID** 는 **BIT** 타입을 반환하므로 **TO_CHAR** 로 감쌀 수 없다. 하이픈(-)으로 구분된 표준 문자열 표현이 필요하면 :func:`UUID_FORMAT` 함수를 사용한다.


UUID_FORMAT
===========

.. function:: UUID_FORMAT ( uuid )

    하이픈 없이 표현된 UUID 값을 ``8-4-4-4-12`` 형식의 하이픈으로 구분된 문자열(예: ``a1b2c3d4-e5f6-a7b8-c9d0-e1f2a3b4c5d6``)로 변환하여 반환한다.

    :param uuid: 하이픈이 없는 UUID 값으로, 32자리 16진수 문자열 또는 128비트 **BIT** 값을 입력한다.
    :rtype: STRING

    .. code-block:: sql

        SELECT UUID_FORMAT(SYS_GUID());

    ::

        uuid_format(sys_guid())
        ========================================
        '93821004-3A7B-4455-927A-8697FB2571FF'

    :func:`UUID` 함수는 **BIT** 값을 반환하므로, 다음과 같이 **UUID_FORMAT** 으로 감싸 하이픈 형식의 문자열로 변환할 수 있다.

    .. code-block:: sql

        SELECT UUID_FORMAT(UUID());

    ::

        uuid_format(uuid())
        ========================================
        '8F2A1C4B-5D6E-47F8-A9B0-C1D2E3F4A5B6'
