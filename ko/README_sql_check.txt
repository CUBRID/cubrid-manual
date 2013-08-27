In 9.2.0,
0. install 9.2.0
1. CUBRID_CHARSET=en_US.utf8
2. LC_ALL=en_US.utf8
3. cubrid deletedb demodb
4. $CUBRID/demo/make_cubrid_demo.sh

In 8.4.4,

0. install 8.4.4 in a different directory and run . .cubrid.sh
1. cubrid deletedb demodb
2. $CUBRID/demo/make_cubrid_demo.sh

# run below 0~3 on the 8.4.4 and 9.2.0
# to change source, please change make_diff.py; RB-8.4.4

sh 0.clear_result.sh

sh 1.make_sql.sh

sh 2.make_csql.sh

sh 3.run_csql.sh 

# run below on the 9.2.0 (right side version of diff)

sh 4.make_diff.sh

sh 5.run_diff.sh

