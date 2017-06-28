
:meta-keywords: call statement, cubrid method type
:meta-description: The methods are written in C with built-in functions of CUBRID database system, and are called by the CALL statement.

******
Method
******

The methods are written in C with built-in functions of CUBRID database system, and are called by the **CALL** statement. A method program is loaded and linked with the application currently running by the dynamic loader when the method is called. The return value created as a result of the method execution is passed to the caller.

Method Type
===========

The CSQL language supports the following two types of methods: class and instance methods.

*   The **class method** is a method called by a class object. It is usually used to create a new class instance or to initialize it. It is also used to access or update class attributes.

*   The **instance method** is a method called by a class instance. It is used more often than the class method because most operations are executed in the instance. For example, an instance method can be written to calculate or update the instance attribute. This method can be called from any instance of the class in which the method is defined or of the sub class that inherits the method.

The method inheritance rules are similar to those of the attribute inheritance. The sub class inherits classes and instance methods from the super class. The sub class can follow the class or instance method definition from the super class.

The rules for resolving method name conflicts are same as those for attribute name conflicts. For details about attribute/method inheritance conflicts, see :ref:`class-conflict-resolution`.

CALL Statement
==============

The **CALL** statement is used to call a method defined in the database. Both class and instance methods can be called by the **CALL** statement. If you want to see example of using the CALL statement, see :ref:`authorization-method`. ::

    CALL <method_call> ;

    <method_call> ::=
        method_name ([<arg_value> [{, <arg_value> } ...]]) ON <call_target> [<to_variable>] |
        method_name (<call_target> [, <arg_value> [{, <arg_value>} ...]] ) [<to_variable>]

        <arg_value> ::=
            any CSQL expression

        <call_target> ::=
            an object-valued expression

        <to_variable> ::=
            INTO variable |
            TO variable

*   The *method_name* is either the method name defined in the table or the system-defined method name. A method requires one or more parameters. If there is no parameter for the method, a set of blank parentheses must be used.

*   <*call_target*> can use an object-valued expression that contains a class name, a variable, another method call (which returns an object). To call a class method for a class object, you must place the **CLASS** keyword before the <*call_target*>. In this case, the table name must be the name of the class where the table method is defined. To call a record method, you must specify the expression representing the record object. You can optionally store the value returned by the table or record method in the <*to_variable*>. This returned variable value can be used in the **CALL** statement just like the <*call_target*> or <*arg_value*> parameter.

*   Calling nested methods is possible when other *method_call* is the <*call_target*> of the method or given as one of the <*arg_value*> parameters. 
