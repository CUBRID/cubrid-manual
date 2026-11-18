
:meta-keywords: cubrid ruby driver, cubrid ruby api, ruby sample
:meta-description: CUBRID Ruby driver implements the interface to enable access from applications in Ruby to CUBRID database server and official one is available as a RubyGem package. CUBRID Ruby driver is written based on CCI API.

*************
Ruby 드라이버
*************

CUBRID Ruby 드라이버는 Ruby로 작성한 응용 프로그램에서 CUBRID 데이터베이스를 사용할 수 있게 하는 드라이버로, RubyGem 패키지 형태로 제공된다.

CUBRID Ruby 드라이버는 CCI API를 기반으로 작성되었으므로, CCI API 및 CCI에 적용되는 **CCI_DEFAULT_AUTOCOMMIT** 과 같은 설정 파라미터에 영향을 받는다.

.. FIXME: 별도로 Ruby 드라이버를 다운로드하거나 Ruby 드라이버에 대한 최신 정보를 확인하려면 http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-driver\ 에 접속한다.

Ruby 설치 및 설정
=================

**기본 환경**

*   Ruby 1.8.7 이상
*   CUBRID gem
*   ActiveRecord gem

**Linux**

**gem** 을 사용하여 CUBRID Connector를 설치할 수 있다. 다음과 같이 **sudo** 명령어에 **-E** 옵션을 사용하여 **sudo** 명령어가 CUBRID 데이터베이스 설치 경로 환경 변수를 변경하지 않도록 해야 한다. ::

    sudo -E gem install cubrid

**Windows**

다음 명령어를 실행하여 최신 버전의 CUBRID Ruby 드라이버를 설치한다. ::

    gem install cubrid

.. FIXME: .. note:: 

.. FIXME:     Gem Installer가 설치되어 있지 않다면 http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-driver-installation-instructions 를 참고한다.

Ruby 예제 프로그램
==================

여기에서는 Ruby로 CUBRID 데이터베이스에 대한 작업을 수행하는 예제 프로그램을 작성한다. 예제로 다음과 같이 테이블을 생성한다.

.. code-block:: sql

    CREATE TABLE countries(
        id INTEGER AUTO_INCREMENT,
        code CHARACTER VARYING(3) NOT NULL UNIQUE,
        name CHARACTER VARYING(40) NOT NULL UNIQUE,
        record_date DATETIME DEFAULT SYSDATETIME NOT NULL,
        CONSTRAINT pk_countries_id PRIMARY KEY(id)
    );
    
    CREATE TABLE cities(
        id INTEGER AUTO_INCREMENT NOT NULL UNIQUE,
        name CHARACTER VARYING(40) NOT NULL,
        country_id INTEGER NOT NULL,
        record_date DATETIME DEFAULT SYSDATETIME NOT NULL,
        FOREIGN KEY (country_id) REFERENCES countries(id) ON DELETE RESTRICT ON UPDATE RESTRICT,
        CONSTRAINT pk_cities_id PRIMARY KEY(id)
    );

**라이브러리 로드**

예제 프로그램으로 *tutorial.rb* 라는 파일을 생성하고 다음과 같은 기본 설정을 작성한다.

.. code-block:: ruby

    require 'rubygems'
    require 'active_record'
    require 'pp'

**데이터베이스 연결**

다음과 같이 파라미터를 정의하여 데이터베이스 연결을 생성한다.

.. code-block:: ruby

    ActiveRecord::Base.establish_connection(
     :adapter => "cubrid",
     :host => "localhost",
     :database => "demodb" ,
     :user => "dba"
    )

**데이터베이스에 객체 삽입**

테이블을 조작하기 전에 테이블을 ActiveRecord의 클래스와 매핑해야 한다.

.. code-block:: ruby

    class Country < ActiveRecord::Base
    end
     
    class City < ActiveRecord::Base
    end
     
    Country.create(:code => 'ROU', :name => 'Romania')
    Country.create(:code => 'HUN', :name => 'Hungary')
    Country.create(:code => 'DEU', :name => 'Germany')
    Country.create(:code => 'FRA', :name => 'France')
    Country.create(:code => 'ITA', :name => 'Italy', :record_date => Time.now)
    Country.create(:code => 'SPN', :name => 'Spain')

**데이터베이스에서 레코드 조회**

다음과 같이 데이터베이스에서 레코드를 조회한다.

.. code-block:: ruby

    romania = Country.find(1)
    pp(romania)
     
    romania = Country.where(:code => 'ROU')
    pp(romania)
     
    Country.find_each do |country|
     pp(country)
    end

**데이터베이스 레코드 갱신**

여기에서는 다음과 같이 *Spain* 의 *code* 를 *'SPN'* 에서 *'ESP'* 로 변경한다.

.. code-block:: ruby

    Country.transaction do
     spain = Country.where(:code => 'SPN')[0]
     spain.code = 'ESP'
     spain.save
    end

**데이터베이스 레코드 삭제**

데이터베이스의 레코드를 삭제하는 코드는 다음과 같다.

.. code-block:: ruby

    Country.transaction do
     spain = Country.where(:code => 'ESP')[0]
     spain.destroy
    end

**연관(association)을 이용한 작업**

국가에 도시를 추가하는 방법 중 하나는 *Country* 를 조회하여 *Country* 의 *code* 를 새로운 *City* 객체에 할당하는 것이다.

.. code-block:: ruby

    romania = Country.where(:code => 'ROU')[0]
    City.create(:country_id => romania.id, :name => 'Bucharest');

더 좋은 방법은 다음과 같이 ActiveRecord에 관계를 알리고 이를 Country 클래스에 선언하는 것이다.

.. code-block:: ruby

    class Country < ActiveRecord::Base
     has_many :cities, :dependent => :destroy
    end
     
    class City < ActiveRecord::Base
    end

위 코드에 따라 한 국가는 여러 개의 도시를 가질 수 있다. 이제 다음과 같이 간단하게 국가에 새 도시를 추가할 수 있다. 이 방법을 사용하면 도시에 접근할 때 참조되는 국가의 모든 도시들을 얻을 수 있으므로 유용하게 사용할 수 있다.

.. code-block:: ruby

    italy = Country.where(:code => 'ITA')[0]
    italy.cities.create(:name => 'Milano');
    italy.cities.create(:name => 'Napoli');
     
    pp (romania.cities)
    pp (italy.cities)

또한 다음과 같은 코드로 국가를 삭제하면 그 국가의 모든 도시가 삭제된다.

.. code-block:: ruby

    romania.destroy

**ActiveRecord** 는 일대일이나 다대다(many-to-many)와 같은 관계도 지원한다.

**메타데이터 관리**

ActiveRecord를 사용하면 코드를 수정하지 않아도 다른 데이터베이스를 사용할 수 있다.

**데이터베이스 구조 정의**

**ActiveRecord::Schema.define** 을 사용하여 새 테이블을 정의할 수 있다. 예를 들면 다음과 같이 일대다(one-to-many)로 대응되는 책에 대한 테이블(*books*)과 저자에 대한 테이블(*authors*)을 생성할 수 있다.

.. code-block:: ruby

    ActiveRecord::Schema.define do
     create_table :books do |table|
      table.column :title, :string, :null => false
      table.column :price, :float, :null => false
      table.column :author_id, :integer, :null => false
     end
      
     create_table :authors do |table|
      table.column :name, :string, :null => false
      table.column :address, :string
      table.column :phone, :string
     end
     
     add_index :books, :author_id
    end

CUBRID에서 지원하는 칼럼 타입은 **:string**, **:text**, **:integer**, **:float**, **:decimal**, **:datetime**, **:timestamp**, **:time**, **:boolean**, **:bit**, **:smallint**, **:bigint**, **:char** 이다. 현재 **:binary** 는 지원하지 않는다.

**테이블 칼럼 관리**

**ActiveRecord::Migration** 의 기능을 사용하여 테이블의 칼럼을 추가하거나 업데이트, 삭제할 수 있다.

.. code-block:: ruby

    ActiveRecord::Schema.define do
     create_table :todos do |table|
      table.column :title, :string
      table.column :description, :string
     end
      
     change_column :todos, :description, :string, :null => false
     add_column :todos, :created, :datetime, :default => Time.now
     rename_column :todos, :created, :record_date
     remove_column :todos, :record_date
      
    end

**데이터베이스 스키마 덤프**

**ActiveRecord::SchemaDumper.dump** 를 사용하여 현재 사용 중인 스키마의 정보를 덤프할 수 있다. 덤프된 스키마 정보는 플랫폼과 상관없이 사용할 수 있는 형식으로 저장되며 Ruby ActiveRecord에서도 사용할 수 있다. 

단, **:bigint**, **:bit** 등과 같이 특정 데이터베이스에서 사용되는 커스텀 칼럼 타입을 사용한다면 제대로 동작하지 않을 수 있다.

**서버 용량 정보 획득**

현재 연결에서 다음과 같이 데이터베이스 정보를 획득할 수 있다.

.. code-block:: ruby

    puts "Maximum column length        : " + ActiveRecord::Base.connection.column_name_length.to_s
    puts "SQL statement maximum length : " + ActiveRecord::Base.connection.sql_query_length.to_s
    puts "Quoting : '''test'''         : " + ActiveRecord::Base.connection.quote("'''test'''")
    
**데이터베이스 생성**

CUBRID에서는 데이터베이스 생성을 **cubrid create** 유틸리티 명령어로만 처리하기 때문에, 프로그램 내에서는 데이터베이스를 생성할 수 없다.

.. code-block:: ruby

    ActiveRecord::Schema.define do
     create_database('not_supported')
    end

Ruby API
========

http://ftp.cubrid.org/CUBRID_Docs/Drivers/Ruby/\ 를 참고한다.
