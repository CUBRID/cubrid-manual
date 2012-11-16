*************
Ruby 드라이버
*************

**Ruby Driver**

**Ruby Overview**

CUBRID Ruby driver implements the interface to enable access from applications in Ruby to CUBRID database server and official one is available as a RubyGem package.

CUBRID Ruby driver is written based on CCI API so affected by CCI configurations such as
**CCI_DEFAULT_AUTOCOMMIT**
.

To download Ruby driver or get the latest information, click
`http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-driver>`_
.

**Installing and Configuring Ruby**

**Requirements**

*   Ruby 1.8.7 or later



*   CUBRID gem



*   ActiveRecord gem



**Linux**

You can install the CUBRID Connector through
**gem**
. Make sure that you add the
**-E**
option so that the environment path where CUBRID has been installed cannot be reset by the
**sudo**
command.

sudo -E gem install cubrid

**Windows**

Enter the command line below to install the latest version of CUBRID Ruby driver.

gem install cubrid

**Note**
If you do not have RubyInstaller, see
`http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-driver-installation-instructions <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-driver-installation-instructions>`_
.

**Ruby Sample Program**

This section will explain how to use Ruby ActiveRecord adapter to work with CUBRID database. Create tables by executing the following SQL script.

CREATE TABLE "countries"(

    "id" integer AUTO_INCREMENT,

    "code" character varying(3) NOT NULL UNIQUE,

    "name" character varying(40) NOT NULL UNIQUE,

    "record_date" datetime DEFAULT sysdatetime NOT NULL,

    CONSTRAINT pk_countries_id PRIMARY KEY("id")

);

CREATE TABLE "cities"(

    "id" integer AUTO_INCREMENT NOT NULL UNIQUE,

    "name" character varying(40) NOT NULL,

    "country_id" integer NOT NULL,

    "record_date" datetime DEFAULT sysdatetime NOT NULL,

    FOREIGN KEY ("country_id") REFERENCES "countries"("id") ON DELETE RESTRICT ON UPDATE RESTRICT,

    CONSTRAINT pk_cities_id PRIMARY KEY("id")

);

**Loading Library**

Create a new file named
*tutorial.rb*
and add basic configuration.

require 'rubygems'

require 'active_record'

require 'pp'

**Establishing Database Connection**

Define the connection parameters as follows:

ActiveRecord::Base.establish_connection(

 :adapter => "cubrid",

 :host => "localhost",

 :database => "demodb" ,

 :user => "dba"

)

**Inserting Objects into a Database**

Before starting to operate on tables, you must declare the two tables mappings in the database as ActiveRecord classes.

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

**Selecting Records from a Database**

Select records from a database as follows:

romania = Country.find(1)

pp(romania)

 

romania = Country.where(:code => 'ROU')

pp(romania)

 

Country.find_each do |country|

 pp(country)

end

**Updating Database Records**

Change the
*Spain*
code from
*'SPN'*
to
*'ESP'*
.

Country.transaction do

 spain = Country.where(:code => 'SPN')[0]

 spain.code = 'ESP'

 spain.save

end

**Deleting Database Records**

Delete records from a database as follows:

Country.transaction do

 spain = Country.where(:code => 'ESP')[0]

 spain.destroy

end

**Working with Associations**

One method to add cities to a country would be to select the
*Country*
and assign the country code to a new 
*City*
object.

romania = Country.where(:code => 'ROU')[0]

City.create(:country_id => romania.id, :name => 'Bucharest');

A more elegant solution would be to let ActiveRecord know about this relationship and declare it in the
*Country*
class.

class Country < ActiveRecord::Base

 has_many :cities, :dependent => :destroy

end

 

class City < ActiveRecord::Base

end

In the code above, it is declared that one country can have many cities. Now it will be very easy to add new city to a country.

italy = Country.where(:code => 'ITA')[0]

italy.cities.create(:name => 'Milano');

italy.cities.create(:name => 'Napoli');

 

pp (romania.cities)

pp (italy.cities)

This would be very helpful because when we access cities we get all the cities recorded for the referenced country. Another use is that when you delete the country, all its cities are removed. All is done in one statement.

romania.destroy

ActiveRecord also supports other relationship including one-to-one, many-to-many, etc.

**Working with Metadata**

ActiveRecord enables the code to work with on different database backends without modifying the code.

**Defining a database structure**

A new table can be defined using
**ActiveRecord::Schema.define**
. Let's create two tables: books and authors with a one-to-many relationship between
*authors*
and
*books*
(one-to-many).

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

CUBRID-supported column types are
**:string**
,
**:text**
,
**:integer**
,
**:float**
,
**:decimal**
,
**:datetime**
,
**:timestamp**
,
**:time**
,
**:boolean**
,
**:bit**
,
**:monetary**
,
**:smallint**
,
**:bigint**
, and
**:char**
. Currently,
**:binary**
is not supported.

**Managing table columns**

You can add, update, delete columns by using features from 
**ActiveRecord::Migration**
.

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

**Dumping database schema**

You can use
**ActiveRecord::SchemaDumper.dump**
to dump information for currently used schema. This is done into a platform independent format that is understood by Ruby ActiveRecord.

Note that if you are using custom column types database specific (
**:bigint**
,
**:bit**
,
**:monetary**
), this may work.

**Obtaing Server Capabilities**

You can get database information extracted from the current connections as in the example below:

puts "Maximum column length        : " + ActiveRecord::Base.connection.column_name_length.to_s

puts "SQL statement maximum length : " + ActiveRecord::Base.connection.sql_query_length.to_s

puts "Quoting : '''test'''         : " + ActiveRecord::Base.connection.quote("'''test'''")

**Creating a schema**

Due to the way CUBRID is functioning, you cannot programmatically create a schema as in the following example:

ActiveRecord::Schema.define do

 create_database('not_supported')

end

**Note**
To get the latest information about Ruby driver, click
`http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-driver>`_
.

**Ruby API**

For more information about CUBRID Ruby API, see CUBRID Ruby API Documentation (
`http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#Ruby_API <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#ruby_api>`_
).

*   `Connection Class <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#connection_class>`_

    *   `Data Constants <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#data_constants>`_



    *   `auto_commit= <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#auto_commit.3d>`_



    *   `auto_commit? <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#auto_commit.3f>`_



    *   `connect <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#connect>`_



    *   `close <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#close>`_



    *   `commit <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#commit>`_



    *   `rollback <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#rollback>`_



    *   `glo_new <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#glo_new>`_



    *   `query <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#query>`_



    *   `prepare <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#prepare>`_



    *   `to_s <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#to_s>`_



    *   `server_version <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#server_version>`_





*   `Statement Class <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#statement_class>`_

    *   `Data Types <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#data_types>`_



    *   `affected_rows <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#affected_rows>`_



    *   `bind <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#bind>`_



    *   `close <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#close_2>`_



    *   `column_info <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#column_info>`_



    *   `each <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#each>`_



    *   `each_hash <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#each_hash>`_



    *   `execute <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#execute>`_



    *   `fetch <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#fetch>`_



    *   `fetch_hash <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#fetch_hash>`_



    *   `get_oid <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#get_oid>`_





*   `Oid Class <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#oid_class>`_

    *   `[](col_name) <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#.5b.5d.28col_name.29>`_



    *   `[]=(col_name, obj) <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#.5b.5d.3d.28col_name.2c_obj.29>`_



    *   `drop <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#drop>`_



    *   `each <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#each_2>`_



    *   `lock <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#lock>`_



    *   `refresh <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#refresh>`_



    *   `save <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#save>`_



    *   `table <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#table>`_



    *   `to_hash <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#to_hash>`_



    *   `to_s <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-api-documentation#to_s_2>`_





**Note**
To get the latest information about Ruby driver, click
`http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-ruby-driver>`_
.
