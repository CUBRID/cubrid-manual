-----------------------------
시스템 패키지
-----------------------------

CUBRID 에서는 사용자의 편의를 위해 시스템 패키지를 제공한다. 
이는 널리 사용되는 PL/SQL의 DBMS_OUTPUT 패키지와 유사한 기능을 제공하여, 기존에 다른 DBMS를 사용했던 개발자들이 쉽게 적응할 수 있도록 돕는다. 
향후 버전에서 더 많은 패키지가 추가되어 CUBRID의 기능이 확장될 예정이며 현재 버전에서는 DBMS_OUTPUT 패키지만을 제공하고 있다.

.. _dbms_output:

DBMS_OUTPUT
==============================

DBMS_OUTPUT 패키지는 문자열 메시지를 버퍼에 저장하고 읽어오기 위한 기능을 제공한다.
이 패키지는 주로 저장 프로시저나 저장 함수의 디버깅을 위해 유용하고 사용할 수 있다.

이 섹션에서는 DBMS_OUTPUT 패키지의 사용법과 활용 예시를 설명한다.
DBMS_OUTPUT 패키지의 함수는 다음과 같다:

        * :ref:`dbms-output-enable`
        * :ref:`dbms-output-disable`
        * :ref:`dbms-output-put`
        * :ref:`dbms-output-put_line`
        * :ref:`dbms-output-new_line`
        * :ref:`dbms-output-get_line`
        * :ref:`dbms-output-get_lines`

.. _dbms-output-enable:

DBMS_OUTPUT.ENABLE
----------------------

.. function:: DBMS_OUTPUT.ENABLE (size)

        DBMS_OUTPUT 패키지를 활성화하고, 메시지를 저장할 버퍼의 크기를 설정한다. 
        
        :param size: 버퍼의 크기를 지정하며, 이 값은 바이트 단위로 지정한다. 최대 크기는 32767 바이트이며 이 값을 초과하면 오류가 발생한다.

.. code-block:: sql
        
        CREATE OR REPLACE FUNCTION test() RETURN VARCHAR
        AS 
        BEGIN
                DBMS_OUTPUT.ENABLE(10000);
                DBMS_OUTPUT.PUT_LINE('Hello, World!');
        END;

::
    
    Hello, World!


.. note::

        CSQL 인터프리터에서 **;server-output on** 을 호출하면 내부적으로 DBMS_OUTPUT.ENABLE(20000)\을 호출한 것과 같다.
        자세한 내용은 :ref:`CSQL 세션명령어 server-output <server-output>`\을 참고한다.

.. _dbms-output-disable:

DBMS_OUTPUT.DISABLE
----------------------

.. function:: DBMS_OUTPUT.DISABLE ()

        현재 버퍼에 저장된 메시지를 제거하고 버퍼를 비활성화한다. 따라서 DBMS_OUTPUT 패키지 내의 다른 프로시저를 호출하더라도 아무런 출력이 나타나지 않는다. 

.. note::

        CSQL 인터프리터에서 **;server-output off** 을 호출하면 내부적으로 DBMS_OUTPUT.DISABLE()\을 호출한 것과 같다.
        자세한 내용은 :ref:`CSQL 세션명령어 server-output <server-output>`\을 참고한다.

.. _dbms-output-put:

DBMS_OUTPUT.PUT
----------------------

.. function:: DBMS_OUTPUT.PUT (str VARCHAR)

        지정된 문자열을 줄바꿈 없이 버퍼에 저장한다.

        :param str: 저장할 문자열을 지정한다. 저장할 문자열이 NULL이면 아무런 동작도 하지 않는다.

.. _dbms-output-put_line:

DBMS_OUTPUT.PUT_LINE
----------------------

.. function:: DBMS_OUTPUT.PUT_LINE (line VARCHAR)

        지정된 문자열을 버퍼에 저장하고 줄바꿈을 추가한다.

        :param line: 저장할 문자열을 지정한다. 저장할 문자열이 NULL이면 아무런 동작도 하지 않는다.

.. _dbms-output-new_line:

DBMS_OUTPUT.NEW_LINE
----------------------

.. function:: DBMS_OUTPUT.NEW_LINE ()

        버퍼에 줄바꿈 문자를 추가한다. PUT 함수로 문자열을 추가한 후 NEW_LINE 함수를 호출하여 GET_LINE에서 줄 단위로 읽어올 수 있다.

.. _dbms-output-get_line:

DBMS_OUTPUT.GET_LINE
----------------------

.. function:: DBMS_OUTPUT.GET_LINE (line OUT VARCHAR, status OUT INTEGER)

        버퍼에 저장된 문자열 메시지를 중 첫 번째 줄을 읽어온다. 읽어온 줄은 버퍼에서 삭제된다.

        :param line: 버퍼로 부터 읽어온 문자열을 저장한다.
        :param status: 문자열을 성공적으로 읽어왔을 경우 0을, 그렇지 않을 경우 1을 저장한다.

.. _dbms-output-get_lines:

DBMS_OUTPUT.GET_LINES
----------------------

.. function:: DBMS_OUTPUT.GET_LINES (lines OUT VARCHAR, num_lines IN OUT INTEGER)

        버퍼에 저장된 문자열 메시지를 지정된 줄 수만큼 읽어온다. 읽어온 줄은 버퍼에서 삭제된다.

        :param lines: 버퍼로 부터 읽어온 문자열을 저장한다.
        :param numlines: 읽어올 줄의 수를 지정한다.


활용 예시
----------------------

다음은 CSQL 인터프리터로 DBMS_OUTPUT 패키지를 사용한 단순한 예시이다.

.. code-block:: sql

        ;server-output on
        CREATE OR REPLACE FUNCTION test() RETURN VARCHAR
        AS 
        BEGIN
                DBMS_OUTPUT.ENABLE(10000);
                DBMS_OUTPUT.PUT_LINE('Hello, World!');
                DBMS_OUTPUT.PUT_LINE('Hello, CUBRID!');
                DBMS_OUTPUT.PUT_LINE('Hello, DBMS_OUTPUT!');
                RETURN 'Success';
        END;
        SELECT test();

::
        
        test ()
        =======
        'Success'

        <DBMS_OUTPUT>
        ====
        Hello world
        Hello, World!
        Hello, CUBRID!
        Hello, DBMS_OUTPUT!

