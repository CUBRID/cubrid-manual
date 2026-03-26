
:meta-keywords: cubrid any, cubrid some, cubrid all, cubrid between, cubrid exists, cubrid in, cubrid like, cubrid regexp, cubrid rlike, cubrid case clause
:meta-description: A comparison expression is an expression that is included in the WHERE clause of the SELECT, UPDATE and DELETE statements, and in the HAVING clause of the SELECT statement.

***********
비교 연산식
***********

.. _basic-cond-expr:

단순 비교 조건식
================

비교 조건식은 **SELECT**, **UPDATE**, **DELETE** 문의 **WHERE** 절과 **SELECT** 문의 **HAVING** 절에 포함되는 표현식으로서, 결합되는 연산자의 종류에 따라 단순 비교 조건식, **ANY** / **SOME** / **ALL** 조건식, **BETWEEN** 조건식, **EXISTS** 조건식, **IN** / **NOT IN** 조건식, **LIKE** 조건식, **IS NULL** 조건식이 있다.

먼저, 단순 비교 조건식은 두 개의 비교 가능한 데이터 값을 비교한다. 피연산자로 일반 연산식(expression) 또는 부질의(sub-query)가 지정되며, 피연산자 중 어느 하나가 **NULL** 이면 항상 **NULL** 을 반환한다. 단순 비교 조건식에서 사용할 수 있는 연산자는 아래의 표와 같으며, 보다 자세한 내용은 :doc:`/sql/function/comparison_op` 를 참고한다.

**단순 비교 조건식에서 사용할 수 있는 연산자**

+-------------+-------------------------------------------------------+---------+----------+
| 비교 연산자 | 설명                                                  | 조건식  | 리턴 값  |
+=============+=======================================================+=========+==========+
| **=**       | 왼쪽 및 오른쪽 피연산자의 값이 같다.                  | 1=2     | 0        |
+-------------+-------------------------------------------------------+---------+----------+
| **<>**      | 왼쪽 및 오른쪽 피연산자의 값이 다르다.                | 1<>2    | 1        |
| ,           |                                                       |         |          |
| **!=**      |                                                       |         |          |
+-------------+-------------------------------------------------------+---------+----------+
| **>**       | 왼쪽 피연산자는 오른쪽 피연산자보다 값이 크다.        | 1>2     | 0        |
+-------------+-------------------------------------------------------+---------+----------+
| **<**       | 왼쪽 피연산자는 오른쪽 피연산자보다 값이 작다.        | 1<2     | 1        |
+-------------+-------------------------------------------------------+---------+----------+
| **>=**      | 왼쪽 피연산자는 오른쪽 피연산자보다 값이 크거나 같다. | 1>=2    | 0        |
+-------------+-------------------------------------------------------+---------+----------+
| **<=**      | 왼쪽 피연산자는 오른쪽 피연산자보다 값이 작거나 같다. | 1<=2    | 1        |
+-------------+-------------------------------------------------------+---------+----------+

.. _any-some-all-expr:

ANY/SOME/ALL 수량어와 그룹 조건식
=================================

**ANY** / **SOME** / **ALL** 과 같은 수량어를 포함하는 그룹 조건식은 하나의 데이터 값과 리스트에 포함된 값들의 일부 또는 모든 값에 대해서 비교 연산을 수행한다. 즉, **ANY** 또는 **SOME** 이 포함된 그룹 조건식은, 왼쪽의 데이터 값이 오른쪽 피연산자로 지정된 리스트 내의 값 중 최소한 하나에 대해 단순 비교 연산자를 만족할 때 **TRUE** 를 반환한다. 한편, **ALL** 이 포함된 그룹 조건식의 경우, 왼쪽 데이터 값이 오른쪽 리스트 내의 모든 값들에 대해 단순 비교 연산자를 만족할 때 **TRUE** 를 반환한다.

만약, **ANY** 또는 **SOME** 을 포함하는 그룹 조건식에서 **NULL** 을 대상으로 비교 연산을 수행하면 그룹 조건식의 결과로 **UNKNOWN** 또는 **TRUE** 를 반환하고, **ALL** 을 포함하는 그룹 조건식에서 **NULL** 을 대상으로 비교 연산을 수행하면 **UNKNOWN** 또는 **FALSE** 를 반환한다. ::

    expression comp_op SOME expression
    expression comp_op ANY expression
    expression comp_op ALL expression

*   *comp_op* : 비교 연산자 **>**, **<**, **=**, **>=**, **<=** 가 들어갈 수 있다.
*   *expression* (왼쪽) : 단일 값을 가지는 칼럼, 경로 표현식(예: *tbl_name.col_name*), 상수 값 또는 단일 값을 생성하는 산술 함수가 될 수 있다.
*   *expression* (오른쪽) : 칼럼 이름, 경로 표현식, 상수 값의 리스트(집합), 부질의가 될 수 있다. 리스트는 중괄호({}) 안에 표현된 집합을 의미하며, 부질의가 사용되면 부질의의 수행 결과 전부에 대해서 *expression* (왼쪽)와 비교 연산을 수행한다.

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
    
::    
    
               id  name                  dept_name                  salary
    ======================================================================
                1  'Kim       '          'devel'                   4000000
                2  'Moy       '          'sales'                   3000000
                3  'Jones     '          'sales'                   5400000
                4  'Smith     '          'devel'                   5500000
                6  'Smith     '          'devel'                   2400000
     
.. code-block:: sql

    --selecting rows comparing NULL value in the ALL group conditions
    SELECT * FROM condition_tbl WHERE salary > ALL{3000000, 4000000, NULL};

::
    
    There are no results.
     
.. code-block:: sql

    --selecting rows comparing NULL value in the ANY group conditions
    SELECT * FROM condition_tbl WHERE salary > ANY{3000000, 4000000, NULL};

::
    
               id  name                  dept_name                  salary
    ======================================================================
                1  'Kim       '          'devel'                   4000000
                3  'Jones     '          'sales'                   5400000
                4  'Smith     '          'devel'                   5500000
                5  'Kim       '          'account'                 3800000
     
.. code-block:: sql

    --selecting rows where salary*0.9 is less than those salary in devel department
    SELECT * FROM condition_tbl WHERE (
      (0.9 * salary) < ALL (SELECT salary FROM condition_tbl
      WHERE dept_name = 'devel')
    );

::
    
               id  name                  dept_name                  salary
    ======================================================================
                6  'Smith     '          'devel'                   2400000

.. _between-expr:

BETWEEN
=======

**BETWEEN** 조건식은 왼쪽의 데이터 값이 오른쪽에 지정된 두 데이터 값 사이에 존재하는지 비교한다. 이때, 왼쪽의 데이터 값이 비교 대상 범위의 경계값과 동일한 경우에도 **TRUE** 를 반환한다. 한편, **BETWEEN** 키워드 앞에 **NOT** 이 오면 **BETWEEN** 연산의 결과에 **NOT** 연산을 수행하여 결과를 반환한다.

*i* **BETWEEN** *g* **AND** *m* 은 복합 조건식 *i* **>=** *g* **AND** *i* **<=** *m* 과 동일하다.

::

    expression [ NOT ] BETWEEN expression AND expression

*   *expression* : 칼럼 이름, 경로 표현식(예: *tbl_name.col_name*), 상수 값, 산술 표현식, 집계 함수가 될 수 있다. 문자열 표현식인 경우에는 문자의 사전순으로 조건이 평가된다. 표현식 중 하나라도 **NULL** 이 지정되면 **BETWEEN** 조건식의 결과는 **FALSE** 또는 **UNKNOWN** 을 반환한다.

.. code-block:: sql

    --selecting rows where 3000000 <= salary <= 4000000
    SELECT * FROM condition_tbl WHERE salary BETWEEN 3000000 AND 4000000;
    SELECT * FROM condition_tbl WHERE (salary >= 3000000) AND (salary <= 4000000);
    
::
    
               id  name                  dept_name                  salary
    ======================================================================
                1  'Kim       '          'devel'                   4000000
                2  'Moy       '          'sales'                   3000000
                5  'Kim       '          'account'                 3800000
     
.. code-block:: sql

    --selecting rows where salary < 3000000 or salary > 4000000
    SELECT * FROM condition_tbl WHERE salary NOT BETWEEN 3000000 AND 4000000;
    
::

               id  name                  dept_name                  salary
    ======================================================================
                3  'Jones     '          'sales'                   5400000
                4  'Smith     '          'devel'                   5500000
                6  'Smith     '          'devel'                   2400000
     
.. code-block:: sql

    --selecting rows where name starts from A to E
    SELECT * FROM condition_tbl WHERE name BETWEEN 'A' AND 'E';

::

               id  name                  dept_name                  salary
    ======================================================================
                7  'Brown     '          'account'                    NULL

.. _exists-expr:

EXISTS
======

**EXISTS** 조건식은 오른쪽에 지정되는 부질의를 실행한 결과가 하나 이상 존재하면 **TRUE** 를 반환하고, 연산 실행 결과가 공집합이면 **FALSE** 를 반환한다. ::

    EXISTS expression

*   *expression* : 부질의가 지정되며, 부질의 실행 결과가 존재하는지 비교한다. 만약 부질의가 어떤 결과도 만들지 않는다면 조건식 결과는 **FALSE** 이다.

.. code-block:: sql

    --selecting rows using EXISTS and subquery
    SELECT 'raise' FROM db_root WHERE EXISTS(
    SELECT * FROM condition_tbl WHERE salary < 2500000);
    
::
    
      'raise'
    ======================
      'raise'
     
.. code-block:: sql

    --selecting rows using NOT EXISTS and subquery
    SELECT 'raise' FROM db_root WHERE NOT EXISTS(
    SELECT * FROM condition_tbl WHERE salary < 2500000);

::

    There are no results.

.. _in-expr:

IN
==

**IN** 조건식은 왼쪽의 단일 데이터 값이 오른쪽에 지정된 리스트 내에 포함되어 있는지 비교한다. 즉, 왼쪽의 단일 데이터 값이 오른쪽에 지정된 표현식의 원소이면 **TRUE** 를 반환한다. **IN** 키워드 앞에 **NOT** 이 있으면 **IN** 연산의 결과에 **NOT** 연산을 수행하여 결과를 반환한다. ::

    expression [ NOT ] IN expression

*   *expression* (left) : 단일 값을 가지는 칼럼, 경로 표현식, 상수 값 또는 단일 값을 생성하는 산술 함수가 될 수 있다.
*   *expression* (right) : 칼럼 이름, 경로 표현식(예: *tbl_name.col_name*), 상수 값의 리스트(집합), 부질의가 될 수 있다. 리스트는 소괄호(()) 또는 중괄호({}) 안에 표현된 집합을 의미하며, 부질의가 사용되면 부질의의 수행 결과 전부에 대해서 *expression* (left)와 비교 연산을 수행한다.

.. code-block:: sql

    --selecting rows where department is sales or devel
    SELECT * FROM condition_tbl WHERE dept_name IN {'devel','sales'};
    SELECT * FROM condition_tbl WHERE dept_name = ANY{'devel','sales'};
    
::
    
               id  name                  dept_name                  salary
    ======================================================================
                1  'Kim       '          'devel'                   4000000
                2  'Moy       '          'sales'                   3000000
                3  'Jones     '          'sales'                   5400000
                4  'Smith     '          'devel'                   5500000
                6  'Smith     '          'devel'                   2400000
     
.. code-block:: sql

    --selecting rows where department is neither sales nor devel
    SELECT * FROM condition_tbl WHERE dept_name NOT IN {'devel','sales'};
    
::

               id  name                  dept_name                  salary
    ======================================================================
                5  'Kim       '          'account'                 3800000
                7  'Brown     '          'account'                    NULL

.. _is-null-expr:

IS NULL
=======

**IS NULL** 조건식은 왼쪽에 지정된 표현식의 결과가 **NULL** 인지 비교하여, **NULL** 인 경우 **TRUE** 를 반환하며, 조건절 내에서 사용할 수 있다. **NULL** 키워드 앞에 **NOT** 이 있으면 **IS NULL** 연산의 결과에 **NOT** 연산을 수행하여 결과를 반환한다. ::

    expression IS [ NOT ] NULL

*   *expression* : 단일 값을 가지는 칼럼, 경로 표현식(예: *tbl_name.col_name*), 상수 값 또는 단일 값을 생성하는 산술 함수가 될 수 있다.

.. code-block:: sql

    --selecting rows where salary is NULL
    SELECT * FROM condition_tbl WHERE salary IS NULL;
    
::
    
               id  name                  dept_name                  salary
    ======================================================================
                7  'Brown     '          'account'                    NULL
     
.. code-block:: sql

    --selecting rows where salary is NOT NULL
    SELECT * FROM condition_tbl WHERE salary IS NOT NULL;
    
::

               id  name                  dept_name                  salary
    ======================================================================
                1  'Kim       '          'devel'                   4000000
                2  'Moy       '          'sales'                   3000000
                3  'Jones     '          'sales'                   5400000
                4  'Smith     '          'devel'                   5500000
                5  'Kim       '          'account'                 3800000
                6  'Smith     '          'devel'                   2400000
     
.. code-block:: sql

    --simple comparison operation returns NULL when operand is NULL
    SELECT * FROM condition_tbl WHERE salary = NULL;
    
::

    There are no results.

.. _like-expr:

LIKE
====

**LIKE** 조건식은 문자열 데이터 간의 패턴을 비교하는 연산을 수행하여, 검색어와 일치하는 패턴의 문자열이 검색되면 **TRUE** 를 반환한다. 패턴 비교 대상이 되는 타입은 **CHAR**, **VARCHAR**, **STRING** 이며, **BIT** 타입에 대해서는 **LIKE** 검색을 수행할 수 없다. **LIKE** 키워드 앞에 **NOT** 이 있으면 **LIKE** 연산의 결과에 **NOT** 연산을 수행하여 결과를 반환한다.

**LIKE** 연산자 오른쪽에 오는 검색어에는 임의의 문자 또는 문자열에 대응되는 와일드 카드(wild card) 문자열을 포함할 수 있으며, **%** (percent)와 **_** (underscore)를 사용할 수 있다. **%** 는 길이가 0 이상인 임의의 문자열에 대응되며, **_** 는 1개의 문자에 대응된다. 또한, 이스케이프 문자(escape character)는 와일드 카드 문자 자체에 대한 검색을 수행할 때 사용되는 문자로서, 사용자에 의해 길이가 1인 다른 문자(**NULL**, 알파벳 또는 숫자)로 지정될 수 있다. 와일드 카드 문자 또는 이스케이프 문자를 포함하는 문자열을 검색어로 사용하는 예제는 아래를 참고한다. ::

    expression [ NOT ] LIKE pattern [ ESCAPE char ]

*   *expression*\ : 문자열 데이터 타입 칼럼이 지정된다. 패턴 비교는 칼럼 값의 첫 번째 문자부터 시작되며, 대소문자를 구분한다.
*   *pattern*\ : 검색어를 입력하며, 길이가 0 이상인 문자열이 된다. 이때, 검색어 패턴에는 와일드 카드 문자(**%** 또는 **_**)가 포함될 수 있다. 문자열의 길이는 0 이상이다.
*   **ESCAPE** *char* : *char* 에 올 수 있는 문자는 **NULL**, 알파벳, 숫자이다. 만약 검색어의 문자열 패턴이 "_" 또는 "%" 자체를 포함하는 경우 이스케이프 문자가 반드시 지정되어야 한다. 예를 들어, 이스케이프 문자를 백슬래시(\\)로 지정한 후 '10%'인 문자열을 검색하고자 한다면, *pattern*\ 에 '10\%'을 지정해야 한다. 또한, 'C:\\'인 문자열을 검색하고자 한다면, *pattern*\ 에 'C:\\ '을 지정하면 된다.

CUBRID가 지원하는 문자셋에 관한 상세한 설명은 :ref:`char-data-type` 을 참고한다.

LIKE 조건식의 이스케이프 문자 인식은 **cubrid.conf** 파일의 **no_backslash_escapes** 파라미터와 **require_like_escape_character** 파라미터의 설정에 따라 달라진다. 이에 대한 상세한 설명은 :ref:`stmt-type-parameters` 를 참고한다.

.. note::

    *   CUBRID 9.0 미만 버전에서는 UTF-8과 같은 멀티바이트 문자셋 환경에서 입력된 데이터에 대해 문자열 비교 연산을 수행하려면, 1바이트 단위로 문자열 비교를 수행하도록 하는 파라미터(**single_byte_compare** = yes)를 **cubrid.conf** 파일에 추가해야 정상적인 검색 결과를 얻을 수 있다.

    *   CUBRID 9.0 이상 버전에서는 유니코드 문자셋을 지원하므로 **single_byte_compare** 파라미터를 더 이상 사용하지 않는다.

.. code-block:: sql

    --selection rows where name contains lower case 's', not upper case
    SELECT * FROM condition_tbl WHERE name LIKE '%s%';
    
::

               id  name                  dept_name                  salary
    ======================================================================
                3  'Jones     '          'sales'                   5400000
     
.. code-block:: sql

    --selection rows where second letter is 'O' or 'o'
    SELECT * FROM condition_tbl WHERE UPPER(name) LIKE '_O%';
    
::

               id  name                  dept_name                  salary
    ======================================================================
                2  'Moy       '          'sales'                   3000000
                3  'Jones     '          'sales'                   5400000
     
.. code-block:: sql

    --selection rows where name is 3 characters
    SELECT * FROM condition_tbl WHERE name LIKE '___';
    
::

               id  name                  dept_name                  salary
    ======================================================================
                1  'Kim       '          'devel'                   4000000
                2  'Moy       '          'sales'                   3000000
                5  'Kim       '          'account'                 3800000

.. _regexp-rlike:

REGEXP, RLIKE
=============

**REGEXP**, **RLIKE**\ 는 동일하며, 정규 표현식을 이용한 패턴을 매칭하기 위해 사용된다. 정규 표현식은 복잡한 검색 패턴을 표현하는 강력한 방법이다. CUBRID는 Henry Spencer가 구현한 정규 표현식을 사용하며, 이는 POSIX 1003.2 표준을 따른다. 이 페이지는 정규 표현식에 대한 세부 사항을 설명하지는 않으므로, 정규 표현식에 대한 자세한 사항은 Henry Spencer의 regex(7)을 참고한다.

다음은 정규 표현식 패턴의 일부이다.

*   "."은 문자 하나와 매칭된다(줄바꿈 문자(new line)와 캐리지 리턴 문자(carriage return)를 포함).

*   "[...]"은 대괄호 안의 문자 중 하나와 매칭된다. 예를 들어, "[abc]"는 "a", "b" 또는 "c"와 매칭된다. 문자의 범위를 나타내려면 대시(-)를 사용한다. "[a-z]"은 임의의 알파벳 문자 하나와 매칭되고, "[0-9]"는 임의의 숫자 하나와 매칭된다.

*   "*"은 앞의 문자 또는 문자열이 0번 이상 연속으로 나열된 문자열과 매칭된다. 예를 들어, "xabc*"는 "xab", "xabc", "xabcc", "xabcxabc" 등과 매칭되며, "[0-9][0-9]*"는 어떤 숫자와도 매칭된다. 그리고 ".*"은 모든 문자열과 매칭된다.

*   "\\n", "\\t", "\\r", "\\"의 특수 문자를 매칭하기 위해서는 시스템 파라미터 **no_backslash_escapes** (기본값: yes)를 no로 설정하여 백슬래시(\\)를 이스케이프 문자로 허용해야 한다. **no_backslash_escapes**\ 에 대한 자세한 설명은 :ref:`escape-characters`\ 를 참고한다.

**REGEXP**\ 와 **LIKE**\ 의 차이는 다음과 같다.

* **LIKE** 절은 입력값 전체가 패턴과 매칭되어야 성공한다.
* **REGEXP**\ 는 입력값의 일부가 패턴과 매칭되면 성공한다. **REGEXP**\ 에서 전체 값에 대한 패턴 매칭을 하려면, 패턴의 시작에는 "^"을, 끝에는 "$"을 사용해야 한다.
* **LIKE** 절의 패턴은 대소문자를 구분하지만 **REGEXP**\ 에서 정규 표현식의 패턴은 대소문자를 구분하지 않는다. 대소문자를 구분하려면 **REGEXP BINARY** 구문을 사용해야 한다.
* **REGEXP**, **REGEXP BINARY**\ 는 피연산자의 콜레이션을 고려하지 않고 ASCII 인코딩으로 동작한다. 

.. code-block:: sql
    
    SELECT ('a' collate utf8_en_ci REGEXP BINARY 'A' collate utf8_en_ci); 

::

    0

.. code-block:: sql
    
    SELECT ('a' collate utf8_en_cs REGEXP BINARY 'A' collate utf8_en_cs); 

::

    0
    
.. code-block:: sql

    SELECT ('a' COLLATE iso88591_bin REGEXP 'A' COLLATE iso88591_bin);

::

    1
    
.. code-block:: sql

    SELECT ('a' COLLATE iso88591_bin REGEXP BINARY 'A' COLLATE iso88591_bin);

::

    0

아래 구문에서 *expression*\ 에 매칭되는 패턴 *pattern*\ 이 존재하면 1을 반환하며, 그렇지 않은 경우 0을 반환한다. *expression*\ 과 *pattern* 중 하나가 **NULL**\ 이면 **NULL**\ 을 반환한다.

**NOT**\ 을 사용하는 두 번째 구문과 세 번째 구문은 같은 의미이다.

::

    expression REGEXP | RLIKE [BINARY] pattern
    expression NOT REGEXP | RLIKE pattern
    NOT (expression REGEXP | RLIKE pattern)

*   *expression* : 칼럼 또는 입력 표현식
*   *pattern* : 정규 표현식에 사용될 패턴. 대소문자 구분 없음

.. code-block:: sql

    -- When REGEXP is used in SELECT list, enclosing this with parentheses is required. 
    -- But used in WHERE clause, no need parentheses.
    -- case insensitive, except when used with BINARY.
    SELECT name FROM athlete where name REGEXP '^[a-d]';

::
    
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
     
.. code-block:: sql

    -- \n : match a special character, when no_backslash_escapes=no
    SELECT ('new\nline' REGEXP 'new
    line');


::
    
    ('new
    line' regexp 'new
    line')
    =====================================
    1
     
.. code-block:: sql

    -- ^ : match the beginning of a string
    SELECT ('cubrid dbms' REGEXP '^cub');
    
::

    ('cubrid dbms' regexp '^cub')
    ===============================
    1
     
.. code-block:: sql

    -- $ : match the end of a string
    SELECT ('this is cubrid dbms' REGEXP 'dbms$');
    
::

    ('this is cubrid dbms' regexp 'dbms$')
    ========================================
    1
     
.. code-block:: sql

    --.: match any character
    SELECT ('cubrid dbms' REGEXP '^c.*$');
    
::

    ('cubrid dbms' regexp '^c.*$')
    ================================
    1
     
.. code-block:: sql

    -- a+ : match any sequence of one or more a characters. case insensitive.
    SELECT ('Aaaapricot' REGEXP '^A+pricot');
    
::

    ('Aaaapricot' regexp '^A+pricot')
    ================================
    1
     
.. code-block:: sql

    -- a? : match either zero or one a character.
    SELECT ('Apricot' REGEXP '^Aa?pricot');
    
::

    ('Apricot' regexp '^Aa?pricot')
    ==========================
    1
    
.. code-block:: sql

    SELECT ('Aapricot' REGEXP '^Aa?pricot');
    
::

    ('Aapricot' regexp '^Aa?pricot')
    ===========================
    1
     
.. code-block:: sql

    SELECT ('Aaapricot' REGEXP '^Aa?pricot');
    
::

    ('Aaapricot' regexp '^Aa?pricot')
    ============================
    0
     
.. code-block:: sql

    -- (cub)* : match zero or more instances of the sequence abc.
    SELECT ('cubcub' REGEXP '^(cub)*$');
    
::

    ('cubcub' regexp '^(cub)*$')
    ==========================
    1
     
.. code-block:: sql

    -- [a-dX], [^a-dX] : matches any character that is (or is not, if ^ is used) either a, b, c, d or X.
    SELECT ('aXbc' REGEXP '^[a-dXYZ]+');
    
::

    ('aXbc' regexp '^[a-dXYZ]+')
    ==============================
    1
     
.. code-block:: sql

    SELECT ('strike' REGEXP '^[^a-dXYZ]+$');
    
::

    ('strike' regexp '^[^a-dXYZ]+$')
    ================================
    1

.. note::

    다음은 **REGEXP** 조건식을 구현하기 위해 사용한 라이브러리인 RegEx-Specer의 라이선스이다. ::

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

.. _case-expr:

CASE
====

**CASE** 연산식은 **IF** ... **THEN** ... **ELSE** 로직을 SQL 문장으로 표현하며, **WHEN** 에 지정된 비교 연산 결과가 참이면 **THEN** 절의 값을 반환하고 거짓이면 **ELSE** 절에 명시된 값을 반환한다. 만약, **ELSE** 절이 없다면 **NULL** 값을 반환한다. ::

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

**CASE** 조건식은 반드시 키워드 **END** 로 끝나야 하며, *control_expression* 과 데이터 타입과 *simple_when* 절 내의 *expression* 은 비교 가능한 데이터 타입이어야 한다. 또한, **THEN** 과 **ELSE** 절에 지정된 모든 *result* 의 데이터 타입은 서로 같거나, 어느 하나의 공통 데이터 타입으로 변환 가능(convertible)해야 한다.

**CASE** 수식이 반환하는 값의 데이터 타입은 다음과 같은 규칙에 따라 결정된다.

*   **THEN** 절에 명시된 모든 *result* 의 데이터 타입이 같으면, 해당 타입이 리턴 값의 데이터 타입이 된다.
*   모든 *result* 의 데이터 타입이 같지 않더라도 어느 하나의 공통 데이터 타입으로 변환 가능하면, 해당 타입이 리턴 값의 데이터 타입이 된다.
*   *result* 중 어느 하나가 가변 길이 문자열인 경우, 리턴 값의 데이터 타입은 가변 길이 문자열이 된다. 또한, *result* 가 모두 고정 길이 문자열인 경우에는 가장 긴 길이를 가지는 문자열 또는 비트열이 결과로 반환된다.
*   *result* 중 어느 하나가 근사치로 표현되는 수치형이면, 근사치로 표현되고 이때 소수점 이하 자릿수는 모든 *result* 의 유효 숫자를 표현할 수 있도록 결정된다.

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
    
::

                a  case when a=1 then 'one' when a=2 then 'two' else 'other' end
    ===================================
                1  'one'
                2  'two'
                3  'other'
             NULL  'other'
     
.. code-block:: sql

    --case operation with a simple when clause
    SELECT a,
           CASE a WHEN 1 THEN 'one'
                  WHEN 2 THEN 'two'
                  ELSE 'other'
           END
    FROM case_tbl;
    
::

                a  case a when 1 then 'one' when 2 then 'two' else 'other' end
    ===================================
                1  'one'
                2  'two'
                3  'other'
             NULL  'other'
     
.. code-block:: sql

    --result types are converted to a single type containing all of significant figures
    SELECT a,
           CASE WHEN a=1 THEN 1
                WHEN a=2 THEN 1.2345
                ELSE 1.234567890
           END
    FROM case_tbl;
    
::

                a  case when a=1 then 1 when a=2 then 1.2345 else 1.234567890 end
    ===================================
                1  1.000000000
                2  1.234500000
                3  1.234567890
             NULL  1.234567890
     
.. code-block:: sql

    --an error occurs when result types are not convertible
    SELECT a,
           CASE WHEN a=1 THEN 'one'
                WHEN a=2 THEN 'two'
                ELSE 1.2345
           END
    FROM case_tbl;
    
::

    ERROR: Cannot coerce 'one' to type double.
