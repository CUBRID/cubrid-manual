
:meta-keywords: call statement
:meta-description: The CALL statement invokes a stored procedure

****
CALL
****

The **CALL** statement invokes a stored routine defined in the database.
The stored routines supported by CUBRID are as follows.

  * :ref:`create-procedure`
  * :ref:`create-function`
  * :doc:`/sql/method`

::

    CALL <routine_clause> ;
        
        <routine_clause> ::=
            routine_name ([<arg_value> [{, <arg_value> } ...]]) [<method_call_target>] [<to_variable>]

        <arg_value>
            literal |
            :host_variable |
            any CSQL expression

        <method_call_target> ::=
            ON [CLASS] object-valued-expression

        <to_variable> ::= 
            INTO :host_variable |
            TO :host_variable

*   *routine_name*: Specifies the method name or stored function/procedure name. It is not case-sensitive.
*   <*arg_value*>: Specifies the arguments of the stored routine. The argument can be a literal value, host variable, and any expression.
*   <*method_call_target*>: Specifies only when using a method written in C.
    Specifies an object-valued-expression containing a class name, a variable, or another method call (which returns an object).
    To call a class method for a class object, you must place the **CLASS** keyword before the <*call_target*>. 
    In this case, the table name must be the name of the class where the table method is defined. 
    To call a record method, you must specify the expression representing the record object. 
*   <*to_variable*>: The value returned by the stored routine can be stored in a host variable.

For *routine_name* of the **CALL** statement, CUBRID searches a stored routine as follows.

1   It is processed as a method if there is a target class in the **CALL** statement.
2   If there is no target class in the **CALL** statement, it is checked whether a Java stored function/procedure is executed or not; a Java stored function/procedure will be executed if one exists.
3   If no Java stored function/procedure exists in step 2 above, it is checked whether a method is executed or not; a method will be executed if one with the same name exists.

.. code-block:: sql

    CALL Hello() INTO :HELLO;
    CALL Sp_int(3) INTO :i;
    CALL phone_info('Tom','016-111-1111');

The following error occurs if you call a Java stored function/procedure that does not exist.

*    If there is **no argument** in the **CALL** statement, a message "ERROR: Stored procedure/function 'deposit' does not exist." appears because it can be distinguished from a method.  
*    if there is **an argument** in the **CALL** statement, a message "ERROR: Methods require an object as their target." appears because it cannot be distinguished from a method.

.. code-block:: sql

    // there is not argument in the CALL statement
    CALL deposit();
    
::

    ERROR: Stored procedure/function 'deposit' does not exist.

.. code-block:: sql

    // there are arguments in the CALL statement
    CALL deposit('Tom', 3000000);
    
::

    ERROR: Methods require an object as their target.

If the **CALL** statement is nested within another **CALL** statement calling a Java stored function/procedure, or if a subquery is used in calling the Java function/procedure, the **CALL** statement is not executed.

.. code-block:: sql

    CALL phone_info('Tom', CALL sp_int(999));
    CALL phone_info((SELECT * FROM Phone WHERE id='Tom'));
