기본값 인자 지정
------------------------------------

저장 프로시저와 저장 함수의 인자에 기본값을 지정할 수 있다.

* **:=** 또는 **DEFAULT** 키워드를 사용하여 기본값을 지정할 수 있다.
* 기본값을 지정하면 저장 프로시저와 저장 함수의 인수를 생략하고 호출할 수 있다. 생략된 인수는 기본값으로 대체된다.
* 기본값은 리터럴 값으로 지정할 수 있으며, 255 바이트 이하의 문자열 값으로 저장한다. 이 때 크기를 초과하면 오류가 발생한다.
* 기본값에 리터럴 값 외에 허용하는 함수는 다음과 같다.

+-------------------------------+---------------+
| 기본값                        | 데이터 타입   |
+===============================+===============+
| SYS_TIMESTAMP                 | TIMESTAMP     |
+-------------------------------+---------------+
| UNIX_TIMESTAMP()              | INTEGER       |
+-------------------------------+---------------+
| CURRENT_TIMESTAMP             | TIMESTAMP     |
+-------------------------------+---------------+
| SYS_DATETIME                  | DATETIME      |
+-------------------------------+---------------+
| CURRENT_DATETIME              | DATETIME      |
+-------------------------------+---------------+
| SYS_DATE                      | DATE          |
+-------------------------------+---------------+
| CURRENT_DATE                  | DATE          |
+-------------------------------+---------------+
| SYS_TIME                      | TIME          |
+-------------------------------+---------------+
| CURRENT_TIME                  | TIME          |
+-------------------------------+---------------+
| USER, USER()                  | STRING        |
+-------------------------------+---------------+
| TO_CHAR(date_time[, format])  | STRING        |
+-------------------------------+---------------+
| TO_CHAR(number[, format])     | STRING        |
+-------------------------------+---------------+

다음은 기본값에 리터럴 값을 지정하는 간단한 예시이다.

.. code-block:: sql

        CREATE FUNCTION default_args (
                a INT := 1, 
                b INT DEFAULT 2
        ) RETURN INT
        AS BEGIN RETURN a + b; END;

        SELECT default_args(); 
        SELECT default_args(3);
        SELECT default_args(3, 4);

::

          default_args()
        ================
                        3

          default_args(3)
        =================
                        5

          default_args(3, 4)
        ====================
                        7

다음은 기본값에 함수를 지정하는 예시이다.

.. code-block:: sql

        CREATE FUNCTION get_age (
            birth DATE DEFAULT DATE'2000-01-01',
            today DATE DEFAULT SYS_DATE
        ) RETURN INT
        AS
        BEGIN
            RETURN YEAR(today) - YEAR(birth)
                - CASE WHEN TO_CHAR(today, 'MMDD') < TO_CHAR(birth, 'MMDD') THEN 1 ELSE 0 END;
        END;

        SELECT get_age();
        SELECT get_age(DATE'2000-05-10');
        SELECT get_age(DATE'2000-05-10', DATE'2025-03-24');

::        

        get_age()
        =============
                   25

        get_age(date '2000-05-10')
        ============================
                                  24

        get_age(date '2000-05-10', date '2025-03-24')
        ===============================================
                                                     24

