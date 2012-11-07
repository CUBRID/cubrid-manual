*****
MERGE
*****

**MERGE** 문은 하나 또는 그 이상의 원본으로부터 행들을 선택하여 하나의 테이블 또는 뷰로 갱신이나 삽입을 수행하기 위해 사용되며, 대상 테이블 또는 뷰에 갱신할지 또는 삽입할지 결정하는 조건을 지정할 수 있다. **MERGE** 문은 결정적 문장(deterministic statement)으로, 하나의 문장 내에서 대상 테이블의 같은 행을 여러 번 갱신할 수 없다.

**MERGE** 문을 실행하기 위해서는 원본 테이블에 대해 **SELECT** 권한을 가져야 하며, 대상 테이블에 대해 **UPDATE** 절이 포함되어 있으면 **UPDATE** 권한, **DELETE** 절이 포함되어 있으면 **DELETE** 권한, **INSERT** 절이 포함되어 있으면 **INSERT** 권한을 가져야 한다. ::

	MERGE INTO <target> [[AS] <alias>]
	USING <source> [[AS] <alias>], <source> [[AS] <alias>], ...
	ON <join_condition>
	[ <merge_update_clause> ]
	[ <merge_insert_clause> ]
	 
	<merge_update_clause>::=
	WHEN MATCHED THEN UPDATE
	SET <col=expr> [,<col=expr>,…] [WHERE <update_condition>]
	[DELETE WHERE <delete_condition>]
	 
	<merge_insert_clause>::=
	WHEN NOT MATCHED THEN INSERT [(<attributes_list>)]
		VALUES (<expr_list>) [WHERE <insert_condition>]
	
*   <*target*> : 갱신하거나 삽입할 대상 테이블. 여러 개의 테이블 또는 뷰가 될 수 있다.

*   <*source*> : 데이터를 가져올 원본 테이블. 여러 개의 테이블 또는 뷰가 될 수 있으며, 부질의(subquery)도 가능하다.

*   <*join_condition*> : 갱신할 조건을 명시한다.

*   <*merge_update_clause*> : <*join_condition*> 조건이 TRUE이면 대상 테이블의 새로운 칼럼 값들을 지정한다.

    *   **UPDATE** 절이 실행되면, <*target*>에 정의된 **UPDATE** 트리거가 활성화된다.

    *   <*col*> : 업데이트할 칼럼은 반드시 대상 테이블의 칼럼이어야 한다.

    *   <*expr*> : <*source*>와 <*target*>의 칼럼들을 포함하는 표현식도 가능하다. 또는 **DEFAULT** 키워드도 될 수 있다. <*expr*>은 집계 함수를 포함할 수 없다.

    *   <*update_condition*> : 특정 조건이 TRUE일 때만 **UPDATE** 연산을 수행한다. 조건은 원본 테이블과 대상 테이블 둘 다 참조할 수 있다. 이 조건을 만족하지 않는 행들은 업데이트하지 않는다.

    *   <*delete_condition*> : 갱신 이후 삭제할 대상을 지정한다. 이때 **DELETE** 조건은 갱신된 결과 값을 가지고 수행된다. **DELETE** 절이 실행되면 <*target*>에 정의된 **DELETE** 트리거가 활성화된다.

    *   **ON** 조건 절에서 참조된 칼럼은 업데이트할 수 없다.
	
    *   뷰를 업데이트할 때에는 **DEFAULT** 를 명시할 수 없다.

*   <*merge_insert_clause*> : <*join_condition*> 조건이 FALSE이면 대상 테이블의 칼럼으로 값을 삽입한다.

    *   **INSERT** 절이 실행되면, <*target*>에 정의된 **INSERT** 트리거들이 활성화된다.

    *   <*insert_condition*> : 지정한 조건이 TRUE일 때 삽입 연산을 실행한다. <*source*>의 칼럼만 조건에 포함할 수 있다.

    *   <*attribute_list*>: <*target*>에 삽입될 칼럼들이다.

    *   <*expr_list*>: 상수 필터 조건은 모든 원본 테이블의 행들을 대상 테이블에 삽입하는 데 사용될 수 있다. 상수 필터 조건의 예로 ON (1=1)과 같은 것이 있다.

    *   <*merge_update_clause*>만 지정하거나 <*merge_update_clause*>와 함께 지정할 수 있다. 둘 다 명시한다면 순서는 바뀌어도 된다.

다음은 source_table의 값을 target_table에 합치는 예이다.

.. code-block:: sql

	-- source_table
	CREATE TABLE source_table (a INT, b INT, c INT);
	INSERT INTO source_table VALUES (1, 1, 1);
	INSERT INTO source_table VALUES (1, 3, 2);
	INSERT INTO source_table VALUES (2, 4, 5);
	INSERT INTO source_table VALUES (3, 1, 3);
	 
	-- target_table
	CREATE TABLE target_table (a INT, b INT, c INT);
	INSERT INTO target_table VALUES (1, 1, 4);
	INSERT INTO target_table VALUES (1, 2, 5);
	INSERT INTO target_table VALUES (1, 3, 2);
	INSERT INTO target_table VALUES (3, 1, 6);
	INSERT INTO target_table VALUES (5, 5, 2);
	 
	MERGE INTO target_table tt USING source_table st
	ON (st.a=tt.a AND st.b=tt.b)
	WHEN MATCHED THEN UPDATE SET tt.c=st.c
		 DELETE WHERE tt.c = 1
	WHEN NOT MATCHED THEN INSERT VALUES (st.a, st.b, st.c);
	 
	-- the result of above query
	SELECT * FROM target_table;
				a            b            c
	=======================================
				1            2            5
				1            3            2
				3            1            3
				5            5            2
				2            4            5

위의 예에서 source_table의 칼럼 a, b와 target_table의 칼럼 a, b의 값이 같은 경우 target_table의 칼럼 c를 source_table의 칼럼 c값으로 갱신하고, 그렇지 않은 경우 source_table의 레코드 값을 target_table에 삽입하는 예이다. 단, 갱신된 레코드에서 target_table의 칼럼 c의 값이 1이면 해당 레코드는 삭제한다.

다음은 학생들에게 줄 보너스 점수 테이블(bonus)의 레코드를 정리할 때 **MERGE** 문을 이용하는 예제이다.

.. code-block:: sql

	CREATE TABLE bonus (std_id int, addscore int);
	CREATE INDEX i_scores_std_id on scores (std_id);
	 
	INSERT INTO bonus VALUES (1,10);
	INSERT INTO bonus VALUES (2,10);
	INSERT INTO bonus VALUES (3,10);
	INSERT INTO bonus VALUES (4,10);
	INSERT INTO bonus VALUES (5,10);
	INSERT INTO bonus VALUES (6,10);
	INSERT INTO bonus VALUES (7,10);
	INSERT INTO bonus VALUES (8,10);
	INSERT INTO bonus VALUES (9,10);
	INSERT INTO bonus VALUES (10,10);
	 
	CREATE TABLE std (std_id INT, score INT);
	CREATE INDEX i_std_std_id  ON std (std_id);
	CREATE INDEX i_std_std_id_score ON std (std_id, score);
	 
	INSERT INTO std VALUES (1,60);
	INSERT INTO std VALUES (2,70);
	INSERT INTO std VALUES (3,80);
	INSERT INTO std VALUES (4,35);
	INSERT INTO std VALUES (5,55);
	INSERT INTO std VALUES (6,30);
	INSERT INTO std VALUES (7,65);
	INSERT INTO std VALUES (8,65);
	INSERT INTO std VALUES (9,70);
	INSERT INTO std VALUES (10,22);
	INSERT INTO std VALUES (11,67);
	INSERT INTO std VALUES (12,20);
	INSERT INTO std VALUES (13,45);
	INSERT INTO std VALUES (14,30);
	 
	MERGE INTO bonus t USING (SELECT * FROM std WHERE score < 40) s
	ON t.std_id = s.std_id
	WHEN MATCHED THEN
	UPDATE SET t.addscore=t.addscore+s.score*0.1
	WHEN NOT MATCHED THEN
	INSERT (t.std_id, t.addscore) VALUES (s.std_id, 10+s.score*0.1) WHERE s.score<=30;
	 
	SELECT * FROM bonus ORDER BY 1;
	std_id     addscore
	==========================
				1           10
				2           10
				3           10
				4           14
				5           10
				6           13
				7           10
				8           10
				9           10
			   10           12
			   12           12
			   14           13

위의 예에서 원본 테이블은 score가 40 미만인 std 테이블의 레코드 집합이고, 대상 테이블은 bonus이다. **UPDATE** 절에서는 점수(std.score)가 40점 미만인 학생 번호(std_id)는 4, 6, 10, 12, 14이고 이들 중 보너스 테이블(bonus)에 있는 4, 6, 10번에게는 기존 보너스 점수(bonus.addscore)에 자신의 점수의 10%를 추가로 부여한다. INSERT 절에서는 보너스 테이블에 없는 12, 14번에게는 10점과 자신의 점수의 10%를 추가로 부여한다.

.. note:: 뷰를 대상으로 **MERGE** 를 수행할 때 "WITH CHECK OPTION"을 정상 처리하지 못하면서 "Check option exception" 오류가 발생된다.

.. code-block:: sql

	CREATE TABLE t1(a int, b int);
	INSERT INTO t1 values(1, 100);
	INSERT INTO t1 values(2, 200);
	CREATE TABLE t2(a int, b int);
	INSERT INTO t2 values(1, 99);
	INSERT INTO t2 values(2, 999);
	CREATE VIEW v AS SELECT * FROM t1 WHERE b < 150 WITH CHECK OPTION;
	--should succeed, but check option exception occurs
	MERGE into v
	USING t2
	ON (t2.a=v.a)
	WHEN MATCHED THEN
	UPDATE
	SET v.b=t2.b;
	 
	ERROR: Check option exception on view v.

