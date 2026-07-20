
:meta-keywords: cubrid sleep, cubrid sys_guid, cubrid uuid, cubrid uuid_format

:tocdepth: 3

***************
Other functions
***************

.. contents::

SLEEP
=====

.. function:: SLEEP ( sec )

    This function pauses for the specified time then resumes the operations.

    :param sec: sleep time. The unit is second and inputs double type value.
    :rtype: INT

    .. code-block:: sql

        SELECT SLEEP(3);

    It pauses for 3 seconds.


SYS_GUID
========

.. function:: SYS_GUID () 

    It returns the unique hexadecimal string of 32 characters randomly.
     
     
    .. code-block:: sql 
     
        SELECT SYS_GUID(); 

    ::

        sys_guid()
        ==================================
        '938210043A7B4455927A8697FB2571FF'


UUID
====

.. function:: UUID ( [version] )

    It returns a randomly generated 128-bit UUID (Universally Unique Identifier) as a **BIT(128)** value. The *version* argument specifies the version of the UUID to generate; if omitted, version 4 is used.

    :param version: The version of the UUID to generate, one of **0**, **4**, or **7**. If omitted, it defaults to **4**.
    :rtype: BIT(128)

    *   **UUID()**, **UUID(0)**, and **UUID(4)** all generate a random-based version 4 UUID. The :func:`SYS_GUID` function also generates a version 4 UUID and differs from **UUID(4)** only in its return type: **SYS_GUID** returns a 32-digit hexadecimal **string (STRING)**, while **UUID(4)** returns a **BIT(128)** value.
    *   **UUID(7)** generates a time-ordered version 7 UUID. It follows the `RFC 9562 <https://www.rfc-editor.org/rfc/rfc9562>`_ standard format, except that the leading 8 bits of the ``rand_a`` field are used as a counter to guarantee monotonicity in generation order within a single executor (thread).

    .. code-block:: sql

        SELECT UUID();

    ::

        uuid()
        ======================
        X'8f2a1c4b5d6e47f8a9b0c1d2e3f4a5b6'

    .. note::

        *   Passing **NULL** as the version argument raises an error (``UUID(NULL)``).
        *   **UUID** returns a **BIT** value, so it cannot be wrapped with **TO_CHAR**. If you need the standard hyphenated string representation, use the :func:`UUID_FORMAT` function.


UUID_FORMAT
===========

.. function:: UUID_FORMAT ( uuid )

    It converts a UUID value expressed without hyphens into a hyphenated string in the ``8-4-4-4-12`` format (for example, ``a1b2c3d4-e5f6-a7b8-c9d0-e1f2a3b4c5d6``).

    :param uuid: A UUID value without hyphens, given as a 32-digit hexadecimal string or a 128-bit **BIT** value.
    :rtype: STRING

    .. code-block:: sql

        SELECT UUID_FORMAT(SYS_GUID());

    ::

        uuid_format(sys_guid())
        ========================================
        '93821004-3a7b-4455-927a-8697fb2571ff'

    Because the :func:`UUID` function returns a **BIT** value, you can wrap it with **UUID_FORMAT** to convert it into a hyphenated string, as shown below.

    .. code-block:: sql

        SELECT UUID_FORMAT(UUID());

    ::

        uuid_format(uuid())
        ========================================
        '8f2a1c4b-5d6e-47f8-a9b0-c1d2e3f4a5b6'
