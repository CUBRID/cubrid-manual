1. CUBRID_CHARSET=en_US.utf8
2. LC_ALL=en_US.utf8
3. cubrid deletedb demodb
4. $CUBRID/demo/make_cubrid_demo.sh

./make_csql_sh.sh  => SQL is extracted and shell script for CSQL is prepared.
The result is stored into csql.sh
./run_csql.sh  => run csql.sh which is created by make_csql_sh.sh. The result is
stored into out.txt
./show_diff.sh  => show the difference between answer.txt out.txt

then, compare between out.txt and answer.txt.
