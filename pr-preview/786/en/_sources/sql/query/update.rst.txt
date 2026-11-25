
:meta-keywords: update statement, update multiple table
:meta-description: You can update the column value of a record stored in the target table or view to a new one by using the UPDATE statement.

******
UPDATE
******

You can update the column value of a record stored in the target table or view to a new one by using the **UPDATE** statement. Specify the name of the column to update and a new value in the **SET** clause, and specify the condition to be used to extract the record to be updated in the :ref:`where-clause`. You can one or more tables only with one **UPDATE** statement.

.. note:: Updating a view with **JOIN** syntax is possible from 10.0 version.

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

In case of only one table is to be updated, you can specify :ref:`order-by-clause` or :ref:`limit-clause`. You can also limit the number of records to be updated in the :ref:`limit-clause`. You can use the update with the :ref:`order-by-clause` if you want to maintain the execution order or lock order of triggers. 

.. note:: Previous versions of CUBRID 9.0 allow only one table for <*table_specifications*>.

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
