***********************
날짜/시간 함수와 연산자
***********************

.. function:: ADDDATE (date, INTERVAL expr unit)
.. function:: ADDDATE (date, days)
.. function:: DATE_ADD (date, INTERVAL expr unit)

	**ADDDATE** 함수와 **DATE_ADD** 함수는 동일하며, 특정 **DATE** 값에 대해 덧셈 또는 뺄셈을 실행한다. 리턴 값은 **DATE** 타입 또는 **DATETIME** 타입이다. **DATETIME** 타입을 반환하는 경우는 다음과 같다.

	* 첫 번째 인자가 **DATETIME** 타입 또는 **TIMESTAMP** 타입인 경우
	* 첫 번째 인자가 **DATE** 타입이고 **INTERVAL** 값의 단위가 날짜 단위 미만으로 지정된 경우

	위의 경우 외에 **DATETIME** 타입의 결과 값을 반환하려면 :func:`CAST` 함수를 이용하여 첫 번째 인자 값의 타입을 변환해야 한다. 연산 결과의 날짜가 해당 월의 마지막 날짜를 초과하면, 해당 월의 말일을 적용하여 유효한 **DATE** 값을 반환한다.

	입력 인자의 날짜와 시간 값이 모두 0이면 시스템 파라미터 **return_null_on_function_errors** 의 값에 따라 다른 값을 반환한다. **return_null_on_function_errors** 가 yes이면 **NULL** 을 반환하고 no이면 에러를 반환하며, 기본값은 **no** 이다.

	계산 결과가 '0000-00-00 00:00:00'과 '0001-01-01 00:00:00' 사이이면, 날짜와 시간 값이 모두 0인 **DATE** 또는 **DATETIME** 타입의 값을 반환한다. 그러나 JDBC 프로그램에서는 연결 URL 속성인 zeroDateTimeBehavior의 설정에 따라 동작이 달라진다("API 레퍼런스 > JDBC API > JDBC 프로그래밍 > 연결 설정" 참고).

	:param date: **DATE**, **DATETIME** 또는 **TIMESTAMP** 타입의 연산식이며, 시작 날짜를 의미한다. 만약, '2006-07-00'와 같이 유효하지 않은 **DATE** 값이 지정되면, 에러를 반환한다.
	:param expr: 시작 날짜로부터 더할 시간 간격 값(interval value)을 의미하며, **INTERVAL** 키워드 뒤에 음수가 명시되면 시작 날짜로부터 시간 간격 값을 뺀다.
	:param unit: *expr* 수식에 명시된 시간 간격 값의 단위를 의미하며, 아래의 테이블을 참고하여 시간 간격 값 해석을 위한 형식을 지정할 수 있다. *expr* 의 단위 값이 *unit* 에서 요구하는 단위 값의 개수보다 적을 경우 가장 작은 단위부터 채운다. 예를 들어, **HOUR_SECOND** 의 경우 'HOURS:MINUTES:SECONDS'와 같이 3개의 값이 요구되는데, "1:1" 처럼 2개의 값만 주어지면 'MINUTES:SECONDS'로 간주한다.
	:rtype: **DATE** 또는 **DATETIME** 

	**unit에 대한 expr 형식**

	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| unit 값            | expr 형식**                               | 예                                                           |
	|                    |                                           |                                                              |
	+====================+===========================================+==============================================================+
	| MILLISECOND        | MILLISECONDS                              | ADDDATE(SYSDATE, INTERVAL 123 MILLISECOND)                   |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| SECOND             | SECONDS                                   | ADDDATE(SYSDATE, INTERVAL 123 SECOND)                        |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| MINUTE             | MINUTES                                   | ADDDATE(SYSDATE, INTERVAL 123 MINUTE)                        |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| HOUR               | HOURS                                     | ADDDATE(SYSDATE, INTERVAL 123 HOUR)                          |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| DAY                | DAYS                                      | ADDDATE(SYSDATE, INTERVAL 123 DAYS)                          |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| WEEK               | WEEKS                                     | ADDDATE(SYSDATE, INTERVAL 123 WEEKS)                         |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| MONTH              | MONTHS                                    | ADDDATE(SYSDATE, INTERVAL 12 MONTH)                          |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| QUARTER            | QUARTERS                                  | ADDDATE(SYSDATE, INTERVAL 12 QUARTER)                        |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| YEAR               | YEARS                                     | ADDDATE(SYSDATE, INTERVAL 12 YEAR)                           |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| SECOND_MILLISECOND | 'SECONDS.MILLISECONDS'                    | ADDDATE(SYSDATE, INTERVAL '12.123' SECOND_MILLISECOND)       |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| MINUTE_MILLISECOND | 'MINUTES:SECONDS.MILLISECONDS'            | ADDDATE(SYSDATE, INTERVAL '12:12.123' MINUTE_MILLISECOND)    |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| MINUTE_SECOND      | 'MINUTES:SECONDS'                         | ADDDATE(SYSDATE, INTERVAL '12:12' MINUTE_SECOND)             |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| HOUR_MILLISECOND   | 'HOURS:MINUTES:SECONDS.MILLISECONDS'      | ADDDATE(SYSDATE, INTERVAL '12:12:12.123' HOUR_MILLISECOND)   |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| HOUR_SECOND        | 'HOURS:MINUTES:SECONDS'                   | ADDDATE(SYSDATE, INTERVAL '12:12:12' HOUR_SECOND)            |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| HOUR_MINUTE        | 'HOURS:MINUTES'                           | ADDDATE(SYSDATE, INTERVAL '12:12' HOUR_MINUTE)               |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| DAY_MILLISECOND    | 'DAYS HOURS:MINUTES:SECONDS.MILLISECONDS' | ADDDATE(SYSDATE, INTERVAL '12 12:12:12.123' DAY_MILLISECOND) |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| DAY_SECOND         | 'DAYS HOURS:MINUTES:SECONDS'              | ADDDATE(SYSDATE, INTERVAL '12 12:12:12' DAY_SECOND)          |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| DAY_MINUTE         | 'DAYS HOURS:MINUTES'                      | ADDDATE(SYSDATE, INTERVAL '12 12:12' DAY_MINUTE)             |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| DAY_HOUR           | 'DAYS HOURS'                              | ADDDATE(SYSDATE, INTERVAL '12 12' DAY_HOUR)                  |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+
	| YEAR_MONTH         | 'YEARS-MONTHS'                            | ADDDATE(SYSDATE, INTERVAL '12-13' YEAR_MONTH)                |
	+--------------------+-------------------------------------------+--------------------------------------------------------------+

	.. code-block:: sql

		SELECT SYSDATE, ADDDATE(SYSDATE,INTERVAL 24 HOUR), ADDDATE(SYSDATE, 1);
		 
		   SYS_DATE    date_add( SYS_DATE , INTERVAL 24 HOUR)   adddate( SYS_DATE , 1)
		==============================================================================
		  03/30/2010  12:00:00.000 AM 03/31/2010               03/31/2010
		 
		--it substracts days when argument < 0
		SELECT SYSDATE, ADDDATE(SYSDATE,INTERVAL -24 HOUR), ADDDATE(SYSDATE, -1);
		 
		   SYS_DATE    date_add( SYS_DATE , INTERVAL -24 HOUR)   adddate( SYS_DATE , -1)
		==============================================================================
		  03/30/2010  12:00:00.000 AM 03/29/2010               03/29/2010
		 
		--when expr is not fully specified for unit
		SELECT SYS_DATETIME, ADDDATE(SYS_DATETIME, INTERVAL '1:20' HOUR_SECOND);
		 
		   SYS_DATETIME                   date_add( SYS_DATETIME , INTERVAL '1:20' HOUR_SECOND)
		=======================================================================================
		  06:18:24.149 PM 06/28/2010     06:19:44.149 PM 06/28/2010                            
		 
		SELECT ADDDATE('0000-00-00', 1 );
		 
		ERROR: Conversion error in date format.
		 
		SELECT ADDDATE('0001-01-01 00:00:00', -1);
		 
		adddate('0001-01-01 00:00:00', -1)
		======================
		'12:00:00.000 AM 00/00/0000'

.. function:: ADDTIME(expr1, expr2)

	**ADDTIME** 함수는 특정 시간 값에 대해 덧셈 또는 뺄셈을 실행한다. 첫 번째 인자는 **DATE**, **DATETIME**, **TIMESTAMP** 또는 **TIME** 타입이며, 두 번째 인자는 **TIME**, **DATETIME** 또는 **TIMESTAMP** 타입이다. 두 번째 인자는 반드시 시간을 포함해야 하며, 두 번째 인자의 날짜는 무시된다. 각 인자의 타입에 따른 반환 타입은 다음과 같다.

	+-------------------+----------------------------------+-----------+-------------------------------------+
	| 첫 번째 인자 타입 | 두 번째 인자 타입                | 반환 타입 | 참고                                |
	+===================+==================================+===========+=====================================+
	| TIME              | TIME, DATETIME, TIMESTAMP        | TIME      | 결과 값은 24시를 넘어서는 안 된다.  |
	+-------------------+----------------------------------+-----------+-------------------------------------+
	| DATE              | TIME, DATETIME, TIMESTAMP        | DATETIME  |                                     |
	+-------------------+----------------------------------+-----------+-------------------------------------+
	| DATETIME          | TIME, DATETIME, TIMESTAMP        | DATETIME  |                                     |
	+-------------------+----------------------------------+-----------+-------------------------------------+
	| 날짜/시간 문자열  | TIME, DATETIME, TIMESTAMP        | VARCHAR   | 결과 문자열은 시간을                |
	|                   | 또는 시간 문자열                 |           | 포함한 문자열이다.                  |
	+-------------------+----------------------------------+-----------+-------------------------------------+

	:param expr1: **DATE**, **DATETIME**, **TIMESTAMP**, **TIME** 타입 또는 날짜/시간 문자열
	:param expr2: **DATETIME**, **TIMESTAMP**, **TIME** 타입 또는 시간 문자열

	.. code-block:: sql
	
		SELECT ADDTIME(datetime'2007-12-31 23:59:59', time'1:1:2');
		 addtime(datetime '2007-12-31 23:59:59', time '1:1:2')
		========================================================
		01:01:01.000 AM 01/01/2008
		 
		SELECT ADDTIME(time'01:00:00', time'02:00:01');
		 addtime(time '01:00:00', time '02:00:01')
		============================================
		03:00:01 AM

.. function:: ADD_MONTHS ( date_argument , month )

	**ADD_MONTHS** 함수는 **DATE** 타입의 연산식 *date_argument* 에 *month* 를 더한 후, **DATE** 타입의 값을 반환한다. 인자로 지정된 값의 일(*dd*)이 연산 결과값의 월에 존재하면 해당 일(*dd*)을 반환하고, 존재하지 않으면 해당 월의 마지막 날(<*dd*)을 반환한다. 또한, 연산 결과값이 **DATE** 타입의 표현 범위를 초과하는 경우, 에러를 반환한다.

	:param date_argument: **DATE** 타입의 연산식을 지정한다. **TIMESTAMP** 나 **DATETIME** 값을 지정하려면 **DATE** 타입으로 명시적 변환을 해야 한다. 값이 **NULL** 이면 **NULL** 을 반환한다.
	:param month: *date_argument* 에 더할 개월 수를 지정하며, 양수와 음수 모두 지정될 수 있다. 만약, 정수가 아닌 타입의 값이 주어지면 묵시적으로 변환(소수점 아래 첫째자리를 반올림 처리)하여 정수형 타입으로 변환한다. 값이 **NULL** 이면 **NULL** 을 반환한다.

	.. code-block:: sql
	
		--it returns DATE type value by adding month to the first argument
		 
		SELECT ADD_MONTHS(DATE '2008-12-25', 5), ADD_MONTHS(DATE '2008-12-25', -5);
		  add_months(date '2008-12-25', 5)   add_months(date '2008-12-25', -5)
		=======================================================================
		  05/25/2009                         07/25/2008
		 
		 
		SELECT ADD_MONTHS(DATE '2008-12-31', 5.5), ADD_MONTHS(DATE '2008-12-31', -5.5);
		  add_months(date '2008-12-31', 5.5)   add_months(date '2008-12-31', -5.5)
		===========================================================================
		  06/30/2009                           06/30/2008
		 
		SELECT ADD_MONTHS(CAST (SYS_DATETIME AS DATE), 5), ADD_MONTHS(CAST (SYS_TIMESTAMP AS DATE), 5);
		  add_months( cast( SYS_DATETIME  as date), 5)   add_months( cast( SYS_TIMESTAMP  as date), 5)
		================================================================================
		  07/03/2010                                     07/03/2010

.. function:: CURDATE ()
.. function:: CURRENT_DATE ()
.. function:: CURRENT_DATE
.. function:: SYS_DATE
.. function:: SYSDATE

	**CURDATE**(), **CURRENT_DATE**, **CURRENT_DATE**(), **SYS_DATE**, **SYSDATE** 는 모두 동일하며, 현재 날짜를 **DATE** 타입(*MM*/*DD*/*YYYY*)으로 반환한다. 산술 연산의 단위는 일(day)이다. 입력 인자의 연, 월, 일이 모두 0이면 시스템 파라미터 **return_null_on_function_errors** 의 값에 따라 다른 값을 반환한다. **return_null_on_function_errors** 가 yes이면 **NULL** 을 반환하고 no이면 에러를 반환하며, 기본값은 **no** 이다.

	:rtype: DATE
	
	.. code-block:: sql
	
		--it returns the current date in DATE type
		SELECT CURDATE(), CURRENT_DATE(), CURRENT_DATE, SYS_DATE, SYSDATE;
		 
		   SYS_DATE    SYS_DATE    SYS_DATE    SYS_DATE    SYS_DATE
		============================================================
		  04/01/2010  04/01/2010  04/01/2010  04/01/2010  04/01/2010
		 
		--it returns the date 60 days added to the current date
		SELECT CURDATE()+60;
		 
		   SYS_DATE +60
		===============
		   05/31/2010

.. function:: CURRENT_DATETIME ()
.. function:: CURRENT_DATETIME
.. function:: NOW ()
.. function:: SYS_DATETIME
.. function:: SYSDATETIME

	**CURRENT_DATETIME**, **CURRENT_DATETIME**(), **NOW**(), **SYS_DATETIME**, **SYSDATETIME** 는 동일하며, 현재 날짜를 **DATETIME** 타입으로 반환한다. 산술 연산의 단위는 밀리초(milli-sec)다.

	:rtype: DATETIME
	
	.. code-block:: sql

		--it returns the current date and time in DATETIME type
		SELECT NOW(), SYS_DATETIME;
		 
		   SYS_DATETIME                   SYS_DATETIME
		==============================================================
		  04:08:09.829 PM 02/04/2010     04:08:09.829 PM 02/04/2010
		 
		--it returns the timestamp value 1 hour added to the current sys_datetime value
		SELECT TO_CHAR(SYSDATETIME+3600*1000, 'YYYY-MM-DD HH:MI');
		  to_char( SYS_DATETIME +3600*1000, 'YYYY-MM-DD HH:MI', 'en_US')
		======================
		  '2010-02-04 04:08'

.. function:: CURTIME ()
.. function:: CURRENT_TIME
.. function:: CURRENT_TIME ()
.. function:: SYS_TIME
.. function:: SYSTIME

	**CURTIME**(), **CURRENT_TIME**, **CURRENT_TIME**(), **SYS_TIME**, **SYSTIME** 는 모두 동일하며, 현재 시간을 **TIME** 타입(*HH*:*MI*:*SS*)으로 반환한다. 산술 연산의 단위는 초(sec)다.

	:rtype: TIME
	
	.. code-block:: sql

		--it returns the current time in TIME type
		SELECT CURTIME(), CURRENT_TIME(), CURRENT_TIME, SYS_TIME, SYSTIME;
		   SYS_TIME     SYS_TIME     SYS_TIME     SYS_TIME     SYS_TIME
		=================================================================
		  04:37:34 PM  04:37:34 PM  04:37:34 PM  04:37:34 PM  04:37:34 PM
		 
		--it returns the time value 1 hour added to the current sys_time
		SELECT CURTIME()+3600;
		   SYS_TIME +3600
		=================
		   05:37:34 PM

.. function:: CURRENT_TIMESTAMP
.. function:: CURRENT_TIMESTAMP ()
.. function:: SYS_TIMESTAMP
.. function:: SYSTIMESTAMP
.. function:: LOCALTIME
.. function:: LOCALTIME ()
.. function:: LOCALTIMESTAMP
.. function:: LOCALTIMESTAMP ()

	CURRENT_TIMESTAMP**, **CURRENT_TIMESTAMP**(), **SYS_TIMESTAMP**, **SYSTIMESTAMP**, **LOCALTIME**, **LOCALTIME**(), **LOCALTIMESTAMP**, **LOCALTIMESTAMP**()는 동일하며, 현재 날짜와 시간을 **TIMESTAMP** 타입으로 반환한다. 산술 연산의 단위는 초(sec)다.

	:rtype: TIMESTAMP
	
	.. code-block:: sql

		--it returns the current date and time in TIMESTAMP type
		SELECT LOCALTIME, SYS_TIMESTAMP;
		 SYS_TIMESTAMP              SYS_TIMESTAMP
		==============================================================================
		  07:00:48 PM 04/01/2010     07:00:48 PM 04/01/2010
		 
		--it returns the timestamp value 1 hour added to the current sys_timestamp value
		SELECT CURRENT_TIMESTAMP()+3600;
		 SYS_TIMESTAMP +3600
		===========================
		  08:02:42 PM 04/01/2010

.. function:: DATE (date)

	**DATE** 함수는 지정된 인자로부터 날짜 부분을 추출하여 '*MM*/*DD*/*YYYY*' 형식 문자열로 반환한다. 지정 가능한 인자는 **DATE**, **TIMESTAMP**, **DATETIME**타입이며, 리턴 값은 **VARCHAR** 타입이다

	인자의 연, 월, 일에는 0을 입력할 수 없으나, 예외적으로 날짜와 시간이 모두 0인 값을 입력한 경우에는 연, 월, 일 값이 모두 0인 문자열을 반환한다.

	:param date: **DATE**, **TIMESTAMP**, **DATETIME** 타입이 지정될 수 있다.
	:rtype: STRING

	.. code-block:: sql

		SELECT DATE('2010-02-27 15:10:23');
		 date('2010-02-27 15:10:23')
		==============================
		  '02/27/2010'
		 
		SELECT DATE(NOW());
		 date( SYS_DATETIME )
		======================
		  '04/01/2010'
		 
		SELECT DATE('0000-00-00 00:00:00');
		 date('0000-00-00 00:00:00')
		===============================
		 '00/00/0000'

.. function:: DATEDIFF (date1, date2)

	**DATEDIFF** 함수는 주어진 두 개의 인자로부터 날짜 부분을 추출하여 두 값의 차이를 일 단위 정수로 반환한다. 지정 가능한 인자는 **DATE**, **TIMESTAMP**, **DATETIME** 타입이며, 리턴 값의 타입은 **INTEGER** 이다.

	입력 인자의 날짜와 시간 값이 모두 0이면 시스템 파라미터 **return_null_on_function_errors** 의 값에 따라 다른 값을 반환한다. **return_null_on_function_errors** 가 yes이면 **NULL** 을 반환하고 no이면 에러를 반환하며, 기본값은 **no** 이다.

	:param date1,date2: 날짜를 포함하는 타입(**DATE**, **TIMESTAMP**, **DATETIME**) 또는 해당 타입의 값을 나타내는 문자열이 지정될 수 있다. 유효하지 않은 문자열이 지정되면 에러를 반환한다.
	:rtype: INT

	.. code-block:: sql

		SELECT DATEDIFF('2010-2-28 23:59:59','2010-03-02');
		 datediff('2010-2-28 23:59:59', '2010-03-02')
		===============================================
													 -2
		 
		SELECT DATEDIFF('0000-00-00 00:00:00', '2010-2-28 23:59:59');
		ERROR: Conversion error in date format.

.. function:: DATE_SUB (date, INTERVAL expr unit)
.. function:: SUBDATE(date, INTERVAL expr unit)
.. function:: SUBDATE(date, days)

	**DATE_SUB**()와 **SUBDATE**()는 동일하며, 특정 **DATE** 값에 대해 뺄셈 또는 덧셈을 실행한다. 리턴 값은 **DATE** 타입 또는 **DATETIME** 타입이다. 연산 결과의 날짜가 해당 월의 마지막 날짜를 초과하면, 해당 월의 말일을 적용하여 유효한 **DATE** 값을 반환한다.
	
	입력 인자의 날짜와 시간 값이 모두 0이면 시스템 파라미터 **return_null_on_function_errors** 의 값에 따라 다른 값을 반환한다. **return_null_on_function_errors** 가 yes이면 **NULL** 을 반환하고 no이면 에러를 반환하며, 기본값은 **no** 이다.

	계산 결과가 '0000-00-00 00:00:00'과 '0001-01-01 00:00:00' 사이이면, 날짜와 시간 값이 모두 0인 **DATE** 또는 **DATETIME** 타입의 값을 반환한다. 그러나 JDBC 프로그램에서는 연결 URL 속성인 zeroDateTimeBehavior의 설정에 따라 동작이 달라진다("API 레퍼런스 > JDBC API > JDBC 프로그래밍 > 연결 설정" 참고).

	:param date: **DATE**, **DATETIME** 또는 **TIMESTAMP** 타입의 연산식이며, 시작 날짜를 의미한다. 만약, '2006-07-00'와 같이 유효하지 않은 **DATE** 값이 지정되면, 에러를 반환한다.
	:param expr: 시작 날짜로부터 뺄 시간 간격 값(interval value)을 의미하며, **INTERVAL** 키워드 뒤에 음수가 명시되면 시작 날짜로부터 시간 간격 값을 더한다.
	:param unit: *expr* 수식에 명시된 시간 간격 값의 단위를 의미하며, *unit* 값에 대한 *expr* 인자의 값은 :func:`ADDDATE` 의 표를 참고한다.
	:rtype: DATE or DATETIME

	.. code-block:: sql

		SELECT SYSDATE, SUBDATE(SYSDATE,INTERVAL 24 HOUR), SUBDATE(SYSDATE, 1);
		   SYS_DATE    date_sub( SYS_DATE , INTERVAL 24 HOUR)   subdate( SYS_DATE , 1)
		==============================================================================
		  03/30/2010  12:00:00.000 AM 03/29/2010               03/29/2010
		 
		--it adds days when argument < 0
		SELECT SYSDATE, SUBDATE(SYSDATE,INTERVAL -24 HOUR), SUBDATE(SYSDATE, -1);
		   SYS_DATE    date_sub( SYS_DATE , INTERVAL -24 HOUR)   subdate( SYS_DATE , -1)
		==============================================================================
		  03/30/2010  12:00:00.000 AM 03/31/2010               03/31/2010
		 
		SELECT SUBDATE('0000-00-00 00:00:00', -50);
		ERROR: Conversion error in date format.
		 
		SELECT SUBDATE('0001-01-01 00:00:00', 10);
		 subdate('0001-01-01 00:00:00', 10)
		==============================
		 '12:00:00.000 AM 00/00/0000'

.. function:: DAY (date)
.. function:: DAYOFMONTH (date)

	**DAY** 함수와 **DAYOFMONTH** 함수는 동일하며, 지정된 인자로부터 1~31 범위의 일(day)을 반환한다. 인자로 **DATE**, **TIMESTAMP**, **DATETIME** 타입을 지정할 수 있으며, **INTEGER** 타입을 반환한다.

	인자의 연, 월, 일에는 0을 입력할 수 없으나, 예외적으로 연, 월, 일이 모두 0인 값을 입력한 경우에는 0을 반환한다.

	:param date: 날짜
	:rtype: INT

	.. code-block:: sql

		SELECT DAYOFMONTH('2010-09-09');
		   dayofmonth('2010-09-09')
		===========================
								  9
		 
		SELECT DAY('2010-09-09 19:49:29');
		   day('2010-09-09 19:49:29')
		=============================
									9
		 
		SELECT DAYOFMONTH('0000-00-00 00:00:00');
		   dayofmonth('0000-00-00 00:00:00')
		====================================
										   0

.. function:: DAYOFWEEK (date)

	**DAYOFWEEK** 함수는 지정된 인자로부터 1~7 범위의 요일(1: 일요일, 2: 월요일, ..., 7: 토요일)을 반환한다. 요일 인덱스는 ODBC 표준과 같다. 인자로 **DATE**, **TIMESTAMP**, **DATETIME** 타입을 지정할 수 있으며, **INTEGER** 타입을 반환한다.

	입력 인자의 연, 월, 일이 모두 0이면 시스템 파라미터 **return_null_on_function_errors** 의 값에 따라 다른 값을 반환한다. **return_null_on_function_errors** 가 yes이면 **NULL** 을 반환하고 no이면 에러를 반환하며, 기본값은 **no** 이다.

	:param date: 날짜
	:rtype: INT

	.. code-block:: sql

		SELECT DAYOFWEEK('2010-09-09');
		   dayofweek('2010-09-09')
		==========================
								 5
		 
		SELECT DAYOFWEEK('2010-09-09 19:49:29');
		 dayofweek('2010-09-09 19:49:29')
		=================================
										5
		 
		SELECT DAYOFWEEK('0000-00-00');
		ERROR: Conversion error in date format.

.. function:: DAYOFYEAR (date)

	**DAYOFYEAR** 함수는 지정된 인자로부터 1~366 범위의 일(day of year)을 반환한다. 인자로 **DATE**, **TIMESTAMP**, **DATETIME** 타입을 지정할 수 있으며, **INTEGER** 타입을 반환한다.

	입력 인자의 날짜 값이 모두 0이면 시스템 파라미터 **return_null_on_function_errors** 의 값에 따라 다른 값을 반환한다. **return_null_on_function_errors** 가 yes이면 **NULL** 을 반환하고 no이면 에러를 반환하며, 기본값은 **no** 이다.

	:param date: 날짜
	:rtype: INT

	.. code-block:: sql

		SELECT DAYOFYEAR('2010-09-09');
		   dayofyear('2010-09-09')
		==========================
							   252
		 
		SELECT DAYOFYEAR('2010-09-09 19:49:29');
		dayofyear('2010-09-09 19:49:29')
		=================================
									252
		 
		SELECT DAYOFYEAR('0000-00-00');
		ERROR: Conversion error in date format.

.. function:: EXTRACT ( field FROM date-time_argument )

	**EXTRACT** 연산자는 날짜/시간 값을 반환하는 연산식 *date-time_argument* 중 일부분을 추출하여 **INTEGER** 타입으로 반환한다. 
	
	입력 인자의 연, 월, 일에는 0을 입력할 수 없으나, 예외적으로 날짜와 시간이 모두 0인 값을 입력한 경우에는 0을 반환한다.

	:param field: 날짜/시간 수식에서 추출할 값을 지정한다. (YEAR, MONTH, DAY, HOUR, MINUTE, SECOND, MILLISECOND)
	:param date-time_argument: 날짜/시간 값을 반환하는 연산식이다. 이 연산식의 값은 **TIME**, **DATE**, **TIMESTAMP**, **DATETIME** 타입 중 하나여야 하며, **NULL** 이 지정된 경우에는 **NULL** 값이 반환된다.
	:rtype: INT

	.. code-block:: sql

		SELECT EXTRACT(MONTH FROM DATETIME '2008-12-25 10:30:20.123' );
		  extract(month  from datetime '2008-12-25 10:30:20.123')
		=========================================================
															   12
		 
		SELECT EXTRACT(HOUR FROM DATETIME '2008-12-25 10:30:20.123' );
		 extract(hour  from datetime '2008-12-25 10:30:20.123')
		=========================================================
															   10
		 
		SELECT EXTRACT(MILLISECOND FROM DATETIME '2008-12-25 10:30:20.123' );
		 extract(millisecond  from datetime '2008-12-25 10:30:20.123')
		=========================================================
															  123
		 
		SELECT EXTRACT(MONTH FROM '0000-00-00 00:00:00');
		 extract(month from '0000-00-00 00:00:00')
		==========================================
												 0

.. function:: FROM_DAYS (N)

	**FROM_DAYS** 함수는 **INTEGER** 타입을 인자로 입력하면 **DATE** 타입의 날짜를 반환한다.

	**FROM_DAYS** 함수는 그레고리력(Gregorian Calendar) 출현(1582년) 이전은 고려하지 않았으므로 1582년 이전의 날짜에 대해서는 사용하지 않는 것을 권장한다.

	인자로 0~3,652,424 범위의 정수를 입력할 수 있다. 0~365 범위의 값을 인자로 입력하면 0을 반환한다. 최대값인 3,652,424는 9999년의 마지막 날을 의미한다.

	:param N: 0~3,652,424 범위의 정수
	:rtype: DATE

	.. code-block:: sql

		SELECT FROM_DAYS(719528);
		   from_days(719528)
		====================
		  01/01/1970
		 
		SELECT FROM_DAYS('366');
		  from_days('366')
		=================
		  01/03/0001
		 
		SELECT FROM_DAYS(3652424);
		   from_days(3652424)
		=====================
		  12/31/9999
		 
		SELECT FROM_DAYS(0);
		   from_days(0)
		===============
			00/00/0000

.. function:: FROM_UNIXTIME ( unix_timestamp[, format] )

	**FROM_UNIXTIME** 함수는 지정된 인자로부터 'YYYY-MM-DD HH:MM:SS' 형태의 날짜와 시간을 반환한다. 인자로 UNIX의 타임스탬프에 해당하는 **INTEGER** 타입을 입력할 수 있으며, **VARCHAR** 타입을 반환한다. 리턴 값은 현재의 타임 존으로 표현된다.

	*format* 에 입력한 시간 형식에 맞게 결과를 출력하며, 시간 형식은 :func:`DATE_FORMAT` 의 날짜/시간 형식 2를 따른다.

	**TIMESTAMP** 와 UNIX 타임스탬프는 일대일 대응 관계가 아니기 때문에 변환할 때 :func:`UNIX_TIMESTAMP` 함수나 **FROM_UNIXTIME** 함수를 사용하면 값의 일부가 유실될 수 있다. 자세한 설명은 :func:`UNIX_TIMESTAMP` 를 참고한다.

	인자의 연, 월, 일에는 0을 입력할 수 없으나, 예외적으로 날짜와 시간이 모두 0인 값을 입력한 경우에는 날짜와 시간 값이 모두 0인 문자열을 반환한다. 그러나 JDBC 프로그램에서는 연결 URL 속성인 zeroDateTimeBehavior의 설정에 따라 동작이 달라진다("API 레퍼런스 > JDBC API > JDBC 프로그래밍 > 연결 설정" 참고).

	:param unix_timestamp: 양의 정수
	:param format: 시간 형식. :func:`DATE_FORMAT` 의 날짜/시간 형식 2를 따른다.
	:rtype: STRING

	.. code-block:: sql

		SELECT FROM_UNIXTIME(1234567890);
		   from_unixtime(1234567890)
		============================
		  01:31:30 AM 02/14/2009
		 
		SELECT FROM_UNIXTIME('1000000000');
		   from_unixtime('1000000000')
		==============================
		  04:46:40 AM 09/09/2001
		 
		SELECT FROM_UNIXTIME(1234567890,'%M %Y %W');
		   from_unixtime(1234567890, '%M %Y %W')
		======================
		  'February 2009 Saturday'
		 
		SELECT FROM_UNIXTIME('1234567890','%M %Y %W');
		   from_unixtime('1234567890', '%M %Y %W')
		======================
		  'February 2009 Saturday'
		 
		SELECT FROM_UNIXTIME(0);
		   from_unixtime(0)
		===========================
		   12:00:00 AM 00/00/0000

.. function:: HOUR (time)

	**HOUR** 함수는 지정된 인자로부터 시(hour) 부분을 추출한 정수를 반환한다. 인자로 **TIME**, **TIMESTAMP**, **DATETIME** 타입을 지정할 수 있으며, **INTEGER** 타입을 반환한다.

	:param time: 시간
	:rtype: INT

	.. code-block:: sql

		SELECT HOUR('12:34:56');
		   hour('12:34:56')
		======================
						 12
		 
		SELECT HOUR('2010-01-01 12:34:56');
		   hour('2010-01-01 12:34:56')
		======================
						 12
		 
		SELECT HOUR(datetime'2010-01-01 12:34:56');
		   time(datetime '2010-01-01 12:34:56')
		======================
						 12

.. function:: LAST_DAY ( date_argument )

	**LAST_DAY** 함수는 인자로 지정된 **DATE** 값에서 해당 월의 마지막 날짜 값을 **DATE** 타입으로 반환한다. 
	
	입력 인자의 연, 월, 일이 모두 0이면 시스템 파라미터 **return_null_on_function_errors** 의 값에 따라 다른 값을 반환한다. **return_null_on_function_errors** 가 yes이면 **NULL** 을 반환하고 no이면 에러를 반환하며, 기본값은 **no** 이다. 
	
	:param date_argument: **DATE** 타입의 연산식을 지정한다. **TIMESTAMP** 나 **DATETIME** 값을 지정하려면 **DATE** 타입으로 명시적 변환을 해야 한다. 값이 **NULL** 이면 **NULL** 을 반환한다.
	:rtype: DATE

	.. code-block:: sql

		--it returns last day of the momth in DATE type
		SELECT LAST_DAY(DATE '1980-02-01'), LAST_DAY(DATE '2010-02-01');
		  last_day(date '1980-02-01')   last_day(date '2010-02-01')
		============================================================
		  02/28/1980                    02/28/2010
		 
		--it returns last day of the momth when explicitly casted to DATE type
		SELECT LAST_DAY(CAST (SYS_TIMESTAMP AS DATE)), LAST_DAY(CAST (SYS_DATETIME AS DATE));
		  last_day( cast( SYS_TIMESTAMP  as date))   last_day( cast( SYS_DATETIME  as date))
		================================================================================
		  02/28/2010                                 02/28/2010
		 
		SELECT LAST_DAY('0000-00-00');
		ERROR: Conversion error in date format.

.. function:: MAKEDATE (year, dayofyear)

	**MAKEDATE** 함수는 지정된 인자로부터 날짜를 반환한다. 인자로 1~9999 범위의 연도와 일(day of year)에 해당하는 **INTEGER** 타입을 지정할 수 있으며, 1/1/1~12/31/9999 범위의 **DATE** 타입을 반환한다. 일(day of year)이 해당 연도를 넘어가면 다음 연도가 된다. 예를 들어, MAKEDATE(1999, 366)은 2000-01-01을 반환한다. 단, 연도에 0~69 범위의 값을 입력하면 2000년~2069년으로 처리하고, 70~99 범위의 값을 입력하면 1970년~1999년으로 처리한다.

	*year* 와 *dayofyear* 가 모두 0이면 시스템 파라미터 **return_null_on_function_errors** 의 값에 따라 다른 값을 반환한다. **return_null_on_function_errors** 가 yes이면 **NULL** 을 반환하고 no이면 에러를 반환하며, 기본값은 **no** 이다.

	:param year: 1~9999 범위의 연도
	:param dayofyear: 연도에 0~99의 값을 입력하면 예외적으로 처리하므로, 실제로는 100년 이후의 연도만 사용된다. 따라서 *dayofyear* 의 최대값은 3,615,902이며, MAKEDATE(100, 3615902)는 9999/12/31을 반환한다.
	:rtype: DATE

	.. code-block:: sql

		SELECT MAKEDATE(2010,277);
		   makedate(2010, 277)
		======================
		  10/04/2010
		 
		SELECT MAKEDATE(10,277);
		   makedate(10, 277)
		====================
		  10/04/2010
		 
		SELECT MAKEDATE(70,277);
		   makedate(70, 277)
		====================
		  10/04/1970
		 
		SELECT MAKEDATE(100,3615902);
		   makedate(100, 3615902)
		=========================
		  12/31/9999
		 
		SELECT MAKEDATE(9999,365);
		   makedate(9999, 365)
		======================
		  12/31/9999
		 
		SELECT MAKEDATE(0,0);
		ERROR: Conversion error in date format.

.. function:: MAKETIME(hour, min, sec)

	**MAKETIME** 함수는 지정된 인자로부터 시간을 AM/PM 형태로 반환한다. 인자로 시각, 분, 초에 해당하는 **INTEGER** 타입을 지정할 수 있으며, **DATETIME** 타입을 반환한다.

	:param hour: 시를 나타내는 0~23 범위의 정수
	:param min: 분을 나타내는 0~59 범위의 정수
	:param sec: 초를 나타내는 0~59 범위의 정수
	:rtype: DATETIME

	.. code-block:: sql

		SELECT MAKETIME(13,34,4);
		   maketime(13, 34, 4)
		======================
		  01:34:04 PM
		 
		SELECT MAKETIME('1','34','4');
		   maketime('1', '34', '4')
		===========================
		  01:34:04 AM
		 
		SELECT MAKETIME(24,0,0);
		 
		ERROR: Conversion error in time format.

.. function:: MINUTE (time)

	**MINUTE** 함수는 지정된 인자로부터 0~59 범위의 분(minute)을 반환한다. 인자로 **TIME**, **TIMESTAMP**, **DATETIME** 타입을 지정할 수 있으며, **INTEGER** 타입을 반환한다.

	:param time: 시간
	:rtype: INT

	.. code-block:: sql

		SELECT MINUTE('12:34:56');
		   minute('12:34:56')
		=====================
						   34
		 
		SELECT MINUTE('2010-01-01 12:34:56');
		   minute('2010-01-01 12:34:56')
		================================
									  34
		 
		SELECT MINUTE('2010-01-01 12:34:56.7890');
		   minute('2010-01-01 12:34:56.7890')
		=====================================
										   34

.. function:: MONTH (date)

	**MONTH** 함수는 지정된 인자로부터 1~12 범위의 월(month)을 반환한다. 인자로 **DATE**, **TIMESTAMP**, **DATETIME** 타입을 지정할 수 있으며, **INTEGER** 타입을 반환한다.

	인자의 연, 월, 일에는 0을 입력할 수 없으나, 예외적으로 날짜가 모두 0인 값을 입력한 경우에는 0을 반환한다.	

	:param date: 날짜
	:rtype: INT

	.. code-block:: sql

		SELECT MONTH('2010-01-02');
		   month('2010-01-02')
		======================
							 1
		 
		SELECT MONTH('2010-01-02 12:34:56');
		   month('2010-01-02 12:34:56')
		===============================
									  1
		 
		SELECT MONTH('2010-01-02 12:34:56.7890');
		   month('2010-01-02 12:34:56.7890')
		====================================
										   1
		 
		SELECT MONTH('0000-00-00');
		   month('0000-00-00')
		======================
							 0

.. function:: MONTHS_BETWEEN (date_argument, date_argument)

	**MONTHS_BETWEEN** 함수는 주어진 두 개의 **DATE** 값 간의 차이를 월 단위로 반환하며, 리턴 값은 **DOUBLE** 타입이다. 인자로 지정된 두 날짜가 동일하거나, 해당 월의 말일인 경우에는 정수 값을 반환하지만, 그 외의 경우에는 날짜 차이를 31로 나눈 값을 반환한다.

	:param date_argument:  **DATE** 타입의 연산식을 지정한다. **TIMESTAMP** 나 **DATETIME** 값을 지정하려면 **DATE** 타입으로 명시적 변환을 해야 한다. 값이 **NULL** 이면 **NULL** 을 반환한다.
	:rtype: DOUBLE

	.. code-block:: sql

		--it returns the negative months when the first argument is the previous date
		SELECT MONTHS_BETWEEN(DATE '2008-12-31', DATE '2010-6-30');
		 months_between(date '2008-12-31', date '2010-6-30')
		======================================================
									   -1.800000000000000e+001
		 
		--it returns integer values when each date is the last dat of the month
		SELECT MONTHS_BETWEEN(DATE '2010-6-30', DATE '2008-12-31');
		 months_between(date '2010-6-30', date '2008-12-31')
		======================================================
										1.800000000000000e+001
		 
		--it returns months between two arguments when explicitly casted to DATE type
		SELECT MONTHS_BETWEEN(CAST (SYS_TIMESTAMP AS DATE), DATE '2008-12-25');
		 months_between( cast( SYS_TIMESTAMP  as date), date '2008-12-25')
		====================================================================
													  1.332258064516129e+001
		 
		--it returns months between two arguments when explicitly casted to DATE type
		SELECT MONTHS_BETWEEN(CAST (SYS_DATETIME AS DATE), DATE '2008-12-25');
		 months_between( cast( SYS_DATETIME  as date), date '2008-12-25')
		===================================================================
													 1.332258064516129e+001

.. function:: QUARTER (date)

	**QUARTER** 함수는 지정된 인자로부터 1~4 범위의 분기(quarter)를 반환한다. 인자로 **DATE**, **TIMESTAMP**, **DATETIME** 타입을 지정할 수 있으며, **INTEGER** 타입을 반환한다.

	:param date: 날짜
	:rtype: INT

	.. code-block:: sql

		SELECT QUARTER('2010-05-05');
		   quarter('2010-05-05')
		========================
							   2
		 
		SELECT QUARTER('2010-05-05 12:34:56');
		  quarter('2010-05-05 12:34:56')
		===============================
									  2
		 
		SELECT QUARTER('2010-05-05 12:34:56.7890');
		  quarter('2010-05-05 12:34:56.7890')
		==================================
									  2

.. function:: SEC_TO_TIME (second)

	**SEC_TO_TIME** 함수는 지정된 인자로부터 시, 분, 초를 포함한 시간을 반환한다. 인자로 0~86399 범위의 **INTEGER** 타입을 지정할 수 있으며, **TIME** 타입을 반환한다.

	:param second: 0~86399 범위의 초
	:rtype: TIME

	.. code-block:: sql

		SELECT SEC_TO_TIME(82800);
		   sec_to_time(82800)
		=====================
		  11:00:00 PM
		 
		SELECT SEC_TO_TIME('82800.3');
		   sec_to_time('82800.3')
		=========================
		  11:00:00 PM
		 
		SELECT SEC_TO_TIME(86399)
		   sec_to_time(86399)
		=====================
		  11:59:59 PM

.. function:: SECOND (time)

	**SECOND** 함수는 지정된 인자로부터 0~59 범위의 초(second)를 반환한다. 인자로 **TIME**, **TIMESTAMP**, **DATETIME** 타입을 지정할 수 있으며, **INTEGER** 타입을 반환한다.

	:param time: 시간
	:rtype: INT

	.. code-block:: sql

		SELECT SECOND('12:34:56');
		   second('12:34:56')
		=====================
						   56
		 
		SELECT SECOND('2010-01-01 12:34:56');
		   second('2010-01-01 12:34:56')
		================================
									  56
		 
		SELECT SECOND('2010-01-01 12:34:56.7890');
		   second('2010-01-01 12:34:56.7890')
		=====================================
										   56

.. function:: TIME (time)

	**TIME** 함수는 지정된 인자로부터 시간 부분을 추출하여 'HH:MM:SS' 형태의 **VARCHAR** 타입 문자열을 반환한다. 인자로 **TIME**, **TIMESTAMP**, **DATETIME** 타입을 지정할 수 있다.

	:param time: 시간
	:rtype: STRING

	.. code-block:: sql

		SELECT TIME('12:34:56');
		   time('12:34:56')
		======================
		  '12:34:56'
		 
		SELECT TIME('2010-01-01 12:34:56');
		   time('2010-01-01 12:34:56')
		======================
		  '12:34:56'
		 
		SELECT TIME(datetime'2010-01-01 12:34:56');
		   time(datetime '2010-01-01 12:34:56')
		======================
		  '12:34:56'

.. function:: TIME_TO_SEC (time)

	**TIME_TO_SEC** 함수는 지정된 인자로부터 0~86399 범위의 초를 반환한다. 인자로 **TIME**, **TIMESTAMP**, **DATETIME** 타입을 지정할 수 있으며, **INTEGER** 타입을 반환한다.

	:param time: 시간
	:rtype: INT

	.. code-block:: sql

		SELECT TIME_TO_SEC('23:00:00');
		   time_to_sec('23:00:00')
		==========================
							 82800
		 
		SELECT TIME_TO_SEC('2010-10-04 23:00:00');
		   time_to_sec('2010-10-04 23:00:00')
		=====================================
										82800
		 
		 SELECT TIME_TO_SEC('2010-10-04 23:00:00.1234');
		   time_to_sec('2010-10-04 23:00:00.1234')
		==========================================
											 82800

.. function:: TIMEDIFF (expr1, expr2)

	**TIMEDIFF** 함수는 지정된 두 개의 시간 인자의 시간 차를 반환한다. 날짜/시간 타입인 **TIME**, **DATE**, **TIMESTAMP**, **DATETIME** 타입을 인자로 입력할 수 있으며, 두 인자의 데이터 타입은 같아야 한다. **TIME** 타입을 반환하며, 따라서 두 인자의 시간 차이는 00:00:00~23:59:59 범위여야 한다. 이 범위를 벗어나면 에러를 반환한다.

	:param expr1, expr2: 시간. 두 인자의 데이터 타입은 같아야 한다.
	:rtype: TIME

	.. code-block:: sql

		SELECT TIMEDIFF(time '17:18:19', time '12:05:52');
		   timediff(time '17:18:19', time '12:05:52')
		=============================================
		  05:12:27 AM
		 
		SELECT TIMEDIFF('17:18:19','12:05:52');
		   timediff('17:18:19', '12:05:52')
		===================================
		  05:12:27 AM
		 
		SELECT TIMEDIFF('2010-01-01 06:53:45', '2010-01-01 03:04:05');
		   timediff('2010-01-01 06:53:45', '2010-01-01 03:04:05')
		=========================================================
		  03:49:40 AM              

.. function:: TIMESTAMP (date [,time])

	**TIMESTAMP** 함수는 인자로 날짜/시간 형식의 문자열이 지정되고, 이를 **DATETIME** 타입으로 반환한다. 단일 인자로 **DATE** 형식 문자열('*YYYY*-*MM*-*DD*' 또는 '*MM*/*DD*/*YYYY*') 또는 **TIMESTAMP** 형식 문자열('*YYYY*-*MM*-*DD**HH*:*MI*:*SS*' 또는 '*HH*:*MI*:*SS**MM*/*DD*/*YYYY*')이 지정되면 이를 **DATETIME** 타입으로 반환한다.
	
	두 번째 인자로 **TIME** 형식 문자열('*HH*:*MI*:*SS*')이 주어지면 이를 첫 번째 인자 값에 더한 결과를 **DATETIME** 타입으로 반환한다. 두 번째 인자가 명시되지 않으면, 기본값으로 **12:00:00.000 AM** 이 설정된다.

	:param date: '*YYYY*-*MM*-*DD*', '*MM*/*DD*/*YYYY*', '*YYYY*-*MM*-*DD* *HH*:*MI*:*SS*', '*HH*:*MI*:*SS* *MM*/*DD*/*YYYY*' 형식 문자열이 지정될 수 있다.
	:param time: '*HH*:*MI*:*SS*' 형식 문자열이 지정될 수 있다.
	:rtype: DATETIME

	.. code-block:: sql

		SELECT TIMESTAMP('2009-12-31'), TIMESTAMP('2009-12-31','12:00:00');
		 timestamp('2009-12-31')        timestamp('2009-12-31', '12:00:00')
		=====================================================================
		  12:00:00.000 AM 12/31/2009     12:00:00.000 PM 12/31/2009
		 
		SELECT TIMESTAMP('2010-12-31 12:00:00','12:00:00');
		 timestamp('2010-12-31 12:00:00', '12:00:00')
		===============================================
		  12:00:00.000 AM 01/01/2011
		 
		SELECT TIMESTAMP('13:10:30 12/25/2008');
		 timestamp('13:10:30 12/25/2008')
		===================================
		  01:10:30.000 PM 12/25/2008

.. function:: TO_DAYS (date)

	**TO_DAYS** 함수는 지정된 인자로부터 0년 이후의 날 수를 366~3652424 범위의 값으로 반환한다. 인자로 **DATE** 타입을 지정할 수 있으며, **INTEGER** 타입을 반환한다.
	**TO_DAYS** 함수는 그레고리력(Gregorian Calendar) 출현(1582년) 이전은 고려하지 않았으므로, 1582년 이전의 날짜에 대해서는 사용하지 않는 것을 권장한다.

	:param date: 날짜
	:rtype: INT

	.. code-block:: sql

		SELECT TO_DAYS('2010-10-04');
		   to_days('2010-10-04')
		========================
						  734414
		 
		SELECT TO_DAYS('2010-10-04 12:34:56');
		   to_days('2010-10-04 12:34:56')
		================================
								  734414
		 
		SELECT TO_DAYS('2010-10-04 12:34:56.7890');
		   to_days('2010-10-04 12:34:56.7890')
		======================================
										734414
		 
		SELECT TO_DAYS('1-1-1');
		   to_days('1-1-1')
		===================
						366
		 
		SELECT TO_DAYS('9999-12-31');
		   to_days('9999-12-31')
		========================
						 3652424

.. function:: UNIX_TIMESTAMP ( [date] )

	**UNIX_TIMESTAMP** 함수는 인자를 생략할 수 있으며, 인자를 생략하면 '1970-01-01 00:00:00' UTC 이후 현재 시스템 날짜/시간까지의 초 단위 시간 간격(interval)을 **INTEGER** 타입의 리턴 값을 반환한다. *date* 인자가 지정되면 '1970-01-01 00:00:00' UTC 이후 지정된 날짜/시간까지의 초 단위 시간 간격을 반환한다.

	인자의 연, 월, 일에는 0을 입력할 수 없으나, 예외적으로 날짜와 시간이 모두 0인 값을 입력한 경우에는 0을 반환한다.

	:param date: **DATE** 타입, **TIMESTAMP** 타입, **DATE** 형식 문자열('*YYYY*-*MM*-*DD*' 또는 '*MM*/*DD*/*YYYY*'), **TIMESTAMP** 형식 문자열('*YYYY*-*MM*-*DD* *HH*:*MI*:*SS*', '*HH*:*MI*:*SS* *MM*/*DD*/*YYYY*') 또는 '*YYYYMMDD*' 형식 문자열이 지정될 수 있다.
	:rtype: INT

	.. code-block:: sql

		SELECT UNIX_TIMESTAMP('1970-01-02'), UNIX_TIMESTAMP();
		   unix_timestamp('1970-01-02')   unix_timestamp()
		==================================================
								  54000         1270196737
		 
		SELECT UNIX_TIMESTAMP ('0000-00-00 00:00:00');
		   unix_timestamp('0000-00-00 00:00:00')
		========================================
											   0

.. function:: UTC_DATE ()

	**UTC_DATE** 함수는 UTC 날짜를 'YYYY-MM-DD' 형태로 반환한다.

	:rtype: STRING

	.. code-block:: sql

		SELECT UTC_DATE();
		  utc_date()
		==============
		  01/12/2011

.. function:: UTC_TIME ()

	**UTC_TIME** 함수는 UTC 시간을 'HH:MM:SS' 형태로 반환한다.

	:rtype: STRING

	.. code-block:: sql

		SELECT UTC_TIME();
		  utc_time()
		==============
		  10:35:52 AM

.. function:: WEEK (date[, mode])

	**WEEK** 함수는 지정된 인자로부터 0~53 범위의 주를 반환한다. 인자로 **DATE**, **TIMESTAMP**, **DATETIME** 타입을 지정할 수 있으며, **INTEGER** 타입을 반환한다.

	:param date: 날짜
	:param mode: 0~7 범위의 값
	:rtype: INT

	함수의 두 번째 인자인 *mode* 는 생략할 수 있으며, 0~7 범위의 값을 입력한다. 이 값으로 한 주가 일요일부터 시작하는지 월요일부터 시작하는지, 리턴 값의 범위가 0~53인지 1~53인지 설정한다. *mode* 를 생략하면 시스템 파라미터 **default_week_format** 의 값(기본값: 0)이 사용된다. *mode* 값의 의미는 다음과 같다.

	+----------+-----------+--------+------------------------------------------+
	| mode     | 시작 요일 | 범위   | 해당 연도의 첫 번째 주                   |
	+==========+===========+========+==========================================+
	| 0        | 일요일    | 0~53   | 일요일이 해당 연도에 속하는 첫 번째 주   |
	+----------+-----------+--------+------------------------------------------+
	| 1        | 월요일    | 0~53   | 3일 이상이 해당 연도에 속하는 첫 번째 주 |
	+----------+-----------+--------+------------------------------------------+
	| 2        | 일요일    | 1~53   | 일요일이 해당 연도에 속하는 첫 번째 주   |
	+----------+-----------+--------+------------------------------------------+
	| 3        | 월요일    | 1~53   | 3일 이상이 해당 연도에 속하는 첫 번째 주 |
	+----------+-----------+--------+------------------------------------------+
	| 4        | 일요일    | 0~53   | 3일 이상이 해당 연도에 속하는 첫 번째 주 |
	+----------+-----------+--------+------------------------------------------+
	| 5        | 월요일    | 0~53   | 월요일이 해당 연도에 속하는 첫 번째 주   |
	+----------+-----------+--------+------------------------------------------+
	| 6        | 일요일    | 1~53   | 3일 이상이 해당 연도에 속하는 첫 번째 주 |
	+----------+-----------+--------+------------------------------------------+
	| 7        | 월요일    | 1~53   | 월요일이 해당 연도에 속하는 첫 번째 주   |
	+----------+-----------+--------+------------------------------------------+

	*mode* 값이 0, 1, 4, 5 중 하나이고 날짜가 이전 연도의 마지막 주에 해당하면 **WEEK** 함수는 0을 반환한다. 이때의 목적은 해당 연도에서 해당 주가 몇 번째 주인지를 아는 것이므로, 1999년의 52번째 주에 해당해도 2000년의 날짜가 0번째 주에 해당되는 0을 반환한다.

	.. code-block:: sql
		
		SELECT YEAR('2000-01-01'), WEEK('2000-01-01',0);
		   year('2000-01-01')   week('2000-01-01', 0)
		=============================================
						2000                       0

	시작 요일이 속해있는 주의 연도를 기준으로 해당 날짜가 몇 번째 주인지 알려면, *mode* 값으로 0, 2, 5, 7 중 하나의 값을 사용한다.
	
	.. code-block:: sql

		SELECT WEEK('2000-01-01',2);
			week('2000-01-01', 2)
		========================
							  52

	.. code-block:: sql

		SELECT WEEK('2010-04-05');
		   week('2010-04-05', 0)
		========================
							  14
		 
		SELECT WEEK('2010-04-05 12:34:56',2);
		   week('2010-04-05 12:34:56',2)
		===============================
									  14
		 
		SELECT WEEK('2010-04-05 12:34:56.7890',4);
		   week('2010-04-05 12:34:56.7890',4)
		====================================
										  14

.. function:: WEEKDAY (date)

	**WEEKDAY** 함수는 지정된 인자로부터 0~6 범위의 요일(0: 월요일, 1: 화요일, ..., 6: 일요일)을 반환한다. 인자로 **DATE**, **TIMESTAMP**, **DATETIME** 타입을 지정할 수 있으며, **INTEGER** 타입을 반환한다.

	:param date: 날짜
	:rtype: INT

	.. code-block:: sql
	
		SELECT WEEKDAY('2010-09-09');
		   weekday('2010-09-09')
		========================
							   3
		 
		SELECT WEEKDAY('2010-09-09 13:16:00');
		   weekday('2010-09-09 13:16:00')
		=================================
										3

.. function:: YEAR (date)

	**YEAR** 함수는 지정된 인자로부터 1~9999 범위의 연도를 반환한다. 인자로 **DATE**, **TIMESTAMP**, **DATETIME** 타입을 지정할 수 있으며, **INTEGER** 타입을 반환한다.

	:param date: 날짜
	:rtype: INT

	.. code-block:: sql

		SELECT YEAR('2010-10-04');
		   year('2010-10-04')
		=====================
						 2010
		 
		SELECT YEAR('2010-10-04 12:34:56');
		   year('2010-10-04 12:34:56')
		==============================
								  2010
		 
		SELECT YEAR('2010-10-04 12:34:56.7890');
		   year('2010-10-04 12:34:56.7890')
		===================================
									   2010
