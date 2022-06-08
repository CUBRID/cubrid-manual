
:meta-keywords: cubrid identifier guideline, cubrid legal identifier, database identifier
:meta-description: CUBRID Identifiers can be table names, index names, view names, column names, user names etc.

**********
Identifier
**********

Guidelines for Creating Identifiers
===================================

Table name, index name, view name, column name, user name etc. are included in identifier. The guidelines for creating identifiers are as follows:

*   An identifier must begin with a letter; it must not begin with a number or a symbol.
*   It is not case-sensitive.
*   :doc:`keyword` are not allowed.

.. productionlist::
    identifier: identifier_letter [ { other_identifier }; ]
    identifier_letter: upper_case_letter | lower_case_letter
    other_identifier: identifier_letter | digit | _ | #
    digit: 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9
    upper_case_letter: A | B | C | D | E | F | G | H | I | J | K | L | M | N | O | P| Q | R | S | T | U | V | W | X | Y | Z
    lower_case_letter: a | b | c | d | e | f | g | h | i | j | k | l | m | n | o | p| q | r | s | t | u | v | w | x | y | z

Legal Identifiers
=================

.. rubric:: Beginning with a Letter

An identifier must begin with a letter. All other special characters except operator characters are allowed. The following are examples of legal identifiers. 

::

    a
    a_b
    ssn#
    this_is_an_example_#

.. rubric:: Enclosing in Double Quotes, Square Brackets, or Backtick Symbol

Identifiers or reserved keywords shown as below are not allowed. However, if they are enclosed in double quotes, square brackets, or backtick symbol, they are allowed as an exception. Especially, the double quotations can be used as a symbol enclosing identifiers when the **ansi_quotes** parameter is set to **yes**. If this value is set to **no** double quotations are used as a symbol enclosing character strings. The following are examples of legal identifiers. 

::

    "select"
    "@lowcost"
    "low cost"
    "abc" "def"
    [position]

.. Warning::

    Even though identifiers are enclosed in double quotation marks, square brackets, or backticks, they cannot contain square brackets ([ ]) or dots (.).

    *  Square brackets are used by default to process reserved words when parsing a query inside.
    *  Dots are used in Path Expressions and JSON, and they are also used to separate schema names and object names.

    The following identifiers are not allowed even though they are enclosed in double quotation marks (" ").

    ::

        "he[[o"
        "wor]d"
        "hello[]world"
        "hello.world"

.. note::

    From 10.0 version, if a column name which is a reserved word is used together with a "table name (or alias).", you don't need to wrap up a column name with a double quotes.

    .. code-block:: sql 

        CREATE TABLE tbl ("int" int, "double" double); 

        SELECT t.int FROM tbl t; 
        
    In the above SELECT query, "int" is a column name which is used together with "t.".

Illegal Identifiers
===================

.. rubric:: Beginning with special characters or numbers

An identifier starting with a special character or a number is not allowed. As an exception, a underline (_) and a sharp symbol (#) are allowed for the first character. 

::

    _a
    #ack
    %nums
    2fer
    88abs

.. rubric:: An identifier containing a space

An identifier that a space within characters is not allowed. 

::

    col1 t1

.. rubric:: An identifier containing operator special characters

An identifier which contains operator special characters (+, -, \*, /, %, ||, !, < , > , =, \|, ^, & , ~ ) is not allowed. 

::

    col+
    col~
    col& &

The maximum length of an identifier name
========================================

The following table summarizes the maximum byte length allowable for each identifier name. Note that the unit is byte and the number of characters and the bytes are different by the character set used (for example, the length of one Korean character in UTF-8 is 3 bytes).

+-----------------------+-------------------+
| Identifier            | Maximum Bytes     |
+=======================+===================+
| Database              | 17                |
+-----------------------+-------------------+
| User                  | 31                |
+-----------------------+-------------------+
| Table                 | 222               |
+-----------------------+-------------------+
| Column                | 254               |
+-----------------------+-------------------+
| Index                 | 254               |
+-----------------------+-------------------+
| Constraint            | 254               |
+-----------------------+-------------------+
| Java Stored Procedure | 254               |
+-----------------------+-------------------+
| Trigger               | 222               |
+-----------------------+-------------------+
| View                  | 222               |
+-----------------------+-------------------+
| Serial                | 222               |
+-----------------------+-------------------+

.. note::

    Prior to version 11.2, the maximum length of table, trigger, and serial names were 254 bytes. Since version 11.2, table, trigger, and serial names contain schema names. Therefore, identifier names cannot exceed 222 bytes except for 31 bytes, which is the length of the owner name, and 1 byte, which is the length of the delimiter (.).

.. note::

    Automatically created constraint name like a name of primary key(pk_<table_name>_<column_name>) or foreign key(fk_<table_name>_<column_name>) also does not allow over the maximum name length of the identifier, 254 bytes.
