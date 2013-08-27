my_pwd=${PWD}
cd $CUBRID/databases/demodb
cubrid deletedb demodb
sh $CUBRID/demo/make_cubrid_demo.sh
cd $my_pwd
rm *.sql
rm *.sql.out
rm *.sql.err

rm admin/*.sql
rm admin/*.sql.out
rm admin/*.sql.err

rm api/*.sql
rm api/*.sql.out
rm api/*.sql.err

rm sql/*.sql
rm sql/*.sql.out
rm sql/*.sql.err

rm sql/function/*.sql
rm sql/function/*.sql.out
rm sql/function/*.sql.err

rm sql/query/*.sql
rm sql/query/*.sql.out
rm sql/query/*.sql.err

rm sql/schema/*.sql
rm sql/schema/*.sql.out
rm sql/schema/*.sql.err

