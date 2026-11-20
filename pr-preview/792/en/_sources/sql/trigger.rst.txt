
:meta-keywords: cubrid trigger, database trigger, trigger condition, trigger action, trigger debugging, trigger example
:meta-description: CUBRID trigger definition, manipulation and mechanics.

*******
Trigger
*******

.. _create-trigger:

CREATE TRIGGER
==============

Guidelines for Trigger Definition
---------------------------------

Trigger definition provides various and powerful functionalities. Before creating a trigger, you must consider the following:

*   **Does the trigger condition expression cause unexpected results (side effect)?**

    You must use the SQL statements within an expectable range.

*   **Does the trigger action change the table given as its event target?**

    While this type of design is not forbidden in the trigger definition, it must be carefully applied, because a trigger can be created that falls into an infinite loop. When the trigger action modifies the event target table, the same trigger can be called again. If a trigger occurs in a statement that contains a **WHERE** clause, there is no side effect in the table affected by the **WHERE** clause.

*   **Does the trigger cause unnecessary overhead?**

    If the desired action can be expressed more effectively in the source, implement it directly in the source.

*   **Is the trigger executed recursively?**

    If the trigger action calls a trigger and this trigger calls the previous trigger again, a recursive loop is created in the database. If a recursive loop is created, the trigger may not be executed correctly, or the current session must be forced to terminate to break the ongoing infinite loop.

*   **Is the trigger definition unique?**

    A trigger defined in the same table or the one started in the same action becomes the cause of an unrecoverable error. A trigger in the same table must have a different trigger event. In addition, trigger priority must be explicitly and unambiguously defined.

Trigger Definition
------------------

A trigger is created by defining a trigger target, condition and action to be performed in the **CREATE TRIGGER** statement. A trigger is a database object that performs a defined action when a specific event occurs in the target table. ::

    CREATE TRIGGER [schema_name.]trigger_name
    [ STATUS { ACTIVE | INACTIVE } ]
    [ PRIORITY key ]
    <event_time> <event_type> [<event_target>]
    [ IF condition ]
    EXECUTE [ AFTER | DEFERRED ] action 
    [COMMENT 'trigger_comment'];
     
    <event_time> ::=
        BEFORE |
        AFTER  |
        DEFERRED
     
    <event_type> ::=
        INSERT |
        STATEMENT INSERT |
        UPDATE |
        STATEMENT UPDATE |
        DELETE |
        STATEMENT DELETE |
        ROLLBACK |
        COMMIT
     
    <event_target> ::=
        ON [schema_name.]table_name |
        ON [schema_name.]table_name [ (column_name) ]
     
    <condition> ::=
        expression
     
    <action> ::=
        REJECT |
        INVALIDATE TRANSACTION |
        PRINT message_string |
        INSERT statement |
        UPDATE statement |
        DELETE statement

*   *schema_name*: Specifies the schema name of the trigger. If omitted, the schema name of the current session is used.
*   *trigger_name*: specifies the name of the trigger to be defined.
*   [ **STATUS** { **ACTIVE** | **INACTIVE** } ]: Defines the state of the trigger (if not defined, the default value is **ACTIVE**).

    *   If **ACTIVE** state is specified, the trigger is executed every time the corresponding event occurs.
    *   If **INACTIVE** state is specified, the trigger is not executed even when the corresponding event occurs. The state of the trigger can be modified. For details, see :ref:`alter-trigger` section.

*   [ **PRIORITY** *key* ]: specifies a trigger priority if multiple triggers are called for an event. *key* must be a floating point value that is not negative. If the priority is not defined, the lowest priority 0 is assigned. Triggers having the same priority are executed in a random order. The priority of triggers can be modified. For details, see :ref:`alter-trigger` section.

*   <*event_time*>: specifies the point of time when the conditions and actions are executed. **BEFORE**, **AFTER** or **DEFERRED** can be specified. For details, see the :ref:`trigger-event-time` section.
*   <*event_type*>: trigger types are divided into a user trigger and a table trigger. For details, see the :ref:`trigger-event-type` section.
*   <*event_target*>: An event target is used to specify the target for the trigger to be called. For details, see the :ref:`trigger-event-target` section.

*   <*condition*>: specifies the trigger condition. For details, see the :ref:`trigger-condition` section.
*   <*action*>: specifies the trigger action. For details, see the :ref:`trigger-action` section.
*   *trigger_comment*: specifies a trigger's comment.

.. note::

    *   **DBA** and **DBA** members can create triggers in different schemas. If a user is neither **DBA** nor **DBA** member, triggers can only be created in the schema of that user.

The following example shows how to create a trigger that rejects the update if the number of medals won is smaller than 0 when an instance of the *participant* table is updated.
As shown below, the update is rejected if you try to change the number of gold (*gold*) medals that Korea won in the 2004 Olympic Games to a negative number.

.. code-block:: sql

    CREATE TRIGGER medal_trigger
    BEFORE UPDATE ON participant
    IF new.gold < 0 OR new.silver < 0 OR new.bronze < 0
    EXECUTE REJECT;
     
    UPDATE participant SET gold = -5 WHERE nation_code = 'KOR'
    AND host_year = 2004;

::

    ERROR: The operation has been rejected by trigger "medal_trigger".

.. _trigger-event-time:

Event Time
----------

Specifies the point of time when trigger conditions and actions are executed. The types of event time are **BEFORE**, **AFTER** and **DEFERRED**.

*   **BEFORE**: checks the condition before the event is processed.
*   **AFTER**: checks the condition after the event is processed.
*   **DEFERRED**: checks the condition at the end of the transaction for the event. If you specify **DEFERRED**, you cannot use **COMMIT** or **ROLLBACK** as the event type.

Trigger Type
------------

**User Trigger**

*   A trigger relevant to a specific user of the database is called a user trigger.
*   A user trigger has no event target and is executed only by the owner of the trigger (the user who created the trigger).
*   Event types that define a user trigger are **COMMIT** and **ROLLBACK**.

**Table Trigger**

*   A trigger that has a table as the event target is called a table trigger (class trigger).
*   A table trigger can be seen by all users who have the **SELECT** authorization on a target table.
*   Event types that define a table trigger are instance and statement events.

.. _trigger-event-type:

Trigger Event Type
------------------

*   Instance events: An event type whose unit of operation is an instance. The types of instance (record) events are as follows:

    *   **INSERT**
    *   **UPDATE**
    *   **DELETE**

*   Statement events: If you define a statement event as an event type, the trigger is called only once when the trigger starts even when there are multiple objects (instances) affected by the given statement (event). The types of statement events are as follows:

    *   **STATEMENT INSERT**
    *   **STATEMENT UPDATE**
    *   **STATEMENT DELETE**

*   Other events: **COMMIT** and **ROLLBACK** cannot be applied to individual instances.

    *   **COMMIT**
    *   **ROLLBACK**

The following example shows how to use an instance event. The *example* trigger is called by each instance affected by the database update. For example, if the *score* values of five instances in the *history* table are modified, the trigger is called five times. 

.. code-block:: sql

    CREATE TABLE update_logs(event_code INTEGER, score VARCHAR(10), dt DATETIME);
    
    CREATE TRIGGER example
    BEFORE UPDATE ON history(score)
    EXECUTE INSERT INTO update_logs VALUES (obj.event_code, obj.score, SYSDATETIME);

If you want the trigger to be called only once, before the first instance of the *score* column is updated, use the **STATEMENT** **UPDATE** type as the following example.

The following example shows how to use a statement event. If you define a statement event, the trigger is called only once before the first instance gets updated even when there are multiple instances affected by the update.

.. code-block:: sql

    CREATE TRIGGER example
    BEFORE STATEMENT UPDATE ON history(score)
    EXECUTE PRINT 'There was an update on history table';

.. note::

    *   You must specify the event target when you define an instance or statement event as the event type.
    *   **COMMIT** and **ROLLBACK** cannot have an event target.

.. _trigger-event-target:

Trigger Event Target
--------------------

An event target specifies the target for the trigger to be called. The target of a trigger event can be specified as a table or column name. If a column name is specified, the trigger is called only when the specified column is affected by the event. If a column is not specified, the trigger is called when any column of the table is affected. Only **UPDATE** and **STATEMENT UPDATE** events can specify a column as the event target.

The following example shows how to specify the *score* column of the *history* table as the event target of the *example* trigger.

.. code-block:: sql

    CREATE TABLE update_logs(event_code INTEGER, score VARCHAR(10), dt DATETIME);
    
    CREATE TRIGGER example
    BEFORE UPDATE ON history(score)
    EXECUTE INSERT INTO update_logs VALUES (obj.event_code, obj.score, SYSDATETIME);

Combination of Event Type and Target
------------------------------------

A database event calling triggers is identified by the trigger event type and event target in a trigger definition. The following table shows the trigger event type and target combinations, along with the meaning of the CUBRID database event that the trigger event represents.

+----------------+------------------+----------------------------------------------------------------------+
| Event Type     | Event Target     | Corresponding Database Activity                                      |
+================+==================+======================================================================+
| **UPDATE**     | Table            | Trigger is called when the UPDATE statement for a table is executed. |
+----------------+------------------+----------------------------------------------------------------------+
| **INSERT**     | Table            | Trigger is called when the INSERT statement for a table is executed. |
+----------------+------------------+----------------------------------------------------------------------+
| **DELETE**     | Table            | Trigger is called when the DELETE statement for a table is executed. |
+----------------+------------------+----------------------------------------------------------------------+
| **COMMIT**     | None             | Trigger is called when database transaction is committed.            |
+----------------+------------------+----------------------------------------------------------------------+
| **ROLLBACK**   | None             | Trigger is called when database transaction is rolled back.          |
+----------------+------------------+----------------------------------------------------------------------+

.. _trigger-condition:

Trigger Condition
-----------------

You can specify whether a trigger action is to be performed by defining a condition when defining the trigger.

*   If a trigger condition is specified, it can be written as an independent compound expression that evaluates to true or false. In this case, the expression can contain arithmetic and logical operators allowed in the **WHERE** clause of the **SELECT** statement. The trigger action is performed if the condition is true; if it is false, action is ignored.

*   If a trigger condition is omitted, the trigger becomes an unconditional trigger, which refers to that the trigger action is performed whenever it is called.

The following example shows how to use a correlation name in an expression within a condition. If the event type is **INSERT**, **UPDATE** or **DELETE**, the expression in the condition can refer to the correlation names **obj**, **new** or **old** to access a specific column. This example prefixes **obj** to the column name in the trigger condition to show that the *example* trigger tests the condition based on the current value of the *record* column.

.. code-block:: sql

    CREATE TRIGGER example
    BEFORE UPDATE ON participant
    IF new.gold < 0 OR new.silver < 0 OR new.bronze < 0
    EXECUTE REJECT;

The following example shows how to use the **SELECT** statement in an expression within a condition. The trigger in this example uses the **SELECT** statement that contains an aggregate function **COUNT** (\*) to compare the value with a constant. The **SELECT** statement must be enclosed in parentheses and must be placed at the end of the expression.

.. code-block:: sql

    CREATE TRIGGER example
    BEFORE INSERT ON participant
    IF 1000 >  (SELECT COUNT(*) FROM participant)
    EXECUTE REJECT;

.. note::

    The expression given in the trigger condition may cause side effects on the database if a method is called while the condition is performed. A trigger condition must be constructed to avoid unexpected side effects in the database.

Correlation Name
----------------

You can access the column values defined in the target table by using a correlation name in the trigger definition. A correlation name is the instance that is actually affected by the database operation calling the trigger. A correlation name can also be specified in a trigger condition or action.

The types of correlation names are **new**, **old** and **obj**. These correlation names can be used only in instance triggers that have an **INSERT**, **UPDATE** or **DELETE** event.

As shown in the table below, the use of correlation names is further restricted by the event time defined for the trigger condition.

+------------+------------+-----------------------+
|            | BEFORE     | AFTER or DERERRED     |
+============+============+=======================+
| **INSERT** | **new**    | **obj**               |
+------------+------------+-----------------------+
| **UPDATE** | **obj**    | **obj**               |
|            |            |                       |
|            | **new**    | **old** (AFTER)       |
+------------+------------+-----------------------+
| **DELETE** | **obj**    | N/A                   |
+------------+------------+-----------------------+

+------------------+-----------------------------------------------------------------------------------------------------------------------+
| Correlation Name | Representative Attribute Value                                                                                        |
+==================+=======================================================================================================================+
| **obj**          | Refers to the current attribute value of an instance. This can be used to access attribute values before an instance  |
|                  | is updated or deleted. It is also used to access attribute values after an instance has been updated or inserted.     |
+------------------+-----------------------------------------------------------------------------------------------------------------------+
| **new**          | Refers to the attribute value proposed by an insert or update operation.                                              |
|                  | The new value can be accessed only before the instance is actually inserted or updated.                               |
+------------------+-----------------------------------------------------------------------------------------------------------------------+
| **old**          | Refers to the attribute value that existed prior to the completion of an update operation. This value is maintained   |
|                  |  only while the trigger is being performed. Once the trigger is completed, the **old** values get lost.               |
+------------------+-----------------------------------------------------------------------------------------------------------------------+

.. _trigger-action:

Trigger Action
--------------

A trigger action describes what to be performed if the trigger condition is true or omitted. If a specific point of time (**AFTER** or **DEFERRED**) is not given in the action clause, the action is executed at once as the trigger event.

The following is a list of actions that can be used for trigger definitions.

*   **REJECT**: discards the operation that initiated the trigger and keeps the former state of the database, if the condition is not true. Once the operation is performed, **REJECT** is allowed only when the action time is **BEFORE** because the operation cannot be rejected. Therefore, you must not use **REJECT** if the action time is **AFTER** or **DERERRED**.

*   **INVALIDATE TRANSACTION**: allows the event operation that called the trigger, but does not allow the transaction that contains the commit to be executed. You must cancel the transaction by using the **ROLLBACK** statement if it is not valid. Such action is used to protect the database from having invalid data after a data-changing event happens.

*   **PRINT**: displays trigger actions on the terminal screen in text messages, and can be used during developments or tests. The results of event operations are not rejected or discarded.
*   **INSERT**: inserts one or more new instances to the table.
*   **UPDATE**: updates one or more column values in the table.
*   **DELETE**: deletes one or more instances from the table.

The following example shows how to define an action when a trigger is created. The *medal_trig* trigger defines **REJECT** in its action. **REJECT** can be specified only when the action time is **BEFORE**.

.. code-block:: sql

    CREATE TRIGGER medal_trig
    BEFORE UPDATE ON participant
    IF new.gold < 0 OR new.silver < 0 OR new.bronze < 0
    EXECUTE REJECT;

.. note::

    *   Trigger may fall into an infinite loop when you use **INSERT** in an action of a trigger where an **INSERT** event is defined.
    *   If a trigger where an **UPDATE** event is defined runs on a partitioned table, you must be careful because the defined partition can be broken or unintended malfunction may occur. To prevent such situation, CUBRID outputs an error so that the **UPDATE** causing changes to the running partition is not executed. Trigger may fall into an infinite loop when you use **UPDATE** in an action of a trigger where an **UPDATE** event is defined.

Trigger's COMMENT
-----------------

You can specify a trigger's comment as follows.

.. code-block:: sql

    CREATE TRIGGER trg_ab BEFORE UPDATE on abc(c) EXECUTE UPDATE cube_ab SET sumc = sumc + 1
    COMMENT 'test trigger comment';

You can see a trigger's comment by running the below statement.

.. code-block:: sql

    SELECT name, comment FROM db_trigger;
    SELECT trigger_name, comment FROM db_trig;

Or you can see a trigger's comment with ;sc command which displays a schema in the CSQL interpreter.

.. code-block:: sql

    $ csql -u dba demodb
    
    csql> ;sc tbl

To change the trigger's comment, refer to **ALTER TRIGGER** syntax on the below.

.. _alter-trigger:

ALTER TRIGGER
=============

In the trigger definition, **STATUS** and **PRIORITY** options can be changed by using the **ALTER** statement. If you need to alter other parts of the trigger (event targets or conditional expressions), you must delete and then re-create the trigger. 

::

    ALTER TRIGGER [schema_name.]trigger_name <trigger_option> ;

    <trigger_option> ::=
        STATUS { ACTIVE | INACTIVE } |
        PRIORITY key

*   *schema_name*: Specifies the schema name of the trigger. If omitted, the schema name of the current session is used.
*   *trigger_name*: specifies the name of the trigger to be changed.
*   **STATUS** { **ACTIVE** | **INACTIVE** }: changes the status of the trigger.
*   **PRIORITY** *key*: changes the priority.

The following example shows how to create the medal_trig trigger and then change its state to **INACTIVE** and its priority to 0.7.

.. code-block:: sql

    CREATE TRIGGER medal_trig
    STATUS ACTIVE
    BEFORE UPDATE ON participant
    IF new.gold < 0 OR new.silver < 0 OR new.bronze < 0
    EXECUTE REJECT;

    ALTER TRIGGER medal_trig STATUS INACTIVE;
    ALTER TRIGGER medal_trig PRIORITY 0.7;

.. note::

    *   Only one *trigger_option* can be specified in a single **ALTER TRIGGER** statement.
    *   To change a table trigger, you must be the trigger owner or granted the **ALTER** authorization on the table where the trigger belongs.
    *   A user trigger can only be changed by its owner. For details on *trigger_option*, see the :ref:`create-trigger` section. The key specified together with the **PRIORITY** option must be a non-negative floating point value.

Trigger's COMMENT
-----------------

You can change a trigger's comment by running **ALTER TRIGGER** syntax as below.

::

    ALTER TRIGGER [schema_name.]trigger_name [trigger_option] 
    [COMMENT ‘comment_string’];

*   *schema_name*: Specifies the schema name of the trigger. If omitted, the schema name of the current session is used.
*   *trigger_name*: specifies the name of the trigger to be changed.
*   *comment_string*: specifies a trigger's comment.

If you want to change only trigger's comment, you can omit trigger options (*trigger_option*).

For *trigger_option*, see :ref:`alter-trigger` on the above.

.. code-block:: sql

    ALTER TRIGGER trg_ab COMMENT 'new trigger comment';

DROP TRIGGER
============

You can drop a trigger by using the **DROP TRIGGER** statement. ::

    DROP TRIGGER [schema_name.]trigger_name ; 

*   *schema_name*: Specifies the schema name of the trigger. If omitted, the schema name of the current session is used.
*   *trigger_name*: specifies the name of the trigger to be dropped.

The following example shows how to drop the medal_trig trigger.

.. code-block:: sql

    DROP TRIGGER medal_trig;

.. note::

    *   A user trigger (i.e. the trigger event is **COMMIT** or **ROLLBACK**) can be seen and dropped only by the owner.
    *   Only one trigger can be dropped by a single **DROP TRIGGER** statement. A table trigger can be dropped by a user who has an **ALTER** authorization on the table.

RENAME TRIGGER
==============

You can change a trigger name by using the **TRIGGER** reserved word in the **RENAME** statement. ::

    RENAME TRIGGER [schema_name.]old_trigger_name {AS | TO} [schema_name.]new_trigger_name ;

*   *schema_name*: Specifies the schema name of the trigger. If omitted, the schema name of the current session is used. The schema of the current trigger and the schema of the trigger to be changed must be the same.
*   *old_trigger_name*: specifies the current name of the trigger.
*   *new_trigger_name*: specifies the name of the trigger to be modified.

.. code-block:: sql

    RENAME TRIGGER medal_trigger AS medal_trig;

.. note::

    *   A trigger name must be unique among triggers owned by the user. However, it can be the same as the table name in the database or the name of a trigger owned by another owner.
    *   To rename a table trigger, you must be the trigger owner or granted the **ALTER** authorization on the table where the trigger belongs. A user trigger can only be renamed by its user.

Deferred Condition and Action
=============================

A deferred trigger action and condition can be executed later or canceled. These triggers include a **DEFERRED** time option in the event time or action clause. If the **DEFERRED** option is specified in the event time and the time is omitted before the action, the action is deferred automatically.

Executing Deferred Condition and Action
---------------------------------------

Executes the deferred condition or action of a trigger immediately. ::

    EXECUTE DEFERRED TRIGGER <trigger_identifier> ;

    <trigger_identifier> ::=
        [schema_name.]trigger_name |
        ALL TRIGGERS

*   *schema_name*: Specifies the schema name of the trigger. If omitted, the schema name of the current session is used.
*   *trigger_name*: executes the deferred action of the trigger when a trigger name is specified.
*   **ALL TRIGGERS**: executes all currently deferred actions.

Dropping Deferred Condition and Action
--------------------------------------

Drops the deferred condition and action of a trigger. ::

    DROP DEFERRED TRIGGER trigger_identifier ;

    <trigger_identifier> ::=
        [schema_name.]trigger_name |
        ALL TRIGGER

*   *schema_name*: Specifies the schema name of the trigger. If omitted, the schema name of the current session is used.
*   *trigger_name* : Cancels the deferred action of the trigger when a trigger name is specified.
*   **ALL TRIGGERS** : Cancels currently deferred actions.

Granting Trigger Authorization
------------------------------

Trigger authorization is not granted explicitly. Authorization on the table trigger is automatically granted to the user if the authorization is granted on the event target table described in the trigger definition. In other words, triggers that have table targets (**INSERT**, **UPDATE**, etc.) are seen by all users. User triggers (**COMMIT** and **ROLLBACK**) are seen only by the user who defined the triggers. All authorizations are automatically granted to the trigger owner.

.. note::

    *   To define a table trigger, you must have an **ALTER** authorization on the table.
    *   To define a user trigger, the database must be accessed by a valid user.

Trigger on REPLACE and INSERT ... ON DUPLICATE KEY UPDATE
=========================================================

When the **REPLACE** statement and **INSERT ...  ON DUPLICATE KEY UPDATE** statements are executed, the trigger is executed in CUBRID, while **DELETE**, **UPDATE**, **INSERT** jobs occur internally. The following table shows the order in which the trigger is executed in CUBRID depending on the event that occurred when the **REPLACE** or **INSERT ...  ON DUPLICATE KEY UPDATE** statement is executed. Both the **REPLACE** statement and the **INSERT ...  ON DUPLICATE KEY UPDATE** statement do not execute triggers in the inherited class (table).

**Execution Sequence of Triggers in the REPLACE and the INSERT ...  ON DUPLICATE KEY UPDATE statements**

+--------------------------------------------------+------------------------------------+
| Event                                            | Execution Sequence of Triggers     |
+==================================================+====================================+
| REPLACE                                          | BEFORE DELETE >                    |
| When a record is deleted and new one is inserted | AFTER DELETE >                     |
|                                                  | BEFORE INSERT >                    |
|                                                  | AFTER INSERT                       |
+--------------------------------------------------+------------------------------------+
| INSERT ...  ON DUPLICATE KEY UPDATE              | BEFORE UPDATE >                    |
| When a record is updated                         | AFTER UPDATE                       |
+--------------------------------------------------+------------------------------------+
| REPLACE, INSERT ...  ON DUPLCATE KEY UPDATE      | BEFORE INSERT >                    |
| Only when a record is inserted                   | AFTER INSERT                       |
+--------------------------------------------------+------------------------------------+

The following example shows that **INSERT ... ON DUPLICATE KEY UPDATE** and **REPLACE** are executed in the *with_trigger* table and records are inserted to the *trigger_actions* table as a consequence of the execution.

.. code-block:: sql

    CREATE TABLE with_trigger (id INT UNIQUE);
    INSERT INTO with_trigger VALUES (11);
     
    CREATE TABLE trigger_actions (val INT);
     
    CREATE TRIGGER trig_1 BEFORE INSERT ON with_trigger EXECUTE INSERT INTO trigger_actions VALUES (1);
    CREATE TRIGGER trig_2 BEFORE UPDATE ON with_trigger EXECUTE INSERT INTO trigger_actions VALUES (2);
    CREATE TRIGGER trig_3 BEFORE DELETE ON with_trigger EXECUTE INSERT INTO trigger_actions VALUES (3);
     
    INSERT INTO with_trigger VALUES (11) ON DUPLICATE KEY UPDATE id=22;
     
    SELECT * FROM trigger_actions;

::
    
              va
    ==============
                2
     
.. code-block:: sql

    REPLACE INTO with_trigger VALUES (22);
     
    SELECT * FROM trigger_actions;
    
::
    
              va
    ==============
                2
                3
                1

Trigger Debugging
=================

Once a trigger is defined, it is recommended to check whether it is running as intended. Sometimes the trigger takes more time than expected in processing. This means that it is adding too much overhead to the system or has fallen into a recursive loop. This section explains several ways to debug the trigger.

The following example shows a trigger that was defined to fall into a recursive *loop_tgr* when it is called. A *loop_tgr* trigger is somewhat artificial in its purpose; it can be used as an example of debugging trigger.

.. code-block:: sql

    CREATE TRIGGER loop_tgr
    BEFORE UPDATE ON participant(gold)
    IF new.gold > 0
    EXECUTE UPDATE participant
            SET gold = new.gold - 1
            WHERE nation_code = obj.nation_code AND host_year = obj.host_year;

Viewing Trigger Execution Log
-----------------------------

You can view the execution log of the trigger from a terminal by using the **SET TRIGGER TRACE** statement. ::

    SET TRIGGER TRACE <switch> ;

    <switch> ::=
        ON |
        OFF

*   **ON**: executes **TRACE** until the switch is set to **OFF** or the current database session terminates.
*   **OFF**: stops the **TRACE**.

The following example shows how to execute the **TRACE** and the *loop_tgr* trigger to view the trigger execution logs. To identify the trace for each condition and action executed when the trigger is called, a message is displayed on the terminal. The following message appears 15 times because the *loop_tgr* trigger is executed until the *gold* value becomes 0.

.. code-block:: sql

    SET TRIGGER TRACE ON;
    UPDATE participant SET gold = 15 WHERE nation_code = 'KOR' AND host_year = 1988;

::

    TRACE: Evaluating condition for trigger "loop".
    TRACE: Executing action for trigger "loop".

Limiting Nested Trigger
-----------------------

With the **MAXIMUM DEPTH** keyword of the **SET TRIGGER** statement, you can limit the number of triggers to be initiated at each step. By doing so, you can prevent a recursively called trigger from falling into an infinite loop. ::

    SET TRIGGER [ MAXIMUM ] DEPTH count ;

*   *count*: A positive integer value that specifies the number of times that a trigger can recursively start another trigger or itself. If the number of triggers reaches the maximum depth, the database request stops(aborts) and the transaction is marked as invalid. The specified **DEPTH** applies to all other triggers except the current session. The maximum value is 32.

The following example shows how to configure the maximum number of times of recursive trigger calling to 10. This applies to all triggers that start subsequently. In this example, the *gold* column value is updated to 15, so the trigger is called 16 times in total. This exceeds the currently set maximum depth and the following error message occurs.

.. code-block:: sql

    SET TRIGGER MAXIMUM DEPTH 10;
    UPDATE participant SET gold = 15 WHERE nation_code = 'KOR' AND host_year = 1988;
     
::

    ERROR: Maximum trigger depth 10 exceeded at trigger "loop_tgr".

Trigger Example
===============

This section covers trigger definitions in the demo database. The triggers created in the *demodb* database are not complex, but use most of the features available in CUBRID. If you want to maintain the original state of the *demodb* database when testing such triggers, you must perform a rollback after changes are made to the data.

Triggers created by the user in the own database can be as powerful as applications created by the user.

The following trigger created in the *participant* table rejects an update to the medal column (*gold*, *silver*, *bronze*) if a given value is smaller than 0. The evaluation time must be **BEFORE** because a correlation name new is used in the trigger condition. Although not described, the action time of this trigger is also **BEFORE**.

.. code-block:: sql

    CREATE TRIGGER medal_trigger
    BEFORE UPDATE ON participant
    IF new.gold < 0 OR new.silver < 0 OR new.bronze < 0
    EXECUTE REJECT;

The trigger *medal_trigger* starts when the number of gold (*gold*) medals of the country whose nation code is 'BLA' is updated. Since the trigger created does not allow negative numbers, the example below will not be updated.

.. code-block:: sql

    UPDATE participant
    SET gold = -10
    WHERE nation_code = 'BLA';

The following trigger has the same condition as the one above except that **STATUS ACTIVE** is added. If the **STATUS** statement is omitted, the default value is **ACTIVE**. You can change **STATUS** to **INACTIVE** by using the **ALTER TRIGGER** statement.

You can specify whether or not to execute the trigger depending on the **STATUS** value.

.. code-block:: sql

    CREATE TRIGGER medal_trig
    STATUS ACTIVE
    BEFORE UPDATE ON participant
    IF new.gold < 0 OR new.silver < 0 OR new.bronze < 0
    EXECUTE REJECT;
     
    ALTER TRIGGER medal_trig
    STATUS INACTIVE;

The following trigger shows how integrity constraint is enforced when a transaction is committed. This example is different from the previous ones, in that one trigger can have specific conditions for multiple tables.

.. code-block:: sql

    CREATE TRIGGER check_null_first
    BEFORE COMMIT
    IF 0 < (SELECT count(*) FROM athlete WHERE gender IS NULL)
    OR 0 < (SELECT count(*) FROM game WHERE nation_code IS NULL)
    EXECUTE REJECT;

The following trigger delays the update integrity constraint check for the *record* table until the transaction is committed. Since the **DEFERRED** keyword is given as the event time, the trigger is not executed at the time.

.. code-block:: sql

    CREATE TRIGGER deferred_check_on_record
    DEFERRED UPDATE ON record
    IF obj.score = '100'
    EXECUTE INVALIDATE TRANSACTION;

Once completed, the update in the *record* table can be confirmed at the last point (commit or rollback) of the current transaction. The correlation name **old** cannot be used in the conditional clause of the trigger where **DEFERRED UPDATE** is used. Therefore, you cannot create a trigger as the following.

.. code-block:: sql

    CREATE TABLE foo (n int);
    CREATE TRIGGER foo_trigger
        DEFERRED UPDATE ON foo
        IF old.n = 100
        EXECUTE PRINT 'foo_trigger';

If you try to create a trigger as shown above, an error message is displayed and the trigger fails.

::

    ERROR: Error compiling condition for 'foo_trigger' : old.n is not defined.

The correlation name **old** can be used only with **AFTER**.
