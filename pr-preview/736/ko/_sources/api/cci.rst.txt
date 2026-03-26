
:meta-keywords: CCI driver, database c driver, cubrid cci, cubrid c driver
:meta-description: CUBRID CCI (C Client Interface) driver implements an interface to enable access from C-based application to CUBRID database server through broker. It is also used as back-end infrastructure for creating tools (PHP, ODBC, etc.) which use the CAS application servers.

.. _cci-overview:

CCI 개요
========

CCI(C Client Interface)는 CUBRID 브로커와 응용 클라이언트 사이에 위치하여, C 기반의 응용 클라이언트가 브로커를 통해 CUBRID 데이터베이스 서버로 접근할 수 있는 인터페이스로서, 브로커 응용 서버(CAS)를 이용하는 도구(예: PHP, ODBC)를 만들기 위한 하부 구조로도 사용된다. 여기서, CUBRID 브로커는 응용 클라이언트로부터 받은 질의를 데이터베이스에 전달하고, 실행 결과를 응용 클라이언트로 전송하는 역할을 수행한다.

CCI를 사용하기 위해서는 헤더 파일과 라이브러리 파일이 필요하다.

+-----------------+-------------------+-------------------+
|                 | **Windows**       | **Unix/Linux**    |
+=================+===================+===================+
| C 헤더 파일     | include/cas_cci.h | include/cas_cci.h |
+-----------------+-------------------+-------------------+
| 정적 라이브러리 | lib/cascci.lib    | lib/libcascci.a   |
+-----------------+-------------------+-------------------+
| 동적 라이브러리 | bin/cascci.dll    | lib/libcascci.so  |
+-----------------+-------------------+-------------------+

CCI 드라이버는 CUBRID에서 제공되는 C 언어 인터페이스로, CUBRID 설치 패키지에 포함되어 있다. CCI는 브로커를 통해서 접속하므로 다른 인터페이스인 JDBC, PHP, ODBC, Python, Ruby 등과 동일하게 관리될 수 있다. 실제로 PHP, ODBC, Python, Ruby 인터페이스는 CCI를 기반으로 개발되었다. 단, JDBC는 CCI를 기반으로 개발되지 않았다.

.. image:: /images/image54.jpg

.. FIXME: 별도로 CCI 드라이버를 다운로드하거나 CCI 드라이버에 대한 최신 정보를 확인하려면 `http://www.cubrid.org/wiki_apis/entry/cubrid-cci-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-cci-driver>`_ 에 접속한다.

CCI 프로그래밍
==============

CCI 응용 프로그램 작성
----------------------

CCI를 이용하는 응용 프로그램은 기본적으로 CAS와 연결하기, 질의 준비, 질의 수행, 응답 처리, 연결 끊기의 과정을 통해 CUBRID를 이용한다. 각 과정에서 CCI는 연결 핸들(connection handle), 질의 핸들(query handle), 응답 핸들(response handle)을 통해 응용 프로그램과 소통한다.

브로커 파라미터인 :ref:`CCI_DEFAULT_AUTOCOMMIT <cci_default_autocommit>`\ 으로 응용 프로그램 시작 시 자동 커밋 모드의 기본값을 설정할 수 있으며, 브로커 파라미터 설정을 생략하면 기본값은 **ON** 이다. 응용 프로그램 내에서 자동 커밋 모드를 변경하려면 :c:func:`cci_set_autocommit` 함수를 이용하며, 자동 커밋 모드가 **OFF** 이면 :c:func:`cci_end_tran` 함수를 이용하여 명시적으로 트랜잭션을 커밋하거나 롤백해야 한다.

기본적인 작성 순서는 다음과 같으며, prepared statement 사용을 위해서는 변수에 데이터를 바인딩하는 작업이 추가된다. 이를 예제 1 및 예제 2에 구현하였다.

*   데이터베이스 연결 핸들 열기(관련 함수: :c:func:`cci_connect`, :c:func:`cci_connect_with_url`)

*   prepared statement를 위한 요청 핸들 얻기 (관련 함수: :c:func:`cci_prepare`)

*   prepared statement에 데이터 바인딩하기(관련 함수: :c:func:`cci_bind_param`)

*   prepared statement 실행하기(관련 함수: :c:func:`cci_execute`)

*   실행 결과 처리하기(관련 함수: :c:func:`cci_cursor`, :c:func:`cci_fetch`, :c:func:`cci_get_data`, :c:func:`cci_get_result_info`)

*   요청 핸들 닫기(관련 함수: :c:func:`cci_close_req_handle`)

*   데이터베이스 연결 핸들 닫기(관련 함수: :c:func:`cci_disconnect`)

*   데이터베이스 연결 풀 사용하기(관련 함수: :c:func:`cci_property_create`, :c:func:`cci_property_destroy`, :c:func:`cci_property_set`, :c:func:`cci_datasource_create`, :c:func:`cci_datasource_destroy`, :c:func:`cci_datasource_borrow`, :c:func:`cci_datasource_release`, :c:func:`cci_datasource_change_property`)

.. note::

    *   Windows에서 CCI 응용 프로그램을 컴파일하려면 "WINDOWS"가 define되어야 하므로 "-DWINDOWS" 옵션을 컴파일러에 반드시 포함하도록 한다.
    *   스레드 기반 프로그램에서 데이터베이스 연결은 각 스레드마다 독립적으로 사용해야 한다.
    *   자동 커밋 모드에서 SELECT 문 수행 이후 모든 결과 셋이 fetch되지 않으면 커밋이 되지 않는다. 따라서, 자동 커밋 모드라 하더라도 프로그램 내에서 결과 셋에 대한 fetch 도중 어떠한 오류가 발생한다면 반드시 :c:func:`cci_end_tran` 을 호출하여 트랜잭션을 종료 처리하도록 한다. 

**예제 1**

.. code-block:: c

    // Example to execute a simple query
    // In Linux: gcc -o simple simple.c -m64 -I${CUBRID}/include -lnsl ${CUBRID}/lib/libcascci.so -lpthread
    
    #include <stdio.h>
    #include "cas_cci.h"  
    #define BUFSIZE  (1024)
     
    int
    main (void)
    {
        int con = 0, req = 0, col_count = 0, i, ind;
        int error;
        char *data;
        T_CCI_ERROR cci_error;
        T_CCI_COL_INFO *col_info;
        T_CCI_CUBRID_STMT stmt_type;
        char *query = "select * from code";
        
        //getting a connection handle for a connection with a server
        con = cci_connect ("localhost", 33000, "demodb", "dba", "");
        if (con < 0)
        {
            printf ("cannot connect to database\n");
            return 1;
        }
     
        //preparing the SQL statement
        req = cci_prepare (con, query, 0, &cci_error);
        if (req < 0)
        {
            printf ("prepare error: %d, %s\n", cci_error.err_code,
                    cci_error.err_msg);
            goto handle_error;
        }
     
        //getting column information when the prepared statement is the SELECT query
        col_info = cci_get_result_info (req, &stmt_type, &col_count);
        if (col_info == NULL)
        {
            printf ("get_result_info error: %d, %s\n", cci_error.err_code,
                    cci_error.err_msg);
            goto handle_error;
        }
     
        //Executing the prepared SQL statement
        error = cci_execute (req, 0, 0, &cci_error);
        if (error < 0)
        {
            printf ("execute error: %d, %s\n", cci_error.err_code,
                    cci_error.err_msg);
            goto handle_error;
        }
        while (1)
        {
     
            //Moving the cursor to access a specific tuple of results
            error = cci_cursor (req, 1, CCI_CURSOR_CURRENT, &cci_error);
            if (error == CCI_ER_NO_MORE_DATA)
            {
                break;
            }
            if (error < 0)
            {
                printf ("cursor error: %d, %s\n", cci_error.err_code,
                        cci_error.err_msg);
                goto handle_error;
            }
     
            //Fetching the query result into a client buffer
            error = cci_fetch (req, &cci_error);
            if (error < 0)
            {
                printf ("fetch error: %d, %s\n", cci_error.err_code,
                        cci_error.err_msg);
                goto handle_error;
            }
            for (i = 1; i <= col_count; i++)
            {
     
                //Getting data from the fetched result
                error = cci_get_data (req, i, CCI_A_TYPE_STR, &data, &ind);
                if (error < 0)
                {
                    printf ("get_data error: %d, %d\n", error, i);
                    goto handle_error;
                }
                printf ("%s\t|", data);
            }
            printf ("\n");
        }
     
        //Closing the request handle
        error = cci_close_req_handle (req);
        if (error < 0)
        {
            printf ("close_req_handle error: %d, %s\n", cci_error.err_code,
                    cci_error.err_msg);
            goto handle_error;
        }
     
        //Disconnecting with the server
        error = cci_disconnect (con, &cci_error);
        if (error < 0)
        {
            printf ("error: %d, %s\n", cci_error.err_code, cci_error.err_msg);
            goto handle_error;
        }
     
        return 0;
     
    handle_error:
        if (req > 0)
            cci_close_req_handle (req);
        if (con > 0)
        cci_disconnect (con, &cci_error);
     
        return 1;
    }

**예제 2**

.. code-block:: c

    // Example to execute a query with a bind variable
    // In Linux: gcc -o cci_bind cci_bind.c -m64 -I${CUBRID}/include -lnsl ${CUBRID}/lib/libcascci.so -lpthread

    #include <stdio.h>
    #include <string.h>
    #include "cas_cci.h"
    #define BUFSIZE  (1024)

    int
    main (void)
    {
        int con = 0, req = 0, col_count = 0, i, ind;
        int error;
        char *data;
        T_CCI_ERROR cci_error;
        T_CCI_COL_INFO *col_info;
        T_CCI_CUBRID_STMT stmt_type;
        char *query = "select * from nation where name = ?";
        char namebuf[128];

        //getting a connection handle for a connection with a server
        con = cci_connect ("localhost", 33000, "demodb", "dba", "");
        if (con < 0)
        {
            printf ("cannot connect to database\n");
            return 1;
        }

        //preparing the SQL statement
        req = cci_prepare (con, query, 0, &cci_error);
        if (req < 0)
        {
            printf ("prepare error: %d, %s\n", cci_error.err_code,
                  cci_error.err_msg);
            goto handle_error;
        }

        //Binding date into a value
        strcpy (namebuf, "Korea");
        error =
        cci_bind_param (req, 1, CCI_A_TYPE_STR, namebuf, CCI_U_TYPE_STRING,
                        CCI_BIND_PTR);
        if (error < 0)
        {
            printf ("bind_param error: %d ", error);
            goto handle_error;
        }

        //getting column information when the prepared statement is the SELECT query
        col_info = cci_get_result_info (req, &stmt_type, &col_count);
        if (col_info == NULL)
        {
            printf ("get_result_info error: %d, %s\n", cci_error.err_code,
                  cci_error.err_msg);
            goto handle_error;
        }

        //Executing the prepared SQL statement
        error = cci_execute (req, 0, 0, &cci_error);
        if (error < 0)
        {
            printf ("execute error: %d, %s\n", cci_error.err_code,
                  cci_error.err_msg);
            goto handle_error;
        }

        //Executing the prepared SQL statement
        error = cci_execute (req, 0, 0, &cci_error);
        if (error < 0)
        {
            printf ("execute error: %d, %s\n", cci_error.err_code,
                  cci_error.err_msg);
            goto handle_error;
        }

        while (1)
        {
        
            //Moving the cursor to access a specific tuple of results
            error = cci_cursor (req, 1, CCI_CURSOR_CURRENT, &cci_error);
            if (error == CCI_ER_NO_MORE_DATA)
            {
                break;
            }
            if (error < 0)
            {
                printf ("cursor error: %d, %s\n", cci_error.err_code,
                      cci_error.err_msg);
                goto handle_error;
            }

            //Fetching the query result into a client buffer
            error = cci_fetch (req, &cci_error);
            if (error < 0)
            {
                printf ("fetch error: %d, %s\n", cci_error.err_code,
                      cci_error.err_msg);
                goto handle_error;
            }
            for (i = 1; i <= col_count; i++)
            {

                //Getting data from the fetched result
                error = cci_get_data (req, i, CCI_A_TYPE_STR, &data, &ind);
                if (error < 0)
                {
                    printf ("get_data error: %d, %d\n", error, i);
                    goto handle_error;
                }
                if (ind == -1)
                {
                    printf ("NULL\t");
                }
                else
                {
                    printf ("%s\t|", data);
                }
            }
                printf ("\n");
        }

        //Closing the request handle
        error = cci_close_req_handle (req);
        if (error < 0)
        {
            printf ("close_req_handle error: %d, %s\n", cci_error.err_code,
                    cci_error.err_msg);
            goto handle_error;
        }

        //Disconnecting with the server
        error = cci_disconnect (con, &cci_error);
        if (error < 0)
        {
            printf ("error: %d, %s\n", cci_error.err_code, cci_error.err_msg);
            goto handle_error;
        }

        return 0;
      
    handle_error:
        if (req > 0)
            cci_close_req_handle (req);
        if (con > 0)
            cci_disconnect (con, &cci_error);
        return 1;
    }

**예제 3**

.. code-block:: c

    // Example to use connection/statement pool in CCI
    // In Linux: gcc -o cci_pool cci_pool.c -m64 -I${CUBRID}/include -lnsl ${CUBRID}/lib/libcascci.so -lpthread

    #include <stdio.h>
    #include "cas_cci.h"
     
    int main ()
    {
        T_CCI_PROPERTIES *ps = NULL;
        T_CCI_DATASOURCE *ds = NULL;
        T_CCI_ERROR err;
        T_CCI_CONN cons;
        int rc = 1, i;
        
        ps = cci_property_create ();
        if (ps == NULL)
        {
            fprintf (stderr, "Could not create T_CCI_PROPERTIES.\n");
            rc = 0;
            goto cci_pool_end;
        }
        
        cci_property_set (ps, "user", "dba");
        cci_property_set (ps, "url", "cci:cubrid:localhost:33000:demodb:::");
        cci_property_set (ps, "pool_size", "10");
        cci_property_set (ps, "max_wait", "1200");
        cci_property_set (ps, "pool_prepared_statement", "true");
        cci_property_set (ps, "login_timeout", "300000");
        cci_property_set (ps, "query_timeout", "3000");
        
        ds = cci_datasource_create (ps, &err);
        if (ds == NULL)
        {
            fprintf (stderr, "Could not create T_CCI_DATASOURCE.\n");
            fprintf (stderr, "E[%d,%s]\n", err.err_code, err.err_msg);
            rc = 0;
            goto cci_pool_end;
        }
        
        for (i = 0; i < 3; i++)
        {
            cons = cci_datasource_borrow (ds, &err);
            if (cons < 0)
            {
                fprintf (stderr,
                        "Could not borrow a connection from the data source.\n");
                fprintf (stderr, "E[%d,%s]\n", err.err_code, err.err_msg);
                continue;
            }
            // put working code here.
            cci_work (cons);
            cci_datasource_release (ds, cons, &err);

        }
        
    cci_pool_end:
      cci_property_destroy (ps);
      cci_datasource_destroy (ds);
     
      return 0;
    }
     
    // working code
    int cci_work (T_CCI_CONN con)
    {
        T_CCI_ERROR err;
        char sql[4096];
        int req, res, error, ind;
        int data;
        
        cci_set_autocommit (con, CCI_AUTOCOMMIT_TRUE);
        cci_set_lock_timeout (con, 100, &err);
        cci_set_isolation_level (con, TRAN_REP_CLASS_COMMIT_INSTANCE, &err);
        
        error = 0;
        snprintf (sql, 4096, "SELECT host_year FROM record WHERE athlete_code=11744");
        req = cci_prepare (con, sql, 0, &err);
        if (req < 0)
        {
            printf ("prepare error: %d, %s\n", err.err_code, err.err_msg);
            return error;
        }
        
        res = cci_execute (req, 0, 0, &err);
        if (res < 0)
        {
            printf ("execute error: %d, %s\n", err.err_code, err.err_msg);
            goto cci_work_end;
        }
        
        while (1)
        {
        error = cci_cursor (req, 1, CCI_CURSOR_CURRENT, &err);
        if (error == CCI_ER_NO_MORE_DATA)
        {
            break;
        }
        if (error < 0)
        {
            printf ("cursor error: %d, %s\n", err.err_code, err.err_msg);
            goto cci_work_end;
        }
        
        error = cci_fetch (req, &err);
        if (error < 0)
        {
            printf ("fetch error: %d, %s\n", err.err_code, err.err_msg);
            goto cci_work_end;
        }
        
        error = cci_get_data (req, 1, CCI_A_TYPE_INT, &data, &ind);
        if (error < 0)
        {
            printf ("get data error: %d\n", error);
            goto cci_work_end;
        }
        printf ("%d\n", data);
        }
        
        error = 1;
    cci_work_end:
        cci_close_req_handle (req);
        return error;
    }


라이브러리 적용
---------------

CCI를 이용한 응용 프로그램을 작성했다면 프로그램 특성에 따라 정적 링크 형태로 프로그램을 수행시킬 것인지, 아니면 동적으로 CCI를 호출하여 사용할 것인지를 결정하여 프로그램을 빌드한다. :ref:`cci-overview` 의 표를 참조하여 사용할 라이브러리를 결정한다.

다음은 유닉스/Linux에서 동적인 라이브러리를 사용하여 링크하는 Makefile의 예제이다. ::

    CC=gcc
    CFLAGS = -g -Wall -I. -I$CUBRID/include
    LDFLAGS = -L$CUBRID/lib -lcascci -lnsl
    TEST_OBJS = test.o
    EXES = test
    all: $(EXES)
    test: $(TEST_OBJS)
        $(CC) -o $@ $(TEST_OBJS) $(LDFLAGS)

다음은 Windows에서 정적 라이브러리를 적용하기 위한 설정이다.

.. image:: /images/image55.png

BLOB/CLOB 사용
--------------
**LOB 데이터 저장**

CCI 응용 프로그램에서 다음 함수를 사용하여 **LOB** 데이터 파일을 생성하고 데이터를 바인딩할 수 있다.

*   **LOB** 데이터 파일 생성하기 (관련 함수: :c:func:`cci_blob_new`, :c:func:`cci_blob_write`)
*   **LOB** 데이터를 바인딩하기 (관련 함수: :c:func:`cci_bind_param`)
*   **LOB** 구조체에 대한 메모리 해제하기 (관련 함수: :c:func:`cci_blob_free`)

**예제**

.. code-block:: c

    int con = 0; /* connection handle */
    int req = 0; /* request handle */
    int res;
    int n_executed;
    int i;
    T_CCI_ERROR error;
    T_CCI_BLOB blob = NULL;
    char data[1024] = "bulabula";
     
    con = cci_connect ("localhost", 33000, "tdb", "PUBLIC", "");
    if (con < 0) {
        goto handle_error;
    }
    req = cci_prepare (con, "insert into doc (doc_id, content) values (?,?)", 0, &error);
    if (req< 0)
    {
        goto handle_error;
    }
     
    res = cci_bind_param (req, 1 /* binding index*/, CCI_A_TYPE_STR, "doc-10", CCI_U_TYPE_STRING, CCI_BIND_PTR);
     
    /* Creating an empty LOB data file */
    res = cci_blob_new (con, &blob, &error);
    res = cci_blob_write (con, blob, 0 /* start position */, 1024 /* length */, data, &error);
     
    /* Binding BLOB data */
    res = cci_bind_param (req, 2 /* binding index*/, CCI_A_TYPE_BLOB, (void *)blob, CCI_U_TYPE_BLOB, CCI_BIND_PTR);
     
    n_executed = cci_execute (req, 0, 0, &error);
    if (n_executed < 0)
    {
        goto handle_error;
    }
     
    /* Commit */
    if (cci_end_tran(con, CCI_TRAN_COMMIT, &error) < 0)
    {
        goto handle_error;
    }
     
    /* Memory free */
    cci_blob_free(blob);
    return 0;
     
    handle_error:
    if (blob != NULL)
    {
        cci_blob_free(blob);
    }
    if (req > 0)
    {
        cci_close_req_handle (req);
    }
    if (con > 0)
    {
        cci_disconnect(con, &error);
    }
    return -1;

**LOB 데이터 조회**

CCI 응용 프로그램에서 다음 함수를 사용하여 **LOB** 데이터를 조회할 수 있다. **LOB** 타입 칼럼에 데이터를 입력하면 실제 **LOB** 데이터는 외부 저장소 내 파일에 저장되고 **LOB** 타입 칼럼에는 해당 파일을 참조하는 Locator 값이 저장되므로, 파일에 저장된 **LOB** 데이터를 조회하기 위해서는 :c:func:`cci_get_data` 가 아닌 :c:func:`cci_blob_read` 함수를 호출해야 한다.

*   **LOB** 타입 칼럼 메타 데이터(Locator) 인출하기 (관련 함수: :c:func:`cci_get_data`)
*   **LOB** 데이터를 인출하기 (관련 함수: :c:func:`cci_blob_read`)
*   **LOB** 구조체에 대한 메모리 해제하기 (관련 함수: :c:func:`cci_blob_free`)

**예제**

.. code-block:: c

    int con = 0; /* connection handle */
    int req = 0; /* request handle */
    int ind; /* NULL indicator, 0 if not NULL, -1 if NULL*/
    int res;
    int i;
    T_CCI_ERROR error;
    T_CCI_BLOB blob;
    char buffer[1024];
     
    con = cci_connect ("localhost", 33000, "image_db", "PUBLIC", "");
    if (con < 0)
    {
        goto handle_error;
    }
    req = cci_prepare (con, "select content from doc_t", 0 /*flag*/, &error);
    if (req< 0)
    {
        goto handle_error;
    }
     
    res = cci_execute (req, 0/*flag*/, 0/*max_col_size*/, &error);
     
    while (1) {
        res = cci_cursor (req, 1/* offset */, CCI_CURSOR_CURRENT/* cursor position */, &error);
        if (res == CCI_ER_NO_MORE_DATA)
        {
            break;
        }
        res = cci_fetch (req, &error);
     
        /* Fetching CLOB Locator */
        res = cci_get_data (req, 1 /* colume index */, CCI_A_TYPE_BLOB,
        (void *)&blob /* BLOB handle */, &ind /* NULL indicator */);
        /* Fetching CLOB data */
        res = cci_blob_read (con, blob, 0 /* start position */, 1024 /* length */, buffer, &error);
        printf ("content = %s\n", buffer);
    }
     
    /* Memory free */
    cci_blob_free(blob);
    res=cci_close_req_handle(req);
    res = cci_disconnect (con, &error);
    return 0;
     
    handle_error:
    if (req > 0)
    {
        cci_close_req_handle (req);
    }
    if (con > 0)
    {
        cci_disconnect(con, &error);
    }
    return -1;

.. _cci-error-codes:

CCI 에러 코드와 에러 메시지
---------------------------

CCI API 함수는 에러 발생 시 반환 값이 음수인 CCI 에러 코드 혹은 CAS(브로커 응용 서버) 에러 코드를 반환한다. CCI 에러 코드는 CCI API 함수에서 발생하며, CAS 에러
코드는 CAS에서 발생한다.

*   모든 에러 코드의 값은 0보다 작은 음수이다.
*   T_CCI_ERROR err_buf를 인자로 가지는 모든 함수의 에러 코드와 에러 메시지는 err_buf.err_code와 err_buf.err_msg에서 확인할 수 있다.
*   T_CCI_ERROR err_buf 인자가 없는 함수의 에러 메시지는 :c:func:`cci_get_err_msg` 함수를 이용하여 에러 코드가 나타내는 에러 메시지를 출력할 수 있다.
*   에러 번호가 -20002부터 -20999 사이이면, CCI API 함수에서 발생하는 에러이다.
*   에러 번호가 -10000부터 -10999 사이이면, CAS에서 발생하는 에러를 CCI API 함수가 전달받아 반환하는 에러이다. CAS 에러는 :ref:`cas-error`\ 를 참고한다.
*   함수가 리턴하는 에러 코드의 값이 **CCI_ER_DBMS** (-20001)인 경우, 데이터베이스 서버에서 발생하는 에러이다. 데이터베이스 서버 에러와 관련한 내용은 :ref:`database-server-error`\를 참고한다.

.. warning::

    서버에서 에러가 발생한 경우 함수가 리턴하는 에러 코드인 **CCI_ER_DBMS** 와 err_buf.err_code 값이 서로 다름에 주의한다. 서버 에러 외에 err_buf에 저장되는 모든 에러 코드는 함수가 리턴하는 에러 코드와 동일하다.

.. note::

    CUBRID 9.0 미만 버전에서의 CCI, CAS 에러 코드는 CUBRID 9.0 이상 버전의 에러 코드와 다른 값을 가진다. 따라서 에러 코드명을 사용하여 개발한 사용자는 응용 프로그램을 재컴파일하여 사용해야 하며, 에러 코드 번호를 직접 부여하여 개발한 사용자는 번호 값을 바꾼 후 응용 프로그램을 재컴파일해야 한다.

데이터베이스 에러 버퍼(err_buf)는 **cas_cci.h** 헤더 파일의 **T_CCI_ERROR**  구조체 변수이다. 사용법은 아래의 예제 프로그램을 참고한다.

**CCI_ER** 로 시작되는 CCI 에러 코드는 **$CUBRID/include/cas_cci.h** 파일에 **T_CCI_ERROR_CODE** 라는 enum 구조체 내에 정의되어 있다. 따라서 프로그램 코드에서 이 에러 코드 명을 사용하려면 코드 상단에 **#include "cas_cci.h"** 를 입력하여 헤더 파일을 포함해야 한다.

아래의 프로그램은 에러 메시지를 출력한다. 이때 :c:func:`cci_prepare` 가 리턴하는 에러 코드 값 req의 값은 **CCI_ER_DMBS** 이고, 데이터베이스 에러 버퍼의 **cci_error.err_code** 에는 서버 에러 코드인 -493이, **cci_error.err_msg** 에는 'Syntax: Unknown class "notable". select * from notable'이라는 에러 메시지가 저장된다.

.. code-block:: c

    // gcc -o err err.c -m64 -I${CUBRID}/include -lnsl ${CUBRID}/lib/libcascci.so -lpthread
    #include <stdio.h>
    #include "cas_cci.h"
     
    #define BUFSIZE  (1024)
     
    int
    main (void)
    {
        int con = 0, req = 0, col_count = 0, i, ind;
        int error;
        char *data;
        T_CCI_ERROR err_buf;
        char *query = "select * from notable";
     
        //getting a connection handle for a connection with a server
        con = cci_connect ("localhost", 33000, "demodb", "dba", "");
        if (con < 0)
        {
            printf ("cannot connect to database\n");
            return 1;
        }
     
        //preparing the SQL statement
        req = cci_prepare (con, query, 0, &err_buf);
        if (req < 0)
        {
            if (req == CCI_ER_DBMS)
            {
                printf ("error from server: %d, %s\n", err_buf.err_code, err_buf.err_msg);
            }
            else
            {
                printf ("error from cci or cas: %d, %s\n", err_buf.err_code, err_buf.err_msg);
            }
            goto handle_error;
        }
        // ...
    }

다음은 CCI 함수의 에러 코드를 나타낸다. CAS 에러는 :ref:`cas-error`\ 를 참고한다.

+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| 에러 코드명(에러 번호)                   | 에러 메시지                                                   | 비고                                                                                             |
+==========================================+===============================================================+==================================================================================================+
| CCI_ER_DBMS(-20001)                      | CUBRID DBMS Error                                             | 서버에서 에러가 발생한 경우 함수가 반환하는 에러 코드. 실패 원인은 T_CCI_ERROR 구조체에 저장되는 |
|                                          |                                                               | err_code와 err_msg로 확인 가능.                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_CON_HANDLE(-20002)                | Invalid connection handle                                     |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_NO_MORE_MEMORY(-20003)            | Memory allocation error                                       | 사용 가능한 메모리가 부족함.                                                                     |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_COMMUNICATION(-20004)             | Cannot communicate with server                                |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_NO_MORE_DATA(-20005)              | Invalid cursor position                                       |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_TRAN_TYPE(-20006)                 | Unknown transaction type                                      |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_STRING_PARAM(-20007)              | Invalid string argument                                       | :c:func:`cci_prepare`, :c:func:`cci_prepare_and_execute` 에서 sql_stmt가 NULL이면 발생하는 에러  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_TYPE_CONVERSION(-20008)           | Type conversion error                                         | 주어진 타입의 값을 실제 데이터의 타입으로 변경할 수 없음.                                        |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_BIND_INDEX(-20009)                | Parameter index is out of range                               | 바인드할 데이터의 index가 유효하지 않음.                                                         |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_ATYPE(-20010)                     | Invalid T_CCI_A_TYPE value                                    |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_NOT_BIND(-20011)                  |                                                               | 사용되지 않음                                                                                    |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_PARAM_NAME(-20012)                | Invalid T_CCI_DB_PARAM value                                  |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_COLUMN_INDEX(-20013)              | Column index is out of range                                  |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_SCHEMA_TYPE(-20014)               |                                                               | 사용되지 않음                                                                                    |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_FILE(-20015)                      | Cannot open file                                              | 파일을 열거나 읽기/쓰기 실패함.                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_CONNECT(-20016)                   | Cannot connect to CUBRID CAS                                  | 서버와 연결 시도 시 CAS 접속에 실패함.                                                           |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_ALLOC_CON_HANDLE(-20017)          | Cannot allocate connection handle %                           |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_REQ_HANDLE(-20018)                | Cannot allocate request handle %                              |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_INVALID_CURSOR_POS(-20019)        | Invalid cursor position                                       |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_OBJECT(-20020)                    | Invalid oid string                                            |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_CAS(-20021)                       |                                                               | 사용되지 않음                                                                                    |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_HOSTNAME(-20022)                  | Unknown host name                                             |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_OID_CMD(-20023)                   | Invalid T_CCI_OID_CMD value                                   |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_BIND_ARRAY_SIZE(-20024)           | Array binding size is not specified                           |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_ISOLATION_LEVEL(-20025)           | Unknown transaction isolation level                           |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_SET_INDEX(-20026)                 | Invalid set index                                             | T_CCI_SET 구조체에 포함된 set원소를 가져올 때 잘못된 위치가 지정됨.                              |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_DELETED_TUPLE(-20027)             | Current row was deleted %                                     |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_SAVEPOINT_CMD(-20028)             | Invalid T_CCI_SAVEPOINT_CMD value                             | :c:func:`cci_savepoint` 함수의 인자로 유효하지 않은 T_CCI_SAVEPOINT_CMD 값이 사용됨.             |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_THREAD_RUNNING(-20029)            |                                                               I                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_INVALID_URL(-20030)               | Invalid url string                                            |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_INVALID_LOB_READ_POS(-20031)      | Invalid lob read position                                     |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_INVALID_LOB_HANDLE(-20032)        | Invalid lob handle                                            |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_NO_PROPERTY(-20033)               | Could not find a property                                     |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_PROPERTY_TYPE(-20034)             | Invalid property type                                         |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_INVALID_DATASOURCE(-20035)        | Invalid CCI datasource                                        |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_DATASOURCE_TIMEOUT(-20036)        | All connections are used                                      |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_DATASOURCE_TIMEDWAIT(-20037)      | pthread_cond_timedwait error                                  |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_LOGIN_TIMEOUT(-20038)             | Connection timed out                                          |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_QUERY_TIMEOUT(-20039)             | Request timed out                                             |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_RESULT_SET_CLOSED(-20040)         |                                                               |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_INVALID_HOLDABILITY(-20041)       | Invalid holdability mode. The only accepted values are 0 or 1 |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_NOT_UPDATABLE(-20042)             | Request handle is not updatable                               |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_INVALID_ARGS(-20043)              | Invalid argument                                              |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+
| CCI_ER_USED_CONNECTION(-20044)           | This connection is used already.                              |                                                                                                  |
+------------------------------------------+---------------------------------------------------------------+--------------------------------------------------------------------------------------------------+

**C Type Definition**

다음은 CCI API 함수에서 사용하는 구조체들이다.

+--------------------------+----------+-----------------------------------------+-----------------------------+
| 이름                     | 타입     | 멤버                                    | 설명                        |
+==========================+==========+=========================================+=============================+
| **T_CCI_ERROR**          | struct   | char err_msg[1024]                      | 데이터베이스 에러 정보 표현 |
|                          |          +-----------------------------------------+                             |
|                          |          | int err_code                            |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_BIT**            | struct   | int size                                | bit 타입 표현               |
|                          |          +-----------------------------------------+                             |
|                          |          | char \*buf                              |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_DATE**           | struct   | short yr                                | datetime, timestamp, date,  |
|                          |          +-----------------------------------------+ time 타입 표현              |
|                          |          | short mon                               |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | short day                               |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | short hh                                |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | short mm                                |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | short ss                                |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | short ms                                |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_DATE_TZ**        | struct   | short yr                                | timezone과 date/time        |
|                          |          +-----------------------------------------+ 타입 표현                   |
|                          |          | short mon                               |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | short day                               |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | short hh                                |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | short mm                                |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | short ss                                |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | short ms                                |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | char tz[64]                             |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_SET**            | void*    |                                         | set 타입 표현               |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_COL_INFO**       | struct   | **T_CCI_U_EXT_TYPE**                    | **SELECT**                  |
|                          |          | type                                    | 문에 대한 칼럼 정보 표현    |
|                          |          +-----------------------------------------+                             |
|                          |          | char is_non_null                        |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | short scale                             |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | int precision                           |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | char \*col_name                         |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | char \*real_attr                        |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | char \*class_name                       |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_QUERY_RESULT**   | struct   | int result_count                        | batch 실행에 대한 결과      |
|                          |          +-----------------------------------------+                             |
|                          |          | int stmt_type                           |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | char \*err_msg                          |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | char oid[32]                            |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_PARAM_INFO**     | struct   | **T_CCI_PARAM_MODE**                    | input 파라미터에 대한       |
|                          |          | mode                                    | 정보 표현                   |
|                          |          +-----------------------------------------+                             |
|                          |          | **T_CCI_U_TYPE**                        |                             |
|                          |          | type                                    |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | short scale                             |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | int precision                           |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_U_EXT_TYPE**     | unsigned |                                         | 데이터베이스 타입 정보      |
|                          | char     |                                         |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_U_TYPE**         | enum     | **CCI_U_TYPE_UNKNOWN**                  | 데이터베이스 타입 정보      |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_NULL**                     |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_CHAR**                     |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_STRING**                   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_BIT**                      |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_VARBIT**                   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_NUMERIC**                  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_INT**                      |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_SHORT**                    |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_FLOAT**                    |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_DOUBLE**                   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_DATE**                     |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_TIME**                     |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_TIMESTAMP**                |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_SET**                      |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_MULTISET**                 |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_SEQUENCE**                 |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_OBJECT**                   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_BIGINT**                   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_DATETIME**                 |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_BLOB**                     |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_CLOB**                     |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_ENUM**                     |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_UINT**                     |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_USHORT**                   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_UBIGINT**                  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_TIMESTAMPTZ**              |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_TIMESTAMPLTZ**             |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_DATETIMETZ**               |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_U_TYPE_DATETIMELTZ**              |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_A_TYPE**         | enum     | **CCI_A_TYPE_STR**                      | API에서 사용되는 타입       |
|                          |          |                                         | 정보 표현                   |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_A_TYPE_INT**                      |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_A_TYPE_FLOAT**                    |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_A_TYPE_DOUBLE**                   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_A_TYPE_BIT**                      |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_A_TYPE_DATE**                     |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_A_TYPE_SET**                      |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_A_TYPE_BIGINT**                   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_A_TYPE_BLOB**                     |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_A_TYPE_CLOB**                     |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_A_TYPE_CLOB**                     |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_A_TYPE_REQ_HANDLE**               |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_A_TYPE_UINT**                     |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_A_TYPE_UBIGINT**                  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_A_TYPE_DATE_TZ**                  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_A_TYPE_UINT**                     |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_DB_PARAM**       | enum     | **CCI_PARAM_ISOLATION_LEVEL**           | 시스템 파라미터 이름        |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_PARAM_LOCK_TIMEOUT**              |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_PARAM_MAX_STRING_LENGTH**         |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_PARAM_AUTO_COMMIT**               |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_SCH_TYPE**       | enum     | **CCI_SCH_CLASS**                       |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_VCLASS**                      |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_QUERY_SPEC**                  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_ATTRIBUTE**                   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_CLASS_ATTRIBUTE**             |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_METHOD**                      |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_CLASS_METHOD**                |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_METHOD_FILE**                 |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_SUPERCLASS**                  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_SUBCLASS**                    |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_CONSTRAIT**                   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_TRIGGER**                     |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_CLASS_PRIVILEGE**             |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_ATTR_PRIVILEGE**              |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_DIRECT_SUPER_CLASS**          |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_PRIMARY_KEY**                 |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_IMPORTED_KEYS**               |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_EXPORTED_KEYS**               |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_SCH_CROSS_REFERENCE**             |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_CUBRID_STMT**    | enum     | **CUBRID_STMT_ALTER_CLASS**             |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
|                          |          | **CUBRID_STMT_ALTER_SERIAL**            |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_COMMIT_WORK**             |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_REGISTER_DATABASE**       |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_CREATE_CLASS**            |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_CREATE_INDEX**            |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_CREATE_TRIGGER**          |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_CREATE_SERIAL**           |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_DROP_DATABASE**           |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_DROP_CLASS**              |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_DROP_INDEX**              |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_DROP_LABEL**              |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_DROP_TRIGGER**            |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_DROP_SERIAL**             |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_EVALUATE**                |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_RENAME_CLASS**            |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_ROLLBACK_WORK**           |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_GRANT**                   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_REVOKE**                  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_STATISTICS**              |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_INSERT**                  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_SELECT**                  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_UPDATE**                  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_DELETE**                  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_CALL**                    |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_GET_ISO_LVL**             |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_GET_TIMEOUT**             |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_GET_OPT_LVL**             |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_SET_OPT_LVL**             |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_SCOPE**                   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_GET_TRIGGER**             |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_SET_TRIGGER**             |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_SAVEPOINT**               |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_PREPARE**                 |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_ATTACH**                  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_USE**                     |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_REMOVE_TRIGGER**          |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_RENAME_TRIGGER**          |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_ON_LDB**                  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_GET_LDB**                 |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_SET_LDB**                 |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_GET_STATS**               |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_CREATE_USER**             |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_DROP_USER**               |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_ALTER_USER**              |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_SET_SYS_PARAMS**          |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_ALTER_INDEX**             |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_CREATE_STORED_PROCEDURE** |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_DROP_STORED_PROCEDURE**   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_PREPARE_STATEMENT**       |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_EXECUTE_PREPARE**         |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_DEALLOCATE_PREPARE**      |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_TRUNCATE**                |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_DO**                      |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_SELECT_UPDATE**           |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_SET_SESSION_VARIABLES**   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_DROP_SESSION_VARIABLES**  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_MERGE**                   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_SET_NAMES**               |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_ALTER_STORED_PROCEDURE**  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CUBRID_STMT_KILL**                    |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_CURSOR_POS**     | enum     | **CCI_CURSOR_FIRST**                    |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_CURSOR_CURRENT**                  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_CURSOR_LAST**                     |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_TRAN_ISOLATION** | enum     | **TRAN_READ_COMMITTED**                 |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **TRAN_REPEATABLE_READ**                |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **TRAN_SERIALIZABLE**                   |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+
| **T_CCI_PARAM_MODE**     | enum     | **CCI_PARAM_MODE_UNKNOWN**              |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_PARAM_MODE_IN**                   |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_PARAM_MODE_OUT**                  |                             |
|                          |          +-----------------------------------------+                             |
|                          |          | **CCI_PARAM_MODE_INOUT**                |                             |
+--------------------------+----------+-----------------------------------------+-----------------------------+

.. note::

    칼럼에서 정의한 크기보다 큰 문자열을 **INSERT** / **UPDATE** 하면 문자열이 잘려서 입력된다.

CCI 예제 프로그램
=================

예제 프로그램은 CUBRID 설치 과정에서 기본적으로 배포되는 데이터베이스인 *demodb* 를 활용하여 CCI를 사용하는 응용 프로그램을 간단하게 작성한 것이다. 예제를 통하여 CAS와 연결하기, 질의 준비, 질의 수행, 응답 처리, 연결 끊기 등의 과정을 따라한다. 예제는 Linux 기반의 동적 링크를 적용하는 방법으로 작성되었다.

다음은 예제에서 사용하는 *demodb* 데이터베이스의 *olympic* 테이블의 스키마 정보이다. ::

    csql> ;sc olympic
     
    === <Help: Schema of a Class> ===
     
     
     <Class Name>
     
         olympic
     
     <Attributes>
     
         host_year            INTEGER NOT NULL
         host_nation          CHARACTER VARYING(40) NOT NULL
         host_city            CHARACTER VARYING(20) NOT NULL
         opening_date         DATE NOT NULL
         closing_date         DATE NOT NULL
         mascot               CHARACTER VARYING(20)
         slogan               CHARACTER VARYING(40)
         introduction         CHARACTER VARYING(1500)
     
     <Constraints>
     
         PRIMARY KEY pk_olympic_host_year ON olympic (host_year)
     
**준비**

예제 프로그램을 수행하기 전에 반드시 확인해야 할 사항은 *demodb* 데이터베이스와 브로커의 가동 여부이다. *demodb* 데이터베이스와 브로커는 **cubrid** 유틸리티를 이용하여 시작할 수 있다. 다음은 **cubrid** 유틸리티를 이용하여 데이터베이스 서버와 브로커를 가동하는 예제이다. ::

    [tester@testdb ~]$ cubrid server start demodb
    @ cubrid master start
    ++ cubrid master start: success
    @ cubrid server start: demodb
     
    This may take a long time depending on the amount of recovery works to do.
     
    CUBRID 9.2
     
    ++ cubrid server start: success
    [tester@testdb ~]$ cubrid broker start
    @ cubrid broker start
    ++ cubrid broker start: success

**빌드**

프로그램 소스와 Makefile이 준비된 상태에서 **make** 를 수행하면 *test* 라는 실행 파일이 생성된다. 정적 라이브러리를 사용하면 추가로 파일을 배포할 필요가 없고 속도가 빠르다. 하지만, 프로그램의 크기와 메모리 사용량이 커지는 단점이 있다. 동적 라이브러리를 사용하면 성능상의 오버헤드는 있지만, 메모리와 프로그램 크기에 있어 최적화를 이룰 수 있다.

다음은 Linux에서 **make** 를 사용하지 않고 동적인 라이브러리를 사용하여 테스트 프로그램을 빌드하는 명령 행의 예제이다. ::

    cc -o test test.c -I$CUBRID/include -L$CUBRID/lib -lnsl -lcascci

**예제 코드**

.. code-block:: c

    #include <stdio.h>
    #include <cas_cci.h>
    char *cci_client_name = "test";
    int main (int argc, char *argv[])
    {
        int con = 0, req = 0, col_count = 0, res, ind, i;
        T_CCI_ERROR error;
        T_CCI_COL_INFO *res_col_info;
        T_CCI_CUBRID_STMT stmt_type;
        char *buffer, db_ver[16];
        printf("Program started!\n");
        if ((con=cci_connect("localhost", 30000, "demodb", "PUBLIC", ""))<0) {
            printf( "%s(%d): cci_connect fail\n", __FILE__, __LINE__);
            return -1;
        }
       
        if ((res=cci_get_db_version(con, db_ver, sizeof(db_ver)))<0) {
            printf( "%s(%d): cci_get_db_version fail\n", __FILE__, __LINE__);
            goto handle_error;
        }
        printf("DB Version is %s\n",db_ver);
        if ((req=cci_prepare(con, "select * from event", 0,&error))<0) {
            if (req < 0) {
                printf( "%s(%d): cci_prepare fail(%d)\n", __FILE__, __LINE__,error.err_code);
            }
            goto handle_error;
        }
        printf("Prepare ok!(%d)\n",req);
        res_col_info = cci_get_result_info(req, &stmt_type, &col_count);
        if (!res_col_info) {
            printf( "%s(%d): cci_get_result_info fail\n", __FILE__, __LINE__);
            goto handle_error;
        }
       
        printf("Result column information\n"
               "========================================\n");
        for (i=1; i<=col_count; i++) {
            printf("name:%s  type:%d(precision:%d scale:%d)\n",
                CCI_GET_RESULT_INFO_NAME(res_col_info, i),
                CCI_GET_RESULT_INFO_TYPE(res_col_info, i),
                CCI_GET_RESULT_INFO_PRECISION(res_col_info, i),
                CCI_GET_RESULT_INFO_SCALE(res_col_info, i));
        }
        printf("========================================\n");
        if ((res=cci_execute(req, 0, 0, &error))<0) {
            if (req < 0) {
                printf( "%s(%d): cci_execute fail(%d)\n", __FILE__, __LINE__,error.err_code);
            }
            goto handle_error;
        }
       
        while (1) {
            res = cci_cursor(req, 1, CCI_CURSOR_CURRENT, &error);
            if (res == CCI_ER_NO_MORE_DATA) {
                printf("Query END!\n");
                break;
            }
            if (res<0) {
                if (req < 0) {
                    printf( "%s(%d): cci_cursor fail(%d)\n", __FILE__, __LINE__,error.err_code);
                }
                goto handle_error;
            }
           
            if ((res=cci_fetch(req, &error))<0) {
                if (res < 0) {
                    printf( "%s(%d): cci_fetch fail(%d)\n", __FILE__, __LINE__,error.err_code);
                }
                goto handle_error;
            }
           
            for (i=1; i<=col_count; i++) {
                if ((res=cci_get_data(req, i, CCI_A_TYPE_STR, &buffer, &ind))<0) {
                    printf( "%s(%d): cci_get_data fail\n", __FILE__, __LINE__);
                    goto handle_error;
                }
                printf("%s \t|", buffer);
            }
            printf("\n");
        }
        if ((res=cci_close_req_handle(req))<0) {
            printf( "%s(%d): cci_close_req_handle fail", __FILE__, __LINE__);
           goto handle_error;
        }
        if ((res=cci_disconnect(con, &error))<0) {
            if (res < 0) {
                printf( "%s(%d): cci_disconnect fail(%d)", __FILE__, __LINE__,error.err_code);
            }
            goto handle_error;
        }
        printf("Program ended!\n");
        return 0;
       
        handle_error:
        if (req > 0)
            cci_close_req_handle(req);
        if (con > 0)
            cci_disconnect(con, &error);
        printf("Program failed!\n");
        return -1;
    }
