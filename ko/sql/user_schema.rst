
*************
사용자 스키마
*************

스키마는 데이터베이스 객체의 논리적인 모음이다. 스키마 이름을 통해 객체를 식별할 수 있다는 것이며, 스키마 객체를 물리적으로 분리된 저장공간에 저장하지 않는다. 객체는 하나의 스키마에만 존재할 수 있으며 동시에 다른 스키마에 존재할 수 없다. 스키마는 사용자가 생성할 수 없으며, 데이터베이스 사용자 생성 시 해당 사용자는 사용자 이름과 동일한 이름의 단일 스키마를 소유하게 된다. 스키마에는 테이블, 시리얼, 트리거, 동의어 및 저장 프로시저/함수 가 포함된다. 스키마의 객체는 동일한 스키마 내에서 고유한 이름을 가져야 하지만 다른 스키마의 객체와 중복되는 이름을 가질 수 있다.

객체에 접근하려면 사용자는 "스키마 이름.테이블 이름"의 경로 표현식을 사용해야 한다. 사용자의 스키마가 객체의 스키마와 (소유자) 동일한 경우 경로 표현식에서 스키마 이름을 생략할 수 있다. 스키마 이름을 생략하면 해당 사용자의 스키마 이름이 사용된다. "스키마 이름.테이블 이름"의 경로 표현식을 사용하더라도 사용자는 해당 객체를 사용할 수 있는 권한이 있어야 한다.

.. note::

    11.2 버전 이후부터 "스키마 ≒ 데이터베이스" 개념이 "스키마 ≒ 사용자"로 변경되었다. 11.2 버전 이전에는 데이터베이스에 단일 스키마만 가능했지만 11.2 버전부터는 사용자 별로 스키마를 사용할 수 있다. 따라서 테이블 이름만으로는 다른 사용자가 소유한 테이블에 접근할 수 없으며, 스키마 (소유자) 이름을 접두사로 붙여서 사용해야 해당 테이블에 접근할 수 있다.

    기본 예제 데이터베이스인 demodb에서는 다음과 같은 차이점이 있다.

    .. code-block:: shell

        csql -u dba demodb

    .. code-block:: sql

        SELECT name FROM athlete LIMIT 1;

            ERROR: before ' ; '
            Unknown class "dba.athlete".

        SELECT name FROM public.athlete LIMIT 1;

              name
            ======================
              'Fernandez Jesus'

테이블, 시리얼, 트리거, 동의어 및 저장 프로시저/함수를 관리하는 시스템 테이블에 (_db_class, db_serial, db_trigger, _db_synonym, _db_stored_procedure) unique_name 칼럼이 추가되었다. unique_name 칼럼은 스키마 이름이 접두사로 붙은 이름을 저장한다. _db_class의 unique_name 칼럼에서 시스템 테이블 이름은 스키마 이름이 접두사로 붙지 않는다.

.. code-block:: shell

    csql -u public demodb

.. code-block:: sql

    CREATE TABLE table_1 (column_1 INTEGER);
    CREATE TABLE table_2 (column_1 INTEGER);
    CREATE TRIGGER trigger_1 AFTER INSERT ON table_1 EXECUTE INSERT INTO table_2 VALUES (obj.column_1);

.. code-block:: shell

    csql -u dba demodb

.. code-block:: sql

    SELECT unique_name, class_name, owner.name FROM _db_class ORDER BY unique_name;

          unique_name              class_name               owner.name
        ==============================================================
          '_db_attribute'          '_db_attribute'          'DBA'
          '_db_auth'               '_db_auth'               'DBA'
          '_db_charset'            '_db_charset'            'DBA'
          '_db_class'              '_db_class'              'DBA'
          '_db_collation'          '_db_collation'          'DBA'
              ...
          'db_attr_setdomain_elm'  'db_attr_setdomain_elm'  'DBA'
          'db_attribute'           'db_attribute'           'DBA'
          'db_auth'                'db_auth'                'DBA'
          'db_authorization'       'db_authorization'       'DBA'
          'db_authorizations'      'db_authorizations'      'DBA'
              ...
          'public.athlete'         'athlete'                'PUBLIC'
          'public.code'            'code'                   'PUBLIC'
          'public.event'           'event'                  'PUBLIC'
          'public.game'            'game'                   'PUBLIC'
          'public.history'         'history'                'PUBLIC'

    SELECT unique_name, name, owner.name FROM db_serial ORDER BY unique_name;

          unique_name               name               owner.name
        =========================================================
          'public.athlete_ai_code'  'athlete_ai_code'  'PUBLIC'
          'public.event_no'         'event_no'         'PUBLIC'
          'public.stadium_no'       'stadium_no'       'PUBLIC'

    SELECT unique_name, name, owner.name FROM db_trigger ORDER BY unique_name;

          unique_name         name         owner.name
        =============================================
          'public.trigger_1'  'trigger_1'  'PUBLIC'

    SELECT unique_name, name, owner.name, target_unique_name, target_name, target_owner.name FROM _db_synonym ORDER BY unique_name;

	  unique_name           name           owner.name           target_unique_name           target_name           target_owner.name   
        =================================================================================================================================
          'public.synonym_1'    'synonym_1'    'PUBLIC'             'public.synonym_target_1'    'synonym_target_1'    'PUBLIC' 

    SELECT unique_name, sp_name, owner.name FROM _db_stored_procedure WHERE is_system_generated = 0 ORDER BY unique_name;

	  unique_name                  sp_name                owner.name 
        ================================================================
	  'public.stored_procedure_1'  'stored_procedure_1'  'PUBLIC'
