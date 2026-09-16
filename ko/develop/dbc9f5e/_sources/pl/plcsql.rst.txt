
:meta-keywords: cubrid sql, pl/csql
:meta-description: This chapter describes PL/CSQL Spec.

*****************************
PL/CSQL
*****************************

CUBRID는 PL/CSQL(Procedural Language extension of CUBRID SQL)로 저장 함수와 프로시저를 개발할 수 있도록 지원한다.
PL/CSQL은 CUBRID SQL의 절차적 언어로의 확장으로서, 선언적 언어인 SQL만으로 구현하기 어려운
조건문, 반복문, 변수, 에러 처리, 내부 프로시저/함수를 통한 모듈화 같은 절차적 언어의 특성을 지원한다.
물론, SQL 문을 PL/CSQL 프로그램 안에 포함시켜 함께 사용할 수 있다.

.. toctree::
        :maxdepth: 2

        plcsql_overview
        plcsql_decl
        plcsql_stmt
        plcsql_expr
