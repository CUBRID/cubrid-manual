****
분할
****

분할 기법(partitioning)은 하나의 테이블을 여러 독립적인 논리적 단위로 분할하는 기법이다. 분할 기법에서 사용하는 분할된 단위를 분할(partition)이라 한다. 분할은 주로 관리의 편의, 성능, 가용성의 목적으로 사용한다. 분할을 적용함으로써 얻을 수 있는 효과는 다음과 같다.

*   대용량 테이블의 관리 향상
*   데이터 조회 시 접근 범위를 줄임으로써 성능 향상
*   디스크 I/O를 분산함으로써 성능 향상 및 물리적 부하 감소
*   여러 분할로 나눔으로써 전체 데이터의 훼손 가능성 감소 및 가용성 향상
*   스토리지 비용의 최적화

CUBRID는 영역 분할(Range Partitioning), 해시 분할(Hash Partitioning), 리스트 분할(List Partitioning)의 세 가지 분할을 제공한다.

한 테이블이 가질 수 있는 최대 분할 수는 1024이다. 테이블의 각 분할은 그 테이블의 서브 테이블로 생성된다. 분할 정의를 통해 생성된 서브 테이블은 사용자가 임의로 내용을 변경하거나 삭제할 수 없다. 서브 테이블의 이름은 '*class_name*__p__*partition_name*'의 형식으로 시스템 테이블에 등록된다. 데이터베이스 사용자는 db_class 뷰와 db_partition 뷰에서 분할의 정보를 확인할 수 있다. 또 다른 확인 방법은 CUBRID 매니저나 CSQL 인터프리터의 ;sc <테이블명> 명령을 사용하는 것이다.

.. _partition-data-type:

**분할 표현식에 사용할 수 있는 데이터 타입**

분할 키로 사용할 수 있는 칼럼의 데이터 타입은 다음과 같다.

*   **CHAR**
*   **VARCHAR**
*   **NCHAR**
*   **VARNCHAR**
*   **SMALLINT**
*   **INT**
*   **BIGINT**
*   **DATE**
*   **TIME**
*   **TIMESTAMP**
*   **ENUM**

다음과 같은 연산자 함수를 분할키에 적용하는 분할 표현식에 사용할 수 있다.

* 숫자 관련 연산자 함수

  +, -, \*, /, :func:`MOD`, :func:`FLOOR`, :func:`CEIL`, :func:`POWER`, :func:`ROUND`, :func:`ABS`, :func:`TRUNC`

* 문자열 관련 연산자 함수

  :func:`POSITION`, :func:`SUBSTRING`, :func:`OCTET_LENGTH`, :func:`BIT_LENGTH`, :func:`CHAR_LENGTH`, :func:`LOWER`, :func:`UPPER`, :func:`TRIM`, :func:`LTRIM`, :func:`RTRIM`, :func:`LPAD`, :func:`RPAD`, :func:`REPLACE`, :func:`TRANSLATE`

* 날짜 관련 연산자 함수

  :func:`ADD_MONTHS`, :func:`LAST_DAY`, :func:`MONTHS_BETWEEN`, :func:`SYS_DATE`, :func:`SYS_TIME`, :func:`SYS_TIMESTAMP`, :func:`TO_DATE`, :func:`TO_NUMBER`, :func:`TO_TIME`, :func:`TO_TIMESTAMP`, :func:`TO_CHAR`

* 기타 관련 연산자 함수

  :func:`EXTRACT`, :func:`CAST`

영역 분할
=========

.. _defining-range-partitions:

영역 분할 정의
--------------

영역 분할은 **PARTITION BY RANGE** 문을 이용하여 정의한다. ::

	CREATE TABLE(
	...
	)
	PARTITION BY RANGE ( <partition_expression> ) (
	PARTITION <partition_name> VALUES LESS THAN ( <range_value> ),
	PARTITION <partition_name> VALUES LESS THAN ( <range_value> ) ),
	... )
	)

*   *partition_expression* : 분할 표현식을 지정한다. 표현식은 분할 대상이 되는 칼럼 명을 지정하거나 함수를 사용하여 지정할 수 있다. 사용 가능한 데이터 타입과 함수에 대한 자세한 설명은 :ref:`분할 표현식에 사용할 수 있는 데이터 타입 <partition-data-type>` 을 참조한다.
*   *partition_name* : 분할 명을 지정한다.
*   *range_value* : 분할의 기준이 되는 값을 지정한다.

다음은 올림픽 참가국 정보를 담은 *participant2* 테이블을 생성하고 참가한 올림픽의 개최연도를 2000년도 전/후로 영역 분할하는 데이터를 삽입하는 예제이다. 데이터 삽입 시 88년, 96년 올림픽에 참가한 국가는 *before_2000* 에, 나머지 국가는 *before_2008* 에 저장된다.

.. code-block:: sql

	CREATE TABLE participant2 (host_year INT, nation CHAR(3), gold INT, silver INT, bronze INT)
	PARTITION BY RANGE (host_year)
	(PARTITION before_2000 VALUES LESS THAN (2000),
	PARTITION before_2008 VALUES LESS THAN (2008) );
	 
	INSERT INTO participant2 VALUES (1988, 'NZL', 3, 2, 8);
	INSERT INTO participant2 VALUES (1988, 'CAN', 3, 2, 5);
	INSERT INTO participant2 VALUES (1996, 'KOR', 7, 15, 5);
	INSERT INTO participant2 VALUES (2000, 'RUS', 32, 28, 28);
	INSERT INTO participant2 VALUES (2004, 'JPN', 16, 9, 12);

다음과 같이 영역 분할에서 분할 키 값이 **NULL** 이면 첫 번째 분할에 저장된다.

.. code-block:: sql

	INSERT INTO participant2 VALUES(NULL, 'AAA', 0, 0, 0);

.. note::

	*   한 테이블이 가질 수 있는 최대 분할 개수는 1024이다.
	
	*   분할 키 값이 **NULL** 이면, 첫 번째 분할에 저장된다

.. _range-partitioning-redefinition:

영역 분할 재정의
----------------

**ALTER** 문의 **REORGANIZE PARTITION** 절을 이용하여 분할을 재정의한다. 재정의를 통해 여러 개의 분할을 한 개에 결합할 수 있으며, 한 개의 분할을 여러 개로 분리할 수 있다. ::

	ALTER {TABLE | CLASS} <table_name>
	REORGANIZE PARTITION
	<alter partition name comma list>
	INTO ( <partition definition comma list> )
	 
	partitiondefinition comma list:
	PARTITION <partition_name> VALUES LESS THAN ( <range_value> ),.... 

*   *table_name* : 재정의할 테이블의 이름을 지정한다.
*   *alter partition name comma list* : 재정의할 분할을 지정한다. 여러 개인 경우 쉼표(,) 구분한다.
*   *partition definition comma list* : 재정의 내용을 정의한다. 여러 개인 경우 쉼표(,)로 구분한다.

다음은 *participant2* 테이블의 *before_2000* 분할을 *before_1996* 과 *before_2000* 으로 재분할하는 예제이다.

.. code-block:: sql

	CREATE TABLE participant2 ( host_year INT, nation CHAR(3), gold INT, silver INT, bronze INT)
	PARTITION BY RANGE (host_year)
	( PARTITION before_2000 VALUES LESS THAN (2000),
	 PARTITION before_2008 VALUES LESS THAN (2008) );
	 
	ALTER TABLE participant2 REORGANIZE PARTITION before_2000 INTO (
	PARTITION before_1996 VALUES LESS THAN (1996),
	PARTITION before_2000 VALUES LESS THAN (2000)
	);

다음은 예제 1에서 재정의했던 분할을 다시 *before_2000* 하나로 결합하는 예제이다.

.. code-block:: sql

	ALTER TABLE participant2 REORGANIZE PARTITION before_1996, before_2000 INTO
	(PARTITION before_2000 VALUES LESS THAN (2000) );

.. note::

	*   영역 및 리스트 분할 테이블을 재정의할 때, 새로운 분할 테이블에는 중복된 영역이나 값은 허용되지 않는다.
	
	*   **REORGANIZE PARTITION** 절을 사용해 테이블의 분할 종류를 변경할 수 없다. 예를 들어, 영역 분할을 해시 분할로 변경할 수 없으며, 그 반대도 마찬가지이다.
	
	*   분할 추가 후 최대 분할의 개수는 1,024개를 넘지 못하며, 분할 삭제 후 최소 1개 이상의 분할이 남아 있어야 한다. 영역 분할 테이블은 인접한 분할만 재정의할 수 있다.

.. _range-partitioning-append:

영역 분할 추가
--------------

**ALTER** 구문의 **ADD PARTITION** 절을 이용하여 분할된 테이블에 분할을 추가한다. ::

	ALTER {TABLE | CLASS} <table_name>
	ADD PARTITION <partition definitions comma list>
	partition definition comma list:
	PARTITION <partition_name> VALUES LESS THAN ( <range_value> ),...

*   *table_name* : 분할을 추가할 테이블의 이름을 지정한다.
*   *partition definition comma list* : 추가할 분할을 정의한다. 여러 개인 경우 쉼표(,)로 구분한다.

현재 *participant2* 테이블에는 2008년 이전 올림픽 정보에 관한 분할만 정의되어 있다. 다음은 2012년 올림픽 정보가 저장될 *before_2012* 분할과 2016년 올림픽 정보가 저장될 *before_2016* 분할을 추가하는 예제이다.

.. code-block:: sql

	ALTER TABLE participant2 ADD PARTITION (
	PARTITION before_2012 VALUES LESS THAN (2012),
	PARTITION before_2016 VALUES LESS THAN MAXVALUE );

.. note::

	*   영역 분할을 추가할 때는 분할 기준 값이 기존의 분할보다 큰 값만 추가할 수 있다. 따라서, 위의 예제처럼 **MAXVALUE** 로 최대값을 설정하면 더 이상 분할을 추가할 수 없다(분할 재정의를 통해서 **MAXVALUE** 를 다른 값으로 변경하면 분할 추가 가능).

	*   기존의 분할보다 작은 분할 기준 값을 추가하려면 분할 재정의를 이용한다(:ref:`range-partitioning-redefinition` 참조).

영역 분할 삭제
--------------

**ALTER** 구문의 **DROP PARTITION** 절을 이용하여 분할을 삭제한다. ::

	ALTER {TABLE | CLASS} <table_name>
	DROP PARTITION <partition_name>

*   *table_name* : 분할된 테이블의 이름을 지정한다.
*   *partition_name* : 삭제할 분할의 이름을 지정한다.

다음은 *participant2* 테이블의 *before_2000* 분할을 삭제한다. 

.. code-block:: sql

	ALTER TABLE participant2 DROP PARTITION before_2000;

.. note::

	*   분할된 테이블을 삭제하면 해당 분할 내에 저장된 데이터도 모두 삭제된다.
	
	*   데이터는 유지한 채 테이블의 분할을 변경하는 경우 **ALTER TABLE** ... **REORGANIZE PARTITION** 문을 사용한다(:ref:`range-partitioning-redefinition` 참조).
	
	*   분할을 삭제할 경우 삭제된 행의 수를 반환하지 않는다. 테이블과 분할을 유지한 채로 데이터만 삭제하고 싶은 경우 **DELETE** 문을 수행한다.

해시 분할
=========

해시 분할 정의
--------------

해시 분할은 **PARTITION BY HASH** 문을 이용하여 정의한다. ::

	CREATE TABLE (
	...
	)
	( PATITION BY HASH ( <partition_expression> )
	 PATITIONS ( <number_of_partitions> )
	)
	
*   *partition_expression* : 분할 표현식을 지정한다. 표현식은 분할 대상이 되는 칼럼 이름이나 함수를 사용하여 지정할 수 있다.
*   *number_of_partitions* : 원하는 분할의 수를 지정한다.

다음은 국가 코드와 국가 이름의 정보를 담은 *nation2* 테이블을 생성하고 *code* 값을 기준으로 4개의 해시 분할을 정의하는 예제이다. 해시 분할은 분할의 수만 지정하고 이름은 지정하지 않으므로 p0, p1과 같이 자동으로 이름이 부여된다.

.. code-block:: sql

	CREATE TABLE nation2
	( code CHAR(3),
	  name VARCHAR(50) )
	PARTITION BY HASH ( code) PARTITIONS 4;

다음은 예제 1에서 생성한 해시 분할에 데이터를 삽입하는 예제이다. 해시 분할에 값을 입력하면 분할 키의 해시 값에 따라 저장될 분할이 결정된다. 해시 분할에서 분할키 값이 **NULL** 이면 첫 번째 분할에 저장된다.

.. code-block:: sql

	INSERT INTO nation2 VALUES ('KOR','Korea');
	INSERT INTO nation2 VALUES ('USA','USA United States of America');
	INSERT INTO nation2 VALUES ('FRA','France');
	INSERT INTO nation2 VALUES ('DEN','Denmark');
	INSERT INTO nation2 VALUES ('CHN','China');
	INSERT INTO nation2 VALUES (NULL,'AAA');

.. note::

	한 테이블이 가질 수 있는 최대 분할 개수는 1024이다.

해시 분할 재정의
----------------

**ALTER** 문의 **COALESCE PARTITION** 절을 이용하여 재정의할 수 있다. 해시 분할이 재정의되는 경우 인스턴스는 그대로 보존된다. ::

	ALTER {TABLE | CLASS} <table_name>
	COALESCE PARTITION <unsigned integer>

*   *table_name* : 재정의할 테이블의 이름을 지정한다.
*   *unsigned integer* : 삭제하려는 분할의 개수를 지정한다.

다음은 *nation2* 테이블의 분할의 개수를 4개에서 3개로 줄이는 예제이다.

.. code-block:: sql

	ALTER TABLE nation2 COALESCE PARTITION 1;
	
.. note::

	*   분할의 개수를 감소시키는 재편성 결합만 가능하다.
	
	*   분할의 수를 늘리고자 하는 경우에는 영역 분할에서와 같은 **ALTER TABLE** ... **ADD PARTITION** 구문을 이용한다(자세한 내용은 :ref:`range-partitioning-append` 참조).
	
	*   분할 재정의 후에 최소 1개 이상의 분할이 남아 있어야 한다.

리스트 분할
===========

리스트 분할 정의
----------------

리스트 분할은 **PARTITIION BY LIST** 문을 이용하여 정의한다. ::

	CREATE TABLE(
	...
	)
	PARTITION BY LIST ( <partition_expression> ) (
	PARTITION <partition_name> VALUES IN ( <partition_value_list> ),
	PARTITION <partition_name> VALUES IN ( <partition_value_ list>, ...
	);

*   *partition_expression* : 분할 표현식을 지정한다. 표현식은 분할 대상이 되는 칼럼 명을 지정하거나 함수를 사용하여 지정할 수 있다. 사용 가능한 데이터 타입과 함수에 대한 자세한 내용은 :ref:`분할 표현식에 사용할 수 있는 데이터 타입 <partition-data-type>` 을 참조한다.
*   *partition_name* : 분할 명을 지정한다.
*   *partition_value_list* : 분할의 기준이 되는 값의 목록을 지정한다.

다음은 선수의 이름과 종목 정보를 담고있는 *athlete2* 테이블을 생성하고 종목에 따른 리스트 분할을 정의하는 예제이다.

.. code-block:: sql

	CREATE TABLE athlete2( name VARCHAR(40), event VARCHAR(30) )
	PARTITION BY LIST (event) (
	PARTITION event1 VALUES IN ('Swimming', 'Athletics ' ),
	PARTITION event2 VALUES IN ('Judo', 'Taekwondo', 'Boxing'),
	PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball'));

다음은 예제 1에서 생성한 리스트 분할에 데이터를 삽입하는 예제이다. 마지막 질의와 같이 데이터 삽입 시 분할 표현식에서 기술하였던 리스트에 없는 값으로 삽입하는 경우에는 삽입이 이루어지지 않는다.

.. code-block:: sql

	INSERT INTO athlete2 VALUES ('Hwang Young-Cho', 'Athletics');
	INSERT INTO athlete2 VALUES ('Lee Seung-Yuop', 'Baseball');
	INSERT INTO athlete2 VALUES ('Moon Dae-Sung','Taekwondo');
	INSERT INTO athlete2 VALUES ('Cho In-Chul', 'Judo');
	INSERT INTO athlete2 VALUES ('Hong Kil-Dong', 'Volleyball');

다음은 분할키 값이 **NULL** 인 경우에 삽입이 이루어지지 않고 에러가 발생함을 보여주는 예제이다. **NULL** 값을 삽입 가능하도록 분할을 정의하려면 두 번째 코드와 같이 **NULL** 값을 리스트로 갖는 분할을 정의하면 된다.

.. code-block:: sql

	INSERT INTO athlete2 VALUES ('Hong Kil-Dong','NULL');
	 
	CREATE TABLE athlete2( name VARCHAR(40), event VARCHAR(30) )
	PARTITION BY LIST (event) (
	PARTITION event1 VALUES IN ('Swimming', 'Athletics ' ),
	PARTITION event2 VALUES IN ('Judo', 'Taekwondo','Boxing'),
	PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball', NULL));

.. note::

	한 테이블이 가질 수 있는 최대 분할 개수는 1024이다.

리스트 분할 재정의
------------------

**ALTER** 문의 **REORGANIZE PARTITION** 절을 이용하여 재정의할 수 있다. 재정의를 통해 여러 개의 분할을 한 개로 결합할 수 있으며, 한 개의 분할을 여러 개로 분리할 수 있다. ::

	ALTER {TABLE | CLASS} <table_name>
	REORGANIZEPARTITION
	<alter partition name comma list>
	INTO ( <partition definition comma list> )
	partition definition comma list:
	PARTITION <partition_name> VALUES IN ( <partition_value_list>),... 

*   *table_name* : 재정의할 테이블의 이름을 지정한다.
*   *alter partition name comma list* : 재정의할 분할을 지정한다. 여러 개인 경우 쉼표(,)로 구분한다.
*   *partition definition comma list* : 재정의 내용을 정의한다. 여러 개인 경우 쉼표(,)로 구분한다.

다음은 종목에 따라 리스트 분할한 *athlete2* 테이블을 생성하고 분할 *event2* 를 *event2_1* (유도), *event2_2* (태권도, 복싱)로 재정의하는 예제이다.

.. code-block:: sql

	CREATE TABLE athlete2( name VARCHAR(40), event VARCHAR(30) )
	PARTITION BY LIST (event) (
	PARTITION event1 VALUES IN ('Swimming', 'Athletics ' ),
	PARTITION event2 VALUES IN ('Judo', 'Taekwondo','Boxing'),
	PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball'));

	ALTER TABLE athlete2 REORGANIZE PARTITION event2 INTO
	(PARTITION event2_1 VALUES IN ('Judo'),
	PARTITION event2_2 VALUES IN ( 'Taekwondo','Boxing'));

다음은 예제 1에서 분할한 *event2_1* 과 *event2_2* 를 다시 *event2* 하나로 결합하는 예제이다.

.. code-block:: sql

	ALTER TABLE athlete2 REORGANIZE PARTITION event2_1, event2_2 INTO
	(PARTITION event2 VALUES IN('Judo','Taekwondo','Boxing'));

리스트 분할 삭제
----------------

**ALTER** 구문의 **DROP PARTITION** 절을 이용하여 분할을 삭제할 수 있다. ::

	ALTER {TABLE | CLASS} <table_name>
	DROP PARTITION <partition_name>

*   *table_name* : 분할된 테이블의 이름을 지정한다.
*   *partition_name* : 삭제할 분할의 이름을 지정한다.

다음은 종목에 따라 리스트 분할한 *athlete2* 테이블을 생성하고 *event3* 분할을 삭제하는 예제이다.

.. code-block:: sql

	CREATE TABLE athlete2( name VARCHAR(40), event VARCHAR(30) )
	PARTITION BY LIST (event) (
	PARTITION event1 VALUES IN ('Swimming', 'Athletics ' ),
	PARTITION event2 VALUES IN ('Judo', 'Taekwondo','Boxing'),
	PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball'));
	
	ALTER TABLE athlete2 DROP PARTITION event3;

분할에서 데이터 조회와 조작
===========================

분할에서 데이터 조회
--------------------

데이터를 조회할 때에는 분할 테이블뿐만 아니라 각 분할에 대해서도 **SELECT** 문을 이용하여 조회가 가능하다.

다음은 종목에 따라 리스트 분할한 *athlete2* 테이블을 생성하고 데이터를 삽입한 뒤 *event1* 분할과 *event2* 분할을 조회하는 예제이다.

.. code-block:: sql

	CREATE TABLE athlete2( name VARCHAR(40), event VARCHAR(30) )
	PARTITION BY LIST (event) (
	PARTITION event1 VALUES IN ('Swimming', 'Athletics ' ),
	PARTITION event2 VALUES IN ('Judo', 'Taekwondo','Boxing'),
	PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball')
	);

	INSERT INTO athlete2 VALUES ('Hwang Young-Cho', 'Athletics');
	INSERT INTO athlete2 VALUES ('Lee Seung-Yuop', 'Baseball');
	INSERT INTO athlete2 VALUES ('Lee Sun-Hee','Taekwondo');
	INSERT INTO athlete2 VALUES ('Cho In-Chul', 'Judo');

	SELECT * from athlete2__p__event1;
	  name                  event
	============================================
	  'Hwang Young-Cho'     'Athletics'

	SELECT * from athlete2__p__event2;
	  name                  event
	============================================
	  'Lee Sun-Hee'         'Taekwondo'
	  'Cho In-Chul'         'Judo'
  
.. note:

	분할 테이블의 각 분할에 대해서 직접적인 데이터 삽입, 갱신, 삭제 등 데이터 조작은 허용되지 않는다.

분할 키 값의 변경에 의한 데이터 이동
------------------------------------

**설명**

분할의 분할 키 값이 변경되면 변경된 인스턴스는 분할 표현식에 의해서 다른 분할로 이동할 수 있다.

다음은 분할 키 값이 변경되어 인스턴스가 다른 분할로 이동하는 것을 보여주는 예제이다.

*event1* 분할에 저장되어 있는 황영조 선수의 종목 정보를 'Athletics'에서 'Football'로 바꾸면 인스턴스가 *event3* 분할로 이동된다.

.. code-block:: sql

	CREATE TABLE athlete2( name VARCHAR(40), event VARCHAR(30) )
	PARTITION BY LIST (event) (
	PARTITION event1 VALUES IN ('Swimming', 'Athletics ' ),
	PARTITION event2 VALUES IN ('Judo', 'Taekwondo','Boxing'),
	PARTITION event3 VALUES IN ('Football', 'Basketball', 'Baseball'));
	
	INSERT INTO athlete2 VALUES ('Hwang Young-Cho', 'Athletics');
	INSERT INTO athlete2 VALUES ('Lee Seung-Yuop', 'Baseball');

	SELECT * FROM athlete2__p__event1;
	  name                  event
	============================================
	  'Hwang Young-Cho'     'Athletics'

	UPDATE athlete2 SET event = 'Football' WHERE name = 'Hwang Young-Cho';

	SELECT * FROM athlete2__p__event3;
	  name                  event
	============================================
	  'Lee Seung-Yuop'      'Baseball'
	  'Hwang Young-Cho'     'Football'
  
.. note::

	분할 키 값의 변경에 의한 분할 간 데이터 이동은 내부적으로 삭제와 삽입을 수반하여 성능 저하의 원인이 될 수 있으므로 사용에 주의한다.

분할에 대한 로컬 인덱스와 글로벌 인덱스
---------------------------------------

분할 테이블에서 생성되는 인덱스는 로컬 인덱스 또는 글로벌 인덱스로 구분된다. 글로벌 인덱스는 모든 분할들로부터 데이터를 유지하는 하나의 인덱스 구조를 정의하지만, 로컬 인덱스는 각 분할마다 하나의 인덱스를 정의한다. 로컬 인덱스가 될 것인지 혹은 글로벌 인덱스가 될 것인지의 선택을 사용자가 제어할 수는 없으며, 다음 규칙에 따라 시스템이 자동으로 결정한다.

*   모든 기본 키는 글로벌 인덱스이다.
*   모든 외래 키는 로컬 인덱스이다.
*   모든 비고유 인덱스는 로컬 인덱스이다.
*   고유 인덱스는 로컬 또는 글로벌 인덱스이다. 분할 키가 고유 인덱스에 속하면 로컬 인덱스이고, 그렇지 않으면 글로벌 인덱스이다.

분할 프루닝
-----------

분할 프루닝(partition pruning)은 검색 조건을 통해 데이터 검색 범위를 한정시키는 기능이다. 질의에서 필요한 데이터를 포함하고 있지 않은 분할은 검색 과정에서 제외시킨다. 이를 통해 디스크로부터 인출되는 데이터의 양과 처리 시간을 크게 줄이고 질의 성능 및 자원 사용률을 개선할 수 있다.

.. note::

	CUBRID 9.0 미만 버전에서 분할 프루닝은 질의 컴파일 단계에서 수행되었으나, CUBRID 9.0 이상 버전에서는 질의 실행 단계에 서버 단에서 수행된다. 따라서 기존보다 더 복잡하고 다양한 질의들에 대해서 분할 프루닝을 수행할 수 있지만, 분할 프루닝 질의에 대해 질의 계획 정보를 출력할 수 없으며 **ORDER BY SKIP** 최적화, **GROUP BY SKIP** 최적화를 지원하지 않는다.

다음은 참가한 올림픽의 개최연도에 따라 영역 분할하는 *olympic2* 테이블을 생성하고 2000년도 시드니 올림픽 이후의 올림픽에 참가한 국가를 조회하는 질의이다. **WHERE** 절에서 분할 키에 대하여 상수 값과 동등 비교하거나 범위 비교하는 경우 분할 프루닝이 발생한다.

예제의 경우 2000보다 작은 연도 값을 가진 *before_1996* 분할은 접근하지 않는다.

.. code-block:: sql

	CREATE TABLE olympic2
	( opening_date DATE, host_nation VARCHAR(40))
	PARTITION BY RANGE ( EXTRACT (YEAR FROM opening_date) )
	( PARTITION before_1996 VALUES LESS THAN (1996),
	  PARTITION before_MAX VALUES LESS THAN MAXVALUE );
	 
	SELECT opening_date, host_nation FROM olympic2 WHERE EXTRACT ( YEAR FROM (opening_date)) >= 2000;

다음은 분할 프루닝이 되지 않는 경우에 사용자가 특정 분할을 지정하여 데이터를 조회함으로써 분할 프루닝의 효과를 얻는 방법을 보여주는 예제이다.

예제에서 첫 번째 질의는 비교 값이 분할 표현식과 같은 형식이 아니므로 분할 프루닝이 일어나지 않는다. 따라서 두 번째 질의와 같이 알맞은 분할을 지정하여 분할 프루닝이 발생하는 것과 같은 기능을 사용할 수 있다.

.. code-block:: sql

	SELECT host_nation FROM olympic2 WHERE opening_date >= '2000 - 01 - 01';

	SELECT host_nation FROM olympic2__p__before_max WHERE opening_date >= '2000 - 01 - 01';

다음은 해시 분할 테이블인 *manager* 테이블에서 분할 프루닝이 발생하도록 검색 조건을 지정한 예제이다. 해시 분할의 경우 **WHERE** 절에서 분할 키에 대하여 상수 값과 동등 비교를 하는 경우에만 분할 프루닝이 발생한다.

.. code-block:: sql

	CREATE TABLE manager (
	code INT,
	name VARCHAR(50))
	PARTITION BY HASH (code) PARTITIONS 4;
	 
	SELECT * FROM manager WHERE code = 10053;

.. note::

	* 분할 표현식과 비교되는 값은 서로 같은 형식이어야 한다.
	
	* 해시 분할과 리스트 분할에서 프루닝이 가능하려면 **WHERE** 절에 다음의 분할 키 표현식을 사용해야 한다. 아래의 상수 표현식은 테이블 칼럼을 포함하지 않는 표현식이며, 다른 조건은 사용할 수 없다.
	
		* < *분할 키* > = < *상수 표현식* >
		* < *분할 키* > { IN | = SOME | = ANY } ( < *상수 표현식 리스트* > )
		
	* 영역 분할에서 프루닝이 가능하려면 **WHERE** 절에 다음의 분할 키 표현식을 사용해야 한다.
	
		* < *분할 키* > { < | > | = | <= | >= | } < *상수 표현식* >
		* < *분할 키* > BETWEEN < *상수 표현식* > AND < *상수 표현식* >

분할 관리
=========

일반 테이블을 분할 테이블로 변경
--------------------------------

일반 테이블을 분할 테이블로 변경하려면 **ALTER TABLE** 문을 이용한다. **ALTER TABLE** 문을 이용하여 세 종류의 분할 모드로 변경 가능하다. 분할 테이블로 변경하면 기존 테이블에 있던 데이터는 분할 정의에 따라 각 분할로 이동 저장된다. 일반 테이블의 데이터를 분할 테이블로 이동하는 것이므로 데이터 양에 따라 긴 작업 시간이 필요할 수 있다. ::

	ALTER {TABLE | CLASS} table_name
	PARTITION BY {RANGE | HASH | LIST } ( <partition_expression> )
	( PARTITION partition_name VALUES LESS THAN { MAXVALUE | ( <partition_value_option> ) }
	| PARTITION partition_name VALUES IN ( <partition_value_option list) > ]
	| PARTITION <UNSINGED_INTEGER> )

	<partition_expression>
	expression_
	<partition_value_option>
	literal_

*   *table_name* : 변경하려는 테이블의 이름을 지정한다.
*   *partition_expression* : 분할 표현식을 지정한다. 표현식은 분할 대상이 되는 칼럼 명을 지정하거나 함수를 사용하여 지정할 수 있다. 사용 가능한 데이터 타입과 함수에 대한 자세한 내용은 :ref:`분할 표현식에 사용할 수 있는 데이터 타입 <partition-data-type>` 을 참조한다.
*   *partition_name* : 분할명을 지정한다.
*   *partition_value_option* : 분할의 기준이 되는 값 또는 값의 목록을 지정한다.

다음은 record 테이블을 영역, 리스트, 해시 분할로 각각 변경하는 예제이다.

.. code-block:: sql

	ALTER TABLE record PARTITION BY RANGE (host_year)
	( PARTITION before_1996 VALUES LESS THAN (1996),
	  PARTITION after_1996 VALUES LESS THAN MAXVALUE);

	ALTER TABLE record PARTITION BY list (unit)
	( PARTITION time_record VALUES IN ('Time'),
	  PARTITION kg_record VALUES IN ('kg'),
	  PARTITION meter_record VALUES IN ('Meter'),
	  PARTITION score_record VALUES IN ('Score') );

	ALTER TABLE record
	PARTITION BY HASH (score) PARTITIONS 4;

.. note::

	분할 조건을 충족하지 않는 데이터가 존재하는 경우에는 분할이 정의되지 않는다.

분할 테이블을 일반 테이블로 변경
--------------------------------

기존에 정의된 분할 테이블을 일반 테이블로 변경하려면 **ALTER TABLE** 문을 이용한다. 분할을 제거한다고 해서 테이블의 데이터가 삭제되는 것은 아니다. ::

	ALTER {TABLE | CLASS} <table_name>
	REMOVE PARTITIONING

*   *table_name* : 변경하고자 하는 테이블의 이름을 지정한다.

다음은 분할 테이블인 *nation2* 를 일반 테이블로 변경하는 예제이다.

.. code-block:: sql

	ALTER TABLE nation2 REMOVE PARTITIONING;

분할 PROMOTE 문
---------------

분할(partition) **PROMOTE** 문은 분할 테이블에서 사용자가 지정한 분할을 독립적인 일반 테이블로 승격(promote)한다. 이것은 거의 접근하지 않는 매우 오래된 데이터를 쌓아놓을(archiving) 목적으로만 유지하려 할 때 유용하다. 해당 분할을 일반 테이블로 승격함으로써 유용한 데이터는 더 적은 수의 분할을 갖게 되므로 접근 부하는 줄이고 오래된 데이터는 편리하게 보존할 수 있다.

분할 **PROMOTE** 문은 영역 분할(range partition) 테이블과 리스트 분할(list partition) 테이블에만 허용된다. 해시 분할 테이블은 사용자가 제어할 수 있는 방법이 없으므로 승격을 허용하지 않는다.

분할이 일반 테이블로 승격될 때 그 테이블은 오직 데이터와 비고유 로컬 인덱스만 상속받는다. 이것은 다음의 테이블 속성들이 승격된 테이블에 저장되지 않는다는 것을 의미한다.

*   기본 키
*   외래 키
*   고유 인덱스
*   **AUTO_INCREMENT** 속성 및 시리얼
*   트리거
*   메서드
*   상속 관계(수퍼클래스와 서브클래스)

다음 속성들은 승격된 테이블에서도 그대로 사용된다.

*   레코드 속성(칼럼 타입들)
*   테이블 속성
*   로컬 인덱스(고유 인덱스, 기본 키, 외래 키가 아닌 일반 인덱스)

**제약 사항**

*   외래 키가 존재하는 분할 테이블의 분할은 승격할 수 없다.
*   해시 분할 테이블을 승격하는 것은 허용되지 않는다.

::

	ALTER TABLE identifier PROMOTE PARTITION <identifier_list>

*   <*identifier_list*> : 승격할 분할 이름

다음은 리스트 분할을 승격한 예이다.

.. code-block:: sql

	CREATE TABLE t(i int) PARTITION BY LIST(i) (
		partition p0 values in (1, 2, 3),
		partition p1 values in (4, 5, 6),
		partition p2 values in (7, 8, 9),
		partition p3 values in (10, 11, 12)
	);
	 
	ALTER TABLE t PROMOTE PARTITION p1, p2;

승격 이후 테이블 *t* 의 파티션은 *p0*, *p3* 만 가지게 되며, *p1*, *p2* 는 각각 *t__p__p1*, *t__p__p2* 인 테이블로 접근할 수 있다. ::

	csql> ;schema t
	=== <Help: Schema of a Class> ===
	 <Class Name>
		 t
	 <Sub Classes>
		 t__p__p0
		 t__p__p3
	 <Attributes>
		 i                    INTEGER
	 <Partitions>
		 PARTITION BY LIST ([i])
		 PARTITION p0 VALUES IN (1, 2, 3)
		 PARTITION p3 VALUES IN (10, 11, 12)
	 
	csql> ;schema t__p__p1
	=== <Help: Schema of a Class> ===
	 <Class Name>
		 t__p__p1
	 <Attributes>
		 i                    INTEGER
	 
다음은 범위 분할을 승격한 예이다.

.. code-block:: sql

	CREATE TABLE t(i int, j int) PARTITION BY RANGE(i) (
			PARTITION p0 VALUES LESS THAN (1),
			PARTITION p1 VALUES LESS THAN (10),
			PARTITION p2 VALUES LESS THAN (100),
			PARTITION p3 VALUES LESS THAN MAXVALUE
		  );
	 
	CREATE UNIQUE INDEX u_t_i ON t(i);
	CREATE INDEX i_t_j ON t(j);
	 
	ALTER TABLE t PROMOTE PARTITION p1, p2;

승격 이후 테이블 *t* 의 파티션은 *p0*, *p3* 만 가지게 되며, *p1*, *p2* 는 각각 *t__p__p1*, *t__p__p2* 인 테이블로 접근할 수 있다. 승격된 테이블 *t__p__p1*, *t__p__p2* 에는 기본 키, 외래 키, 고유 키 등 테이블의 일부 속성이나 인덱스가 제거된 상태라는 점에 주의한다. ::

	csql> ;schema t
	=== <Help: Schema of a Class> ===
	 <Class Name>
		 t
	 <Sub Classes>
		 t__p__p0
		 t__p__p3
	 <Attributes>
		 i                    INTEGER
		 j                    INTEGER
	 <Constraints>
		UNIQUE u_t_i ON t (i)
		INDEX i_t_j ON t (j)
	 <Partitions>
		 PARTITION BY RANGE ([i])
		 PARTITION p0 VALUES LESS THAN (1)
		 PARTITION p3 VALUES LESS THAN MAXVALUE
	 
	csql> ;schema t__p__p1
	=== <Help: Schema of a Class> ===
	 <Class Name>
		 t__p__p1
	 <Attributes>
		 i                    INTEGER
		 j                    INTEGER
	 <Constraints>
		INDEX idx_t_j ON t (j)
	
분할 테이블을 이용하여 VIEW 생성
--------------------------------

분할 테이블의 각 분할을 이용하여 뷰를 정의할 수 있다. 이 때, 생성된 뷰를 이용하여 데이터를 조회할 수 있지만, 데이터 삽입, 삭제, 갱신은 할 수 없다.

다음은 참가연도에 따라 영역 분할된 *participant2* 테이블을 생성하고 *participant2__p__before_2000* 분할을 이용하여 뷰를 생성, 조회하는 예제이다.

.. code-block:: sql

	CREATE TABLE participant2 (host_year INT, nation CHAR(3), gold INT, silver INT, bronze INT)
	PARTITION BY RANGE (host_year)
	( PARTITION before_2000 VALUES LESS THAN (2000),
	 PARTITION before_2008 VALUES LESS THAN (2008) );

	INSERT INTO participant2 VALUES (1988, 'NZL', 3, 2, 8);
	INSERT INTO participant2 VALUES (1988, 'CAN', 3, 2, 5);
	INSERT INTO participant2 VALUES (1996, 'KOR', 7, 15, 5);
	INSERT INTO participant2 VALUES (2000, 'RUS', 32, 28, 28);
	INSERT INTO participant2 VALUES (2004, 'JPN', 16, 9, 12);

	CREATE VIEW v_2000 AS
	SELECT * FROM participant2__p__before_2000
	WHERE host_year = 1988;

	SELECT * FROM v_2000;
		host_year  nation                       gold       silver       bronze
	==========================================================================
			 1988  'NZL'                           3            2            8
			 1988  'CAN'                           3            2            5


분할 테이블의 통계 정보 갱신
----------------------------

질의 수행 시 분할 프루닝을 통해 검색할 범위를 한정하므로 질의 계획에는 분할 정보를 포함하지 않게 되어, 분할 테이블에서 통계 정보 갱신은 더 이상 불필요하다.

.. note::

	CUBRID 9.0 미만 버전에서는 **ANALYZE PARTITION** 구문을 통해 분할 테이블의 통계 정보를 갱신했는데, CUBRID 9.0 버전부터는 이 구문 수행 시 실제로 아무런 동작도 하지 않지만 이전 버전과의 호환을 위해 오류로 처리하지는 않는다.

분할과 상속 관계
----------------

분할들(partitions)은 계층 구조 체인의 일부가 될 수 없으며, 분할 테이블(partitioned table)과 하위 클래스(subclass) 관계를 가지는 것과 다르다. 실제로 분할 테이블은 상위 클래스(superclass)와 하위 클래스(subclass)를 갖게 되지만, CUBRID는 하나의 분할이 오직 하나의 상위 클래스(superclass), 즉 하나의 분할 테이블만 가지며 여러 개의 하위 클래스(subclasses)를 가지지 않도록 보장한다.
