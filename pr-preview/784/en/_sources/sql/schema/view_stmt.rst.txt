
:meta-keywords: view definition, create view, drop view, alter view, rename view, create view with check, create updatable view
:meta-description: Define views in CUBRID database using create view, alter view, drop view and rename view statements.

**************************
VIEW DEFINITION STATEMENTS
**************************

CREATE VIEW
===========

A view is a virtual table that does not exist physically. You can create a view by using an existing table or a query. **VIEW** and **VCLASS** are used interchangeably.

Use **CREATE VIEW** statement to create a view. A view with the same name as an existing table or synonym cannot be created. Regarding writing view name, see :doc:`/sql/identifier`. 

::

    CREATE [OR REPLACE] {VIEW | VCLASS} [schema_name.]view_name
    [<subclass_definition>]
    [(view_column_name [COMMENT 'column_comment_string'], ...)]
    [INHERIT <resolution>, ...]
    [AS <select_statement>]
    [WITH CHECK OPTION] 
    [COMMENT [=] 'view_comment_string'];
                                    
        <subclass_definition> ::= {UNDER | AS SUBCLASS OF} [schema_name.]superclass_name, ...
        <resolution> ::= [CLASS | TABLE] {column_name} OF [schema_name.]superclass_name [AS alias]

*   **OR REPLACE**: If the keyword **OR REPLACE** is specified after **CREATE**, the existing view is replaced by a new one without displaying any error message, even when the *view_name* overlaps with the existing view name.

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *view_name*: specifies the name of a view to be created. It must be unique in a database.
*   *view_column_name*: defines the column of a view.
*   **AS** <*select_statement*>: A valid **SELECT** statement must be specified. A view is created based on this.
*   **WITH CHECK OPTION**: If this option is specified, the update or insert operation is possible only when the condition specified in the **WHERE** clause of the <*select_statement*> is satisfied. Therefore, this option is used to disallow the update of a virtual table that violates the condition.
*   *view_comment_string*: specifies a view's comment.
*   *column_comment_string*: specifies a column's comment.

.. code-block:: sql

    CREATE TABLE a_tbl (
        id INT NOT NULL,
        phone VARCHAR(10)
    );
    INSERT INTO a_tbl VALUES(1,'111-1111'), (2,'222-2222'), (3, '333-3333'), (4, NULL), (5, NULL);
    
    --creating a new view based on AS select_statement from a_tbl
    CREATE VIEW b_view AS SELECT * FROM a_tbl WHERE phone IS NOT NULL WITH CHECK OPTION;
    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
     
.. code-block:: sql

    --WITH CHECK OPTION doesn't allow updating column value which violates WHERE clause
    UPDATE b_view SET phone=NULL;
     
::

    ERROR: Check option exception on view b_view.

The below updates the old view's definition, In addition, this adds a view's comment.

.. code-block:: sql

    --creating view which name is as same as existing view name
    CREATE OR REPLACE VIEW b_view AS SELECT * FROM a_tbl ORDER BY id DESC COMMENT 'changed view';
     
    --the existing view has been replaced as a new view by OR REPLACE keyword
    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                5  NULL
                4  NULL
                3  '333-3333'
                2  '222-2222'
                1  '111-1111'

The below adds a comment to a view's columns.

.. code-block:: sql

    CREATE OR REPLACE VIEW b_view(a COMMENT 'column id', b COMMENT 'column phone') AS SELECT * FROM a_tbl ORDER BY id DESC;

Condition for Creating Updatable VIEW
-------------------------------------

A virtual table is updatable if it satisfies the following conditions:

*   The **FROM** clause must include the updatable table or view only.

    In versions lower than CUBRID 9.0, only one updatable table can be included to the **FROM** clause it requires. However, two tables in parentheses like FROM (class_x, class_y) can be updated since the two were expressed as one table. In version of CUBRID 9.0 or higher, more than one updatable table is allowed. The **FROM** clause must include only one table or updatable view. However, two tables included in parentheses as in **FROM** (class_x, class_y) can be updated because they represent one table.

*   A **JOIN** syntax can be included.

    .. note:: In versions lower than CUBRID 10.0, you cannot update a view which is created with a **JOIN** syntax.

*   The **DISTINCT** or **UNIQUE** statement should not be included.
*   The **GROUP BY... HAVING** statement should not be included.
*   Aggregate functions such as **SUM** or **AVG** should not be included.
*   The entire query must consist of queries that can be updated by **UNION ALL**, not by **UNION**. However, the table should exist only in one of the queries that constitute **UNION ALL**.
*   If a record is inserted into a view created by using the **UNION ALL** statement, the system determines into which table the record will be inserted. This cannot be done by the user. To control this, the user must manually insert the row or create a separate view for insertion.

Even when all rules above are satisfied, columns that contains following contents cannot be updated.

*   Path expressions (example: *tbl_name.col_name*)
*   Numeric type column that includes an arithmetic operator

Even though the column defined in the view is updatable, a view can be updated only when an appropriate update authorization is granted on the table included in the **FROM** clause. Also there must be an access authorization to a view. The way to grant an access authorization to a view is the same to grant an access authorization to a table. For details on granting authorization, see :ref:`granting-authorization`.

View's COMMENT
--------------

You can specify a view's comment as follows.

.. code-block:: sql

    CREATE OR REPLACE VIEW b_view AS SELECT * FROM a_tbl ORDER BY id DESC COMMENT 'changed view';

You can see the specified comment of a view by running this syntax.

.. code-block:: sql

    SHOW CREATE VIEW view_name;
    SELECT vclass_name, comment from db_vclass;

Or you can see the view's comment with ;sc command which displays the schema in the CSQL interpreter.

.. code-block:: sql

    $ csql -u dba demodb
    
    csql> ;sc b_view

Also, you can add a comment for each column of the view.

.. code-block:: sql

    CREATE OR REPLACE VIEW b_view (a COMMENT 'a comment', b COMMENT 'b comment') 
    AS SELECT * FROM a_tbl ORDER BY id DESC COMMENT 'view comment';
    
To see how to change a comment of a view, refer to ALTER VIEW syntax on the below.

ALTER VIEW
==========

ADD QUERY Clause
----------------

You can add a new query to a query specification by using the **ADD QUERY** clause of the **ALTER VIEW** statement. 1 is assigned to the query defined when a virtual table was created, and 2 is assigned to the query added by the **ADD QUERY** clause. ::

    ALTER [VIEW | VCLASS] [schema_name.]view_name
    ADD QUERY <select_statement>
    [INHERIT <resolution> , ...] ;
     
        <resolution> ::= {column_name} OF [schema_name.]superclass_name [AS alias]

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *view_name*: specifies the name of a view where the query to be added.
*   <*select_statement*>: specifies the query to be added.

.. code-block:: sql

    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
                4  NULL
                5  NULL
     
.. code-block:: sql
     
    ALTER VIEW b_view ADD QUERY SELECT * FROM a_tbl WHERE id IN (1,2);
    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
                4  NULL
                5  NULL
                1  '111-1111'
                2  '222-2222'

AS SELECT Clause
----------------

You can change the **SELECT** query defined in the virtual table by using the **AS SELECT** clause in the **ALTER VIEW** statement. This function is working like the **CREATE OR REPLACE** statement. You can also change the query by specifying the query number 1 in the **CHANGE QUERY** clause of the **ALTER VIEW** statement. ::

    ALTER [VIEW | VCLASS] [schema_name.]view_name AS <select_statement> ;

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *view_name*: specifies the name of a view to be modified.
*   <*select_statement*>: specifies the new query statement to replace the **SELECT** statement defined when a view is created.

.. code-block:: sql

    ALTER VIEW b_view AS SELECT * FROM a_tbl WHERE phone IS NOT NULL;
    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'

CHANGE QUERY Clause
-------------------

You can change the query defined in the query specification by using the **CHANGE QUERY** clause reserved word of the **ALTER VIEW** statement. ::

    ALTER [VIEW | VCLASS] [schema_name.]view_name
    CHANGE QUERY [integer] <select_statement> ;

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *view_name*: specifies the name of a view to be modified.
*   *integer*: specifies the number value of the query to be modified. The default value is 1.
*   <*select_statement*>: specifies the new query statement to replace the query whose query number is *integer*.

.. code-block:: sql

    --adding select_statement which query number is 2 and 3 for each
    ALTER VIEW b_view ADD QUERY SELECT * FROM a_tbl WHERE id IN (1,2);
    ALTER VIEW b_view ADD QUERY SELECT * FROM a_tbl WHERE id = 3;
    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
                4  NULL
                5  NULL
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
     
.. code-block:: sql

    --altering view changing query number 2
    ALTER VIEW b_view CHANGE QUERY 2 SELECT * FROM a_tbl WHERE phone IS NULL;
    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
                4  NULL
                5  NULL
                4  NULL
                5  NULL
                3  '333-3333'

DROP QUERY Clause
-----------------

You can drop a query defined in the query specification by using the **DROP QUERY** of the **ALTER VIEW** statement.

.. code-block:: sql

    ALTER VIEW b_view DROP QUERY 2,3;
    SELECT * FROM b_view;
     
::

               id  phone
    ===================================
                1  '111-1111'
                2  '222-2222'
                3  '333-3333'
                4  NULL
                5  NULL

COMMENT Clause
--------------

You can change a view's comment, columns' comment, or attributes' comment with **COMMENT** clause of **ALTER VIEW** syntax.

::

    ALTER [VIEW | VCLASS] [schema_name.]view_name
    COMMENT [=] 'view_comment_string' |
    COMMENT ON {COLUMN | CLASS ATTRIBUTE} <column_comment_definition> [, <column_comment_definition>] ;

        <column_comment_definition> ::= column_name [=] 'column_comment_string'

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *view_name*: Specifies the name of a view to be modified.
*   *column_name*: Specifies the name of a column to be modified.
*   *view_comment_string*: Specifies a view's comment.
*   *column_comment_string*: Specifies a column's comment.

The following example shows how to change a view's comments.

.. code-block:: sql

    ALTER VIEW v1 COMMENT = 'changed view v1 comment';

You can change the column comment by specifying one or more columns after the ON COLUMN keyword.
The following example shows how to change a column's comments.

.. code-block:: sql

    ALTER VIEW v1 COMMENT ON COLUMN c1 = 'changed view column c1 comment';
    ALTER VIEW v1 COMMENT ON COLUMN c2 = 'changed view column c2 comment', c3 = 'changed view column c3 comment';

Below is a syntax to show a column's comment.
But the SHOW CREATE VIEW statement shows only view comments.

.. code-block:: sql

    SHOW CREATE VIEW v1 /* view_name */ ;

    SELECT attr_name, class_name, comment 
    FROM db_attribute
    WHERE class_name = 'v1' /* lowercase_view_name */ ;

    SHOW FULL COLUMNS FROM v1 /* view_name */ ;

You can see this comment with the ";sc view_name" command in the CSQL interpreter.

::

    $ csql -u dba demodb
    
    csql> ;sc v1

DROP VIEW
=========

You can drop a view by using the **DROP VIEW** clause. The way to drop a view is the same as to drop a regular table.  If you also specify IF EXISTS clause, no error will be happened even if a target view does not exist. ::

    DROP [VIEW | VCLASS] [IF EXISTS] [schema_name.]view_name [{, [schema_name.]view_name}] ;

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used.
*   *view_name*: specifies the name of a view to be dropped.

.. code-block:: sql

    DROP VIEW b_view;

RENAME VIEW
===========

You can change the view name by using the **RENAME VIEW** statement. ::

    RENAME [VIEW | VCLASS] [schema_name.]old_view_name {AS | TO} [schema_name.]new_view_name [{, [schema_name.]old_view_name {AS | TO} [schema_name.]new_view_name}] ;

*   *schema_name*: Specifies the schema name. If omitted, the schema name of the current session is used. The schema of the view to be changed and the schema of the new view must be the same.
*   *old_view_name*: specifies the name of a view to be modified.
*   *new_view_name*: specifies the new name of a view.

The following example shows how to rename a view name to *game_2004*.

.. code-block:: sql

    RENAME VIEW game_2004 AS info_2004;
