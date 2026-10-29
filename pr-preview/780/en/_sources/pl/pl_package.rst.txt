-----------------------------
System Package
-----------------------------

CUBRID provides system packages for user convenience. 
More packages will be added in future versions to expand CUBRID's functionality, but currently, only the DBMS_OUTPUT package is available.

.. _dbms_output:

DBMS_OUTPUT
==============================

The DBMS_OUTPUT package provides functionality to store and read string messages in the DBMS_OUTPUT buffer. 
Developers of stored procedures/functions can use the PUT_LINE or PUT functions of this package to accumulate relevant messages in the DBMS_OUTPUT buffer.
Client tools like CSQL or DBeaver use the ENABLE, DISABLE, GET_LINE, and GET_LINES functions of this package 
to activate/deactivate the message storage feature and retrieve messages accumulated in the buffer. 
Developers can effectively use these messages for monitoring program progress or debugging.

This section explains the usage and examples of the DBMS_OUTPUT package.
The functions of the DBMS_OUTPUT package are as follows:

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

        Enable the DBMS_OUTPUT message buffer and sets the size of the buffer to store messages.

        :param size: Specifies the size of the buffer in bytes. The maximum size is 32767 bytes, and exceeding this value will result in an error.

.. note::

        Calling **;server-output on** in the CSQL interpreter is internally equivalent to calling the default DBMS_OUTPUT.ENABLE(20000).
        For more details, refer to :ref:`CSQL session command server-output <server-output>`.

.. _dbms-output-disable:

DBMS_OUTPUT.DISABLE
----------------------

.. function:: DBMS_OUTPUT.DISABLE ()

        Clears the messages stored in the current buffer and deactivates it.
        Consequently, no output will appear even if other procedures in the DBMS_OUTPUT package are called.

.. note::

        Calling **;server-output off** in the CSQL interpreter is internally equivalent to calling DBMS_OUTPUT.DISABLE(). 
        For more details, refer to :ref:`CSQL session command server-output <server-output>`.

.. _dbms-output-put:

DBMS_OUTPUT.PUT
----------------------

.. function:: DBMS_OUTPUT.PUT (str VARCHAR)

        Stores the specified string in the buffer without a newline.

        :param str: Specifies the string to store. If the string to store is NULL, no action is taken.

.. _dbms-output-put_line:

DBMS_OUTPUT.PUT_LINE
----------------------

.. function:: DBMS_OUTPUT.PUT_LINE (line VARCHAR)

        Stores the specified string in the buffer and adds a newline.

        :param line: Specifies the string to store. If the string to store is NULL, no action is taken.

.. _dbms-output-new_line:

DBMS_OUTPUT.NEW_LINE
----------------------

.. function:: DBMS_OUTPUT.NEW_LINE ()

        Adds a newline character to the buffer. After adding a string with the PUT function, call the NEW_LINE function to read it line by line with GET_LINE.

.. _dbms-output-get_line:

DBMS_OUTPUT.GET_LINE
----------------------

.. function:: DBMS_OUTPUT.GET_LINE (line OUT VARCHAR, status OUT INTEGER)

        Reads the first line of the string message stored in the buffer. The read line is then deleted from the buffer.

        :param line: Stores the string read from the buffer.
        :param status: Stores 0 if the string is successfully read, otherwise stores 1.

.. _dbms-output-get_lines:

DBMS_OUTPUT.GET_LINES
----------------------

.. function:: DBMS_OUTPUT.GET_LINES (lines OUT VARCHAR, num_lines IN OUT INTEGER)

        Reads the specified number of lines of string messages stored in the buffer. The read lines are deleted from the buffer.

        :param lines: Stores the string read from the buffer.
        :param numlines: Specifies the number of lines to read.


Usage Example
----------------------

The following is a simple example of using the DBMS_OUTPUT package with the CSQL interpreter.
The PUT_LINE function is used by the stored function developer, and the ENABLE, DISABLE, GET_LINE functions are used internally by the CSQL interpreter for functionality implementation.

.. code-block:: sql

        ;server-output on   -- CSQL internally calls DBMS_OUTPUT.ENABLE

        CREATE OR REPLACE FUNCTION test() RETURN VARCHAR
        AS
        BEGIN
                DBMS_OUTPUT.PUT_LINE('Hello, World!');
                DBMS_OUTPUT.PUT_LINE('Hello, CUBRID!');
                DBMS_OUTPUT.PUT_LINE('Hello, DBMS_OUTPUT!');
                RETURN 'Success';
        END;

        SELECT test();

        ;server-output off  -- CSQL internally calls DBMS_OUTPUT.DISABLE
        
::

        test ()
        =======
        'Success'

        <DBMS_OUTPUT>       <-- CSQL internally calls DBMS_OUTPUT.GET_LINE multiple times to retrieve messages for output
        ====
        Hello, World!
        Hello, CUBRID!
        Hello, DBMS_OUTPUT!
