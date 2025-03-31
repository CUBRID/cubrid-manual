------------------------------------
Using Default Arguments
------------------------------------

You can specify default values for the arguments of stored procedures and stored functions.

* You can specify default values using the **:=** or **DEFAULT** keywords.
* When a default value is specified, you can omit the argument when calling the stored procedure or function. The omitted argument will be replaced with the default value.
* Default values can be specified as literal values and are stored as string values up to 255 bytes. An error will occur if this size is exceeded.
* The following functions are allowed as default values in addition to literal values:

+-------------------------------+---------------+
| Default Value                 | Data Type     |
+===============================+===============+
| SYS_TIMESTAMP                 | TIMESTAMP     |
+-------------------------------+---------------+
| UNIX_TIMESTAMP()              | INTEGER       |
+-------------------------------+---------------+
| CURRENT_TIMESTAMP             | TIMESTAMP     |
+-------------------------------+---------------+
| SYS_DATETIME                  | DATETIME      |
+-------------------------------+---------------+
| CURRENT_DATETIME              | DATETIME      |
+-------------------------------+---------------+
| SYS_DATE                      | DATE          |
+-------------------------------+---------------+
| CURRENT_DATE                  | DATE          |
+-------------------------------+---------------+
| SYS_TIME                      | TIME          |
+-------------------------------+---------------+
| CURRENT_TIME                  | TIME          |
+-------------------------------+---------------+
| USER, USER()                  | STRING        |
+-------------------------------+---------------+
| TO_CHAR(date_time[, format])  | STRING        |
+-------------------------------+---------------+
| TO_CHAR(number[, format])     | STRING        |
+-------------------------------+---------------+

Here is a simple example of specifying a literal value as a default:

.. code-block:: sql

        CREATE FUNCTION default_args (
                a INT := 1, 
                b INT DEFAULT 2
        ) RETURN INT
        AS BEGIN RETURN a + b; END;

        SELECT default_args(); 
        SELECT default_args(3);
        SELECT default_args(3, 4);

::

          default_args()
        ================
                        3

          default_args(3)
        =================
                        5

          default_args(3, 4)
        ====================
                        7

Here is an example of specifying a function as a default value:

.. code-block:: sql

        CREATE FUNCTION get_age (
            birth DATE DEFAULT DATE'2000-01-01',
            today DATE DEFAULT SYS_DATE
        ) RETURN INT
        AS
        BEGIN
            RETURN YEAR(today) - YEAR(birth)
                - CASE WHEN TO_CHAR(today, 'MMDD') < TO_CHAR(birth, 'MMDD') THEN 1 ELSE 0 END;
        END;

        SELECT get_age();
        SELECT get_age(DATE'2000-05-10');
        SELECT get_age(DATE'2000-05-10', DATE'2025-03-24');

::        

        get_age()
        =============
                   25

        get_age(date '2000-05-10')
        ============================
                                  24

        get_age(date '2000-05-10', date '2025-03-24')
        ===============================================
                                                     24
