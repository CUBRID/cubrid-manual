********
TRUNCATE
********

You can delete all records in the specified table by using the **TRUNCATE** statement.

This statement internally delete first all indexes and constraints defined in a table and then deletes all records. Therefore, it performs the job faster than using the **DELETE FROM** *table_name* statement without a **WHERE** clause.

If the **PRIMARY KEY** constraint is defined in the table and this is referred by one or more **FOREIGN KEY**, it follows the **FOREIGN KEY ACTION**. If the **ON DELETE** action of **FOREIGN KEY** is **RESTRICT** or **NO_ACTION**, the **TRUNCATE** statement returns an error. If it is **CASCADE**, it deletes **FOREIGN KEY**. The **TRUNCATE** statement initializes the **AUTO INCREMENT** column of the table. Therefore, if data is inserted, the **AUTO INCREMENT** column value increases from the initial value.

.. note:: 

    To execute the **TRUNCATE** statement, the authorization of **ALTER**, **INDEX**, and **DELETE** is required on the table. For granting authorization, see :ref:`granting-authorization`.

::

    TRUNCATE [ TABLE ] <table_name>

*   *table_name* : Specifies the name of the table that contains the data to be deleted.

.. code-block:: sql

    CREATE TABLE a_tbl(A INT AUTO_INCREMENT(3,10) PRIMARY KEY);
    INSERT INTO a_tbl VALUES (NULL),(NULL),(NULL);
    SELECT * FROM a_tbl;
    
::

                a
    =============
                3
                13
                23

.. code-block:: sql

    --AUTO_INCREMENT column value increases from the initial value after truncating the table
    TRUNCATE TABLE a_tbl;
    INSERT INTO a_tbl VALUES (NULL);
    SELECT * FROM a_tbl;
    
::

                a
    =============
                3
