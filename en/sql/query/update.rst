
:meta-keywords: update statement, update multiple table
:meta-description: You can update the column value of a record stored in the target table or view to a new one by using the UPDATE statement.

******
UPDATE
******

You can update the column value of a record stored in the target table or view to a new one by using the **UPDATE** statement.
Specify the name of the column to update and a new value in the **SET** clause, and specify the condition to be used to extract the record to be updated in the :ref:`where-clause`.
You can one or more tables only with one **UPDATE** statement.

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

The following is allowed only when a single table is specified in <*table_specifications*>:

* :ref:`order-by-clause` can be specified.
  If :ref:`order-by-clause` is specified, records are updated in the order of the specified column.
  This is useful for maintaining the order of trigger execution and the order of locking.

* :ref:`limit-clause` can be specified.
  If :ref:`limit-clause` is specified, the number of records to be updated can be limited.

* Analytic functions can be used in the <*expr*> of the **SET** clause.
  However, if a **SELECT** query is specified in <*expr*>, analytic functions can be used in the **SELECT** query regardless of the number of tables specified in <*table_specifications*>.

.. note::

    In CUBRID versions prior to 9.0, only a single table can be specified in <*table_specifications*>.

.. note::

    From CUBRID 10.0 onward, updates to views containing **JOIN** clauses are possible.

The following example shows how to update one table.

.. code-block:: sql

    --creating a new table having all records copied from a_tbl1
    CREATE TABLE a_tbl5 AS SELECT * FROM a_tbl1;
    SELECT * FROM a_tbl5 WHERE name IS NULL;

::
    
               id  name                  phone
    =========================================================
             NULL  NULL                  '000-0000'
                4  NULL                  '000-0000'
                5  NULL                  '000-0000'
                7  NULL                  '777-7777'
     
.. code-block:: sql

    UPDATE a_tbl5 SET name='yyy', phone='999-9999' WHERE name IS NULL LIMIT 3;
    SELECT * FROM a_tbl5;
     
::

               id  name                  phone
    =========================================================
             NULL  'yyy'                 '999-9999'
                1  'aaa'                 '000-0000'
                2  'bbb'                 '000-0000'
                3  'ccc'                 '333-3333'
                4  'yyy'                 '999-9999'
                5  'yyy'                 '999-9999'
                6  'eee'                 '000-0000'
                7  NULL                  '777-7777'
     
.. code-block:: sql

    -- using triggers, that the order in which the rows are updated is modified by the ORDER BY clause.
     
    CREATE TABLE t (i INT,d INT);
    CREATE TRIGGER trigger1 BEFORE UPDATE ON t IF new.i < 10 EXECUTE PRINT 'trigger1 executed';
    CREATE TRIGGER trigger2 BEFORE UPDATE ON t IF new.i > 10 EXECUTE PRINT 'trigger2 executed';
    INSERT INTO t VALUES (15,1),(8,0),(11,2),(16,1), (6,0),(1311,3),(3,0);
    UPDATE t  SET i = i + 1 WHERE 1 = 1;
     
::

    trigger2 executed
    trigger1 executed
    trigger2 executed
    trigger2 executed
    trigger1 executed
    trigger2 executed
    trigger1 executed
     
.. code-block:: sql

    TRUNCATE TABLE t;
    INSERT INTO t VALUES (15,1),(8,0),(11,2),(16,1), (6,0),(1311,3),(3,0);
    UPDATE t SET i = i + 1 WHERE 1 = 1  ORDER BY i;
     
::

    trigger1 executed
    trigger1 executed
    trigger1 executed
    trigger2 executed
    trigger2 executed
    trigger2 executed
    trigger2 executed

The following example shows how to update multiple tables after joining them.

.. code-block:: sql

    CREATE TABLE a_tbl(id INT PRIMARY KEY, charge DOUBLE);
    CREATE TABLE b_tbl(rate_id INT, rate DOUBLE);
    INSERT INTO a_tbl VALUES (1, 100.0), (2, 1000.0), (3, 10000.0);
    INSERT INTO b_tbl VALUES (1, 0.1), (2, 0.0), (3, 0.2), (3, 0.5);
    
    UPDATE
     a_tbl INNER JOIN b_tbl ON a_tbl.id=b_tbl.rate_id
    SET
      a_tbl.charge = a_tbl.charge * (1 + b_tbl.rate)
    WHERE a_tbl.charge > 900.0;

For *a_tbl* table and *b_tbl* table, which join the **UPDATE** statement, when the number of rows of *a_tbl* which joins one row of *b_tbl* is two or more and the column to be updated is included in *a_tbl*, update is executed by using the value of the row detected first among the rows of *b_tbl*.

In the above example, when the number of rows with *id* = 5, the **JOIN** condition column, is one in *a_tbl* and two in *b_tbl*, *a_tbl.charge*, the update target column in the row with *a_tbl.id* = 5, uses the value of *rate* of the first row in *b_tbl* only.

For more details on join syntax, see :ref:`join-query`.

The following shows to update a view.

.. code-block:: sql 

    CREATE TABLE tbl1(a INT, b INT); 
    CREATE TABLE tbl2(a INT, b INT); 
    INSERT INTO tbl1 VALUES (5,5),(4,4),(3,3),(2,2),(1,1); 
    INSERT INTO tbl2 VALUES (6,6),(4,4),(3,3),(2,2),(1,1); 
    CREATE VIEW vw AS SELECT tbl2.* FROM tbl2 LEFT JOIN tbl1 ON tbl2.a=tbl1.a WHERE tbl2.a<=3; 

    UPDATE vw SET a=1000; 

The below result for an UPDATE statement depends on the value of the  :ref:`update_use_attribute_references <update_use_attribute_references>` parameter.
      
.. code-block:: sql 

    CREATE TABLE tbl(a INT, b INT); 
    INSERT INTO tbl values (10, NULL); 

    UPDATE tbl SET a=1, b=a; 
      
If the value of this parameter is yes, the updated value of "b" from the above UPDATE query will be 1 as being affected by "a=1".

.. code-block:: sql 
  
    SELECT * FROM tbl; 

:: 
  
    1, 1 
      
If the value of this parameter is no, the updated value of "b" from the above UPDATE query will be NULL as being affected by the value of "a" which is stored at this record, not by "a=1".

.. code-block:: sql 
  
    SELECT * FROM tbl; 
      
:: 
  
    1, NULL

The following is an example of using analytic functions in the **SET** clause when a single table or multiple tables are specified in the **UPDATE** statement

.. code-block:: sql

    DROP TABLE IF EXISTS a_tbl, b_tbl;

    CREATE TABLE a_tbl (id INT);
    INSERT INTO a_tbl VALUES (1), (2), (3), (4), (5);

    CREATE TABLE b_tbl (id INT, val INT, update_val DOUBLE, join_update_val DOUBLE);
    INSERT INTO b_tbl (id, val) SELECT a.id, b.id FROM a_tbl a, a_tbl b WHERE b.id <= a.id;

Analytic functions can be used in the **SET** clause when a single table is specified.

.. code-block:: sql

    -- update using analytic functions when a single table is specified
    UPDATE b_tbl SET update_val = AVG (val) OVER (PARTITION BY id);

    SELECT DISTINCT id, TO_CHAR (update_val) AS update_val FROM b_tbl;

	           id  update_val
	===================================
	            1  '1'
	            2  '1.5'
	            3  '2'
	            4  '2.5'
	            5  '3'

Analytic functions cannot be used in the **SET** clause when multiple tables are specified.

.. code-block:: sql

    -- update using analytic functions when multiple tables are specified
    UPDATE a_tbl a, b_tbl b SET b.join_update_val = AVG (b.val) OVER (PARTITION BY b.id) WHERE a.id = b.id;

	ERROR: before '  where a.id = b.id; '
	Nested analytic functions are not allowed.

However, if a **SELECT** query is specified in the **SET** clause, analytic functions can be used in the **SELECT** query regardless of the number of tables specified.

.. code-block:: sql

    -- update using analytic functions in subqueries
    UPDATE b_tbl c SET c.join_update_val = (SELECT AVG (b.val) OVER (PARTITION BY b.id) FROM a_tbl a, b_tbl b WHERE a.id = b.id AND a.id = c.id LIMIT 1);

    SELECT DISTINCT id, TO_CHAR (join_update_val) AS join_update_val FROM b_tbl;

	           id  join_update_val
	===================================
	            1  '1'
	            2  '1.5'
	            3  '2'
	            4  '2.5'
	            5  '3'

Table extensions can be used to perform updates on tables on the remote server as well as on the local server. The following is an example of updating a remote table.

.. code-block:: sql

    --at remote srv1
    --creating a new table having all records copied from a_tbl1
    --origin is a local server
    CREATE TABLE a_tbl5 AS SELECT * FROM a_tbl1@origin;

    --at local
    SELECT * FROM a_tbl5@srv1 WHERE name IS NULL;
               id  name                  phone
    =========================================================
             NULL  NULL                  '000-0000'
                4  NULL                  '000-0000'
                5  NULL                  '000-0000'
                7  NULL                  '777-7777'

    --at local
    UPDATE a_tbl5@srv1 SET name='yyy', phone='999-9999' WHERE name IS NULL LIMIT 3;
    SELECT * FROM a_tbl5@srv1;
               id  name                  phone
    =========================================================
             NULL  'yyy'                 '999-9999'
                1  'aaa'                 '000-0000'
                2  'bbb'                 '000-0000'
                3  'ccc'                 '333-3333'
                4  'yyy'                 '999-9999'
                5  'yyy'                 '999-9999'
                6  'eee'                 '000-0000'

The following is an example of performing an update after joining multiple tables, including remote tables.

.. code-block:: sql

    --at remote srv1
    --creating a table b_tbl
    CREATE TABLE b_tbl(rate_id INT, rate DOUBLE);
    --at local
    INSERT INTO a_tbl VALUES (1, 100.0), (2, 1000.0), (3, 10000.0);
    INSERT INTO b_tbl@srv1 VALUES (1, 0.1), (2, 0.0), (3, 0.2), (3, 0.5);
    UPDATE
     a_tbl INNER JOIN b_tbl@srv1 b_tbl ON a_tbl.id=b_tbl.rate_id
    SET
      a_tbl.charge = a_tbl.charge * (1 + b_tbl.rate)
    WHERE a_tbl.charge > 900.0;

.. warning::

    As shown below, UPDATE ... JOIN queries that include local and remote tables and update the remote table are not allowed.

.. code-block:: sql

    UPDATE
     a_tbl INNER JOIN b_tbl@srv1 b_tbl ON a_tbl.id=b_tbl.rate_id
    SET
      b_tbl.charge = a_tbl.charge * (1 + b_tbl.rate)
    WHERE a_tbl.charge > 900.0;
