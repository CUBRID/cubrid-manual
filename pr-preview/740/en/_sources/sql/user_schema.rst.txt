
***********
User Schema
***********

A schema is a logical collection of database objects. The object can be identified through the schema name, and the schema object is not stored in a physically separate storage space. An object can only exist in one schema and cannot exist in another schema at the same time. A schema cannot be created by a user, and when a database user is created, the user owns a single schema with the same name as the user name. A schema contains tables, serials, and triggers. An object in a schema must have a unique name within the same schema, but it can have a name that is duplicated with an object in another schema. Table, serial, and trigger names are prefixed with the schema name, so the schema can be identified by the name.

To access the object, the user must use the path expression of "schema_name.table_name". If the user's schema is the same as the object's schema (owner), the schema name can be omitted from the path expression. If the schema name is omitted, the user's schema name is used. Even if the path expression of “schema_name.table_name” is used, the user must have permission to use the object.

.. note::

    Since version 11.2, "Schema ≒ Database" has been changed to "Schema ≒ User". Before version 11.2, there was only a single schema in the database. Since version 11.2, schema is available for each user. It is not possible to access a table owned by another user only by the table name. Tables can be accessed using the schema (owner) name as a prefix.

    The following difference occurs in demodb, which is the default example database.

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

A unique_name column has been added to the system tables (_db_class, db_serial, db_trigger) that manage tables, serials, and triggers. The unique_name column stores names prefixed with schema names. In the unique_name column of _db_class, the system table name is not prefixed with the schema name.

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
