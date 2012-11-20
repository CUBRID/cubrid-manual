*************************************
Conditional Expressions and Functions
*************************************

CASE
====

The **CASE** expression uses the SQL statement to perform an **IF** ... **THEN** statement. When a result of comparison expression specified in a **WHEN** clause is true, a value specified in **THEN** value is returned. A value specified in an **ELSE** clause is returned otherwise. If no **ELSE** clause exists, **NULL** is returned. ::

	CASE control_expression simple_when_list
	[ else_clause ]
	END
	 
	CASE searched_when_list
	[ else_clause ]
	END
	 
	simple_when :
	WHEN expression THEN result
	 
	searched_when :
	WHEN search_condition THEN result
	 
	else_clause :
	ELSE result
	 
	result :
	expression | NULL

**The CASE** expression must end with the END keyword. A *control_expression* argument and an *expression argument* in *simple_when* expression should be comparable data types. The data types of *result* specified in the **THEN** ... **ELSE** statement should all same, or they can be convertible to common data type.

The data type for a value returned by the **CASE** expression is determined based on the following rules.

*   If data types for result specified in the **THEN** statement are all same, a value with the data type is returned.
*   If data types can be convertible to common data type even though they are not all same, a value with the data type is returned.
*   If any of values for *result* is a variable length string, a value data type is a variable length string. If values for *result* are all a fixed length string, the longest character string or bit string is returned.
*   If any of values for result is an approximate numeric data type, a value with a numeric data type is returned. The number of digits after the decimal point is determined  to display all significant figures.

.. code-block:: sql

	--creating a table
	CREATE TABLE case_tbl( a INT);
	INSERT INTO case_tbl VALUES (1);
	INSERT INTO case_tbl VALUES (2);
	INSERT INTO case_tbl VALUES (3);
	INSERT INTO case_tbl VALUES (NULL);
	 
	--case operation with a search when clause
	SELECT a,
		   CASE WHEN a=1 THEN 'one'
				WHEN a=2 THEN 'two'
				ELSE 'other'
		   END
	FROM case_tbl;
				a  case when a=1 then 'one' when a=2 then 'two' else 'other' end
	===================================
				1  'one'
				2  'two'
				3  'other'
			 NULL  'other'
	 
	--case operation with a simple when clause
	SELECT a,
		   CASE a WHEN 1 THEN 'one'
				  WHEN 2 THEN 'two'
				  ELSE 'other'
		   END
	FROM case_tbl;
				a  case a when 1 then 'one' when 2 then 'two' else 'other' end
	===================================
				1  'one'
				2  'two'
				3  'other'
			 NULL  'other'
	 
	 
	--result types are converted to a single type containing all of significant figures
	SELECT a,
		   CASE WHEN a=1 THEN 1
				WHEN a=2 THEN 1.2345
				ELSE 1.234567890
		   END
	FROM case_tbl;
				a  case when a=1 then 1 when a=2 then 1.2345 else 1.234567890 end
	===================================
				1  1.000000000
				2  1.234500000
				3  1.234567890
			 NULL  1.234567890
	 
	--an error occurs when result types are not convertible
	SELECT a,
		   CASE WHEN a=1 THEN 'one'
				WHEN a=2 THEN 'two'
				ELSE 1.2345
		   END
	FROM case_tbl;
	ERROR: Cannot coerce 'one' to type double.

Conditional Functions
=====================

.. function:: COALESCE (expression [, ...])

	The **COALESCE** function has more than one expression as an argument. If a first argument is non-**NULL**, the corresponding value is returned if it is **NULL**, a second argument is returned. If all expressions which have an argument are **NULL**, **NULL** is returned. Therefore, this function is generally used to replace **NULL** with other default value.

	Operation is performed by converting the type of every argument into that with the highest priority. If there is an argument whose type cannot be converted, the type of every argument is converted into a **VARCHAR** type. The following list shows priority of conversion based on input argument type.

	*   **CHAR** < **VARCHAR**
	*   **NCHAR** < **NCHAR VARING**
	*   **BIT** < **VARBIT**
	*   **SHORT** < **INT** < **BIGINT** < **NUMERIC** < **FLOAT** < **DOUBLE**
	*   **DATE** < **TIMESTAMP** < **DATETIME**

	For example, if a type of a is **INT**, b, **BIGINT**, c, **SHORT**, and d, **FLOAT**, then **COALESCE** (a, b, c, d) returns a **FLOAT** type. If a type of a is **INTEGER**, b, **DOULBE* , c, **FLOAT**, and d, **TIMESTAMP**, then **COALESCE** (a, b, c, d) returns a **VARCHAR** type.

	**COALESCE** (*a, b*) works the same as the **CASE** statement as follows: ::

		CASE WHEN a IS NOT NULL
		THEN a
		ELSE b
		END

	.. code-block:: sql

		SELECT * FROM case_tbl;
					a
		=============
					1
					2
					3
				 NULL
		 
		--substituting a default value 10.0000 for NULL valuse
		SELECT a, COALESCE(a, 10.0000) FROM case_tbl;
					a  coalesce(a, 10.0000)
		===================================
					1  1.0000
					2  2.0000
					3  3.0000
				 NULL  10.0000

.. function:: DECODE( expression, search, result [, search, result]* [, default] )

	As well as a **CASE** expression, the **DECODE** function performs the same functionality as the **IF** ... **THEN** ... **ELSE** statement. It compares the *expression* argument with *search* argument, and returns the *result* corresponding to *search* that has the same value. It returns *default* if there is no *search* with the same value, and returns **NULL** if *default* is omitted. An expression argument and a search argument to be comparable should be same or convertible each other. The number of digits after the decimal point is determined to display all significant figures including valid number of all *result*.

	**DECODE** (*a*, *b*, *c*, *d*, *e, f*) has the same meaning as the **CASE** statement below. ::

		CASE WHEN a = b THEN c
		WHEN a = d THEN e
		ELSE f
		END

	.. code-block:: sql

		SELECT * FROM case_tbl;
					a
		=============
					1
					2
					3
				 NULL
		 
		--Using DECODE function to compare expression and search values one by one
		SELECT a, DECODE(a, 1, 'one', 2, 'two', 'other') FROM case_tbl;
					a  decode(a, 1, 'one', 2, 'two', 'other')
		===================================
					1  'one'
					2  'two'
					3  'other'
				 NULL  'other'
		 
		 
		--result types are converted to a single type containing all of significant figures
		SELECT a, DECODE(a, 1, 1, 2, 1.2345, 1.234567890) FROM case_tbl;
					a  decode(a, 1, 1, 2, 1.2345, 1.234567890)
		===================================
					1  1.000000000
					2  1.234500000
					3  1.234567890
				 NULL  1.234567890
		 
		--an error occurs when result types are not convertible
		SELECT a, DECODE(a, 1, 'one', 2, 'two', 1.2345) FROM case_tbl;
		 
		ERROR: Cannot coerce 'one' to type double.

.. function:: IF ( expression1, expression2, expression3 )

	The **IF** function returns *expression2* if the value of the arithmetic expression specified as the first parameter is **TRUE**, or *expression3* if the value is **FALSE** or **NULL**. *expression2* and *expression3* which are returned as a result must be the same or of a convertible common type. If one is explicitly **NULL**, the result of the function follows the type of the non-**NULL** parameter.

	**IF** (*a*, *b*, *c*) has the same meaning as the **CASE** statement in the following example: ::

		CASE WHEN a IS TRUE THEN b
		ELSE c
		END

	.. code-block:: sql

		SELECT * FROM case_tbl;
					a
		=============
					1
					2
					3
				 NULL
		 
		--IF function returns the second expression when the fist is TRUE
		SELECT a, IF(a=1, 'one', 'other') FROM case_tbl;
					a   if(a=1, 'one', 'other')
		===================================
					1  'one'
					2  'other'
					3  'other'
				 NULL  'other'
		 
		--If function in WHERE clause
		SELECT * FROM case_tbl WHERE IF(a=1, 1, 2) = 1;
					a
		=============
					1

.. function:: IFNULL ( expr1, expr2 )
.. function:: NVL ( expr1, expr2 )

	The **IFNULL** function is working like the **NVL** function; however, only the **NVL** function supports collection type as well. The **IFNULL** function (which has two arguments) returns *expr1* if the value of the first expression is not **NULL** or returns *expr2*, otherwise.
	
	Operation is performed by converting the type of every argument into that with the highest priority. If there is an argument whose type cannot be converted, the type of every argument is converted into a **VARCHAR** type. The following list shows priority of conversion based on input argument type.

	*   **CHAR** < **VARCHAR**
	*   **NCHAR** < **NCHAR VARING**
	*   **BIT** < **VARBIT**
	*   **SHORT** < **INT** < **BIGINT** < **NUMERIC** < **FLOAT** < **DOUBLE**
	*   **DATE** < **TIMESTAMP** < **DATETIME**

	For example, if a type of a is **INT** and b is **BIGINT**, then **IFNULL** (a, b) returns a **BIGINT** type. If a type of a is **INTEGER** and b is **TIMESTAMP**, then **IFNULL** (a, b) returns a **VARCHAR** type.

	**IFNULL** (*a*, *b*) or **NVL** (*a*, *b*) has the same meaning as the **CASE** statement below. ::

		CASE WHEN a IS NULL THEN b
		ELSE a
		END

	.. code-block:: sql

		SELECT * FROM case_tbl;
					a
		=============
					1
					2
					3
				 NULL
		 
		--returning a specific value when a is NULL
		SELECT a, NVL(a, 10.0000) FROM case_tbl;
					a  nvl(a, 10.0000)
		===================================
					1  1.0000
					2  2.0000
					3  3.0000
				 NULL  10.0000
		 
		--IFNULL can be used instead of NVL and return values are converted to the string type
		SELECT a, IFNULL(a, 'UNKNOWN') FROM case_tbl;
					a   ifnull(a, 'UNKNOWN')
		===================================
					1  '1'
					2  '2'
					3  '3'
				 NULL  'UNKNOWN'

.. function:: NULLIF (expr1, expr2)

	The **NULLIF** function returns **NULL** if the two expressions specified as the parameters are identical, and returns the first parameter value otherwise.

	**NULLIF** (*a*, *b*) is the same of the **CASE** statement. ::

		CASE
		WHEN a = b THEN NULL
		ELSE a
		END

	.. code-block:: sql

		SELECT * FROM case_tbl;

		SELECT * FROM case_tbl;
					a
		=============
					1
					2
					3
				 NULL
		 
		--returning NULL value when a is 1
		SELECT a, NULLIF(a, 1) FROM case_tbl;
					a  nullif(a, 1)
		===========================
					1          NULL
					2             2
					3             3
				 NULL          NULL
		 
		--returning NULL value when arguments are same
		SELECT NULLIF (1, 1.000)  FROM db_root;
		  nullif(1, 1.000)
		======================
		  NULL
		 
		--returning the first value when arguments are not same
		SELECT NULLIF ('A', 'a')  FROM db_root;
		  nullif('A', 'a')
		======================
		  'A'

.. function:: NVL2 ( expr1, expr2, expr3 )

	Three parameters are specified for the **NVL2** function. The second expression (*expr2*) is returned if the first expression (*expr1*) is not **NULL**; the third expression (*expr3*) is returned if it is **NULL**.

	Operation is performed by converting the type of every argument into that with the highest priority. If there is an argument whose type cannot be converted, the type of every argument is converted into a **VARCHAR** type. The following list shows priority of conversion based on input argument type.

	*   **CHAR** < **VARCHAR**
	*   **NCHAR** < **NCHAR VARING**
	*   **BIT** < **VARBIT**
	*   **SHORT** < **INT** < **BIGINT** < **NUMERIC** < **FLOAT** < **DOUBLE**
	*   **DATE** < **TIMESTAMP** < **DATETIME**

	For example, if a type of a is **INT**, b, **BIGINT**, and c, **SHORT**, then **NVL2** (a, b, c) returns a **BIGINT** type. If a type of a is **INTEGER**, b, **DOUBLE**, and c, **TIMESTAMP**, then **NVL2** (a, b, c) returns a **VARCHAR** type.

	.. code-block:: sql
	
		SELECT * FROM case_tbl;
					a
		=============
					1
					2
					3
				 NULL
		 
		--returning a specific value of INT type
		SELECT a, NVL2(a, a+1, 10.5678) FROM case_tbl;
					a  nvl2(a, a+1, 10.5678)
		====================================
					1                      2
					2                      3
					3                      4
				 NULL                     11

.. _basic-cond-expr:

Conditional Expressions
=======================

A conditional expression is an expression that is included in the **WHERE** clause of the **SELECT**, **UPDATE** and **DELETE** statements, and in the **HAVING** clause of the **SELECT** statement. There are simple comparison, **ANY** / **SOME** / **ALL**, **BETWEEN**, **EXISTS**, **IN** / **NOT IN**, **LIKE** and **IS NULL** conditional expressions, depending on the kinds of the operators combined.

A simple comparison conditional expression compares two comparable data values. Expressions or subqueries are specified as operands, and the conditional expression always returns
**NULL** if one of the operands is **NULL**. The following table shows operators that can be used in the simple comparison conditional expressions. For details, see :doc:`/sql/function/comparison_op`.

**Operators for Conditional Expressions**

+-------------------------+----------------------------------------------------------------------------+----------------------------+------------------+
| Comparison Operator     | Description                                                                | Conditional Expression     | Return Value     |
+=========================+============================================================================+============================+==================+
| **=**                   | A value of left operand is the same as that of right operand.              | 1=2                        | 0                |
+-------------------------+----------------------------------------------------------------------------+----------------------------+------------------+
| **<>**                  | A value of left operand is not the same as that of right operand.          | 1<>2                       | 1                |
| ,                       |                                                                            |                            |                  |
| **!=**                  |                                                                            |                            |                  |
+-------------------------+----------------------------------------------------------------------------+----------------------------+------------------+
| **>**                   | A value of left operand is greater than that of right operand.             | 1>2                        | 0                |
+-------------------------+----------------------------------------------------------------------------+----------------------------+------------------+
| **<**                   | A value of left operand is less than that of right operand.                | 1<2                        | 1                |
+-------------------------+----------------------------------------------------------------------------+----------------------------+------------------+
| **>=**                  | A value of left operand is equal to or greater than that of right operand. | 1>=2                       | 0                |
+-------------------------+----------------------------------------------------------------------------+----------------------------+------------------+
| **<=**                  | A value of left operand is equal to or less than that of right operand.    | 1<=2                       | 1                |
+-------------------------+----------------------------------------------------------------------------+----------------------------+------------------+

.. _any-some-all-expr:

ANY/SOME/ALL Conditional Expressions
====================================

A conditional expression that includes quantifiers such as **ANY/SOME/ALL** performs comparison operation on one data value and on some or all values included in the list. A conditional expression that includes **ANY** or **SOME** returns **TRUE** if the value of the data on the left satisfies simple comparison with at least one of the values in the list specified as an operand on the right. A conditional expression that includes **ALL** returns **TRUE** if the value of the data on the left satisfies simple comparison with all values in the list on the right.

When a comparison operation is performed on **NULL** in a conditional expression that includes **ANY** or **SOME**, **UNKNOWN** or **TRUE** is returned as a result; when a comparison operation is performed on **NULL** in a conditional expression that includes **ALL**, **UNKNOWN** or **FALSE** is returned. ::

	expression comp_op SOME expression
	expression comp_op ANY expression
	expression comp_op ALL expression

*   *comp_op* : A comparison operator >, = or <= can be used.
*   *expression* (left): A single-value column, path expression (ex.: *tbl_name.col_name*), constant value or arithmetic function that produces a single value can be used.
*   *expression* (right): A column name, path expression, list (set) of constant values or subquery can be used. A list is a set represented within braces ({}). If a subquery is used, *expression* (left) and comparison operation on all results of the subquery execution is performed.

.. code-block:: sql

	--creating a table
	 
	CREATE TABLE condition_tbl (id int primary key, name char(10), dept_name VARCHAR, salary INT);
	INSERT INTO condition_tbl VALUES(1, 'Kim', 'devel', 4000000);
	INSERT INTO condition_tbl VALUES(2, 'Moy', 'sales', 3000000);
	INSERT INTO condition_tbl VALUES(3, 'Jones', 'sales', 5400000);
	INSERT INTO condition_tbl VALUES(4, 'Smith', 'devel', 5500000);
	INSERT INTO condition_tbl VALUES(5, 'Kim', 'account', 3800000);
	INSERT INTO condition_tbl VALUES(6, 'Smith', 'devel', 2400000);
	INSERT INTO condition_tbl VALUES(7, 'Brown', 'account', NULL);
	 
	--selecting rows where department is sales or devel
	SELECT * FROM condition_tbl WHERE dept_name = ANY{'devel','sales'};
			   id  name                  dept_name                  salary
	======================================================================
				1  'Kim       '          'devel'                   4000000
				2  'Moy       '          'sales'                   3000000
				3  'Jones     '          'sales'                   5400000
				4  'Smith     '          'devel'                   5500000
				6  'Smith     '          'devel'                   2400000
	 
	--selecting rows comparing NULL value in the ALL group conditions
	SELECT * FROM condition_tbl WHERE salary > ALL{3000000, 4000000, NULL};
	There are no results.
	 
	--selecting rows comparing NULL value in the ANY group conditions
	SELECT * FROM condition_tbl WHERE salary > ANY{3000000, 4000000, NULL};
			   id  name                  dept_name                  salary
	======================================================================
				1  'Kim       '          'devel'                   4000000
				3  'Jones     '          'sales'                   5400000
				4  'Smith     '          'devel'                   5500000
				5  'Kim       '          'account'                 3800000
	 
	--selecting rows where salary*0.9 is less than those salary in devel department
	SELECT * FROM condition_tbl WHERE (
	(0.9 * salary) < ALL (SELECT salary FROM condition_tbl
	WHERE dept_name = 'devel')
	);
			   id  name                  dept_name                  salary
	======================================================================
				6  'Smith     '          'devel'                   2400000

.. _between-expr:

BETWEEN Conditional Expression
==============================

The **BETWEEN** conditional expression makes a comparison to determine whether the data value on the left exists between two data values specified on the right. It returns **TRUE** even when the data value on the left is the same as a boundary value of the comparison target range. If **NOT** comes before the **BETWEEN** keyword, the result of a **NOT** operation on the result of the **BETWEEN** operation is returned.

*i* **BETWEEN** *g* **AND** *m* and the compound condition *i* **>= g AND** *i* <= *m* have the same effect. ::

	expression [ NOT ] BETWEEN expression AND expression

*   *expression* : A column name, path expression (ex.: *tbl_name.col_name*), constant value, arithmetic expression or aggregate function can be used. For a character string expression, the conditions are evaluated in alphabetical order. If **NULL** is specified for at least one of the expressions, the **BETWEEN** predicate returns **UNKNOWN** as a result.

.. code-block:: sql

	--selecting rows where 3000000 <= salary <= 4000000
	SELECT * FROM condition_tbl WHERE salary BETWEEN 3000000 AND 4000000;
	SELECT * FROM condition_tbl WHERE (salary >= 3000000) AND (salary <= 4000000);
			   id  name                  dept_name                  salary
	======================================================================
				1  'Kim       '          'devel'                   4000000
				2  'Moy       '          'sales'                   3000000
				5  'Kim       '          'account'                 3800000
	 
	--selecting rows where salary < 3000000 or salary > 4000000
	SELECT * FROM condition_tbl WHERE salary NOT BETWEEN 3000000 AND 4000000;
			   id  name                  dept_name                  salary
	======================================================================
				3  'Jones     '          'sales'                   5400000
				4  'Smith     '          'devel'                   5500000
				6  'Smith     '          'devel'                   2400000
	 
	--selecting rows where name starts from A to E
	SELECT * FROM condition_tbl WHERE name BETWEEN 'A' AND 'E';
			   id  name                  dept_name                  salary
	======================================================================
				7  'Brown     '          'account'                    NULL

.. _exists-expr:

EXISTS Conditional Expression
=============================

The **EXISTS** conditional expression returns **TRUE** if one or more results of the execution of the subquery specified on the right exist, and returns **FALSE** if the result of the operation is an empty set. ::

	EXISTS expression

*   *expression* : Specifies a subquery and compares to determine whether the result of the subquery execution exists. If the subquery does not produce any result, the result of the conditional expression is **FALSE**.

.. code-block:: sql

	--selecting rows using EXISTS and subquery
	SELECT 'raise' FROM db_root WHERE EXISTS(
	SELECT * FROM condition_tbl WHERE salary < 2500000);
	  'raise'
	======================
	  'raise'
	 
	--selecting rows using NOT EXISTS and subquery
	SELECT 'raise' FROM db_root WHERE NOT EXISTS(
	SELECT * FROM condition_tbl WHERE salary < 2500000);
	There are no results.

.. _in-expr:

IN Conditional Expression
=========================

The **IN** conditional expression compares to determine whether the single data value on the left is included in the list specified on the right. That is, the predicate returns **TRUE** if the single data value on the left is an element of the expression specified on the right. If **NOT** comes before the **IN** keyword, the result of a **NOT** operation on the result of the **IN** operation is returned. ::

	expression [ NOT ] IN expression

*   *expression* (left): A single-value column, path expression (ex.: *tbl_name.col_name*), constant value or arithmetic function that produces a single value can be used.
*   *expression* (right): A column name, path expression, list (set) of constant values or subquery can be used. A list is a set represented within parentheses (()) or braces ({}). If a subquery is used, comparison with expression(left) is performed for all results of the subquery execution.

.. code-block:: sql

	--selecting rows where department is sales or devel
	SELECT * FROM condition_tbl WHERE dept_name IN {'devel','sales'};
	SELECT * FROM condition_tbl WHERE dept_name = ANY{'devel','sales'};
			   id  name                  dept_name                  salary
	======================================================================
				1  'Kim       '          'devel'                   4000000
				2  'Moy       '          'sales'                   3000000
				3  'Jones     '          'sales'                   5400000
				4  'Smith     '          'devel'                   5500000
				6  'Smith     '          'devel'                   2400000
	 
	--selecting rows where department is neither sales nor devel
	SELECT * FROM condition_tbl WHERE dept_name NOT IN {'devel','sales'};
			   id  name                  dept_name                  salary
	======================================================================
				5  'Kim       '          'account'                 3800000
				7  'Brown     '          'account'                    NULL

.. _is-null-expr:

IS NULL Conditional Expression
==============================

The **IS NULL** conditional expression compares to determine whether the expression specified on the left is **NULL**, and if it is **NULL**, returns **TRUE** and it can be used in the conditional expression. If **NOT** comes before the **NULL** keyword, the result of a **NOT** operation on the result of the **IS NULL** operation is returned.

	expression IS [ NOT ] NULL

*   *expression* : A single-value column, path expression (ex.: *tbl_name.col_name*), constant value or arithmetic function that produces a single value can be used. 

.. code-block:: sql

	--selecting rows where salary is NULL
	SELECT * FROM condition_tbl WHERE salary IS NULL;
			   id  name                  dept_name                  salary
	======================================================================
				7  'Brown     '          'account'                    NULL
	 
	--selecting rows where salary is NOT NULL
	SELECT * FROM condition_tbl WHERE salary IS NOT NULL;
			   id  name                  dept_name                  salary
	======================================================================
				1  'Kim       '          'devel'                   4000000
				2  'Moy       '          'sales'                   3000000
				3  'Jones     '          'sales'                   5400000
				4  'Smith     '          'devel'                   5500000
				5  'Kim       '          'account'                 3800000
				6  'Smith     '          'devel'                   2400000
	 
	--simple conparison operation returns NULL when operand is NULL
	SELECT * FROM condition_tbl WHERE salary = NULL;
	There are no results.

.. function:: ISNULL (expression)

	The **ISNULL** function performs a comparison to determine if the result of the expression specified as an argument is **NULL**. The function returns 1 if it is **NULL** or 0 otherwise. You can check if a certain value is **NULL**. This function is working like the **ISNULL** expression.

	:param expression: An arithmetic function that has a single-value column, path expression (ex.: *tbl_name.col_name*), constant value is specified.
	:rtype: INT

	.. code-block:: sql

		--Using ISNULL function to select rows with NULL value
		SELECT * FROM condition_tbl WHERE ISNULL(salary);
				   id  name                  dept_name                  salary
		======================================================================
					7  'Brown     '          'account'                    NULL

.. _like-expr:

LIKE Conditional Expression
===========================

The **LIKE** conditional expression compares patterns between character string data, and returns **TRUE** if a character string whose pattern matches the search word is found. Pattern comparison target types are **CHAR**, **VARCHAR** and **STRING**. The **LIKE** search cannot be performed on an **NCHAR** or **BIT** type. If **NOT** comes before the **LIKE** keyword, the result of a **NOT** operation on the result of the **LIKE** operation is returned.

A wild card string corresponding to any character or character string can be included in the search word on the right of the **LIKE** operator. % (percent) and _ (underscore) can be used. .% corresponds to any character string whose length is 0 or greater, and _ corresponds to one character. An escape character is a character that is used to search for a wild card character itself, and can be specified by the user as another character (**NULL**, alphabet, or number whose length is 1. See below for an example of using a character string that includes wild card or escape characters. ::

	expression [ NOT ] LIKE expression [ ESCAPE char]

*   *expression* (left): Specifies the data type column of the character string. Pattern comparison, which is case-sensitive, starts from the first character of the column.
*   *expression* (right): Enters the search word. A character string with a length of 0 or greater is required. Wild card characters (% or _) can be included as the pattern of the search word. The length of the character string is 0 or greater.
*   **ESCAPE** *char* : **NULL**, alphabet, or number is allowed for *char*. If the string pattern of the search word includes "_" or "%" itself, an ESCAPE character must be specified. For example, if you want to search for the character string "10%" after specifying backslash (\) as the ESCAPE character, you must specify "10\%" for the expression (right). If you want to search for the character string "C:\", you can specify "C:\\" for the expression (right).

**Remark**

The **LIKE** conditional expression is case sensitive. To disable case sensitive, use the :ref:`regexp-rlike`.

For details about character sets supported in CUBRID, see :ref:`char-data-type`.

Whether to detect the escape characters of the LIKE conditional expression is determined depending on the configuration of **no_backslash_escapes** and **require_like_escape_character** in the **cubrid.conf** file. For details, see :ref:`stmt-type-parameters`.

.. note::

	In CUBRID 2008 R4.x or lower versions, to execute string comparison operation for data entered in the multibyte charset environment such as UTF-8, the parameter setting (**single_byte_compare** = yes) which compares strings by 1 byte should be added to the **cubrid.conf** file for a successful search result. However, the versions after CUBRID 2008 R4.x support Unicode charset so the **single_byte_compare** parameter is no longer used.

.. code-block:: sql

	--selection rows where name contains lower case 's', not upper case
	SELECT * FROM condition_tbl WHERE name LIKE '%s%';
			   id  name                  dept_name                  salary
	======================================================================
				3  'Jones     '          'sales'                   5400000
	 
	--selection rows where second letter is 'O' or 'o'
	SELECT * FROM condition_tbl WHERE UPPER(name) LIKE '_O%';
			   id  name                  dept_name                  salary
	======================================================================
				2  'Moy       '          'sales'                   3000000
				3  'Jones     '          'sales'                   5400000
	 
	--selection rows where name is 3 characters
	SELECT * FROM condition_tbl WHERE name LIKE '___';
			   id  name                  dept_name                  salary
	======================================================================
				1  'Kim       '          'devel'                   4000000
				2  'Moy       '          'sales'                   3000000
				5  'Kim       '          'account'                 3800000

.. _regexp-rlike:

REGEXP/RLIKE Conditional Expressions
====================================

The conditional expressions **REGEXP** and **RLIKE** are used interchangeably; a regular expressions is a powerful way to specify a pattern for a complex search. CUBRID uses Henry Spencer's implementation of regular expressions, which conforms the POSIX 1003.2 standards. The details on regular expressions are not described in this page. For more information on regular expressions, see Henry Spencer's regex(7).

The following list describes basic characteristics of regular expressions.

*   "." matches any single character(including new-line and carriage-return).
*   "[...]" matches one of characters within square brackets. For example, "[abc]" matches "a", "b", or "c". To represent a range of characters, use a dash (-). "[a-z]" matches any alphabet letter whereas "[0-9]" matches any single number.
*   "*" matches 0 or more instances of the thing proceeding it. For example, "xabc*" matches "xab", "xabc", "xabcc", and "xabcxabc" etc. "[0-9][0-9]*" matches any numbers, and ".*" matches every string.
*   To match special characters such as "\n", "\t", "\r", and "\\", some must be escaped with the backslash (\) by specifying the value of **no_backslash_escapes** (default: yes) to **no**. For details on **no_backslash_escapes**, see :ref:`escape-characters`.

The difference between **REGEXP** and **LIKE** are as follows:

*   The **LIKE** operator succeeds only if the pattern matches the entire value.
*   The **REGEXP** operator succeeds if the pattern matches anywhere in the value. To match the entire value, you should use "^" at the beginning and "$" at the end.
*   The **LIKE** operator is case sensitive, but patterns of regular expressions in **REGEXP** is not case sensitive. To enable case sensitive, you should use **REGEXP BINARY** statement.

In the syntax below, if *expr* matches *pat*, 1 is returned; otherwise, 0 is returned. If either *expr* or *pat* is **NULL**, **NULL** is returned.

The second syntax has the same meaning as the third syntax, which both syntaxes are using **NOT**.

::

	expr REGEXP|RLIKE [BINARY] pat
	expr NOT REGEXP|RLIKE pat
	NOT (expr REGEXP|RLIKE pat)

*   *expr* : Column or input expression
*   *pat* : Pattern used in regular expressions; not case sensitive

.. code-block:: sql

	-- When REGEXP is used in SELECT list, enclosing this with parentheses is required. But used in WHERE clause, no need parentheses.
	-- case insensitive, except when used with BINARY.
	SELECT name FROM athlete where name REGEXP '^[a-d]';
	name
	======================
	'Dziouba Irina'
	'Dzieciol Iwona'
	'Dzamalutdinov Kamil'
	'Crucq Maurits'
	'Crosta Daniele'
	'Bukovec Brigita'
	'Bukic Perica'
	'Abdullayev Namik'
	 
	-- \n : match a special character, when no_backslash_escapes=no
	SELECT ('new\nline' REGEXP 'new
	line');
	('new
	line' regexp 'new
	line')
	=====================================
	1
	 
	-- ^ : match the beginning of a string
	SELECT ('cubrid dbms' REGEXP '^cub');
	('cubrid dbms' regexp '^cub')
	===============================
	1
	 
	-- $ : match the end of a string
	SELECT ('this is cubrid dbms' REGEXP 'dbms$');
	('this is cubrid dbms' regexp 'dbms$')
	========================================
	1
	 
	--.: match any character
	SELECT ('cubrid dbms' REGEXP '^c.*$');
	('cubrid dbms' regexp '^c.*$')
	================================
	1
	 
	-- a+ : match any sequence of one or more a characters. case insensitive.
	SELECT ('Aaaapricot' REGEXP '^A+pricot');
	('Aaaapricot' regexp '^A+pricot')
	================================
	1
	 
	-- a? : match either zero or one a character.
	SELECT ('Apricot' REGEXP '^Aa?pricot');
	('Apricot' regexp '^Aa?pricot')
	==========================
	1
	SELECT ('Aapricot' REGEXP '^Aa?pricot');
	('Aapricot' regexp '^Aa?pricot')
	===========================
	1
	 
	SELECT ('Aaapricot' REGEXP '^Aa?pricot');
	('Aaapricot' regexp '^Aa?pricot')
	============================
	0
	 
	-- (cub)* : match zero or more instances of the sequence abc.
	SELECT ('cubcub' REGEXP '^(cub)*$');
	('cubcub' regexp '^(cub)*$')
	==========================
	1
	 
	-- [a-dX], [^a-dX] : matches any character that is (or is not, if ^ is used) either a, b, c, d or X.
	SELECT ('aXbc' REGEXP '^[a-dXYZ]+');
	('aXbc' regexp '^[a-dXYZ]+')
	==============================
	1
	 
	SELECT ('strike' REGEXP '^[^a-dXYZ]+$');
	('strike' regexp '^[^a-dXYZ]+$')
	================================

**Remark**

The following shows RegEx-Specer's license, which is library used to implement the **REGEXP** conditional expression. ::

	Copyright 1992, 1993, 1994 Henry Spencer. All rights reserved.
	This software is not subject to any license of the American Telephone
	and Telegraph Company or of the Regents of the University of California.
	 
	Permission is granted to anyone to use this software for any purpose on
	any computer system, and to alter it and redistribute it, subject
	to the following restrictions:
	 
	1. The author is not responsible for the consequences of use of this
	software, no matter how awful, even if they arise from flaws in it.
	 
	2. The origin of this software must not be misrepresented, either by
	explicit claim or by omission. Since few users ever read sources,
	credits must appear in the documentation.
	 
	3. Altered versions must be plainly marked as such, and must not be
	misrepresented as being the original software. Since few users
	ever read sources, credits must appear in the documentation.
	 
	4. This notice may not be removed or altered.
