
:meta-keywords: update statement, update multiple table
:meta-description: You can update the column value of a record stored in the target table or view to a new one by using the UPDATE statement.

******
UPDATE
******

You can update the column value of a record stored in the target table or view to a new one by using the **UPDATE** statement.
Specify the name of the column to update and the new value in the **SET** clause, and specify the condition to extract the record to be updated in the :ref:`where-clause`.
You can update one or more tables with a single **UPDATE** statement.

::

    <UPDATE single table>
    UPDATE [schema_name.]table_name | <remote_table_spec> | view_name SET column_name = {<expr> | DEFAULT} [, column_name = {<expr> | DEFAULT} ...]
        [WHERE <search_condition>]
        [ORDER BY {col_name | <expr>}]
        [LIMIT row_count]
     
    <remote_table_spec> ::= [schema_name.]table_name@[schema.name.]server_name [correlation>]
    <UPDATE multiple tables>
    UPDATE <table_specifications> SET column_name = {<expr> | DEFAULT} [, column_name = {<expr> | DEFAULT} ...]
        [WHERE <search_condition>]

*   <*table_specifications*>: You can specify the statement such as **FROM** clause of the **SELECT** statement and one or more tables can be specified.

*   *server_name*: Used when specifying a table of a remote server connected by dblink, not the current server.

*   *column_name*: Specifies the column name to be updated. Columns for one or more tables can be specified.

*   <*expr*> | **DEFAULT**: Specifies a new value for the column and expression or **DEFAULT** keyword can be specified as a value. The **SELECT** statement returning result record also can be specified.

*   <*search_condition*>: Update only data that meets the <*search_condition*> if conditions are specified in the :ref:`where-clause`.

*   *col_name* | <*expr*>: Specifies base column to be updated.

*   *row_count*: Specifies the number of records to be updated after the :ref:`limit-clause`. It can be one of unsigned integer, a host variable or a simple expression.

The following are allowed only when a single table is specified in <*table_specifications*>:

* The :ref:`order-by-clause` can be specified.
  If the :ref:`order-by-clause` is specified, records are updated in the order of the specified column.
  This is useful for maintaining the order of trigger execution and the order of locking.

* The :ref:`limit-clause` can be specified.
  If the :ref:`limit-clause` is specified, the number of records to be updated can be limited.

* Analytic functions can be used in the <*expr*> of the **SET** clause.
  However, if a **SELECT** query is specified in <*expr*>, analytic functions can be used in the **SELECT** query regardless of the number of tables specified in <*table_specifications*>.

.. note::

    In CUBRID versions prior to 9.0, only a single table can be specified in <*table_specifications*>.

.. note::

    From CUBRID 10.0 onward, updates to views containing **JOIN** clauses are possible.

.. _example_single_table_update_using_order_by:

.. rubric:: Example 1. Single Table Update Using ORDER BY Clause

.. code-block:: sql

    DROP TABLE IF EXISTS t1;

    CREATE TABLE t1 (id INT);
    INSERT INTO t1 VALUES (11), (1), (12), (13), (2), (3), (14), (4);

    CREATE TRIGGER trigger1 BEFORE UPDATE ON t1 IF NEW.id < 10 EXECUTE PRINT 'trigger1 executed';
    CREATE TRIGGER trigger2 BEFORE UPDATE ON t1 IF NEW.id > 10 EXECUTE PRINT 'trigger2 executed';

    UPDATE t1  SET id = id + 1;

	trigger2 executed
	trigger1 executed
	trigger2 executed
	trigger2 executed
	trigger1 executed
	trigger1 executed
	trigger2 executed
	trigger1 executed

Using the :ref:`order-by-clause` can change the order in which records are updated.

.. code-block:: sql

    TRUNCATE TABLE t1;
    INSERT INTO t1 VALUES (11), (1), (12), (13), (2), (3), (14), (4);

    UPDATE t1  SET id = id + 1 ORDER BY id;

	trigger1 executed
	trigger1 executed
	trigger1 executed
	trigger1 executed
	trigger2 executed
	trigger2 executed
	trigger2 executed
	trigger2 executed

.. _example_single_table_update_using_limit:

.. rubric:: Example 2. Single Table Update Using LIMIT Clause

This example uses the *LIMIT 3* clause to update only up to 3 records with *name IS NULL*.

.. code-block:: sql

    DROP TABLE IF EXISTS t1;

    CREATE TABLE t1 (id int, name varchar, phone varchar);
    INSERT INTO t1 VALUES (NULL,  NULL, '000-0000');
    INSERT INTO t1 VALUES (   1, 'aaa', '111-1111');
    INSERT INTO t1 VALUES (   2, 'bbb', '222-2222');
    INSERT INTO t1 VALUES (   3, 'ccc', '333-3333');
    INSERT INTO t1 VALUES (   4,  NULL, '000-0000');
    INSERT INTO t1 VALUES (   5,  NULL, '000-0000');
    INSERT INTO t1 VALUES (   6, 'ddd', '000-0000');
    INSERT INTO t1 VALUES (   7,  NULL, '777-7777');

    SELECT * FROM t1 WHERE name IS NULL;

	           id  name                  phone
	=========================================================
	         NULL  NULL                  '000-0000'
	            4  NULL                  '000-0000'
	            5  NULL                  '000-0000'
	            7  NULL                  '777-7777'

    UPDATE t1 SET name='update', phone='999-9999' WHERE name IS NULL LIMIT 3;

    SELECT * FROM t1;

	           id  name                  phone
	=========================================================
	         NULL  'update'              '999-9999'
	            1  'aaa'                 '111-1111'
	            2  'bbb'                 '222-2222'
	            3  'ccc'                 '333-3333'
	            4  'update'              '999-9999'
	            5  'update'              '999-9999'
	            6  'ddd'                 '000-0000'
	            7  NULL                  '777-7777'

.. _example_update_with_joins:

.. rubric:: Example 3. Update Using Joins

When a record in table **A** is joined with multiple records in table **B** in an **UPDATE** statement,
the record in **A** is updated using only the value from the first matching record in **B**.

In this example, a record in table *t1* with an *id* of 3 is joined with two records in table *t2* where *rate_id* is 3.
However, the *charge* column in the *t1* table is updated using only the *rate* value from the first matching record in the *t2* table.

.. code-block:: sql

    DROP TABLE IF EXISTS t1, t2;

    CREATE TABLE t1 (id INT PRIMARY KEY, charge DOUBLE);
    INSERT INTO t1 VALUES (1, 100.0);
    INSERT INTO t1 VALUES (2, 100.0);
    INSERT INTO t1 VALUES (3, 100.0);

    CREATE TABLE t2 (rate_id INT, rate DOUBLE);
    INSERT INTO t2 VALUES (1, 0.1);
    INSERT INTO t2 VALUES (2, 0.2);
    INSERT INTO t2 VALUES (3, 0.3);
    INSERT INTO t2 VALUES (3, 0.5);
    
    UPDATE t1 a INNER JOIN t2 b ON a.id = b.rate_id
    SET a.charge = a.charge * (1 + b.rate);

    SELECT id, TO_CHAR (charge) AS charge FROM t1;

	           id  charge
	===================================
	            1  '110'
	            2  '120'
	            3  '150'

For details, see :ref:`join-query`.

.. _example_view_update:

.. rubric:: Example 4. View Update

.. code-block:: sql

    DROP TABLE IF EXISTS t1, t2;
    DROP VIEW v1;

    CREATE TABLE t1 (id INT, val INT) DONT_REUSE_OID;
    INSERT INTO t1 VALUES (1, 1);
    INSERT INTO t1 VALUES (2, 2);
    INSERT INTO t1 VALUES (3, 3);
    INSERT INTO t1 VALUES (4, 4);
    INSERT INTO t1 VALUES (5, 5);

    CREATE TABLE t2 (id INT, val INT) DONT_REUSE_OID;
    INSERT INTO t2 VALUES (1, 1);
    INSERT INTO t2 VALUES (2, 2);
    INSERT INTO t2 VALUES (3, 3);
    INSERT INTO t2 VALUES (4, 4);
    INSERT INTO t2 VALUES (6, 6);

    CREATE VIEW v1 AS
      SELECT b.id, b.val FROM t2 b LEFT JOIN t1 a ON b.id = a.id WHERE b.id <= 3;

    UPDATE v1 SET val = -1;

    SELECT * from v1;

	           id          val
	==========================
	            1           -1
	            2           -1
	            3           -1

    SELECT * from t2;

	           id          val
	==========================
	            1           -1
	            2           -1
	            3           -1
	            4            4
	            6            6

.. warning::
  
    Records in views that include tables with REUSE OID cannot be updated.

    For details, see :ref:`reuse-oid` and :ref:`dont-reuse-oid`.

.. _example_update_using_update_use_attribute_references:

.. rubric:: Example 5. Update Using the update_use_attribute_references Parameter

The result of this example depends on the value of the :ref:`update_use_attribute_references <update_use_attribute_references>` parameter.

*   If the value of this parameter is yes, *c2* is updated to 10, influenced by c1 = 10.
*   If the value of this parameter is no, *c2* is not influenced by *c1 = 10* and is updated to 1, based on the value of c1 stored in the record.

.. code-block:: sql 

    DROP TABLE IF EXISTS t1;

    CREATE TABLE t1 (c1 INT, c2 INT); 
    INSERT INTO t1 values (1, NULL);

    SET SYSTEM PARAMETERS 'update_use_attribute_references=yes';

    UPDATE t1 SET c1 = 10, c2 = c1;

    SELECT * FROM t1;

	           c1           c2
	==========================
	           10           10

.. code-block:: sql

    TRUNCATE TABLE t1;
    INSERT INTO t1 values (1, NULL);

    SET SYSTEM PARAMETERS 'update_use_attribute_references=no';

    UPDATE t1 SET c1 = 10, c2 = c1;

    SELECT * FROM t1;

	           c1           c2
	==========================
	           10            1

.. _example_single_table_update_using_analytic_functions:

.. rubric:: Example 6. Single Table Update Using Analytic Functions

When only one table is specified in the **UPDATE** statement, analytic functions can be used in the **SET** clause.

.. code-block:: sql

    DROP TABLE IF EXISTS t1, t2;

    CREATE TABLE t1 (id INT);
    INSERT INTO t1 VALUES (1), (2), (3), (4), (5);

    CREATE TABLE t2 (id INT, val INT, update_val DOUBLE, join_update_val DOUBLE);
    INSERT INTO t2 (id, val) SELECT a.id, b.id FROM t1 a, t1 b WHERE b.id <= a.id;

    UPDATE t2 SET update_val = AVG (val) OVER (PARTITION BY id);

    SELECT DISTINCT id, TO_CHAR (update_val) AS update_val FROM t2;

	           id  update_val
	===================================
	            1  '1'
	            2  '1.5'
	            3  '2'
	            4  '2.5'
	            5  '3'

.. _example_multiple_tables_update_using_analytic_functions:

.. rubric:: Example 7. Multiple Tables Update Using Analytic Functions

This example continues from :ref:`example_single_table_update_using_analytic_functions`.

When multiple tables are specified in the **UPDATE** statement, analytic functions cannot be used in the **SET** clause.
However, if a **SELECT** query is specified in the **SET** clause, analytic functions can be used within that **SELECT** query, regardless of the number of tables specified.

.. code-block:: sql

    UPDATE t1 a, t2 b SET b.join_update_val = AVG (b.val) OVER (PARTITION BY b.id) WHERE a.id = b.id;

	ERROR: before '  where a.id = b.id; '
	Nested analytic functions are not allowed.

.. code-block:: sql

    UPDATE t2 c SET c.join_update_val = (SELECT AVG (b.val) OVER (PARTITION BY b.id) FROM t1 a, t2 b WHERE a.id = b.id AND a.id = c.id LIMIT 1);

    SELECT DISTINCT id, TO_CHAR (join_update_val) AS join_update_val FROM t2;

	           id  join_update_val
	===================================
	            1  '1'
	            2  '1.5'
	            3  '2'
	            4  '2.5'
	            5  '3'

.. _example_remote_table_update:

.. rubric:: Example 8. Remote Table Update

By using table extension names, it is possible to update tables not only on the local server but also on remote servers.

.. code-block:: sql

    /**
     * on the remote server (e.g., 192.168.6.21)
     */

    -- DROP SERVER origin;
    CREATE SERVER origin (HOST='localhost', PORT=33000, DBNAME=demodb, USER=public);

    DROP TABLE IF EXISTS t1;

    CREATE TABLE t1 (id int, name varchar, phone varchar);
    INSERT INTO t1@origin VALUES (NULL,  NULL, '000-0000');
    INSERT INTO t1@origin VALUES (   1, 'aaa', '111-1111');
    INSERT INTO t1@origin VALUES (   2, 'bbb', '222-2222');
    INSERT INTO t1@origin VALUES (   3, 'ccc', '333-3333');
    INSERT INTO t1@origin VALUES (   4,  NULL, '000-0000');
    INSERT INTO t1@origin VALUES (   5,  NULL, '000-0000');
    INSERT INTO t1@origin VALUES (   6, 'ddd', '000-0000');
    INSERT INTO t1@origin VALUES (   7,  NULL, '777-7777');

    SELECT * FROM t1;

	           id  name                  phone
	=========================================================
	         NULL  NULL                  '000-0000'
	            1  'aaa'                 '111-1111'
	            2  'bbb'                 '222-2222'
	            3  'ccc'                 '333-3333'
	            4  NULL                  '000-0000'
	            5  NULL                  '000-0000'
	            6  'ddd'                 '000-0000'
	            7  NULL                  '777-7777'

.. code-block:: sql

    /**
     * on the local server (e.g., 192.168.6.22)
     */

    -- DROP SERVER remote;
    CREATE SERVER remote (HOST='192.168.6.21', PORT=33000, DBNAME=demodb, USER=public);

    SELECT * FROM t1@remote WHERE name IS NULL;

	           id  name                  phone
	=========================================================
	         NULL  NULL                  '000-0000'
	            4  NULL                  '000-0000'
	            5  NULL                  '000-0000'
	            7  NULL                  '777-7777'

    UPDATE t1@remote SET name='update', phone='999-9999' WHERE name IS NULL LIMIT 3;

    SELECT * FROM t1@remote;

	           id  name                  phone
	=========================================================
	         NULL  'update'              '999-9999'
	            1  'aaa'                 '111-1111'
	            2  'bbb'                 '222-2222'
	            3  'ccc'                 '333-3333'
	            4  'update'              '999-9999'
	            5  'update'              '999-9999'
	            6  'ddd'                 '000-0000'
	            7  NULL                  '777-7777'

.. _example_local_table_update_using_joins_with_remote_tables:

.. rubric:: Example 9. Local Table Update Using Joins with Remote Tables

.. code-block:: sql

    /**
     * on the remote server (e.g., 192.168.6.21)
     */

    DROP TABLE IF EXISTS t2;

    CREATE TABLE t2 (rate_id INT, rate DOUBLE);

.. code-block:: sql

    /**
     * on the local server (e.g., 192.168.6.22)
     */

    -- DROP SERVER remote;
    CREATE SERVER remote (HOST='192.168.6.21', PORT=33000, DBNAME=demodb, USER=public);

    DROP TABLE IF EXISTS t1;

    CREATE TABLE t1 (id INT PRIMARY KEY, charge DOUBLE);
    INSERT INTO t1 VALUES (1, 100.0);
    INSERT INTO t1 VALUES (2, 100.0);
    INSERT INTO t1 VALUES (3, 100.0);

    INSERT INTO t2@remote VALUES (1, 0.1);
    INSERT INTO t2@remote VALUES (2, 0.2);
    INSERT INTO t2@remote VALUES (3, 0.3);
    INSERT INTO t2@remote VALUES (3, 0.5);

    UPDATE t1 a INNER JOIN t2@remote b ON a.id = b.rate_id
    SET a.charge = a.charge * (1 + b.rate);

    SELECT id, TO_CHAR (charge) AS charge FROM t1;

	           id  charge
	===================================
	            1  '110'
	            2  '120'
	            3  '150'

.. warning::

    In **UPDATE** statements that join local and remote tables, records in the remote table cannot be updated.

    .. code-block:: sql
    
	UPDATE t1 a INNER JOIN t2@remote b ON a.id = b.rate_id
	SET b.rate = b.rate + 0.1;

	    ERROR: before '  a INNER JOIN t2@remote b ON a.id = b.rate_id
	    SET b.rate = b....'
	    dblink: local mixed remote DML is not allowed
