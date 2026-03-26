
:meta-keywords: cubrid incr, cubrid decr
:meta-description: The **INCR** and **DECR** functions are called "click counters" and can be effectively used to quickly update a counter.

:tocdepth: 3

***********************
Click Counter Functions
***********************

.. contents::

INCR, DECR
==========

.. function:: INCR (column_name)
.. function:: DECR (column_name)

    The **INCR** function increases the column's value given as a parameter of the **SELECT** statement by 1. The **DECR** function decreases the value of the column by 1.

    :param column: the name of column defined with SMALLINT, INT or BIGINT type
    :rtype: SMALLINT, INT or BIGINT 
    
The **INCR** and **DECR** functions are called "click counters" and can be effectively used to increase the number of post views for a Bulletin Board System (BBS) type of web service. In a scenario where you want to **SELECT** a post and immediately increase the number of views by 1 using an **UPDATE** statement, you can view the post and increment the number at once by using the **INCR** function in a single **SELECT** statement.

The **INCR** function increments the column value specified as an argument. Only integer type numbers can be used as arguments. If the value is **NULL**, the **INCR** function returns the **NULL**. That is, a value must be valid in order to be incremented by the **INCR** function. The **DECR** function decrements the column value specified as a parameter.

If an **INCR** function is specified in the **SELECT** statement, the **COUNTER** value is incremented by 1 and the query result is displayed with the values before the increment. Furthermore, the **INCR** function does not increment the value of the row(tuple) affected by the query process but rather the one affected by the final result.

If you want to increase or decrease the click counter without specifying **INCR** or **DECR** on the **SELECT** list, specify **WITH INCREMENT FOR** *column* or **WITH INCREMENT FOR** *column* after the WHERE clause. 

.. code-block:: sql

    CREATE TABLE board (id INT, cnt INT, content VARCHAR(8096));
    SELECT content FROM board WHERE id=1 WITH INCREMENT FOR cnt;

.. note::

    *   The **INCR/DECR** functions execute independent of user-defined transactions and is applied automatically to the database by the top operation internally used in the system, apart from the transaction's **COMMIT/ROLLBACK**.
    
    *   When multiple **INCR/DECR** functions are specified in a single **SELECT** statement, the failure of any of the **INCR/DECR** functions leads to the failure of all of them.

    *   The **INCR/DECR** functions apply only to top-level **SELECT** statements. **SUB** **SELECT** statements such as **INSERT** ... **SELECT** ... statement and **UPDATE** table **SET** col = **SELECT** ... statement are not supported. The following example shows where the **INCR** function is not allowed.

        .. code-block:: sql
    
            SELECT b.content, INCR(b.read_count) FROM (SELECT * FROM board WHERE id = 1) AS b

    *   If the **SELECT** statement with **INCR/DECR** functions returns more than one row as a result, it is treated as an error. The final result where only one row exists is valid.

    *   The **INCR/DECR** function can be used only in numerical type. Applicable domains are limited to integer data types such as **SMALLINT**, **INTEGER** and **BIGINT**. They cannot be used in other types.

    *   When the **INCR** function is called, the value to be returned will be the current value, while the value to be stored will be the current value + 1. Execute the following statement to select the value to be stored as a result :

        .. code-block:: sql
    
            SELECT content, INCR(read_count) + 1 FROM board WHERE id = 1;

    *   If the defined maximum value of the type is exceeded, the **INCR** function initializes the column value to 0. Likewise, the column value is also initialized to 0 when the **DECR** function applies to the minimum value. 

    *   Data inconsistency can occur because the **INCR/DECR** functions are executed regardless of **UPDATE** trigger. The following example shows the database inconsistency in that situation.

        .. code-block:: sql

            CREATE TRIGGER event_tr BEFORE UPDATE ON event EXECUTE REJECT;
            SELECT INCR(players) FROM event WHERE gender='M';

    *   The **INCR** / **DECR** functions returns an error in the write-protected broker mode such as slave mode of HA configuration, CSQL Interpreter (csql -r) of read-only, Read Only or Standby Only mode(ACCESS_MODE=RO or SO in cubrid_broker.conf).

**Example**

Suppose that the following three rows of data are inserted into the 'board' table.

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

The following example shows how to increment the value of the 'read_count' column in data whose 'id' value is 1 by using the **INCR** function.

.. code-block:: sql

    SELECT content, INCR(read_count) FROM board WHERE id = 1;

::

      content                read_count
    ===================================
      'text...'                       0

In the example, the column value becomes read_count + 1 as a result of the **INCR** function in the **SELECT** statement. You can check the result using the following **SELECT** statement.

.. code-block:: sql

    SELECT content, read_count FROM board WHERE id = 1;
    
::

      content                read_count
    ===================================
      'text...'                       1
