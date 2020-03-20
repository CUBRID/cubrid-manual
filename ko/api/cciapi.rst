
:meta-keywords: CCI driver, CCI api, cubrid cci
:meta-description: CUBRID CCI API Reference for your C-based back-end application.

CCI API 레퍼런스
================

.. contents::

cci_bind_param
--------------

.. c:function::    int cci_bind_param (int req_handle, int index, T_CCI_A_TYPE a_type, void *value, T_CCI_U_TYPE u_type, char flag)

    prepared statement에서 *bind* 변수에 데이터를 바인딩하기 위하여 사용되는 함수이다. 이때, 주어진 *a_type* 의 *value* 의 값을 실제 바인딩되어야 하는 타입으로 변환하여 저장한다. 이후, :c:func:`cci_execute`\ 가 호출될 때 저장된 데이터가 서버로 전송된다. 같은 *index* 에 대해서 여러 번 :c:func:`cci_bind_param`\ 을 호출할 경우 마지막으로 설정한 값이 유효하다. 

    :param req_handle: (IN) prepared statement의 요청 핸들.
    :param index: (IN) 바인딩될 위치이며, 1부터 시작.
    :param a_type: (IN) *value* 의 타입.
    :param value: (IN) 바인딩될 데이터 값.
    :param u_type: (IN) 데이터베이스에 반영될 데이터 타입.
    :param flag: (IN) bind_flag(:c:type:`CCI_BIND_PTR`).
    :return: 에러 코드(0: 성공)
    
        *   **CCI_ER_BIND_INDEX**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_TYPE_CONVERSION**
        *   **CCI_ER_USED_CONNECTION**

    데이터베이스에 **NULL**\ 을 바인딩하려면 다음의 두 가지 중 하나를 설정한다.
        
    *   *value* 값을 **NULL** 포인터로 설정
    *   *u_type*\ 을 :c:macro:`CCI_U_TYPE_NULL`\로 설정

    다음은 NULL을 바인딩하는 코드의 일부이다.
    
    .. code-block:: c
    
        res = cci_bind_param (req, 2 /* binding index */, CCI_A_TYPE_STR, NULL, CCI_U_TYPE_STRING, CCI_BIND_PTR);
        
    또는 
    
    .. code-block:: c
    
        res = cci_bind_param (req, 2 /* binding index */, CCI_A_TYPE_STR, data, CCI_U_TYPE_NULL, CCI_BIND_PTR);

    가 사용될 수 있다.
    
    *flag*\에 :c:type:`CCI_BIND_PTR`\ 이 설정되어 있을 경우 *value* 변수의 포인터만 복사하고(shallow copy) 값은 복사하지 않는다.
    *flag*\가 설정되지 않는 경우 메모리를 할당하여 *value* 변수의 값을 복사(deep copy)한다. 만약 같은 메모리 버퍼를 이용하여 여러 개의 칼럼을 바인딩할 경우라면, :c:type:`CCI_BIND_PTR` *flag*\를 설정하지 않아야 한다.

    :c:type:`T_CCI_A_TYPE`\ 은 CCI 응용 프로그램 내에서 데이터 바인딩에 사용되는 C 언어의 타입을 의미하며, int, float 등의 primitive 타입과 :c:type:`T_CCI_BIT`, :c:type:`T_CCI_DATE` 등의 CCI 가 정의한 user-defined 타입으로 구성된다. 각 타입에 대한 식별자는 아래의 표와 같이 정의되어 있다.

    +-----------------------------+-----------------------------+
    | a_type                      | value 타입                  |
    +=============================+=============================+
    | **CCI_A_TYPE_STR**          | char \*                     |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_INT**          | int \*                      |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_FLOAT**        | float \*                    |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_DOUBLE**       | double \*                   |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_BIT**          | **T_CCI_BIT** \*            |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_SET**          | **T_CCI_SET**               |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_DATE**         | **T_CCI_DATE** \*           |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_BIGINT**       | int64_t \*                  |
    |                             | (For Windows: __int64 \*)   |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_BLOB**         | **T_CCI_BLOB**              |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_CLOB**         | **T_CCI_CLOB**              |
    +-----------------------------+-----------------------------+
    
    :c:type:`T_CCI_U_TYPE`\ 은 데이터베이스의 칼럼 타입으로, value 인자를 통해 바인딩된 데이터를 이 타입으로 변환한다.
    :c:func:`cci_bind_param` 함수는 C 언어가 이해하는 A 타입의 데이터를 데이터베이스가 이해할 수 있는 U 타입의 데이터로 변환하기 위한 정보를 전달하기 위해서 두 가지 타입을 사용한다.

    U 타입이 허용하는 A 타입은 여러 가지이다. 예를 들어 **CCI_U_TYPE_INT** 는 **CCI_A_TYPE_INT** 외에 **CCI_A_TYPE_STR** 도 A 타입으로 받을 수 있다. 타입 변환은 :ref:`implicit-type-conversion`\ 을 따른다.

    :c:type:`T_CCI_A_TYPE` 및 :c:type:`T_CCI_U_TYPE` enum은 모두 **cas_cci.h** 파일에 정의되어 있다. 각 타입에 대한 식별자 정의는 아래 표를 참고한다.

    +--------------------------+-----------------------+
    | u_type                   | 대응되는 기본 a_type  |
    +==========================+=======================+
    | **CCI_U_TYPE_CHAR**      | **CCI_A_TYPE_STR**    |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_STRING**    | **CCI_A_TYPE_STR**    |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_BIT**       | **CCI_A_TYPE_BIT**    |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_VARBIT**    | **CCI_A_TYPE_BIT**    |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_NUMERIC**   | **CCI_A_TYPE_STR**    |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_INT**       | **CCI_A_TYPE_INT**    |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_SHORT**     | **CCI_A_TYPE_INT**    |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_FLOAT**     | **CCI_A_TYPE_FLOAT**  |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_DOUBLE**    | **CCI_A_TYPE_DOUBLE** |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_DATE**      | **CCI_A_TYPE_DATE**   |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_TIME**      | **CCI_A_TYPE_DATE**   |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_TIMESTAMP** | **CCI_A_TYPE_DATE**   |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_OBJECT**    | **CCI_A_TYPE_STR**    |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_BIGINT**    | **CCI_A_TYPE_BIGINT** |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_DATETIME**  | **CCI_A_TYPE_DATE**   |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_BLOB**      | **CCI_A_TYPE_BLOB**   |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_CLOB**      | **CCI_A_TYPE_CLOB**   |
    +--------------------------+-----------------------+
    | **CCI_U_TYPE_ENUM**      | **CCI_A_TYPE_STR**    |
    +--------------------------+-----------------------+

    날짜를 포함하는 문자열을 **DATE**, **DATETIME** 또는 **TIMESTAMP** 의 입력 인자로 할 때, 날짜 문자열의 형식은 "YYYY/MM/DD" 형식 또는 "YYYY-MM-DD" 형식만 허용한다. 즉, "2012/01/31" 또는 "2012-01-31"과 같은 형식은 허용하지만 "01/31/2012"와 같은 형식은 허용하지 않는다. 날짜를 포함하는 문자열을 날짜 타입의 입력 인자로 하는 예는 다음과 같다.

    .. code-block:: c

        // "CREATE TABLE tbl(aa date, bb datetime)";
         
        char *values[][2] =
        {
            {"1994/11/30", "1994/11/30 20:08:08"},
            {"2008-10-31", "2008-10-31 20:08:08"}
        };
        
        req = cci_prepare(conn, "insert into tbl (aa, bb) values (?, ?)", CCI_PREPARE_INCLUDE_OID, &error);
        
        for(i=0; i< 2; i++)
        {
            res = cci_bind_param(req, 1, CCI_A_TYPE_STR, values[i][0], CCI_U_TYPE_DATE, (char)0);
            res = cci_bind_param(req, 2, CCI_A_TYPE_STR, values[i][1], CCI_U_TYPE_DATETIME, (char)0);
            cci_execute(req, CCI_EXEC_QUERY_ALL, 0, err_buf);
        }

cci_bind_param_array
--------------------

.. c:function:: int cci_bind_param_array(int req_handle, int index, T_CCI_A_TYPE a_type, void *value, int *null_ind, T_CCI_U_TYPE u_type)

    prepare된 *req_handle* 에 대해서 파라미터 배열을 바인딩한다. 이후, :c:func:`cci_execute_array`\ 가 호출될 때 저장된 *value* 포인터에 의해 데이터가 서버로 전송된다. 같은 *index* 에 대해서 여러 번 :c:func:`cci_bind_param_array`\ 가 호출될 경우 마지막 설정된 값이 유효하다. 데이터에 **NULL**\ 을 바인딩할 경우 *null_ind*\ 에 0이 아닌 값을 설정한다. *value* 값이 **NULL** 포인터인 경우, 또는 *u_type*\ 이 **CCI_U_TYPE_NULL**\ 인 경우 모든 데이터가 **NULL**\ 로 바인딩되며 *value*\ 에 의해 사용되는 데이터 버퍼는 재사용될 수 없다. *a_type*\ 에 대한 *value*\ 의 데이터 타입은 :c:func:`cci_bind_param`\ 의 설명을 참조한다.

    :param req_handle: (IN) prepared statement의 요청 핸들
    :param index: (IN) 바인딩될 위치
    :param a_type: (IN) *value* 의 타입
    :param value: (IN) 바인딩될 데이터 값
    :param null_ind: (IN) **NULL** 식별자 배열(0 : not **NULL**, 1 : **NULL**)
    :param u_type: (IN) 데이터베이스에 반영될 데이터 타입
    :return: 에러 코드(0: 성공)
    
        *   **CCI_ER_BIND_INDEX**
        *   **CCI_ER_BIND_ARRAY_SIZE**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_TYPE_CONVERSION**
        *   **CCI_ER_USED_CONNECTION**

cci_bind_param_array_size
-------------------------

.. c:function:: int cci_bind_param_array_size(int req_handle, int array_size)

    :c:func:`cci_bind_param_array`\ 에서 사용될 array의 크기를 결정한다. :c:func:`cci_bind_param_array`\ 가 사용되기 전에 반드시 :c:func:`cci_bind_param_array_size`\ 가 먼저 호출 되어야 한다.

    :param req_handle: (IN) prepared statement의 요청 핸들
    :param array_size: (IN) 바인딩할 배열 크기
    :return: 에러 코드(0: 성공)
    
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_USED_CONNECTION**

cci_bind_param_ex
-----------------

.. c:function:: int cci_bind_param_ex (int req_handle, int index, T_CCI_A_TYPE a_type, void *value, int length, T_CCI_U_TYPE u_type, char flag) 
     
    :c:func:`cci_bind_param`\과 동일한 동작을 수행한다. 다만 문자열 타입인 경우 문자열의 바이트 길이를 지정하는 *length* 인자가 추가로 존재한다. 
     
    :param req_handle: (IN) prepared statement의 요청 핸들
    :param index: (IN) 바인딩될 위치이며, 1부터 시작
    :param a_type: (IN) *value* 의 타입 
    :param value: (IN) 바인딩할 데이터 값
    :param length: (IN) 바인딩할 문자열의 바이트 길이
    :param u_type: (IN) 데이터베이스에 반영될 데이터 타입
    :param flag: (IN) bind_flag(:c:type:`CCI_BIND_PTR`)
     
    :return: 에러 코드(0: 성공) 
  
    *length* 인자는 아래와 같이 '\\0'을 포함하는 문자열을 바인딩하기 위해 사용할 수 있다. 
     
    .. code-block:: c 
  
        cci_bind_param_ex(req, 1, CCI_A_TYPE_STR, "aaa\0bbb", 7, CCI_U_TYPE_STRING, 0); 

cci_blob_free
-------------

.. c:function:: int cci_blob_free(T_CCI_BLOB blob)

    **BLOB** 구조체에 대한 메모리를 해제한다.

    :return: 에러 코드(0: 성공)
    
        *   **CCI_ER_INVALID_LOB_HANDLE**

cci_blob_new
------------

.. c:function:: int cci_blob_new(int conn_handle, T_CCI_BLOB* blob, T_CCI_ERROR* error_buf)

    **LOB** 데이터가 저장될 빈 파일을 하나 생성하고, 해당 파일을 참조하는 Locator를 *blob* 구조체에 반환한다.

    :param conn_handle: (IN) 연결 핸들
    :param blob: (OUT) **LOB** Locator
    :param error_buf: (OUT) 에러 버퍼
    :return: 에러 코드(0: 성공)
    
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_INVALID_LOB_HANDLE**
        *   **CCI_ER_LOGIN_TIMEOUT**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_USED_CONNECTION**

cci_blob_read
-------------

.. c:function:: int cci_blob_read(int conn_handle, T_CCI_BLOB blob, long long start_pos, int length, char *buf, T_CCI_ERROR* error_buf)

    *blob* 에 명시한 **LOB** 데이터 파일의 *start_pos* 부터 *length* 만큼 데이터를 읽어 *buf* 에 저장한 후 이를 반환한다.

    :param conn_handle: (IN) 연결 핸들
    :param blob: (IN) **LOB** Locator
    :param start_pos: (IN) **LOB** 데이터 파일의 위치 인덱스
    :param length: (IN) 파일로부터 가져올 **LOB** 데이터 길이
    :param buf: (IN) 데이터 읽기 버퍼
    :param error_buf: (OUT) 에러 버퍼
    :return: 에러 코드(0: 성공)
    
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_INVALID_LOB_HANDLE**
        *   **CCI_ER_INVALID_LOB_READ_POS**
        *   **CCI_ER_LOGIN_TIMEOUT**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_QUERY_TIMEOUT**
        *   **CCI_ER_USED_CONNECTION**

cci_blob_size
-------------

.. c:function:: long long cci_blob_size(T_CCI_BLOB blob)

    *blob* 에 명시한 데이터 파일의 크기를 반환한다.

    :param blob: (IN) **LOB** Locator
    :return: **BLOB** 데이터 파일의 크기(>=0 : 성공), 에러 코드(<0 : 에러)

        *   **CCI_ER_INVALID_LOB_HANDLE**

cci_blob_write
--------------

.. c:function:: int cci_blob_write(int conn_handle, T_CCI_BLOB blob, long long start_pos, int length, const char *buf, T_CCI_ERROR* error_buf)

    *buf* 로부터 *length* 만큼 데이터를 읽어 *blob* 에 명시한 **LOB** 데이터 파일의 *start_pos* 부터 저장한다.

    :param conn_handle: (IN) 연결 핸들
    :param blob: (IN) **LOB** Locator
    :param start_pos: (IN) **LOB** 데이터 파일의 위치 인덱스
    :param length: (IN) 버퍼로부터 가져올 데이터 길이
    :param buf: (OUT) 데이터 쓰기 버퍼
    :param error_buf: (OUT) 에러 버퍼
    :return: write한 크기(>=0 : 성공), 에러 코드(<0 : 에러)

        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_INVALID_LOB_HANDLE**
        *   **CCI_ER_LOGIN_TIMEOUT**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_QUERY_TIMEOUT**
        *   **CCI_ER_USED_CONNECTION**

cci_cancel
----------

.. c:function:: int cci_cancel(int conn_handle)

    다른 스레드에서 실행 중인 질의를 취소시킨다. Java의 Statement.cancel() 메서드와 같은 기능을 수행한다.

    :param conn_handle: (IN) 연결 핸들
    :return: 에러 코드
        
        *    **CCI_ER_COMMUNICATION**
        *    **CCI_ER_CON_HANDLE**
        *    **CCI_ER_CONNECT**

    다음은 main 함수에서 스레드의 질의 실행을 취소하는 예이다.
    
    .. code-block:: c

        /* gcc -o pthr pthr.c -m64 -I${CUBRID}/include -lnsl ${CUBRID}/lib/libcascci.so -lpthread
        */


        #include <stdio.h>
        #include <cas_cci.h>
        #include <unistd.h>
        #include <pthread.h>
        #include <string.h>
        #include <time.h>

        #define QUERY "select * from db_class A, db_class B, db_class C, db_class D, db_class E"

        static void *thread_main (void *arg);
        static void *execute_statement (int con, char *sql_stmt);

        int
        main (int argc, char *argv[])
        {
            int thr_id = 0, conn_handle = 0, res = 0;
            void *jret;
            pthread_t th;
            char url[1024];
            T_CCI_ERROR error;
            snprintf (url, 1024, "cci:CUBRID:localhost:33000:demodb:PUBLIC::");
    
            conn_handle = cci_connect_with_url_ex (url, NULL, NULL, &error);

            if (conn_handle < 0)
            {
                printf ("ERROR: %s\n", error.err_msg);
                return -1;
            }

            res = pthread_create (&th, NULL, &thread_main, (void *) &conn_handle);

            if (res < 0)
            {
                printf ("thread fork failed.\n");
                return -1;
            }
            else
            {
                printf ("thread started\n");
            }
            sleep (5);
            // If thread_main is still running, below cancels the query of thread_main.
            res = cci_cancel (conn_handle);
            if (res < 0)
            {
                printf ("cci_cancel failed\n");
                return -1;
            }
            else
            {
                printf ("The query was canceled by cci_cancel.\n");
            }
            res = pthread_join (th, &jret);
            if (res < 0)
            {
                printf ("thread join failed.\n");
                return -1;
            }

            printf ("thread_main was cancelled with\n\t%s\n", (char *) jret);
            free (jret);
    
            res = cci_disconnect (conn_handle, &error);
            if (res < 0)
            {
                printf ("ERROR: %s\n", error.err_msg);
              return res;
            }

            return 0;
        }

        void *
        thread_main (void *arg)
        {
            int con = *((int *) arg);
            int ret_val;
            void *ret_ptr;
            T_CCI_ERROR error;

            cci_set_autocommit (con, CCI_AUTOCOMMIT_TRUE);
            ret_ptr = execute_statement (con, QUERY);
            return ret_ptr;
        }

        static void *
        execute_statement (int con, char *sql_stmt)
        {
            int col_count = 1, ind, i, req;
            T_CCI_ERROR error;
            char *buffer;
            char *error_msg;
            int res = 0;
    
            error_msg = (char *) malloc (128);
            if ((req = cci_prepare (con, sql_stmt, 0, &error)) < 0)
            {
                snprintf (error_msg, 128, "cci_prepare ERROR: %s\n", error.err_msg);
                goto conn_err;
            }

            if ((res = cci_execute (req, 0, 0, &error)) < 0)
            {
                snprintf (error_msg, 128, "cci_execute ERROR: %s\n", error.err_msg);
                goto execute_error;
            }
    
            if (res >= 0)
            {
                while (1)
                {
                    res = cci_cursor (req, 1, CCI_CURSOR_CURRENT, &error);
                    if (res == CCI_ER_NO_MORE_DATA)
                    {
                        break;
                    }
                    if (res < 0)
                    {
                        snprintf (error_msg, 128, "cci_cursor ERROR: %s\n",
                            error.err_msg);
                        return error_msg;
                    }

                    if ((res = cci_fetch (req, &error)) < 0)
                    {
                        snprintf (error_msg, 128, "cci_fetch ERROR: %s\n",
                            error.err_msg);
                        return error_msg;
                    }

                    for (i = 1; i <= col_count; i++)
                    {
                        if ((res = cci_get_data (req, i, CCI_A_TYPE_STR, &buffer, &ind)) < 0)
                        {
                            snprintf (error_msg, 128, "cci_get_data ERROR\n");
                            return error_msg;
                        }
                    }
                }
            }

            if ((res = cci_close_query_result (req, &error)) < 0)
            {
                snprintf (error_msg, 128, "cci_close_query_result ERROR: %s\n", error.err_msg);
                return error_msg;
            }
        execute_error:
            if ((res = cci_close_req_handle (req)) < 0)
            {
                snprintf (error_msg, 128, "cci_close_req_handle ERROR\n");
            }
        conn_err:
            return error_msg;
        }

cci_clob_free
-------------

.. c:function:: int cci_clob_free(T_CCI_CLOB clob)

    **CLOB** 구조체에 대한 메모리를 해제한다.

    :param clob: (IN) **LOB** Locator
    :return: 에러 코드(0: 성공)

        *   **CCI_ER_INVALID_LOB_HANDLE**

cci_clob_new
------------

.. c:function:: int cci_clob_new(int conn_handle, T_CCI_CLOB* clob, T_CCI_ERROR* error_buf)

    **LOB** 데이터가 저장될 빈 파일을 하나 생성하고, 해당 파일을 참조하는 Locator를 *clob* 구조체에 반환한다.

    :param conn_handle: (IN) 연결 핸들
    :param clob: (OUT) **LOB** Locator
    :param error_buf: (OUT) 에러 버퍼
    :return: 에러 코드(<0 : 에러)

        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_INVALID_LOB_HANDLE**
        *   **CCI_ER_LOGIN_TIMEOUT**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_USED_CONNECTION**

cci_clob_read
-------------

.. c:function:: int cci_clob_read(int conn_handle, T_CCI_CLOB clob, long start_pos, int length, char *buf, T_CCI_ERROR* error_buf)

    *clob* 에 명시한 **LOB** 데이터 파일의 *start_pos* 부터 *length* 만큼 데이터를 읽어 *buf* 에 저장한 후 이를 반환한다.
    
    :param conn_handle: (IN) 연결 핸들
    :param clob: (IN) **LOB** Locator
    :param start_pos: (IN) **LOB** 데이터 파일의 위치 인덱스
    :param length: (IN) 파일로부터 가져올 **LOB** 데이터 길이
    :param buf: (IN) 데이터 읽기 버퍼
    :param error_buf: (OUT) 에러 버퍼
    :return: read한 크기(>=0 : 성공), 에러 코드(<0 : 에러)

        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_INVALID_LOB_HANDLE**
        *   **CCI_ER_INVALID_LOB_READ_POS**
        *   **CCI_ER_LOGIN_TIMEOUT**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_QUERY_TIMEOUT**
        *   **CCI_ER_USED_CONNECTION**

cci_clob_size
-------------

.. c:function:: long long cci_clob_size(T_CCI_CLOB* clob)

    *clob* 에 명시한 데이터 파일의 크기를 반환한다.

    :param clob: (IN) **LOB** Locator
    :return: **CLOB** 데이터 파일의 크기(>=0 : 성공), 에러 코드(<0 : 에러)

        *   **CCI_ER_INVALID_LOB_HANDLE**

cci_clob_write
--------------

.. c:function:: int cci_clob_write(int conn_handle, T_CCI_CLOB clob, long start_pos, int length, const char *buf, T_CCI_ERROR* error_buf)

    *buf* 로부터 *length* 만큼 데이터를 읽어 *clob* 에 명시한 **LOB** 데이터 파일의 *start_pos* 부터 저장한다.

    :param conn_handle: (IN) 연결 핸들
    :param clob: (IN) **LOB** Locator
    :param start_pos: (IN) **LOB** 데이터 파일의 위치 인덱스
    :param length: (IN) 버퍼로부터 가져올 데이터 길이
    :param buf: (OUT) 데이터 쓰기 버퍼
    :param error_buf: (OUT) 에러 버퍼
    :return: write한 크기(>=0 : 성공), 에러 코드(<0 : 에러)

        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_INVALID_LOB_HANDLE**
        *   **CCI_ER_LOGIN_TIMEOUT**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_QUERY_TIMEOUT**
        *   **CCI_ER_USED_CONNECTION**

cci_close_query_result
----------------------

.. c:function::  int cci_close_query_result(int req_handle, T_CCI_ERROR *err_buf)

    :c:func:`cci_execute`, :c:func:`cci_execute_array` 또는 :c:func:`cci_execute_batch` 함수가 반환한 resultset을 종료(close)한다. 
    요청 핸들(req_handle)의 종료 없이 :c:func:`cci_prepare`\ 를 반복 수행하는 경우 :c:func:`cci_close_req_handle` 함수를 호출하기 전에 이 함수를 호출할 것을 권장한다.
    
    :param req_handle: (IN) 요청 핸들
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드 (0: 성공)
    
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_DBMS**   
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_RESULT_SET_CLOSED**
        *   **CCI_ER_USED_CONNECTION**

cci_close_req_handle
--------------------

.. c:function:: int cci_close_req_handle(int req_handle)

    :c:func:`cci_prepare`\ 로 획득한 요청 핸들을 종료(close)한다. 

    :param req_handle: (IN) 요청 핸들
    :return: 에러 코드(0 : 성공)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_USED_CONNECTION**

cci_col_get
-----------

.. c:function:: int cci_col_get (int conn_handle, char *oid_str, char *col_attr, int *col_size, int *col_type, T_CCI_ERROR *err_buf)

    collection type의 속성 값을 가져온다. 클래스 이름이 C이고 set_attr의 domain이 set(multiset, sequence)인 경우 다음의 질의와 같다.

    .. code-block:: sql
    
        SELECT a FROM C, TABLE(set_attr) AS t(a) WHERE C = oid;

    즉, 멤버 개수가 레코드 개수가 된다.

    :param conn_handle: (IN) 연결 핸들
    :param oid_str: (IN) oid
    :param col_attr: (IN) collection 속성 이름
    :param col_size: (OUT) collection 크기 (-1 : null)
    :param col_type: (OUT) collection 타입 (set, multiset, sequence : u_type)
    :param err_buf: (OUT) 에러 버퍼
    :return: 요청 핸들

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_OBJECT**
        *   **CCI_ER_DBMS**

cci_col_seq_drop
----------------

.. c:function:: int cci_col_seq_drop (int conn_handle, char *oid_str, char *col_attr, int index, T_CCI_ERROR *err_buf)

    sequence 속성 값에 index(base:1) 번째의 멤버를 drop시킨다. 다음은 seq 속성 값에서 첫 번째 값을 삭제하는 예이다. ::

        cci_col_seq_drop(conn_handle, oid_str, seq_attr, 1, err_buf);

    :param conn_handle: (IN) 연결 핸들
    :param oid_str: (IN) oid
    :param col_attr: (IN) collection 속성 이름
    :param index: (IN) 인덱스
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_OBJECT**
        *   **CCI_ER_DBMS**

cci_col_seq_insert
------------------

.. c:function:: int cci_col_seq_insert (int conn_handle, char *oid_str, char *col_attr, int index, char *value, T_CCI_ERROR *err_buf)

    sequence 속성 값에서 index(base:1) 번째에 멤버를 추가시킨다. 다음은 seq 속성 값에서 1번에 값 'a'를 추가하는 예이다. ::

        cci_col_seq_insert(conn_handle, oid_str, seq_attr, 1, "a", err_buf);
    
    :param conn_handle: (IN) 연결 핸들
    :param oid_str: (IN) oid
    :param col_attr: (IN) collection 속성 이름
    :param index: (IN) 인덱스
    :param value: (IN) 순차적 엘리먼트(스트링)
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_OBJECT**
        *   **CCI_ER_DBMS**

cci_col_seq_put
---------------

.. c:function:: int cci_col_seq_put (int conn_handle, char *oid_str, char *col_attr, int index, char *value, T_CCI_ERROR *err_buf)

    sequence 속성 값에 index(base:1) 번째의 멤버를 새로운 값으로 대체한다.. 다음은 seq 속성 값에서 1번 값을 'a'로 대체하는 예이다. ::

        cci_col_seq_put(conn_handle, oid_str, seq_attr, 1, "a", err_buf);

    :param conn_handle: (IN) 연결 핸들
    :param oid_str: (IN) oid
    :param col_attr: (IN) collection 속성 이름
    :param index: (IN) 인덱스
    :param value: (IN) 순차적 값
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_OBJECT**
        *   **CCI_ER_DBMS**

cci_col_set_add
---------------

.. c:function:: int cci_col_set_add (int conn_handle, char *oid_str, char *col_attr, char *value, T_CCI_ERRROR *err_buf)

    set 속성 값에 member 하나를 추가한다. 다음은 set 속성 값에 'a'를 추가하는 예이다. ::

        cci_col_set_add(conn_handle, oid_str, set_attr, "a", err_buf);

    :param conn_handle: (IN) 연결 핸들
    :param oid_str: (IN) oid
    :param col_attr: (IN) collection 속성 이름
    :param value: (IN) set 엘리먼트
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_OBJECT**
        *   **CCI_ER_DBMS**

cci_col_set_drop
----------------

.. c:function:: int cci_col_set_drop (int conn_handle, char *oid_str, char *col_attr, char *value, T_CCI_ERROR *err_buf)

    set 속성 값에서 멤버 하나를 drop시킨다. 다음은 set 속성 값에서 'a'를 삭제하는 예이다. ::

        cci_col_set_drop(conn_handle, oid_str, set_attr, "a", err_buf);
        
    :param conn_handle: (IN) 연결 핸들
    :param oid_str: (IN) oid
    :param col_attr: (IN) collection 속성 이름
    :param value: (IN) set 엘리먼트(스트링)
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_QUERY_TIMEOUT**
        *   **CCI_ER_LOGIN_TIMEOUT**
        *   **CCI_ER_COMMUNICATION**

cci_col_size
------------

.. c:function:: int cci_col_size (int conn_handle, char *oid_str, char *col_attr, int *col_size, T_CCI_ERROR *err_buf)

    set(seq) 속성의 개수를 가져온다.

    :param conn_handle: (IN) 연결 핸들
    :param oid_str: (IN) oid
    :param col_attr: (IN) collection 속성 이름
    :param col_size: (OUT) collection 크기 (-1 : NULL)
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드(0 : 성공)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_OBJECT**
        *   **CCI_ER_DBMS**

cci_connect
-----------

.. c:function:: int cci_connect(char *ip, int port, char *db_name, char *db_user, char *db_password)

    DB 서버에 대한 연결 핸들을 할당받고 해당 서버와 연결을 시도한다. 서버 연결에 성공하면 연결 핸들 ID를 반환하고, 실패하면 에러 코드를 반환한다.

    :param ip: (IN) 서버 IP 문자 스트링 (호스트 이름)
    :param port: (IN) 브로커 포트( **$CUBRID/conf/cubrid_broker.conf** 파일에 설정된 포트를 사용)
    :param db_name: (IN) DB 이름
    :param db_user: (IN) DB 사용자 이름
    :param db_passwd: (IN) DB 사용자 암호
    :return: 연결 핸들 ID(성공), 에러 코드(실패)

        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_HOSTNAME**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CONNECT**

cci_connect_ex
--------------

.. c:function:: int cci_connect_ex(char *ip, int port, char *db_name, char *db_user, char *db_password, T_CCI_ERROR * err_buf)

    **CCI_ER_DBMS** 에러를 반환하면 세부 에러 내용을 DB 에러 버퍼(*err_buf*)를 통해 확인할 수 있다는 점만 :c:func:`cci_connect`\ 와 다르고 나머지는 동일하다.

    :param ip: (IN) 서버 IP 문자 스트링 (호스트 이름)
    :param port: (IN) 브로커 포트( **$CUBRID/conf/cubrid_broker.conf** 파일에 설정된 포트를 사용)
    :param db_name: (IN) DB 이름
    :param db_user: (IN) DB 사용자 이름
    :param db_passwd: (IN) DB 사용자 암호
    :param err_buf: (OUT) 에러 버퍼
    :return: 연결 핸들 ID(성공), 에러 코드(실패)

        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_HOSTNAME**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CONNECT**

cci_connect_with_url
--------------------

.. c:function:: int cci_connect_with_url (char *url, char *db_user, char *db_password)

    *url* 인자로 전달된 접속 정보를 이용하여 데이터베이스로 연결을 시도한다. CCI에서 브로커의 HA 기능을 사용하는 경우 이 함수의 *url* 인자 내의 altHosts 속성을 이용하여, 장애 발생 시 failover할 standby 브로커 서버의 연결 정보를 명시해야 한다. 서버 연결에 성공하면 연결 핸들 ID를 반환하고, 실패하면 에러 코드를 반환한다. 브로커의 HA 기능에 대한 자세한 내용은 :ref:`duplexing-brokers`\를 참고한다.
    
    :param url: (IN) 서버 연결 정보 문자 스트링
    :param db_user: (IN) DB 사용자 이름. NULL이면 *url* 의 <*db_user*>가 사용된다. 이 값이 빈 문자열("")이거나 *url* 내의 <*db_user*>가 정의되지 않은 경우 DB 사용자 이름은 **PUBLIC** 이 된다.
    :param db_passwd: (IN) DB 사용자 암호. NULL이면 *url* 의 <*db_password*>가 사용된다. *url* 내의 <*db_password*>가 정의되지 않은 경우 암호는 빈 문자열("")이 된다. 
    :return: 연결 핸들 ID(성공), 에러 코드(실패)

        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_HOSTNAME**
        *   **CCI_ER_INVALID_URL**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_LOGIN_TIMEOUT**

    ::
    
        <url> ::= cci:CUBRID:<host>:<port>:<db_name>:<db_user>:<db_password>:[?<properties>]
         
        <properties> ::= <property> [&<property>]
        <property> ::= altHosts=<alternative_hosts> [ &rcTime=<time>] [ &loadBalance=true|false]
                     |{login_timeout|loginTimeout}=<milli_sec>
                     |{query_timeout|queryTimeout}=<milli_sec>
                     |{disconnect_on_query_timeout|disconnectOnQueryTimeout}=true|false
                     | logFile=<file_name>
                     | logBaseDir=<dir_name>
                     | logSlowQueries=true|false[&slowQueryThresholdMillis=<milli_sec>]
                     | logTraceApi=true|false
                     | logTraceNetwork=true|false
         
        <alternative_hosts> ::= <host>:<port> [,<host>:<port>]
         
        <host> := HOSTNAME | IP_ADDR
        <time> := SECOND
        <milli_sec> := MILLI SECOND    

    연결 대상과 관련된 속성은 **altHosts** 이며, 타임아웃과 관련된 속성은 **loginTimeout**, **queryTimeout**, **disconnectOnQueryTimeout** 이다. 디버깅용 로그 정보 설정과 관련된 속성은 **logSlowQueries**, **logTraceApi**, **logTraceNetwork** 이다. *url* 인자에 입력하는 모든 속성(property) 이름은 대소문자 구별을 하지 않는다.

    *   *host*: 마스터 데이터베이스의 호스트 이름 또는 IP 주소
    *   *port*: 포트 번호
    *   *db_name*: 데이터베이스 이름
    *   *db_user*: 데이터베이스 사용자 이름
    *   *db_password*: 데이터베이스 사용자 암호. *url* 내의 암호에는 ':'를 포함할 수 없다.

    *   **altHosts** = *standby_broker1_host*, *standby_broker2_host*, ...: active 서버에 연결할 수 없는 경우, 그 다음으로 연결을 시도(failover)할 standby 서버의 브로커 정보를 나타낸다. failover할 브로커를 여러 개 지정할 수 있고, **altHosts** 에 나열한 순서대로 연결을 시도한다.

        .. note:: 메인 호스트와 **altHosts** 브로커들의 **ACCESS_MODE**\ 설정에 **RW**\ 와 **RO**\ 가 섞여 있다 하더라도, 응용 프로그램은 **ACCESS_MODE**\ 와 무관하게 접속 대상 호스트를 결정한다. 따라서 사용자는 접속 대상 브로커의 **ACCESS_MODE**\ 를 감안해서 메인 호스트와 **altHosts**\ 를 정해야 한다.

    *   **rcTime**: 첫 번째로 접속했던 브로커에 장애가 발생한 이후 **altHosts** 에 명시한 브로커로 접속한다(브로커 failover). 이후, **rcTime** 만큼 시간이 경과할 때마다 원래의 브로커에 재접속을 시도한다(기본값 600초).

    *   **loadBalance**: 이 값이 true면 응용 프로그램이 메인 호스트와 **altHosts**\에 지정한 호스트들에 랜덤한 순서로 연결한다(기본값: false)
    
    *   **login_timeout** | **loginTimeout**: 데이터베이스에 로그인 시 타임아웃 값 (단위: msec). 이 시간을 초과하면 **CCI_ER_LOGIN_TIMEOUT** (-38) 에러를 반환한다. 기본값은 **30,000**\ (30초)이다. 이 값이 0인 경우 무한 대기를 의미한다. 이 값은 최초 접속 이후 내부적인 재접속이 발생하는 경우에도 적용된다.

    *   **query_timeout** | **queryTimeout**: :c:func:`cci_prepare`, :c:func:`cci_execute` 등의 함수를 호출했을 때 이 값으로 설정한 시간이 지나면 서버로 보낸 질의 요청에 대한 취소 메시지를 보내고 호출된 함수는 **CCI_ER_QUERY_TIMEOUT** (-39) 에러를 반환한다. 질의를 수행한 함수에서 타임아웃 발생 시 함수의 반환 값은 **disconnect_on_query_timeout**\ 의 설정에 따라 달라질 수 있다. 자세한 내용은 다음의 **disconnect_on_query_timeout**\ 을 참고한다. 
    
        .. note:: :c:func:`cci_execute`\ 에 CCI_EXEC_QUERY_ALL 플래그를 설정하거나 :c:func:`cci_execute_batch` 또는 :c:func:`cci_execute_array`\ 를 사용하여 여러 개의 질의를 한 번에 실행하는 경우, 질의 타임 아웃은 질의 하나에 대해 적용되는 것이 아니라 함수 하나에 대해 적용된다. 즉, 함수 시작 이후 타임아웃이 발생하면 함수 수행이 중단된다.

    *   **disconnect_on_query_timeout** | **disconnectOnQueryTimeout** : 질의 요청 타임아웃 시 즉시 소켓 연결 종료 여부. :c:func:`cci_prepare`, :c:func:`cci_execute` 등의 함수를 호출했을 때 **query_timeout** 으로 설정한 시간이 지나면 질의 취소 요청 후 즉시 소켓 연결을 종료할 것인지, 아니면 질의 취소 요청을 받아들인다는 서버의 응답을 기다릴 것인지를 설정한다. 기본값은 **false** 로, 서버의 응답을 기다린다. 이 값이 **true** 이면 :c:func:`cci_prepare`, :c:func:`cci_execute` 등의 함수 호출 도중 질의 타임아웃이 발생할 때 서버에 질의 취소 메시지를 보낸 후, 소켓을 닫고 **CCI_ER_QUERY_TIMEOUT** (-39) 에러를 반환한다. (브로커가 아닌 데이터베이스 서버 쪽에서 에러가 발생한 경우 -1을 반환한다. 상세 에러를 확인하고 싶으면 "데이터베이스 에러 버퍼"의 에러 코드를 확인한다. 데이터베이스 에러 버퍼에서 에러 코드를 확인하는 방법은 :ref:`CCI 에러 코드와 에러 메시지 <cci-error-codes>` 를 참고한다.) 응용 프로그램이 질의 취소 메시지를 보낸 후 에러를 반환했음에도 불구하고, 데이터베이스 서버는 그 메시지를 받지 못하고 해당 질의를 수행할 수 있음을 주의한다. **false** 이면 서버에 취소 메시지를 보낸 후, 서버의 질의 요청에 대한 응답이 올 때 까지 대기한다.

    *   **logFile**: 디버깅용 로그 파일 이름(기본값: *cci_<handle_id>.log*). <*handle_id*>는 이 함수가 반환하는 연결 핸들 ID이다.

    *   **logBaseDir**: 디버깅용 로그 파일이 생성되는 디렉터리. 경로를 포함한 파일 이름의 형식은 logBaseDir/logFile이 되며, 상대 경로로 지정할 수 있다.

    *   **logSlowQueries**: 디버깅용 슬로우 쿼리 로깅 여부(기본값: **false**)
    *   **slowQueryThresholdMillis**: 디버깅용 슬로우 쿼리 로깅 시 슬로우 쿼리 제한 시간(기본값: **60000**). 단위는 밀리 초이다.
    *   **logTraceApi**: CCI 함수 시작과 끝의 로깅 여부
    *   **logTraceNetwork**: CCI 함수 네트워크 데이터 전송 내용의 로깅 여부

    **예제** ::

        --connection URL string when a property(altHosts) is specified for HA
        URL=cci:CUBRID:192.168.0.1:33000:demodb:::?altHosts=192.168.0.2:33000,192.168.0.3:33000
         
        --connection URL string when properties(altHosts,rcTime) is specified for HA
        URL=cci:CUBRID:192.168.0.1:33000:demodb:::?altHosts=192.168.0.2:33000,192.168.0.3:33000&rcTime=600
         
        --connection URL string when properties(logSlowQueries,slowQueryThresholdMills, logTraceApi, logTraceNetwork) are specified for interface debugging
        URL = "cci:cubrid:192.168.0.1:33000:demodb:::?logSlowQueries=true&slowQueryThresholdMillis=1000&logTraceApi=true&logTraceNetwork=true"

cci_connect_with_url_ex
-----------------------

.. c:function:: int cci_connect_with_url_ex (char *url, char *db_user, char *db_password, T_CCI_ERROR * err_buf)

    **CCI_ER_DBMS** 에러를 반환하면 세부 에러 내용을 데이터베이스 에러 버퍼(*err_buf*)를 통해 확인할 수 있다는 점만 :c:func:`cci_connect_with_url`\ 과 다르고 나머지는 동일하다.

    :param err_buf: (OUT) 에러 버퍼

cci_cursor
----------

.. c:function:: int cci_cursor(int req_handle, int offset, T_CCI_CURSOR_POS origin, T_CCI_ERROR *err_buf)

    :c:func:`cci_execute`\ 로 실행한 질의 결과 내의 특정 레코드에 접근하기 위하여 요청 핸들에 설정된 커서를 이동시킨다. 인자로 지정되는 *origin* 변수 값과 *offset* 값을 통해 커서의 위치가 이동되며, 이동할 커서의 위치가 유효하지 않을 경우 **CCI_ER_NO_MORE_DATA** 를 반환한다.

    :param req_handle: (IN) 요청 핸들
    :param offset: (IN) 이동할 오프셋
    :param origin: (IN) 커서 위치를 나타내는 변수로서, 타입은 **T_CCI_CURSOR_POS** 이다. **T_CCI_CURSOR_POS** enum은 **CCI_CURSOR_FIRST**, **CCI_CURSOR_CURRENT**, **CCI_CURSOR_LAST** 의 세 가지 값으로 구성된다.
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드 (0: 성공)

        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_NO_MORE_DATA**
        *   **CCI_ER_COMMUNICATION**

    **예제**

    .. code-block:: c

        //the cursor moves to the first record
        cci_cursor(req, 1, CCI_CURSOR_FIRST, &err_buf);
         
        //the cursor moves to the next record
        cci_cursor(req, 1, CCI_CURSOR_CURRENT, &err_buf);
         
        //the cursor moves to the last record
        cci_cursor(req, 1, CCI_CURSOR_LAST, &err_buf);
         
        //the cursor moves to the previous record
        cci_cursor(req, -1, CCI_CURSOR_CURRENT, &err_buf);

cci_cursor_update
-----------------

.. c:function:: int cci_cursor_update(int req_handle, int cursor_pos, int index, T_CCI_A_TYPE a_type, void *value, T_CCI_ERROR *err_buf)

    *cursor_pos* 의 커서 위치에 대해서 *index* 번째의 칼럼 값을 *value* 값으로 update한다. 데이터베이스에 **NULL** 로 update할 경우 *value* 를 **NULL** 로 한다. update할 수 있는 조건은 :c:func:`cci_prepare`\ 를 참조한다. 
    
    :param req_handle: (IN) 요청 핸들
    :param cursor_pos: (IN) 커서 위치
    :param index: (IN) 칼럼 인덱스
    :param a_type: (IN) *value* 타입
    :param value: (IN) 새로운 값
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드 (0: 성공)

        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_TYPE_CONVERSION**
        *   **CCI_ER_ATYPE**
    
    *a_type* 에 대한 *value* 의 데이터 타입은 다음과 같다.

    +-----------------------------+-----------------------------+
    | a_type                      | value 타입                  |
    +=============================+=============================+
    | **CCI_A_TYPE_STR**          | char \*                     |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_INT**          | int \*                      |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_FLOAT**        | float \*                    |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_DOUBLE**       | double \*                   |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_BIT**          | **T_CCI_BIT** \*            |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_SET**          | **T_CCI_SET**               |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_DATE**         | **T_CCI_DATE** \*           |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_BIGINT**       | int64_t \*                  |
    |                             | (For Windows: __int64 \*)   |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_BLOB**         | **T_CCI_BLOB**              |
    +-----------------------------+-----------------------------+
    | **CCI_A_TYPE_CLOB**         | **T_CCI_CLOB**              |
    +-----------------------------+-----------------------------+

cci_datasource_borrow
---------------------

.. c:function:: T_CCI_CONN cci_datasource_borrow (T_CCI_DATASOURCE *datasource, T_CCI_ERROR *err_buf)

    **T_CCI_DATASOURCE** 구조체에서 사용할 CCI 연결을 획득한다.

    :param datasource: (IN) CCI 연결을 획득할 **T_CCI_DATASOURCE** 구조체 포인터
    :param err_buf: (OUT) 에러 버퍼 (에러가 발생하면 에러 코드와 메시지를 반환)
    :return: CCI 연결 핸들 식별자 (성공), -1 (실패)

    .. seealso:: 
    
        :c:func:`cci_property_create`,
        :c:func:`cci_property_destroy`,
        :c:func:`cci_property_get`,
        :c:func:`cci_property_set`,
        :c:func:`cci_datasource_create`,
        :c:func:`cci_datasource_destroy`,
        :c:func:`cci_datasource_release`,
        :c:func:`cci_datasource_change_property`
        
cci_datasource_change_property
------------------------------

.. c:function:: int cci_datasource_change_property (T_CCI_DATASOURCE *datasource, const char *key, const char *val)
 
    DATASOURCE의 속성(property) 이름은 *key*에 명시하고, 값을 *val*\에 설정한다. 이 함수를 사용하여 변경한 속성 값은 *datasource* 내 모든 연결에 적용된다.
     
    :param datasource: (IN) CCI 연결을 획득할 T_CCI_DATASOURCE 구조체 포인터
    :param key: (IN) 속성 이름 문자열에 대한 포인터
    :param val: (IN) 속성 값 문자열에 대한 포인터
    :return: 에러 코드(0: 성공)
    
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_NO_PROPERTY**
        *   **CCI_ER_PROPERTY_TYPE**
    
    변경 가능한 속성(property)의 이름 및 값은 다음과 같다.
 
    ========================= =========== ============================== ===========================================================================================================
    속성 이름                 타입        값                             의미
    ========================= =========== ============================== ===========================================================================================================
    default_autocommit        bool        true/false                     autocommit 여부. 기본값은 cubrid_broker.conf의 CCI_DEFAULT_AUTOCOMMIT이며, 이 값의 기본값은 ON(true)임.
    default_lock_timeout      msec        숫자                           lock timeout
    default_isolation         string      :c:func:`cci_property_set`\의  isolation level. 기본값은 cubrid.conf의 isolation_level이며, 
                                          표 참고                        이 값의 기본값은 "READ_COMMITTED"임.
    login_timeout             msec        숫자                           login timeout.  기본값은 0(무한대기)임. prepare 또는 execute 함수 호출 시 내부적으로 재접속이 
                                                                         발생할 수 있으며, 이 때에도 사용됨.
    ========================= =========== ============================== ===========================================================================================================

    **예제**

    .. code-block:: c
    
        ...
        ps = cci_property_create ();
        ...
        ds = cci_datasource_create (ps, &err);
        ...
        cci_datasource_change_property(ds, "login_timeout", "5000");
        cci_datasource_change_property(ds, "default_lock_timeout", "2000");
        cci_datasource_change_property(ds, "default_isolation", "TRAN_REP_CLASS_COMMIT_INSTANCE");
        cci_datasource_change_property(ds, "default_autocommit", "true");
        ...

    .. seealso:: 
    
        :c:func:`cci_property_create`,
        :c:func:`cci_property_destroy`,
        :c:func:`cci_property_get`,
        :c:func:`cci_property_set`,
        :c:func:`cci_datasource_create`,
        :c:func:`cci_datasource_borrow`,
        :c:func:`cci_datasource_destroy`,
        :c:func:`cci_datasource_release`
        
cci_datasource_create
---------------------

.. c:function:: T_CCI_DATASOURCE *cci_datasource_create (T_CCI_PROPERTIES *properties, T_CCI_ERROR *err_buf)

    CCI의 DATASOURCE를 생성한다.

    :param properties: (IN) 설정이 저장된 **T_CCI_PROPERTIES** 구조체 포인터. :c:func:`cci_property_set`\ 으로 속성 값들을 설정한다.
    :param err_buf: (OUT) 에러 버퍼 (에러가 발생하면 에러 코드와 메시지를 반환)
    :return: 생성된 **T_CCI_DATASOURCE** 구조체 포인터 (성공), NULL (실패)

    .. seealso:: 
    
        :c:func:`cci_property_create`,
        :c:func:`cci_property_destroy`,
        :c:func:`cci_property_get`,
        :c:func:`cci_property_set`,
        :c:func:`cci_datasource_borrow`,
        :c:func:`cci_datasource_destroy`,
        :c:func:`cci_datasource_release`,
        :c:func:`cci_datasource_change_property`

cci_datasource_destroy
----------------------

.. c:function:: void cci_datasource_destroy (T_CCI_DATASOURCE *datasource)

    CCI의 DATASOURCE를 삭제한다.

    :param datasource: (IN) 삭제할 **T_CCI_DATASOURCE** 구조체 포인터
    :return: void

    .. seealso:: 
    
        :c:func:`cci_property_create`,
        :c:func:`cci_property_destroy`,
        :c:func:`cci_property_get`,
        :c:func:`cci_property_set`,
        :c:func:`cci_datasource_create`,
        :c:func:`cci_datasource_borrow`,
        :c:func:`cci_datasource_release`,
        :c:func:`cci_datasource_change_property`

cci_datasource_release
----------------------

.. c:function:: int cci_datasource_release (T_CCI_DATASOURCE *datasource, T_CCI_CONN conn, T_CCI_ERROR *err_buf)

    **T_CCI_DATASOURCE** 구조체에 사용을 끝낸 CCI 연결을 반환한다. 연결이 연결 풀에 반환된 이후 재사용하려면 반드시 :c:func:`cci_datasource_borrow` 함수를 재호출해야 한다.

    :param datasource: (IN) CCI 연결을 반환할 **T_CCI_DATASOURCE** 구조체 포인터
    :param conn: (IN) 사용을 끝낸 CCI 연결의 핸들 식별자
    :param err_buf: (OUT) 에러 버퍼 (에러가 발생하면 에러 코드와 메시지를 반환)
    :return: 1 (성공), 0 (실패)

    .. seealso:: 
    
        :c:func:`cci_property_create`,
        :c:func:`cci_property_destroy`,
        :c:func:`cci_property_get`,
        :c:func:`cci_property_set`,
        :c:func:`cci_datasource_create`,
        :c:func:`cci_datasource_destroy`,
        :c:func:`cci_datasource_borrow`,
        :c:func:`cci_datasource_change_property`

cci_disconnect
--------------

.. c:function:: int cci_disconnect(int conn_handle, T_CCI_ERROR *err_buf)

    *conn_handle* 에 대해 생성된 모든 요청 핸들을 삭제한다. 트랜잭션이 진행 중일 경우 :c:func:`cci_end_tran`\ 을 실행한 다음 삭제된다.

    :param conn_handle: (IN) 연결 핸들
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드(0 : 성공)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**

cci_end_tran
------------

.. c:function:: int cci_end_tran(int conn_handle, char type, T_CCI_ERROR *err_buf)

    현재 진행 중인 트랜잭션에 대해서 커밋(commit)이나 롤백(rollback)을 수행한다. 이때, 열려 있는 요청 핸들은 모두 종료되고, 데이터베이스 서버와 연결이 해제된다. 단, 서버와 연결이 끊어진 후에도 해당 연결 핸들은 유효하며, 이는 :c:func:`cci_connect` 함수로 연결 핸들을 하나 할당 받은 경우와 동일한 상태다. *type* 이 **CCI_TRAN_COMMIT** 으로 지정되면 트랜잭션을 커밋하고, **CCI_TRAN_ROLLBACK** 으로 지정되면 트랜잭션을 롤백한다.

    :param conn_handle: (IN) 연결 핸들
    :param type: (IN) **CCI_TRAN_COMMIT** 또는 **CCI_TRAN_ROLLBACK**
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드(0 : 성공)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_TRAN_TYPE**

    브로커 파라미터인 :ref:`CCI_DEFAULT_AUTOCOMMIT <cci_default_autocommit>`\ 으로 응용 프로그램 시작 시 자동 커밋 모드의 기본값을 설정할 수 있으며, 브로커 파라미터 설정을 생략하면 기본값은 **ON**\ 이다. 응용 프로그램 내에서 자동 커밋 모드를 변경하려면 :c:func:`cci_set_autocommit` 함수를 이용하며, 자동 커밋 모드가 **OFF** 이면 :c:func:`cci_end_tran` 함수를 이용하여 명시적으로 트랜잭션을 커밋하거나 롤백해야 한다.

cci_escape_string
-----------------

.. c:function:: long cci_escape_string(int conn_handle, char *to, const char *from, unsigned long length, T_CCI_ERROR *err_buf)

    입력 문자열을 CUBRID 질의문에서 사용할 수 있는 문자열로 변환한다. 이 함수의 인자로 연결 핸들 또는 **no_backslash_escapes** 설정 값, 출력 문자열 포인터, 입력 문자열 포인터, 입력 문자열의 바이트 길이, 오류 정보를 담을 **T_CCI_ERROR** 구조체 변수의 주소가 지정된다.
    
    :param conn_handle: (IN) 연결 핸들 또는 **no_backslash_escapes** 설정 값. 연결 핸들이 주어지는 경우, 연결된 서버의 **no_backslash_escapes** 파라미터 설정 값을 읽어서 변환 방법을 결정한다. 연결 핸들 대신 **CCI_NO_BACKSLASH_ESCAPES_TRUE** 또는 **CCI_NO_BACKSLASH_ESCAPES_FALSE** 설정 값을 전달하여 변환 방법을 결정할 수 있다.
    :param to: (OUT) 결과 문자열
    :param from: (IN) 입력 문자열
    :param length: (IN) 입력 문자열의 최대 바이트 길이
    :param err_buf: (OUT) 에러 버퍼
    :return: 변경된 문자열의 바이트 길이(성공), 에러 코드(실패)
    
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_COMMUNICATION**

    시스템 파라미터 **no_backslash_escapes의** 값이 yes(기본값)이거나 연결 핸들 위치에 **CCI_NO_BACKSLASH_ESCAPES_TRUE** 값을 전달하는 경우, 변환되는 문자는 다음과 같다.

    *   ' (single quote) => ' + ' (escaped single quote)

    시스템 파라미터 **no_backslash_escapes의** 값이 no이거나 연결 핸들 위치에 **CCI_NO_BACKSLASH_ESCAPES_FALSE** 값을 전달하는 경우, 변환되는 문자는 다음과 같다.

    *   \\n (new line character, ASCII 10) => \\ + n (백슬래시 + 알파벳 n)
    *   \\r (carriage return, ASCII 13) => \\ + r (백슬래시 + 알파벳 r)
    *   \\0 (ASCII 0) => \\ + 0 (백슬래시 + 0(ASCII 48)
    *   \\ (백슬래시) => \\ + \\

    결과 문자열을 저장할 공간은 *length* 인자로 사용자가 직접 할당하며, 최대 입력 문자열의 바이트 길이 * 2 + 1만큼이 필요할 수 있다.

cci_execute
-----------

.. c:function:: int cci_execute(int req_handle, char flag, int max_col_size, T_CCI_ERROR *err_buf)

    :c:func:`cci_prepare`\ 를 수행한 SQL 문(prepared statement)을 실행한다. 이 함수의 인자로 요청 핸들, *flag*, fetch하는 칼럼의 문자열 최대 길이, 오류 정보를 담을 **T_CCI_ERROR** 구조체 변수의 주소가 지정된다.

    :param req_handle: (IN) prepared statement의 요청 핸들
    :param flag: (IN) exec flag ( **CCI_EXEC_QUERY_ALL** )
    :param max_col_size: (IN) 문자열 타입인 경우 fetch하는 칼럼의 문자열 최대 길이(단위: 바이트). 이 값이 0이면 전체 길이를 fetch한다.
    :param err_buf: (OUT) 에러 버퍼
    :return: 
        * **SELECT** : 결과 행의 개수를 반환
        * **INSERT**, **UPDATE** : 반영된 행의 개수
        * 기타 질의 : 0
        * 실패 : 에러 코드
      
            *   **CCI_ER_REQ_HANDLE**
            *   **CCI_ER_BIND**
            *   **CCI_ER_DBMS**
            *   **CCI_ER_COMMUNICATION**
            *   **CCI_ER_QUERY_TIMEOUT**
            *   **CCI_ER_LOGIN_TIMEOUT**

    *flag*\를 통해 질의문 수행 방식을 모두 수행하게 하거나 첫번째 질의문만 수행하도록 지정할 수 있다. 
  
    .. note::
    
        2008 R4.4와 9.2 이상 버전에서 *flag* 설정 시 비동기 방식으로 결과를 가져오게 하는 **CCI_EXEC_ASYNC**\ 를 더 이상 지원하지 않는다.
    
    *flag*\에 **CCI_EXEC_QUERY_ALL**\ 을 설정하면 prepare 시에 전달된 여러 개의 질의문(세미콜론으로 여러 개의 질의문을 구분)을 모두 수행하며, 그렇지 않은 경우 제일 앞에 있는 질의문만 수행한다. 
    
    *flag*\에 **CCI_EXEC_QUERY_ALL**\ 을 설정하면 다음의 규칙이 적용된다.

    *   리턴 값은 첫 번째 질의에 대한 결과이다.
    *   어느 하나의 질의에서 에러가 발생할 경우 execute는 실패한 것으로 처리된다.
    *   q1; q2; q3와 같이 구성된 질의에 대해서 q1을 성공하고 q2에서 에러가 발생해도 q1의 수행 결과는 유효하다. 즉, 에러가 발생했을 때, 앞서 성공한 질의 수행에 대해서 롤백하지 않는다.
    *   질의가 성공적으로 수행된 경우 두 번째 질의에 대한 결과는 :c:func:`cci_next_result`\ 를 통해서 얻을 수 있다.

    *max_col_size* 는 prepared statement의 칼럼이 **CHAR**, **VARCHAR**, **BIT**, **VARBIT** 일 경우 클라이언트로 전송되는 칼럼의 문자열 최대 길이를 결정하기 위한 값이며, 이 값이 0이면 전체 길이를 fetch한다.

cci_execute_array
-----------------

.. c:function:: int cci_execute_array(int req_handle, T_CCI_QUERY_RESULT **query_result, T_CCI_ERROR *err_buf)

    prepared statement에 하나 이상의 값이 바인딩되는 경우, 바인딩되는 변수의 값을 배열(array)로 전달받아 각각의 값을 변수에 바인딩하여 질의를 실행한다.

    :param req_handle: (IN) prepared statement의 요청 핸들
    :param query_result: (OUT) 질의 결과
    :param err_buf: (OUT) 데이터베이스 에러 버퍼
    :return: 수행된 질의의 개수(성공), 에러 코드(실패)
    
        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_BIND**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_QUERY_TIMEOUT**
        *   **CCI_ER_LOGIN_TIMEOUT**
    
    데이터를 바인딩하기 위해서는 :c:func:`cci_bind_param_array_size` 함수를 호출하여 배열의 크기를 지정한 후, :c:func:`cci_bind_param_array` 함수를 이용하여 각각의 값을 변수에 바인딩하고, :c:func:`cci_execute_array` 함수를 호출하여 질의를 실행한다. 질의 결과는 T_CCI_QUERY_RESULT 구조체의 배열에 저장된다.

    :c:func:`cci_execute_array` 함수는 *query_result* 변수에 질의 결과를 반환한다. 실행 결과에 대한 정보를 얻기 위해서는 아래와 같은 매크로를 이용할 수 있다. 매크로에서는 입력받는 각 인자에 대한 유효성 검사가 이루어지지 않으므로 주의한다. 
    
    *query_result* 변수의 사용이 끝나면 :c:func:`cci_query_result_free` 함수를 이용하여 질의 결과를 삭제해야 한다.

    +---------------------------------------+---------------------------------+-------------------------------+
    | 매크로                                | 리턴 타입                       |  의미                         |
    +=======================================+=================================+===============================+
    | :c:macro:`CCI_QUERY_RESULT_RESULT`    | int                             | 영향을 끼친 행의 개수         |
    |                                       |                                 | 또는 에러 식별자              |
    |                                       |                                 | (-1: CAS 에러, -2: DBMS 에러) |    
    +---------------------------------------+---------------------------------+-------------------------------+
    | :c:macro:`CCI_QUERY_RESULT_ERR_NO`    | int                             | 질의에 대한 에러 번호         |
    +---------------------------------------+---------------------------------+-------------------------------+
    | :c:macro:`CCI_QUERY_RESULT_ERR_MSG`   | char \*                         | 질의에 대한 에러 메시지       |
    +---------------------------------------+---------------------------------+-------------------------------+
    | :c:macro:`CCI_QUERY_RESULT_STMT_TYPE` | int(**T_CCI_CUBRID_STMT** enum) | 질의문의 타입                 |
    +---------------------------------------+---------------------------------+-------------------------------+

    자동 커밋이 ON인 경우 배열 내의 각 질의가 수행될 때마다 커밋된다.

    .. note :: 
    
        *   2008 R4.3 미만 버전에서 자동 커밋이 ON인 경우 배열 내의 모든 질의가 수행된 이후에 커밋되었으나, 2008 R4.3부터는 질의 하나가 수행될 때마다 커밋된다.
        *   자동 커밋이 OFF일 때 질의문을 일괄 처리하는 cci_execute_array 함수에서 배열 내의 질의 일부에 일반적인 오류가 발생하는 경우, 이를 건너뛰고 다음 질의를 계속 수행한다. 그러나, 교착 상태가 발생하면 트랜잭션을 롤백하고 오류 처리한다.

    .. code-block:: c

        char *query =
            "update participant set gold = ? where host_year = ? and nation_code = 'KOR'";
        int gold[2];
        char *host_year[2];
        int null_ind[2];
        T_CCI_QUERY_RESULT *result;
        int n_executed;
        ...
         
        req = cci_prepare (con, query, 0, &cci_error);
        if (req < 0)
        {
            printf ("prepare error: %d, %s\n", cci_error.err_code, cci_error.err_msg);
            goto handle_error;
        }
         
        gold[0] = 20;
        host_year[0] = "2004";
         
        gold[1] = 15;
        host_year[1] = "2008";
         
        null_ind[0] = null_ind[1] = 0;
        error = cci_bind_param_array_size (req, 2);
        if (error < 0)
        {
            printf ("bind_param_array_size error: %d\n", error);
            goto handle_error;
        }
         
        error =
            cci_bind_param_array (req, 1, CCI_A_TYPE_INT, gold, null_ind, CCI_U_TYPE_INT);
        if (error < 0)
        {
            printf ("bind_param_array error: %d\n", error);
            goto handle_error;
        }
        error =
            cci_bind_param_array (req, 2, CCI_A_TYPE_STR, host_year, null_ind, CCI_U_TYPE_INT);
        if (error < 0)
        {
            printf ("bind_param_array error: %d\n", error);
            goto handle_error;
        }
         
        n_executed = cci_execute_array (req, &result, &cci_error);
        if (n_executed < 0)
        {
            printf ("execute error: %d, %s\n", cci_error.err_code, cci_error.err_msg);
            goto handle_error;
        }
        for (i = 1; i <= n_executed; i++)
        {
            printf ("query %d\n", i);
            printf ("result count = %d\n", CCI_QUERY_RESULT_RESULT (result, i));
            printf ("error message = %s\n", CCI_QUERY_RESULT_ERR_MSG (result, i));
            printf ("statement type = %d\n",
                    CCI_QUERY_RESULT_STMT_TYPE (result, i));
        }
        error = cci_query_result_free (result, n_executed);
        if (error < 0)
        {
            printf ("query_result_free: %d\n", error);
            goto handle_error;
        }
        error = cci_end_tran(con, CCI_TRAN_COMMIT, &cci_error);
        if (error < 0)
        {
            printf ("end_tran: %d, %s\n", cci_error.err_code, cci_error.err_msg);
            goto handle_error;
        }

cci_execute_batch
-----------------

.. c:function:: int cci_execute_batch(int conn_handle, int num_sql_stmt, char **sql_stmt, T_CCI_QUERY_RESULT **query_result, T_CCI_ERROR *err_buf)

    CCI에서 **INSERT** / **UPDATE** / **DELETE** 와 같은 DML 질의를 사용하는 경우에는 여러 작업을 한 번에 처리할 수 있는데, 이러한 배치 작업을 위해서 :c:func:`cci_execute_array` 함수와 :c:func:`cci_execute_batch` 함수가 이용될 수 있다. 단, :c:func:`cci_execute_batch` 함수에서는 prepared statement를 사용할 수 없다. 질의 결과는 **T_CCI_QUERY_RESULT** 구조체의 배열에 저장된다.

    :param conn_handle: (IN) 연결 핸들
    :param num_sql_stmt: (IN) *sql_stmt* 의 개수
    :param sql_stmt: (IN) SQL 문 array
    :param query_result: (OUT) *sql_stmt* 의 결과
    :param err_buf: (OUT) 데이터베이스 에러 버퍼
    :return: 수행된 질의의 개수(성공), 에러 코드(실패)
    
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_QUERY_TIMEOUT**
        *   **CCI_ER_LOGIN_TIMEOUT**
    
    인자로 지정된 *num_sql_stmt* 개의 *sql_stmt* 를 수행하며, *query_result* 변수로 수행된 질의 개수를 반환한다. 각각의 질의에 대한 결과는 :c:macro:`CCI_QUERY_RESULT_RESULT`, :c:macro:`CCI_QUERY_RESULT_ERR_NO`, :c:macro:`CCI_QUERY_RESULT_ERR_MSG`, :c:macro:`CCI_QUERY_RESULT_STMT_TYPE`\ 매크로를 이용할 수 있다. 전체 매크로에 대한 요약 설명은 :c:func:`cci_execute_array`\ 를 참고한다.
    
    매크로에서는 입력받은 인자에 대한 유효성을 검사하지 않으므로 주의한다.

    *query_result* 변수의 사용이 끝나면 :c:func:`cci_query_result_free` 함수를 이용하여 질의 결과를 삭제해야 한다.

    자동 커밋이 ON인 경우 배열 내의 각 질의가 수행될 때마다 커밋된다.
    
    .. note :: 

        *   2008 R4.3 이전 버전에서 자동 커밋이 ON인 경우 배열 내의 모든 질의가 수행된 이후에 커밋되었으나, 2008 R4.3부터는 질의 하나가 수행될 때마다 커밋된다.
        *   자동 커밋이 OFF일 때 질의문을 일괄 처리하는 cci_execute_batch 함수에서 배열 내의 질의 일부에 일반적인 오류가 발생하는 경우, 이를 건너뛰고 다음 질의를 계속 수행한다. 그러나, 교착 상태가 발생하면 트랜잭션을 롤백하고 오류 처리한다.

    .. code-block:: c

        ...
        char **queries;
        T_CCI_QUERY_RESULT *result;
        int n_queries, n_executed;
        ...
        count = 3;
        queries = (char **) malloc (count * sizeof (char *));
        queries[0] =
            "insert into athlete(name, gender, nation_code, event) values('Ji-sung Park', 'M', 'KOR', 'Soccer')";
        queries[1] =
            "insert into athlete(name, gender, nation_code, event) values('Joo-young Park', 'M', 'KOR', 'Soccer')";
        queries[2] =
            "select * from athlete order by code desc limit 2";
            
        //calling cci_execute_batch()
        n_executed = cci_execute_batch (con, count, queries, &result, &cci_error);
        if (n_executed < 0)
        {
            printf ("execute_batch: %d, %s\n", cci_error.err_code, cci_error.err_msg);
            goto handle_error;
        }
        printf ("%d statements were executed.\n", n_executed);
        
        for (i = 1; i <= n_executed; i++)
        {
            printf ("query %d\n", i);
            printf ("result count = %d\n", CCI_QUERY_RESULT_RESULT (result, i));
            printf ("error message = %s\n", CCI_QUERY_RESULT_ERR_MSG (result, i));
            printf ("statement type = %d\n",
                    CCI_QUERY_RESULT_STMT_TYPE (result, i));
        }
        
        error = cci_query_result_free (result, n_executed);
        if (error < 0)                                                                                                                            
        {                                                                                                                                       
            printf ("query_result_free: %d\n", error);   
            goto handle_error;
        }
        ...

cci_execute_result
------------------

.. c:function:: int cci_execute_result(int req_handle, T_CCI_QUERY_RESULT **query_result, T_CCI_ERROR *err_buf)

    질의가 여러 개인 경우 수행 결과(statement type, result count)를 **T_CCI_QUERY_RESULT** 구조체의 배열에 저장한다. 각각의 질의에 대한 결과는 :c:macro:`CCI_QUERY_RESULT_RESULT`, :c:macro:`CCI_QUERY_RESULT_ERR_NO`, :c:macro:`CCI_QUERY_RESULT_ERR_MSG`, :c:macro:`CCI_QUERY_RESULT_STMT_TYPE`\ 매크로를 이용할 수 있다. 전체 매크로에 대한 요약 설명은 :c:func:`cci_execute_array`\를 참고한다.  
    
    매크로에서는 입력받은 인자에 대한 유효성을 검사하지 않으므로 주의한다.
    
    사용된 질의 결과의 메모리는 :c:func:`cci_query_result_free`\를 통해 해제되어야 한다.
    
    :param req_handle: (IN) prepared statement의 요청 핸들
    :param query_result: (OUT) 쿼리 결과
    :param err_buf: (OUT) 에러 버퍼
    :return: 수행된 질의의 개수(성공), 에러 코드(실패)

        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_COMMUNICATION**

    .. code-block:: c
    
        ...
        T_CCI_QUERY_RESULT *qr;
        ...
         
        cci_execute( ... );
        res = cci_execute_result(req_h, &qr, &err_buf);
        if (res < 0) 
        {
            /* error */
        }
        else 
        {
            for (i=1 ; i <= res ; i++) 
            {
                result_count = CCI_QUERY_RESULT_RESULT(qr, i);
                stmt_type = CCI_QUERY_RESULT_STMT_TYPE(qr, i);
            }
            cci_query_result_free(qr, res);
        }
        ...

cci_fetch
---------

.. c:function:: int cci_fetch(int req_handle, T_CCI_ERROR *err_buf)

    :c:func:`cci_execute`\ 로 실행한 질의 결과를 서버 측 CAS로부터 fetch하여 클라이언트 버퍼에 저장한다. fetch된 질의 결과에서 특정 칼럼의 데이터는 :c:func:`cci_get_data` 함수를 이용해서 확인할 수 있다.

    :param req_handle: (IN) 요청 핸들
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드(0: 성공)

        *   **CCI_ER_REQ_HANDLE**
        *   **CAS_ER_HOLDABLE_NOT_ALLOWED**
        *   **CCI_ER_NO_MORE_DATA**
        *   **CCI_ER_RESULT_SET_CLOSED**
        *   **CCI_ER_DELETED_TUPLE**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_NO_MORE_MEMORY**

cci_fetch_buffer_clear
----------------------

.. c:function:: int cci_fetch_buffer_clear(int req_handle)

    클라이언트 버퍼에 임시 저장된 레코드를 삭제한다.

    :param req_handle: (IN) 요청 핸들
    :return: 에러 코드(0: 성공)

        *   **CCI_ER_REQ_HANDLE**

cci_fetch_sensitive
-------------------

.. c:function:: int cci_fetch_sensitive(int req_handle, T_CCI_ERROR *err_buf)

    서버에서 클라이언트로 **SELECT** 질의의 결과가 전송될 때 sensitive column에 대해서 변경된 값으로 전송되도록 한다. *req_handle* 에 의한 결과가 sensitive result가 아닐 경우 :c:func:`cci_fetch`\ 와 동일하다. 리턴 값이 **CCI_ER_DELETED_TUPLE**\ 일 경우 해당 레코드는 삭제된 경우이다.

    :param req_handle: (IN) 요청 핸들
    :param err_buf: (OUT) 데이터베이스 에러 버퍼
    :return: 에러 코드 (0: 성공)

        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_NO_MORE_DATA**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_DELETED_TUPLE**

    sensitive column이란 **SELECT** 리스트 항목 중 결과 재요청 시 업데이트된 값을 제공할 수 있는 항목을 말한다. 주로 어떠한 연산 없이, 예를 들면 집계 연산과 같은 과정이 없이 칼럼을 **SELECT** 리스트의 항목으로 그대로 쓰는 경우 그 칼럼을 sensitive column이라고 말할 수 있다.

    질의 결과를 다시 fetch할 때, sensitive result는 클라이언트 버퍼에 저장된 레코드를 받지 않고, 서버로부터 변경된 값을 받는다.

cci_fetch_size
--------------

.. c:function:: int cci_fetch_size(int req_handle, int fetch_size)

    이 함수는 더 이상 사용되지 않으며(deprecated), 제거될 예정이다. 호출되더라도 무시되어 동작에 어떠한 변화도 발생하지 않는다.

cci_get_autocommit
------------------

.. c:function:: CCI_AUTOCOMMIT_MODE cci_get_autocommit(int conn_handle)

    현재 설정한 자동 커밋 모드(autocommit mode)를 반환한다.

    :param conn_handle: (IN) 연결 핸들
    :return:

        *   **CCI_AUTOCOMMIT_TRUE**: 자동 커밋 모드 ON
        *   **CCI_AUTOCOMMIT_FALSE**: 자동 커밋 모드 OFF
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_USED_CONNECTION**

cci_get_bind_num
----------------

.. c:function:: int cci_get_bind_num(int req_handle)

    입력 바인딩(input binding) 개수를 가져온다. prepare 시 사용된 SQL 문이 여러 개의 질의로 구성되어 있을 경우, 전체 질의에서 사용된 입력 바인딩 개수를 나타낸다.

    :param req_handle: (IN) prepared statement에 대한 요청 핸들
    :return: 입력 바인딩 개수

        *   **CCI_ER_REQ_HANDLE**

cci_get_cas_info
----------------

.. c:function:: int cci_get_cas_info (int conn_handle, char *info_buf, int buf_length, T_CCI_ERROR * err_buf) 
  
    conn_handle에 연결되어 있는 CAS 정보를 조회한다. info_buf에 아래와 같은 형식의 문자열이 리턴된다. 
  
    :: 
  
        <host>:<port>,<cas id>,<cas process id> 

    출력 예는 다음과 같다.
    
    ::
    
        127.0.0.1:33000,1,12916 
  
    CAS ID를 통해 해당 CAS의 SQL 로그 파일을 쉽게 확인할 수 있다. 
  
    보다 자세한 사항은 :ref:`sql-log-check`\ 을 참고한다.
         
    :param conn_handle: (IN) 연결 핸들 
    :param info_buf: (OUT) 연결 정보 버퍼 
    :param buf_length: (IN) 연결 정보 버퍼 길이 
    :param err_buf: (OUT) 에러 버퍼 
    :return: 에러 코드 
     
        * **CCI_ER_INVALID_ARGS** 
        * **CCI_ER_CON_HANDLE** 

cci_get_class_num_objs
----------------------

.. c:function:: int cci_get_class_num_objs(int conn_handle, char *class_name, int flag, int *num_objs, int *num_pages, T_CCI_ERROR *err_buf)

    *class_name* 클래스의 객체 개수와 사용하고 있는 페이지 수를 가져온다. flag가 1일 경우 대략의 값을 가져오고, 0일 경우 정확한 값을 가져온다.

    :param conn_handle: (IN) 연결 핸들
    :param class_name: (IN) 클래스 이름
    :param flag: (IN) 0 또는 1
    :param num_objs: (OUT) 객체 수
    :param num_pages: (OUT) 페이지 수
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드(0: 성공)
    
        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CONNECT**

CCI_GET_COLLECTION_DOMAIN
-------------------------

.. c:macro:: #define CCI_GET_COLLECTION_DOMAIN(u_type)

    *u_type* 이 set, multiset, sequence type인 경우 set, multiset, sequence의 domain을 가져온다. *u_type* 이 set type이 아닐 경우 리턴 값은 *u_type* 과 같다.

    :return: Type (CCI_U_TYPE)

cci_get_cur_oid
---------------

.. c:function:: int cci_get_cur_oid(int req_handle, char *oid_str_buf)

    Execute에서 **CCI_INCLUDE_OID** 가 설정된 경우 현재 fetch된 레코드의 OID를 가져온다. OID는 page, slot, volume에 의한 스트링으로 표현된다.

    :param conn_handle: (IN) 연결 핸들
    :param oid_str_buf: (OUT) OID 스트링
    :return: 에러 코드(0: 성공)

        *   **CCI_ER_REQ_HANDLE**

cci_get_data
------------

.. c:function:: int cci_get_data(int req_handle, int col_no, int type, void *value, int *indicator)

    현재 fetch된 결과에 대해서 *col_no* 번째의 값을 가져온다. 
    
    :param req_handle: (IN) 요청 핸들
    :param col_no: (IN) 칼럼 인덱스. 1부터 시작.
    :param type: (IN) *value* 변수의 데이터 타입(**T_CCI_A_TYPE** 에 정의된 타입을 사용)
    :param value: (OUT) 데이터를 저장할 변수의 주소. *type*\이 CCI_A_TYPE_STR, CCI_A_TYPE_SET, CCI_A_TYPE_BLOB 또는 CCI_A_TYPE_CLOB이고 칼럼의 값이 NULL이면 value의 값도 NULL이다.
    :param indicator: (OUT) **NULL** indicator. (-1: **NULL**)
    
        *   *type* 이 **CCI_A_TYPE_STR** 인 경우: **NULL** 이면 -1을 반환하고, **NULL** 이 아니면 *value* 에 저장된 문자열의 바이트 길이를 반환
        *   *type* 이 **CCI_A_TYPE_STR** 이 아닌 경우: **NULL** 이면 -1을 반환하고, **NULL** 이 아니면 0을 반환
     
    :return: 에러 코드(0: 성공)
    
        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_TYPE_CONVERSION**
        *   **CCI_ER_COLUMN_INDEX**
        *   **CCI_ER_ATYPE**

    주어진 *type* 인자에 따라 *value* 변수의 타입이 결정되고, 이에 따라 *value* 변수로 값 또는 포인터가 복사된다. 값을 복사하는 경우 *value* 변수로 전달되는 주소에 대한 메모리가 할당되어 있어야 한다. 포인터 복사의 경우 응용 클라이언트 라이브러리 내의 포인터를 반환하는 것이므로, 다음 :c:func:`cci_get_data` 함수 호출 시 해당 값이 유효하지 않게 되므로 주의한다.

    포인터 복사에 의해 반환된 포인터는 해제(free)하면 안 된다. 단, 타입이 **CCI_A_TYPE_SET** 인 경우 **T_CCI_SET** 타입의 set 포인터를 메모리에 할당한 후 이를 반환하므로, set 포인터를 사용한 후에는 :c:func:`cci_set_free` 함수를 이용하여 할당된 메모리를 해제해야 한다. 아래는 *type* 인자와 그에 대응하는 *value* 의 데이터 타입을 정리한 표이다.

    +---------------------+------------------------------+-------------------------------------------+
    | type                | value Type                   | Meaning                                   |
    +=====================+==============================+===========================================+
    | CCI_A_TYPE_STR      | char \*\*                    | pointer copy                              |
    +---------------------+------------------------------+-------------------------------------------+
    | CCI_A_TYPE_INT      | int \*                       | value copy                                |
    +---------------------+------------------------------+-------------------------------------------+
    | CCI_A_TYPE_FLOAT    | float \*                     | value copy                                |
    +---------------------+------------------------------+-------------------------------------------+
    | CCI_A_TYPE_DOUBLE   | double \*                    | value copy                                |
    +---------------------+------------------------------+-------------------------------------------+
    | CCI_A_TYPE_BIT      | **T_CCI_BIT** \*             | value copy (pointer copy for each member) |
    +---------------------+------------------------------+-------------------------------------------+
    | CCI_A_TYPE_SET      | **T_CCI_SET** \*             | memory allocation and value copy          |
    +---------------------+------------------------------+-------------------------------------------+
    | CCI_A_TYPE_DATE     | **T_CCI_DATE** \*            | value copy                                |
    +---------------------+------------------------------+-------------------------------------------+
    | CCI_A_TYPE_BIGINT   | int64_t \*                   | value copy                                |
    |                     | (For Windows: __int64 \*)    |                                           |
    +---------------------+------------------------------+-------------------------------------------+
    | CCI_A_TYPE_BLOB     | **T_CCI_BLOB** \*            | memory allocation and value copy          |
    +---------------------+------------------------------+-------------------------------------------+
    | CCI_A_TYPE_CLOB     | **T_CCI_CLOB** \*            | memory allocation and value copy          |
    +---------------------+------------------------------+-------------------------------------------+

    .. note::

        *   **LOB** 타입에 대해 :c:func:`cci_get_data`\ 를 호출하면 **LOB** 타입 칼럼의 메타 데이터(Locator)를 출력하며, **LOB** 타입 칼럼의 데이터를 인출하려면 :c:func:`cci_blob_read`\ 를 호출해야 한다.
    
    다음 예제는 페치한 결과 값을 :c:func:`cci_get_data`\ 를 이용하여 출력하는 코드의 일부이다.
        
    .. code-block:: c
    
        ...
        
        if ((res=cci_get_data(req, i, CCI_A_TYPE_INT, &buffer, &ind))<0) {
            printf( "%s(%d): cci_get_data fail\n", __FILE__, __LINE__);
            goto handle_error;
        }
        if (ind != -1)
                printf("%d \t|", buffer);
        else
                printf("NULL \t|");
        ...
        
cci_get_db_parameter
--------------------

.. c:function:: int cci_get_db_parameter(int conn_handle, T_CCI_DB_PARAM param_name, void *value, T_CCI_ERROR *err_buf)

    데이터베이스에 설정된 파라미터 값을 가져온다. 
    
    :param conn_handle: (IN) 연결 핸들
    :param param_name: (IN) 시스템 파라미터 이름
    :param value: (OUT) 파라미터 값
    :param err_buf: (OUT) 에러 버퍼    
    :return: 에러 코드(0: 성공)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_PARAM_NAME**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CONNECT**

    *param_name* 에 대한 *value* 의 데이터 타입은 다음과 같다.

    +---------------------------------+--------------+----------+
    | param_name                      | value 타입   | note     |
    +=================================+==============+==========+
    | **CCI_PARAM_ISOLATION_LEVEL**   | int \*       | get/set  |
    +---------------------------------+--------------+----------+
    | **CCI_PARAM_LOCK_TIMEOUT**      | int \*       | get/set  |
    +---------------------------------+--------------+----------+
    | **CCI_PARAM_MAX_STRING_LENGTH** | int \*       | get only |
    +---------------------------------+--------------+----------+
    | **CCI_PARAM_AUTO_COMMIT**       | int \*       | get only |
    +---------------------------------+--------------+----------+

    :c:func:`cci_get_db_parameter`, :c:func:`cci_set_db_parameter`\ 에서 **CCI_PARAM_LOCK_TIMEOUT** 의 입출력 단위는 밀리초이다.

    .. warning:: CUBRID 9.0 미만 버전에서 **CCI_PARAM_LOCK_TIMEOUT** 의 출력 단위는 초이므로 주의해야 한다.

    **CCI_PARAM_MAX_STRING_LENGTH** 의 단위는 바이트이며, 브로커 파라미터 **MAX_STRING_LENGTH** 에 정의된 값을 가져온다.

cci_get_db_version
------------------

.. c:function:: int cci_get_db_version(int conn_handle, char *out_buf, int out_buf_size)

    DBMS (Database Management System) 버전을 가져온다.

    :param conn_handle: (IN) 연결 핸들
    :param out_buf: (OUT) 결과 버퍼
    :param out_buf_size: (IN) *out_buf* 크기
    :return: 에러 코드(0: 성공)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CONNECT**

cci_get_err_msg
---------------

.. c:function:: int cci_get_err_msg(int err_code, char *msg_buf, int msg_buf_size)

    에러 코드에 대응되는 에러 메시지를 에러 메시지 버퍼에 저장한다. 에러 코드와 에러 메시지에 대한 내용은 :ref:`CCI 에러 코드와 에러 메시지 <cci-error-codes>` 를 참고한다.

    :param err_code: (IN) 에러 코드
    :param msg_buf: (OUT) 에러 메시지 버퍼
    :param msg_buf_size: (IN) *msg_buf* 크기
    :return: 0 (성공), -1 (실패)

    note:: CUBRID 9.1 부터는 err_buf가 있는 모든 함수에서 err_buf에 값이 저장되는 것을 보장하므로 err_buf 인자가 있는 함수에서는 cci_get_err_msg 함수를 사용할 필요가 없으며, :c:func:`cci_get_error_msg` 함수를 사용할 것을 권장한다.
    
        .. code-block:: c
        
            req = cci_prepare (con, query, 0, &err_buf);
            if (req < 0)
            {
                printf ("error: %d, %s\n", err_buf.err_code, err_buf.err_msg);
                goto handle_error;
            }
    
        9.1 미만 버전에서는 CCI_ER_DBMS 에러가 발생했을 때만 err_buf에 에러 정보가 저장되므로 CCI_ER_DBMS에 따라 다음과 같이 분기하여 처리해야 했다.
        
        .. code-block:: c
        
            req = cci_prepare (con, query, 0, &err_buf);
            if (req < 0)
            {
                if (req == CCI_ER_DBMS)
                {
                    printf ("error: %s\n", err_buf.err_msg);
                }
                else
                {
                    char msg_buf[1024];
                    cci_get_err_msg(req, msg_buf, 1024);
                    printf ("error: %s\n", msg_buf);
                }
                goto handle_error;
            }

        9.1 버전부터는 cci_get_error_msg 함수를 사용하여 위의 분기 코드를 단순화할 수 있다.
        
        .. code-block:: c

            req = cci_prepare (con, query, 0, &err_buf);
            if (req < 0)
            {
                char msg_buf[1024];
                cci_get_error_msg(req, err_buf, msg_buf, 1024);
                printf ("error: %s\n", msg_buf);
                goto handle_error;
            }

cci_get_error_msg
-----------------

.. c:function:: int cci_get_error_msg(int err_code, T_CCI_ERROR *err_buf, char *msg_buf, int msg_buf_size)

    CCI 에러 코드에 대응되는 에러 메시지를 에러 메시지 버퍼에 저장한다. CCI 에러 코드의 값이 **CCI_ER_DBMS** 이면 데이터베이스 서버에서 발생한 에러 메시지를 데이터베이스 에러 버퍼(*err_buf*)에서 전달받아 메시지 버퍼(*msg_buf*)에 저장한다.  에러 코드와 에러 메시지에 대한 내용은 :ref:`CCI 에러 코드와 에러 메시지 <cci-error-codes>`\를 참고한다.

    :param err_code: (IN) 에러 코드
    :param err_buf: (IN) 데이터베이스 에러 버퍼        
    :param msg_buf: (OUT) 에러 메시지 버퍼
    :param msg_buf_size: (IN) *msg_buf* 크기
    :return: 0 (성공), -1 (실패)

cci_get_holdability
-------------------

.. c:function:: int cci_get_holdability(int conn_handle)

    연결 핸들에서 결과 셋에 대한 커서 유지(cursor holdability) 설정 값을 리턴한다. 값이 1이면 커밋 여부에 관계 없이 연결이 종료되거나 결과 셋을 의도적으로 닫기 전까지 커서를 유지(holdable)하고, 0이면 커밋될 때 결과 셋이 닫히면서 커서를 유지하지 않는다(not holdable). 커서 유지에 대한 자세한 설명은 CUBRID SQL 설명서 > 트랜잭션과 잠금 > 커서 유지를 참고한다.

    :param conn_handle: (IN) 연결 핸들
    :return: 0 (not holdable), 1 (holdable)
    
        *   **CCI_ER_CON_HANDLE**

cci_get_last_insert_id
----------------------

.. c:function:: int cci_get_last_insert_id(int conn_handle, void *value, T_CCI_ERROR *err_buf)

    가장 마지막에 수행한 INSERT 문의 기본 키 값을 얻는다.
    
    :param conn_handle: (IN) 연결 핸들
    :param value: (OUT) 결과 버퍼 포인터의 포인터(char \*\*). 가장 마지막에 수행한 INSERT 문의 기본 키 값을 저장. 이 포인터가 가리키는 메모리는 연결 핸들 내부의 고정된 버퍼로 별도로 해제할 필요가 없다.
    :param err_buf: (OUT) 에러 버퍼

    :return: 에러 코드(0: 성공)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_USED_CONNECTION**
        *   **CCI_ER_INVALID_ARGS**

    .. code-block:: c
    
        #include <stdio.h>
        #include "cas_cci.h"

        int main ()
        {
            int con = 0;
            int req = 0;
    
            int error;
            T_CCI_ERROR cci_error;
    
    
            char *query = "insert into t1 values(NULL);";
            char *value = NULL;
    
            con = cci_connect ("localhost", 33000, "demodb", "dba", "");
    
            if (con < 0)
            {
                printf ("con error\n");
                return;
            }

            req = cci_prepare (con, query, 0, &cci_error);
            if (req < 0)
            {
                printf ("cci_prepare error: %d\n", req);
                printf ("cci_error: %d, %s\n", cci_error.err_code, cci_error.err_msg);
                return;
            }
            error = cci_execute (req, 0, 0, &cci_error);
            if (error < 0)
            {
                printf ("cci_execute error: %d\n", error);
                return;
            }

            error = cci_get_last_insert_id (con, &value, &cci_error);
            if (error < 0)
            {
                printf ("cci_get_last_insert_id error: %d\n", error);
                return;
            }

            printf ("## last insert id: %s\n", value);

            error = cci_close_req_handle (req);
            error = cci_disconnect (con, &cci_error);
            return 0;
        }

cci_get_login_timeout
---------------------

.. c:function:: int cci_get_login_timeout(int conn_handle, int *timeout, T_CCI_ERROR *err_buf)
 
    로그인 타임아웃 값을 *timeout*\ 에 반환한다.
    
    :param conn_handle: (IN) 연결 핸들
    :param timeout: (OUT) 로그인 타임아웃 값(단위: 밀리 초)에 대한 포인터
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드(0: 성공)
 
        *   **CCI_ER_INVALID_ARGS**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_USED_CONNECTION**
        
cci_get_query_plan
------------------

.. c:function:: int cci_get_query_plan(int req_handle, char **out_buf_p)

    cci_prepare 함수가 리턴한 요청 핸들(req_handle)에 대한 질의 계획을 결과 버퍼에 출력한다. 
    cci_execute 함수의 호출 여부와 상관 없이 cci_get_query_plan 함수를 호출할 수 있다.

    cci_get_query_plan 함수 호출 후 결과 버퍼의 사용이 끝나면 :c:func:`cci_query_info_free` 함수를 이용하여 cci_get_query_plan 함수에서 생성된 결과 버퍼를 해제해야 한다.

    ::
        
        char *out_buf;
        ...
        req = cci_prepare (con, query, 0, &cci_error);
        ...
        ret = cci_get_query_plan(req, &out_buf); 
        ...
        printf("plan = %s", out_buf);
        cci_query_info_free(out_buf);
        
    :param req_handle: (IN) 요청 핸들
    :param out_buf_p: (OUT) 결과 버퍼 포인터의 포인터
    :return: 에러 코드
        
        *    **CCI_ER_REQ_HANDLE**
        *    **CCI_ER_CON_HANDLE**
        *    **CCI_ER_USED_CONNECTION**
    
    .. seealso:: 
    
        :c:func:`cci_query_info_free`

cci_query_info_free
-------------------

.. c:function:: int cci_query_info_free(char *out_buf)

    cci_get_query_plan 함수에서 할당된 결과 버퍼 메모리를 해제한다.

    :param req_handle: (IN) 요청 핸들
    :param out_buf: (OUT) 결과 버퍼 포인터
    :return: 에러 코드
    
        *    **CCI_ER_NO_MORE_MEMORY**
        
    .. seealso:: 
    
        :c:func:`cci_get_query_plan`

cci_get_query_timeout
---------------------

.. c:function:: int cci_get_query_timeout (int req_handle)

    질의 수행에 대해 설정된 타임아웃 시간을 반환한다.

    :param req_handle: (IN) 요청 핸들
    :return: 현재 요청 핸들에 설정된 제한 시간(timeout) 값. 단위는 msec
    
        *   CCI_ER_REQ_HANDLE

cci_get_result_info
-------------------

.. c:function:: T_CCI_COL_INFO* cci_get_result_info(int req_handle, T_CCI_CUBRID_STMT *stmt_type, int *num)

    prepared statement가 **SELECT** 일 경우, 이 함수를 이용하여 실행 결과에 대한 칼럼 정보가 저장되어 있는 **T_CCI_COL_INFO** 구조체를 가져올 수 있다. **SELECT** 질의가 아닌 경우, **NULL**\ 을 반환하고 *num* 값은 0이 된다.

    :param req_handle: (IN) prepared statement에 대한 요청 핸들
    :param stmt_type: (OUT) command 타입
    :param num: (OUT) **SELECT** 문의 칼럼 개수(*stmt_type* 이 **CUBRID_STMT_SELECT** 일 경우)
    :return: result info 포인터 (성공), **NULL** (실패)
    
    **T_CCI_COL_INFO** 구조체에서 칼럼 정보를 가져오기 위해서 구조체에 직접 접근해도 되지만, 다음과 같이 정의된 매크로를 이용하여 정보를 가져올 수 있다. 각 매크로의 인자로 **T_CCI_COL_INFO** 구조체의 주소와 칼럼 인덱스가 지정되며, 매크로는 **SELECT** 질의에 대해서만 호출할 수 있다. 매크로에서 입력받는 각 인자에 대한 유효성 검사가 이루어지지 않으므로 주의한다. 매크로 리턴 값의 타입이 char*인 경우 메모리 포인터를 해제(free)하지 않아야 한다.

    +--------------------------------------------+------------------+----------------------+
    | 매크로                                     | 리턴 값 타입     | 의미                 |
    +============================================+==================+======================+
    | :c:macro:`CCI_GET_RESULT_INFO_TYPE`        | **T_CCI_U_TYPE** | 칼럼의 type          |
    +--------------------------------------------+------------------+----------------------+
    | :c:macro:`CCI_GET_RESULT_INFO_SCALE`       | short            | 칼럼의 scale         |
    +--------------------------------------------+------------------+----------------------+
    | :c:macro:`CCI_GET_RESULT_INFO_PRECISION`   | int              | 칼럼의 precision     |
    +--------------------------------------------+------------------+----------------------+
    | :c:macro:`CCI_GET_RESULT_INFO_NAME`        | char \*          | 칼럼의 이름          |
    +--------------------------------------------+------------------+----------------------+
    | :c:macro:`CCI_GET_RESULT_INFO_ATTR_NAME`   | char \*          | 칼럼의 속성 이름     |
    +--------------------------------------------+------------------+----------------------+
    | :c:macro:`CCI_GET_RESULT_INFO_CLASS_NAME`  | char \*          | 칼럼의 클래스 이름   |
    +--------------------------------------------+------------------+----------------------+
    | :c:macro:`CCI_GET_RESULT_INFO_IS_NON_NULL` | char (0 or 1)    | 칼럼이NULL 인지 여부 |
    +--------------------------------------------+------------------+----------------------+

    .. code-block:: c
    
        col_info = cci_get_result_info (req, &stmt_type, &col_count);
        if (col_info == NULL)
        {
          printf ("get_result_info error\n");
          goto handle_error;
        }
        
        for (i = 1; i <= col_count; i++)
        {
          printf ("%-12s = %d\n", "type", CCI_GET_RESULT_INFO_TYPE (col_info, i));
          printf ("%-12s = %d\n", "scale",
                  CCI_GET_RESULT_INFO_SCALE (col_info, i));
          printf ("%-12s = %d\n", "precision",
                  CCI_GET_RESULT_INFO_PRECISION (col_info, i));
          printf ("%-12s = %s\n", "name", CCI_GET_RESULT_INFO_NAME (col_info, i));
          printf ("%-12s = %s\n", "attr_name",
                  CCI_GET_RESULT_INFO_ATTR_NAME (col_info, i));
          printf ("%-12s = %s\n", "class_name",
                  CCI_GET_RESULT_INFO_CLASS_NAME (col_info, i));
          printf ("%-12s = %s\n", "is_non_null",
                  CCI_GET_RESULT_INFO_IS_NON_NULL (col_info,i) ? "true" : "false");
        }

CCI_GET_RESULT_INFO_ATTR_NAME
-----------------------------

.. c:macro:: #define CCI_GET_RESULT_INFO_ATTR_NAME(T_CCI_COL_INFO* res_info, int index)

    prepare된 **SELECT** 리스트에서 *index* 번째 칼럼의 실제 속성 이름을 가져오는 매크로이다. 속성 이름이 없는 경우(상수값, 함수 등)는 빈 문자열 (empty string)을 반환한다. 지정된 인자 *res_info* 가 **NULL** 인지, *index* 가 유효한지에 대한 검사는 하지 않는다. 반환된 메모리 포인터는 사용자가 **free**\ ()를 통해 제거할 수 없다.

    :param res_info: (IN) :c:func:`cci_get_result_info` 에 의한 칼럼 정보 포인터
    :param index: (IN) 칼럼 인덱스
    :return: 속성 이름 (char \*)

CCI_GET_RESULT_INFO_CLASS_NAME
------------------------------

.. c:macro:: #define CCI_GET_RESULT_INFO_CLASS_NAME(T_CCI_COL_INFO* res_info, int index)

    prepare된 **SELECT** 리스트에서 *index* 번째 칼럼의 클래스 이름을 가져오는 매크로이다. 지정된 인자 *res_info* 가 **NULL** 인지, *index* 가 유효한지에 대한 검사는 하지 않는다. 반환된 메모리 포인터는 사용자가 **free**\ ()를 통해 제거할 수 없다. 반환된 값은 **NULL**\ 을 가질 수 있다.

    :param res_info: (IN) :c:func:`cci_get_result_info`\ 에 의한 칼럼 정보 포인터
    :param index: (IN) 칼럼 인덱스
    :return: 클래스 이름 (char \*)

CCI_GET_RESULT_INFO_IS_NON_NULL
-------------------------------

.. c:macro:: #define CCI_GET_RESULT_INFO_IS_NON_NULL(T_CCI_COL_INFO* res_info, int index)

    prepare된 **SELECT** 리스트에서 *index* 번째 칼럼이 nullable인지에 대한 값을 가져오는 매크로이다. 지정된 인자 *res_info* 가 **NULL** 인지, *index* 가 유효한지에 대한 검사는 하지 않는다. 
   
    **SELECT** 리스트의 칼럼이 테이블의 칼럼이 아닌 표현식인 경우 NON_NULL 여부를 알 수 없으므로 CCI_GET_RESULT_INFO_IS_NON_NULL 매크로는 일관되게 0을 반환한다.

    :param res_info: (IN) :c:func:`cci_get_result_info`\ 에 의한 칼럼 정보 포인터
    :param index: (IN) 칼럼 인덱스
    :return: 0 : nullable, 1 : non **NULL**

CCI_GET_RESULT_INFO_NAME
------------------------

.. c:macro:: #define CCI_GET_RESULT_INFO_NAME(T_CCI_COL_INFO* res_info, int index)

    prepare된 **SELECT** 리스트에서 *index* 번째 칼럼의 이름을 가져오는 매크로이다. 지정된 인자 *res_info* 가 **NULL** 인지, *index* 가 유효한지에 대한 검사는 하지 않는다. 반환된 메모리 포인터는 사용자가 **free**\ ()를 통해 제거할 수 없다.

    :param res_info: (IN) :c:func:`cci_get_result_info`\ 에 의한 칼럼 정보 포인터
    :param index: (IN) 칼럼 인덱스
    :return: 칼럼 이름 (char \*)

CCI_GET_RESULT_INFO_PRECISION
-----------------------------

.. c:macro:: #define CCI_GET_RESULT_INFO_PRECISION(T_CCI_COL_INFO* res_info, int index)

    prepare된 **SELECT** 리스트에서 *index* 번째 칼럼의 precision을 가져오는 매크로이다. 지정된 인자 *res_info* 가 **NULL** 인지, *index* 가 유효한지에 대한 검사는 하지 않는다.

    :param res_info: (IN) :c:func:`cci_get_result_info`\ 에 의한 칼럼 정보 포인터
    :param index: (IN) 칼럼 인덱스
    :return: precision (int)

CCI_GET_RESULT_INFO_SCALE
-------------------------

.. c:macro:: #define CCI_GET_RESULT_INFO_SCALE(T_CCI_COL_INFO* res_info, int index)

    prepare된 **SELECT** 리스트에서 *index* 번째 칼럼의 scale을 가져오는 매크로이다. 지정된 인자 *res_info* 가 **NULL** 인지, *index* 가 유효한지에 대한 검사는 하지 않는다.

    :param res_info: (IN) :c:func:`cci_get_result_info`\ 에 의한 칼럼 정보 포인터
    :param index: (IN) 칼럼 인덱스
    :return: scale (int)

CCI_GET_RESULT_INFO_TYPE
------------------------

.. c:macro:: #define CCI_GET_RESULT_INFO_TYPE(T_CCI_COL_INFO* res_info, int index)

    prepare된 **SELECT** 리스트에서 *index* 번째 칼럼의 타입을 가져오는 매크로이다. 지정된 인자 *res_info* 가 **NULL** 인지, *index* 가 유효한지에 대한 검사는 하지 않는다.  
    
    어떤 칼럼의 SET 타입 여부를 확인하려면 :c:macro:`CCI_IS_SET_TYPE`\ 을 사용한다.

    :param res_info: (IN) :c:func:`cci_get_result_info`\ 에 의한 칼럼 정보 포인터
    :param index: (IN) 칼럼 인덱스
    :return: 칼럼 타입 (**T_CCI_U_TYPE**)

CCI_IS_SET_TYPE
---------------

.. c:macro:: #define CCI_IS_SET_TYPE(u_type)

    *u_type*\ 이 set type인지를 검사한다.

    :param u_type: (IN)
    :return: 1 : set, 0 : not set

CCI_IS_MULTISET_TYPE
--------------------
.. c:macro:: #define CCI_IS_MULTISET_TYPE(u_type)

    *u_type*\ 이 multiset type인지를 검사한다.

    :param u_type: (IN)
    :return: 1 : multiset, 0 : not multiset

CCI_IS_SEQUENCE_TYPE
--------------------

.. c:macro:: #define CCI_IS_SEQUENCE_TYPE(u_type)

    *u_type*\ 이 sequence type인지를 검사한다.

    :param u_type: (IN)
    :return: 1 : sequence, 0 : not sequence

CCI_IS_COLLECTION_TYPE
----------------------

.. c:macro:: #define CCI_IS_COLLECTION_TYPE(u_type)

    *u_type*\ 이 collection (set, multiset, sequence) type인지를 검사한다.

    :param u_type: (IN)
    :return: 1 : collection (set, multiset, sequence), 0 : not collection

cci_get_version
---------------

.. c:function:: int cci_get_version(int *major, int *minor, int *patch)

    CCI 라이브러리의 버전을 가져온다. 예를 들어 버전 스트링이 "9.2.0.0001" 인 경우, major 버전은 9, minor 버전은 2, patch 버전은 0이 된다.

    :param major: (OUT) major 버전
    :param minor: (OUT) minor 버전
    :param patch: (OUT) patch 버전
    :return: 항상 0(성공)

    .. note::

        Linux용 CUBRID에서는 **strings** 명령을 이용하여 CCI 라이브러리 파일의 버전을 확인할 수 있다 ::
        
            $ strings /home/usr1/CUBRID/lib/libcascci.so | grep VERSION
            VERSION=9.2.0.0001

cci_init
--------

.. c:function::  void cci_init()

    Windows용 CCI 응용 프로그램을 static linking library(.lib)로 컴파일하는 경우에는 반드시 호출해야 하고, 그 이외의 경우는 이 함수를 사용할 필요가 없다.

cci_is_holdable
---------------

.. c:function:: int cci_is_holdable(int req_handle)

    **cci_is_holdable** 함수는 요청 핸들의 연결 유지(holdable) 가능 여부를 리턴한다.

    :param req_handle: (IN) prepared statement에 대한 요청 핸들
    :return: 
    
        *   1: 연결 유지
        *   0: 연결 유지 안 됨
        *   **CCI_ER_REQ_HANDLE**

    .. seealso:: 
    
        :c:func:`cci_prepare`

cci_is_updatable
----------------

.. c:function:: int cci_is_updatable(int req_handle)

    :c:func:`cci_prepare`\ 를 수행한 SQL 문이 업데이트 가능한 결과 셋을 만들 수 있는 질의인지(:c:func:`cci_prepare` 수행 시 *flag* 에 **CCI_PREPARE_UPDATABLE** 이 설정되었는지) 확인하는 함수이다. 업데이트 가능한 질의이면 1을 반환한다.

    :param req_handle: (IN) prepared statement에 대한 요청 핸들
    :return: 
    
        *   1: 업데이트 가능
        *   0: 업데이트 불가능
        *   **CCI_ER_REQ_HANDLE**

cci_next_result
---------------

.. c:function:: int cci_next_result(int req_handle, T_CCI_ERROR *err_buf)

    :c:func:`cci_execute` 수행 시 **CCI_EXEC_QUERY_ALL** *flag*\ 가 설정되면 여러 개의 질의를 순서대로 수행할 수 있게 된다. 이때, 첫 번째 질의 결과는 :c:func:`cci_execute` 함수를 호출한 뒤에 가져오고, 두 번째 질의부터는 **cci_next_result** 함수를 호출할 때마다 질의를 작성한 순서대로 결과를 가져온다. 

    이때 얻은 질의 결과에 대한 칼럼 정보는 :c:func:`cci_execute` 함수 또는 **cci_next_result** 함수를 호출할 때마다 :c:func:`cci_get_result_info` 함수를 호출하면 된다.

    즉, Q1, Q2, Q3 질의 여러 개를 한 번의 :c:func:`cci_prepare` 호출을 통해 수행할 경우 Q1의 결과는 :c:func:`cci_execute` 함수를 호출할 때 가져오며, Q2, Q3의 결과는 **cci_next_result** 함수를 각각 호출하여 가져온다. i
    Q1, Q2, Q3의 질의 결과에 대한 칼럼 정보는 매번 :c:func:`cci_get_result_info` 함수를 호출하여 가져온다.
    
    .. code-block:: c
    
        sql = "SELECT * FROM athlete; SELECT * FROM nation; SELECT * FROM game";
        req = cci_prepare (con, sql, 0, &error);
        ...
        ret = cci_execute (req, CCI_EXEC_QUERY_ALL, 0, &error);
        res_col_info = cci_get_result_info (req, &cmd_type, &col_count);
        ...
        ret = cci_next_result (req, &error);
        res_col_info = cci_get_result_info (req, &cmd_type, &col_count);
        ...
        ret = cci_next_result (req, &error);
        res_col_info = cci_get_result_info (req, &cmd_type, &col_count);

    :param req_handle: (IN) prepared statement에 대한 요청 핸들
    :param err_buf: (OUT) 에러 버퍼        
    :return: 

        *   **SELECT** : 결과 개수
        *   **INSERT**, **UPDATE** : 반영된 레코드 개수
        *   기타 : 0
        *   실패 : 에러 코드
    
            *   **CCI_ER_REQ_HANDLE**
            *   **CCI_ER_DBMS**
            *   **CCI_ER_COMMUNICATION**

    에러 코드가 **CAS_ER_NO_MORE_RESULT_SET** 일 경우 더 이상의 결과 셋이 존재하지 않는다는 것을 의미한다.

cci_oid
-------

.. c:function:: int cci_oid(int conn_handle, T_CCI_OID_CMD cmd, char *oid_str, T_CCI_ERROR *err_buf)

    `cmd` 인자의 값에 따라 다음 동작을 수행한다.
    
    *   CCI_OID_DROP : 해당 oid를 삭제한다.
    *   CCI_OID_IS_INSTANCE : 해당 oid가 instance oid인지를 검사한다.
    *   CCI_OID_LOCK_READ : 해당 oid에 대해 read lock 을 설정한다.
    *   CCI_OID_LOCK_WRITE : 해당 oid에 대해 write lock을 설정한다.

    :param conn_handle: (IN) 연결 핸들
    :param cmd: (IN) CCI_OID_DROP, CCI_OID_IS_INSTANCE, CCI_OID_LOCK_READ, CCI_OID_LOCK_WRITE
    :param oid_str:  (IN) oid    
    :param err_buf: (OUT) 에러 버퍼        
    :return: 

        *   `cmd`\가 CCI_OID_IS_INSTANCE인 경우
        
            *   0 : 인스턴스 아님
            *   1 : 인스턴스
            *   < 0 : 에러
        
        *   `cmd`\가 CCI_OID_DROP, CCI_OID_LOCK_READ 또는 CCI_OID_LOCK_WRITE인 경우
      
            에러 코드(0: 성공)
        
            *   **CCI_ER_CON_HANDLE**
            *   **CCI_ER_CONNECT**
            *   **CCI_ER_OID_CMD**
            *   **CCI_ER_OBJECT**
            *   **CCI_ER_DBMS**

cci_oid_get
-----------

.. c:function:: int cci_oid_get(int conn_handle, char *oid_str, char **attr_name, T_CCI_ERROR *err_buf)

    해당 oid의 속성 값을 가져온다. *attr_name* 은 속성의 array로서 마지막은 반드시 NULL로 끝나야 한다. *attr_name* 이 NULL인 경우 모든 속성에 대한 정보를 가져온다. Request handle의 형태는 "SELECT attr_name FROM oid_class WHERE oid_class = oid"의 SQL문을 실행했을 때와 동일한 형태이다.

    :param conn_handle: (IN) 연결 핸들
    :param oid_str: (IN) oid    
    :param attr_name: (IN) 속성 목록    
    :param err_buf: (OUT) 에러 버퍼        
    :return: 성공 : 요청 핸들, 실패 : 에러 코드

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_CONNECT**

cci_oid_get_class_name
----------------------

.. c:function:: int cci_oid_get_class_name(int conn_handle, char *oid_str, char *out_buf, int out_buf_len, T_CCI_ERROR *err_buf)

    해당 oid의 클래스 이름을 가져온다.

    :param conn_handle: (IN) 연결 핸들
    :param oid_str: (IN) oid    
    :param out_buf: (OUT) out 버퍼
    :param out_buf_len: (IN) *out_buf* 길이
    :param err_buf: (OUT) 에러 버퍼        
    :return: 에러 코드

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_OBJECT**
        *   **CCI_ER_DBMS**

cci_oid_put
-----------

.. c:function:: int cci_oid_put(int conn_handle, char *oid_str, char **attr_name, char **new_val_str, T_CCI_ERROR *err_buf)

    해당 oid의 *attr_name* 속성 값을 *new_val_str* 으로 설정한다. *attr_name* 의 마지막은 반드시 NULL이어야 한다. 모든 타입의 값은 string으로 표현해야 하고, string으로 표현된 값은 서버에서 속성의 타입에 따라 변환되어 데이터베이스에 반영된다. NULL 값을 넣기 위해서는 *new_val_str* [i]의 값을 NULL로 한다.

    :param conn_handle: (IN) 연결 핸들
    :param oid_str: (IN) oid    
    :param attr_name: (IN) 속성 이름 목록
    :param new_val_str: (IN) 새 값의 목록
    :param err_buf: (OUT) 에러 버퍼        
    :return: 에러 코드(0: 성공)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**

cci_oid_put2
------------

.. c:function:: int cci_oid_put2(int conn_handle, char *oidstr, char **attr_name, void **new_val, int *a_type, T_CCI_ERROR *err_buf)

    해당 oid의 *attr_name* 속성 값을 *new_val* 로 설정한다. *attr_name* 의 마지막은 반드시 NULL이어야 한다. NULL 값을 넣기 위해서는 *new_val* [i]의 값을 NULL로 지정한다.

    :param conn_handle: (IN) 연결 핸들
    :param oidstr: (IN) oid    
    :param attr_name: (IN) 속성 이름 목록
    :param new_val: (IN) 새 값 배열
    :param a_type: (IN) *new_val* 타입 배열
    :param err_buf: (OUT) 에러 버퍼        
    :return: 에러 코드(0: 성공)
    
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
    
    *a_type* 에 대한 *new_val* [i]의 타입은 다음 표와 같다.

    **a_type에 대한 new_val[i]의 타입**

    +-----------------------+------------------------------+
    | Type                  | value type                   |
    +=======================+==============================+
    | **CCI_A_TYPE_STR**    | char \*\*                    |
    +-----------------------+------------------------------+
    | **CCI_A_TYPE_INT**    | int \*                       |
    +-----------------------+------------------------------+
    | **CCI_A_TYPE_FLOAT**  | float \*                     |
    +-----------------------+------------------------------+
    | **CCI_A_TYPE_DOUBLE** | double \*                    |
    +-----------------------+------------------------------+
    | **CCI_A_TYPE_BIT**    | **T_CCI_BIT** \*             |
    +-----------------------+------------------------------+
    | **CCI_A_TYPE_SET**    | **T_CCI_SET** \*             |
    +-----------------------+------------------------------+
    | **CCI_A_TYPE_DATE**   | **T_CCI_DATE** \*            |
    +-----------------------+------------------------------+
    | **CCI_A_TYPE_BIGINT** | int64_t \*                   |
    |                       | (Windows는 __int64 \*)       |
    +-----------------------+------------------------------+

    .. code-block:: c

        char *attr_name[array_size]
        void *attr_val[array_size]
        int a_type[array_size]
        int int_val

        ...

        attr_name[0] = "attr_name0"
        attr_val[0] = &int_val
        a_type[0] = CCI_A_TYPE_INT
        attr_name[1] = "attr_name1"
        attr_val[1] = "attr_val1"
        a_type[1] = CCI_A_TYPE_STR

        ...
        attr_name[num_attr] = NULL

        res = cci_put2(con_h, oid_str, attr_name, attr_val, a_type, &error)

cci_prepare
-----------

.. c:function:: int cci_prepare(int conn_handle, char *sql_stmt, char flag,T_CCI_ERROR *err_buf)

    SQL 문에 관한 요청 핸들을 획득하여 SQL 실행을 준비한다. 단, SQL 문이 여러 개의 질의로 구성된 경우, 첫 번째 질의에 대해서만 실행을 준비한다. 이 함수의 인자로 연결 핸들, SQL문,    *flag*    , 오류 정보를 저장할 **T_CCI_ERROR** 구조체 변수의 주소가 지정된다.

    :param conn_handle: (IN) 연결 핸들
    :param sql_stmt: (IN) SQL 문
    :param flag: (IN) prepare flag (CCI_PREPARE_UPDATABLE, CCI_PREPARE_INCLUDE_OID, CCI_PREPARE_HOLDABLE 또는 CCI_PREPARE_CALL)
    :param err_buf: (OUT) 에러 버퍼        
    :return: 성공 : 요청 핸들 ID , 실패 : 에러 코드
    
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_STR_PARAM**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_QUERY_TIMEOUT**
        *   **CCI_ER_LOGIN_TIMEOUT**
    
    *flag* 에 **CCI_PREPARE_UPDATABLE**, **CCI_PREPARE_INCLUDE_OID**, **CCI_PREPARE_HOLDABLE** 또는 **CCI_PREPARE_CALL** 이 설정될 수 있다. *flag*\에 **CCI_PREPARE_UPDATABLE**\이 설정되면 갱신 가능한 결과 셋(updatable resultset)을 만들 수 있으며, 이 경우 **CCI_PREPARE_INCLUDE_OID**\는 자동 설정된다. *flag*\에 **CCI_PREPARE_UPDATABLE**\과 **CCI_PREPARE_HOLDABLE**\을 동시에 사용할 수 없다.
    자바 저장 함수를 호출하고 싶으면 **flag**\에 **CCI_PREPARE_CALL**\을 설정한다. 자바 저장 함수와 관련된 예는 :c:func:`cci_register_out_param` 함수를 참고한다.

    커밋 이후 결과 셋 유지 여부에 대한 설정의 기본값은 커서 유지이다. 따라서 :c:func:`cci_prepare`\의 *flag*\에 **CCI_PREPARE_UPDATABLE**\을 설정하고 싶으면 :c:func:`cci_prepare`\를 호출하기 전에 :c:func:`cci_set_holdability`\을 호출하여 커서를 유지하지 않도록 설정해야 한다.

    **CCI_PREPARE_UPDATABLE** 이 설정되더라도 모든 질의에 대해 갱신 가능한 결과를 만들 수 있는 것은 아니므로 SQL 문을 prepare한 후 :c:func:`cci_is_updatable` 함수를 이용하여 갱신 가능한 결과를 만들 수 있는 질의인지 확인해야 한다. 결과 셋을 갱신하려면 :c:func:`cci_oid_put` 함수 또는 :c:func:`cci_oid_put2` 함수를 사용할 수 있다.

    갱신 가능한 질의의 조건은 다음과 같다.

    *   **SELECT** 질의여야 한다.
    *   질의 결과에 OID가 포함될 수 있는 질의여야 한다.
    *   갱신하고자 하는 칼럼이 **FROM** 절에 명시한 테이블에 속한 칼럼이어야 한다.

    커밋 이후 결과 셋 유지에 대한 설정값이 기본값인 커서 유지이거나 **CCI_PREPARE_HOLDABLE** 이 설정된 채로 prepare되었으면 해당 문장(statement)에 대해 결과 셋을 닫거나 연결을 종료하지 않는 한 커밋 이후에도 커서가 유지된다(:ref:`cursor-holding` 참고).

cci_prepare_and_execute
-----------------------

.. c:function:: int cci_prepare_and_execute(int conn_handle, char *sql_stmt, int max_col_size, int *exec_retval, T_CCI_ERROR *err_buf)

    SQL 문을 즉시 실행하고 SQL 문에 대한 요청 핸들을 반환한다. 이 함수의 인자로는 연결 핸들, SQL 문, fetch하는 칼럼의 문자열 최대 길이, 에러 코드, 오류 정보를 저장할 **T_CCI_ERROR** 구조체 변수의 주소가 지정된다. *max_col_size* 는 SQL 문의 칼럼이 **CHAR**, **VARCHAR**, **BIT**, **VARBIT** 일 경우 클라이언트로 전송되는 칼럼의 문자열 최대 길이를 설정하기 위한 값이며, 이 값이 0이면 전체 길이를 fetch한다.

    :param conn_handle: (IN) 연결 핸들
    :param sql_stmt: (IN) SQL 문
    :param max_col_size: (IN) 문자열 타입인 경우 fetch하는 칼럼의 문자열 최대 길이(단위: 바이트). 이 값이 0이면 전체 길이를 fetch한다.
    :param exec_retval: (OUT) 성공: 영향을 받은 행의 개수, 실패: 에러 코드
    :param err_buf: (OUT) 에러 버퍼        
    :return: 성공 : 요청 핸들 ID, 실패 : 에러 코드

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_STR_PARAM**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_QUERY_TIMEOUT**

cci_property_create
-------------------

.. c:function:: T_CCI_PROPERTIES * cci_property_create ()

    CCI의 DATASOURCE를 설정하기 위한 **T_CCI_PROPERTIES** 구조체를 생성한다.

    :return: 성공: 메모리가 할당된 **T_CCI_PROPERTIES** 구조체 포인터, 실패 : **NULL**

    .. seealso:: 
    
        :c:func:`cci_property_create`,
        :c:func:`cci_property_destroy`,
        :c:func:`cci_property_get`,
        :c:func:`cci_property_set`,
        :c:func:`cci_datasource_create`,
        :c:func:`cci_datasource_destroy`,
        :c:func:`cci_datasource_release`

cci_property_destroy
--------------------

.. c:function:: void cci_property_destroy (T_CCI_PROPERTIES * properties)

    **T_CCI_PROPERTIES** 구조체를 삭제한다.

    :param properties: 삭제할 **T_CCI_PROPERTIES** 구조체 포인터

    .. seealso:: 
    
        :c:func:`cci_property_create`,
        :c:func:`cci_property_destroy`,
        :c:func:`cci_property_get`,
        :c:func:`cci_property_set`,
        :c:func:`cci_datasource_create`,
        :c:func:`cci_datasource_destroy`,
        :c:func:`cci_datasource_release`

cci_property_get
----------------

.. c:function:: char * cci_property_get (T_CCI_PROPERTIES * properties, char *key)

    **T_CCI_PROPERTIES** 구조체에 설정된 속성값을 조회한다.

    :param properties: *key* 에 대응하는 value를 가져올 **T_CCI_PROPERTIES** 구조체 포인터
    :param key: 조회할 속성 이름(설정할 수 있는 속성의 이름과 의미는 :c:func:`cci_property_set` 함수 참고)
    :return: 성공: *key* 에 대응하는 value 문자열의 포인터, 실패: NULL

    .. seealso:: 
    
        :c:func:`cci_property_create`,
        :c:func:`cci_property_destroy`,
        :c:func:`cci_property_get`,
        :c:func:`cci_property_set`,
        :c:func:`cci_datasource_create`,
        :c:func:`cci_datasource_destroy`,
        :c:func:`cci_datasource_release`

cci_property_set
----------------

.. c:function:: int cci_property_set (T_CCI_PROPERTIES *properties, char *key, char *value)

    **T_CCI_PROPERTIES** 구조체에 속성 값을 설정한다. 
    
    :param properties: *key* 와 *value* 를 저장할 **T_CCI_PROPERTIES** 구조체 포인터
    :param key: 속성 이름의 문자열 포인터
    :param value: 속성 값의 문자열 포인터
    :return: 성공: 1, 실패: 0
    
    구조체에 설정할 수 있는 속성의 이름 및 의미는 다음과 같다.
 
    ============================= =========== =============================================== ==========================================================================================================
    속성 이름                     타입        기본값                                          의미
    ============================= =========== =============================================== ==========================================================================================================
    user                          string                                                      DB 사용자 이름
    password                      string                                                      DB 사용자 암호
    url                           string                                                      연결 URL. 연결 URL 문자열을 정의하는 방법은 :c:func:`cci_connect_with_url` 참고
    pool_size                     int         10                                              연결 풀이 가질 수 있는 최대 연결 개수
    max_pool_size                 int         pool_size                                       최초 datasource 생성 시 생성할 전체 연결 개수. 최대값은 INT_MAX
    max_wait                      msec        1000                                            연결을 가져오기 위해 대기하는 최대 시간
    pool_prepared_statement       bool        false                                           statement 풀링 가능 여부. true 또는 false
    max_open_prepared_statement   int         1000                                            statement pool에 유지할 prepared statement의 최대 개수
    login_timeout                 msec        0(무제한)                                       :c:func:`cci_datasource_create`\ 함수로 datasource를 생성하거나 prepare/execute 함수에서  
                                                                                              내부적인 재접속이 발생할 때마다 적용되는 로그인 타임아웃 시간
    query_timeout                 msec        0(무제한)                                       질의 타임아웃 시간
    disconnect_on_query_timeout   bool        no                                              질의 실행이 타임아웃 시간을 초과하여 실행이 취소될 때 연결의 종료 여부. yes 또는 no
    default_autocommit            bool        cubrid_broker.conf의 CCI_DEFAULT_AUTOCOMMIT 값  :c:func:`cci_datasource_create`\ 함수로 datasource를 생성할 때 설정되는 자동 커밋 모드. 
                                                                                              true 또는 false 
    default_isolation             string      cubrid.conf의 isolation_level 값                :c:func:`cci_datasource_create`\ 함수로 datasource를 생성할 때 설정되는 트랜잭션 격리 수준. 
                                                                                              아래 표 참고
    default_lock_timeout          msec        cubrid.conf의 lock_timeout 값                   :c:func:`cci_datasource_create`\ 함수로 datasource를 생성할 때 설정되는 잠금 타임아웃
    ============================= =========== =============================================== ==========================================================================================================
    
    prepared statement의 개수가 **max_open_prepared_statement** 값을 초과하면 가장 오래된 prepared statement가 statement pool에서 해제되며, 추후 해제된 prepared statement가 재사용되면 다시 statement pool에 추가된다. 
    
    하나의 연결 풀이 가질 수 있는 연결 개수는 필요 시 :c:func:`cci_datasource_change_property`\ 를 통해 값을 변경할 수 있지만, max_pool_size를 초과할 수 없다. 연결 개수의 제한을 조절하고자 할 때 이 값을 변경할 수 있다. 예를 들어 평소에는 max_pool_size보다 작게 설정해서 사용하다가 기대 이상으로 많은 연결이 필요해질 때 값을 높이고, 많은 연결이 필요하지 않으면 값을 다시 줄인다.
    
    연결 풀이 가질 수 있는 연결 개수는 pool_size까지 제한되는데, pool_size는 최대 max_pool_size까지 변경될 수 있다.
    
    **login_timeout**, **default_autocommit**, **default_isolation**, **default_lock_timeout**\ 의 값을 설정하면 :c:func:`cci_datasource_borrow`\ 를 호출할 때 이 설정 값을 가지고 연결을 반환한다.
 
    **login_timeout**, **default_autocommit**, **default_isolation**, **default_lock_timeout**\ 은 :c:func:`cci_datasource_borrow` 함수 호출 이후에도 변경할 수 있다. :c:func:`cci_set_login_timeout`, :c:func:`cci_set_autocommit`, :c:func:`cci_set_isolation_level`, :c:func:`cci_set_lock_timeout`\ 을 참고한다. 이름이 **cci_set_**\ 으로 시작하는 함수들에 의해 변경된 값은 변경한 연결 객체에만 적용되며, 연결이 해제된 이후에는 :c:func:`cci_property_set`\ 으로 설정한 값으로 복원된다. :c:func:`cci_property_set`\ 으로 설정한 값이 없을 경우에는 기본값으로 복원된다.
    
    .. note:: 
    
        *   :c:func:`cci_property_set` 함수와 URL 문자열에서 동시에 속성 값을 명시하는 경우, :c:func:`cci_property_set` 함수에서 정의한 값으로 적용된다.
        
        *   **login_timeout**\은 DATASOURCE 객체를 생성하는 경우와 :c:func:`cci_prepare` 또는 :c:func:`cci_execute` 함수에서 내부적인 재접속이 발생하는 경우에 적용된다. 내부적인 재접속은 DATASOURCE에서 연결 객체를 얻은 경우와 :c:func:`cci_connect`/:c:func:`cci_connect_with_url` 함수로 연결 객체를 얻은 경우에서 모두 발생한다.
        
        *   DATASOURCE 객체를 생성하는 시간은 내부적인 재접속 시간보다 오래 걸릴 수 있으므로, 두 가지 경우의 **login_timeout**\ 을 다르게 적용하는 것을 감안할 수 있다. 예를 들어, 전자의 경우를 5000(5초), 후자의 경우를 2000(2초)로 설정하고 싶다면, 후자의 설정을 위해 DATASOURCE 생성 후 :c:func:`cci_set_login_timeout` 함수를 사용한다.
        
        .. CUBRIDSUS-12567 참고

    **default_isolation**\ 은 다음 값 중 하나의 설정값을 가지며, 격리 수준에 대한 자세한 내용은 :ref:`set-transaction-isolation-level`\ 을 참조한다.

    +----------------------------+---------------------------------------+
    | isolation_level            | Configuration Value                   |
    +============================+=======================================+
    | SERIALIZABLE               | "TRAN_SERIALIZABLE"                   |
    +----------------------------+---------------------------------------+
    | REPEATABLE READ            | "TRAN_REP_READ"                       |
    +----------------------------+---------------------------------------+
    | READ COMMITTED             | "TRAN_READ_COMMITTED"                 |
    +----------------------------+---------------------------------------+

    DB 사용자 이름과 암호는 **user** 와 **password** 값을 직접 지정하거나 **url** 속성 내에서 **user** 와 **password** 값을 지정한다. 두 가지 경우가 같이 사용되는 경우 우선 순위는 다음과 같다.

    The following shows how to work as the first priority if both are specified.

    *   둘 다 입력되면 직접 지정하는 값이 우선한다. 
    *   둘 중 하나가 NULL이면 NULL이 아닌 값이 사용된다. 
    *   둘 다 NULL이면 NULL 값으로 사용된다. 
    *   DB 사용자를 직접 지정한 값이 NULL이면 "public", 암호를 직접 지정한 값이 NULL이면 NULL로 설정된다.
    *   암호를 직접 지정한 값이 NULL이면 URL의 설정을 따른다. 
    
    다음 예제에서 DB 사용자 이름은 "dba", 암호는 "cubridpwd" 가 된다.
    
    ::
    
        cci_property_set(ps, "user", "dba");
        cci_property_set(ps, "password", "cubridpwd");
          ...
        cci_property_set(ps, "url", "cci:cubrid:192.168.0.1:33000:demodb:dba:mypwd:?logSlowQueries=true&slowQueryThresholdMillis=1000&logTraceApi=true&logTraceNetwork=true");
        
    .. seealso:: 
    
        :c:func:`cci_property_create`,
        :c:func:`cci_property_destroy`,
        :c:func:`cci_property_get`,
        :c:func:`cci_property_set`,
        :c:func:`cci_datasource_create`,
        :c:func:`cci_datasource_destroy`,
        :c:func:`cci_datasource_release`

cci_query_result_free
---------------------

.. c:function:: int cci_query_result_free(T_CCI_QUERY_RESULT* query_result, int num_query)

    :c:func:`cci_execute_batch`, :c:func:`cci_execute_array` 또는 :c:func:`cci_execute_result` 함수에 의해 수행된 질의 결과를 메모리에서 해제한다.

    :param query_result: (IN) 메모리에서 해제할 질의 결과
    :param num_query: (IN) *query_result* 의 array 개수
    :return: 0: 성공

    .. code-block:: c

        T_CCI_QUERY_RESULT *qr;
        char **sql_stmt;
         
        ...
         
        res = cci_execute_array(conn, &qr, &err_buf);
         
        ...
         
        cci_query_result_free(qr, res);

CCI_QUERY_RESULT_ERR_NO
-----------------------

.. c:macro:: #define CCI_QUERY_RESULT_ERR_NO(T_CCI_QUERY_RESULT* query_result, int index)

    :c:func:`cci_execute_batch`, :c:func:`cci_execute_array`, 또는 :c:func:`cci_execute_result` 함수에 의해 수행된 질의 결과는 **T_CCI_QUERY_RESULT** 타입의 배열로 저장되므로 배열의 항목 별로 질의 결과를 확인해야 한다. 
    
    **CCI_QUERY_RESULT_ERR_NO**\는 *index*\로 지정한 배열 항목에 대한 에러 번호를 가져오며, 에러가 아닌 경우 0을 반환한다.

    :param query_result: (IN) 조회할 질의 결과
    :param index: (IN) 결과 배열의 인덱스(base : 1). 결과 배열 중 특정 위치를 나타냄.
    
    :return: 에러 번호        

CCI_QUERY_RESULT_ERR_MSG
------------------------

.. c:macro:: #define CCI_QUERY_RESULT_ERR_MSG(T_CCI_QUERY_RESULT* query_result, int index)

    :c:func:`cci_execute_batch`, :c:func:`cci_execute_array` 또는 :c:func:`cci_execute_result` 함수에 의해 수행된 질의 결과에 대한 에러 메시지를 가져오며, 에러 메시지가 없을 경우 ""(empty string)을 반환하는 매크로이다. 지정된 인자 *query_result* 가 **NULL** 인지, *index* 가 유효한지에 대한 검사는 하지 않는다.
    
    :param query_result: (IN) 조회할 질의 결과
    :param index: (IN) 칼럼 인덱스(base : 1)
    :return: 에러 메시지

CCI_QUERY_RESULT_RESULT
-----------------------

.. c:macro:: #define CCI_QUERY_RESULT_RESULT(T_CCI_QUERY_RESULT* query_result, int index)

    :c:func:`cci_execute_batch`, :c:func:`cci_execute_array` 또는 :c:func:`cci_execute_result` 함수에 의해 수행된 질의 결과에 대한 result count를 가져오는 매크로이다. 지정된 인자 *query_result* 가 **NULL** 인지, *index* 가 유효한지에 대한 검사는 하지 않는다.

    :param query_result: (IN) 조회할 질의 결과
    :param index: (IN) 칼럼 인덱스(base : 1)
    :return: result count

CCI_QUERY_RESULT_STMT_TYPE
--------------------------

.. c:macro:: #define CCI_QUERY_RESULT_STMT_TYPE(T_CCI_QUERY_RESULT* query_result, int index)

    :c:func:`cci_execute_batch`, :c:func:`cci_execute_array` 또는 :c:func:`cci_execute_result` 함수에 의해 수행된 질의 결과는 **T_CCI_QUERY_RESULT** 타입의 배열로 저장되므로 배열의 항목 별로 질의 결과를 확인해야 한다.
    
    **CCI_QUERY_RESULT_STMT_TYPE**\은 index로 지정한 배열 항목에 대한 statement type을 가져오는 매크로이다.
    
    지정된 인자 *query_result* 가 **NULL** 인지, *index* 가 유효한지에 대한 검사는 하지 않는다.

    :param query_result: (IN) 조회할 질의 결과
    :param index: (IN) 칼럼 인덱스(base : 1)
    :return: statement type (**T_CCI_CUBRID_STMT**)

cci_register_out_param
----------------------

.. c:function:: int cci_register_out_param(int req_handle, int index)

    자바 저장 프로시저에서 아웃 바인드로 정의한 파라미터를 바인딩하기 위해 사용하며, index는 1부터 시작한다.
    이 함수를 호출하기 전에 cci_prepare 함수의 플래그에 **CCI_PREPARE_CALL**\ 을 설정해야 한다.
    
    :param req_handle: (IN) 요청 핸들
    :return: 에러 코드
            
        *    **CCI_ER_BIND_INDEX**
        *    **CCI_ER_REQ_HANDLE**
        *    **CCI_ER_CON_HANDLE**
        *    **CCI_ER_USED_CONNECTION**

    다음은 "Hello, CUBRID"라는 문자열을 출력하는 자바 저장 프로시저의 예이다.
    
    먼저 cubrid.conf에 있는 java_stored_procedure 파라미터의 값을 yes로 설정하고, 데이터베이스를 구동한다.
    
    ::
    
        $ vi cubrid.conf
        java_stored_procedure=yes
    
        $ cubrid service start
        $ cubrid server start demodb
    
    자바 저장 프로시저로 등록할 클래스를 구현하고 컴파일한다.
    
    .. code-block:: java
    
        public class SpCubrid{
            public static void outTest(String[] o) {
                o[0] = "Hello, CUBRID";
            }
        }
        
        %javac SpCubrid.java
    
    컴파일된 자바 클래스를 CUBRID로 로딩한다.
    
    ::
        
        $ loadjava demodb SpCubrid.class
        
    로딩한 Java 클래스를 등록한다.
    
    .. code-block:: sql
        
        -- csql> 
        
        CREATE PROCEDURE test_out(x OUT STRING)
        AS LANGUAGE JAVA
        NAME 'SpCubrid.outTest(java.lang.String[] o)';
        
    CCI 응용 프로그램에서 cci_prepare 함수의 플래그에 CCI_PREPARE_CALL을 지정하고, cci_register_out_param 함수로 아웃 바인드 파라미터 위치를 지정한 후 실행 결과를 페치하여 가져온다. 
    
    .. code-block:: c

        // On Linux, compile with "gcc -g -o jsp jsp.c -I$CUBRID/include/ -L$CUBRID/lib/ -lcascci -lpthread"
        
        #include <stdio.h>
        #include <unistd.h>
        #include <cas_cci.h>
        char *cci_client_name = "test";

        int
        main (int argc, char *argv[])
        {
            int con = 0, req = 0, res, ind, i, col_count;
            T_CCI_ERROR error;
            T_CCI_COL_INFO *res_col_info;
            T_CCI_CUBRID_STMT cmd_type;
            char *buffer, db_ver[16];
    
            if ((con = cci_connect ("localhost", 33000, "demodb", "dba", "")) < 0)
            {
                printf ("%s(%d): cci_connect fail\n", __FILE__, __LINE__);
                return -1;
            }

            if ((req = cci_prepare (con, "call test_out(?)", CCI_PREPARE_CALL, &error)) < 0)
            {
                printf ("%s(%d): cci_prepare fail(%d)\n", __FILE__, __LINE__,
                    error.err_code);
                goto handle_error;
            }
            if ((res = cci_register_out_param (req, 1)) < 0)
            {
                printf ("%s(%d): cci_register_out_param fail(%d)\n", __FILE__, __LINE__,
                    error.err_code);
                goto handle_error;
            }
            if ((res = cci_execute (req, 0, 0, &error)) < 0)
            {
              printf ("%s(%d): cci_execute fail(%d)\n", __FILE__, __LINE__,
                    error.err_code);
              goto handle_error;
            }
            res = cci_cursor (req, 1, CCI_CURSOR_CURRENT, &error);
            if (res == CCI_ER_NO_MORE_DATA)
            {
                printf ("%s(%d): cci_cursor fail(%d)\n", __FILE__, __LINE__,
                    error.err_code);
                goto handle_error;
            }

            if ((res = cci_fetch (req, &error) < 0))
            {
                printf ("%s(%d): cci_fetch(%d, %s)\n", __FILE__, __LINE__,
                    error.err_code, error.err_msg);
              goto handle_error;
            }
            if ((res = cci_get_data (req, 1, CCI_A_TYPE_STR, &buffer, &ind)) < 0)
            {
                printf ("%s(%d): cci_get_data fail\n", __FILE__, __LINE__);
                goto handle_error;
            }
            // ind: string length, buffer: a string which came from the out binding parameter of test_out(?) Java SP.
            if (ind != -1) 
                printf ("%d, (%s)\n", ind, buffer);
            else // if ind== -1, then data is NULL
                printf ("NULL\n");

            if ((res = cci_close_query_result (req, &error)) < 0)
            {
                printf ("%s(%d): cci_close_query_result fail(%d)\n", __FILE__, __LINE__, error.err_code);
                goto handle_error;
            }

            if ((res = cci_close_req_handle (req)) < 0)
            {
                printf ("%s(%d): cci_close_req_handle fail\n", __FILE__, __LINE__);
                goto handle_error;
            }

            if ((res = cci_disconnect (con, &error)) < 0)
            {
                printf ("%s(%d): cci_disconnect fail(%d)\n", __FILE__, __LINE__,
                    error.err_code);
                goto handle_error;
            }
            printf ("Program ended!\n");
            return 0;

        handle_error:
            if (req > 0)
                cci_close_req_handle (req);
            if (con > 0)
                cci_disconnect (con, &error);
            printf ("Program failed!\n");
            return -1;
        }

    .. seealso:: 

        :c:func:`cci_prepare`

cci_row_count
-------------

.. c:function:: int cci_row_count(int conn_handle, int *row_count, T_CCI_ERROR * err_buf)

    가장 최근 수행한 질의에 의해 영향을 받은 행의 개수를 얻는다.
    
    :param conn_handle: (IN) 연결 핸들
    :param row_count: (OUT) 가장 최근 수행한 질의에 의해 영향을 받은 행의 개수
    :param err_buf: (OUT) 에러 버퍼
    
    :return: 에러 코드
    
        *    **CCI_ER_COMMUNICATION**
        *    **CCI_ER_LOGIN_TIMEOUT**
        *    **CCI_ER_NO_MORE_MEMORY**
        *    **CCI_ER_DBMS**

cci_savepoint
-------------

.. c:function:: int cci_savepoint(int conn_handle, T_CCI_SAVEPOINT_CMD cmd, char* savepoint_name, T_CCI_ERROR *err_buf)

    세이브포인트를 설정하거나 설정된 세이브포인트로 트랜잭션 롤백을 수행한다. *cmd* 가 **CCI_SP_SET** 로 지정되면 세이브포인트를 설정하고, **CCI_SP_ROLLBACK** 인 경우 설정된 세이브포인트까지 트랜잭션을 롤백한다.

    :param conn_handle: (IN) 연결 핸들
    :param cmd: (IN) **CCI_SP_SET** 또는 **CCI_SP_ROLLBACK**
    :param savepoint_name: (IN) 세이브포인트 이름
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_QUERY_TIMEOUT**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_DBMS**

    .. code-block:: c

        con = cci_connect( ... );
        ... /* query execute */

        /* "savepoint1"이란 이름의 세이브포인트 설정 */
        cci_savepoint(con, CCI_SP_SET, "savepoint1", err_buf);

        ... /* query execute */

        /* 설정된 세이브포인트 "savepoint1"로 롤백 */
        cci_savepoint(con, CCI_SP_ROLLBACK, "savepoint1", err_buf);

cci_schema_info
---------------

.. c:function:: int cci_schema_info(int conn_handle, T_CCI_SCHEMA_TYPE type, char *class_name, char *attr_name, char flag, T_CCI_ERROR *err_buf)

    스키마 정보를 읽어온다. 성공적으로 수행되었을 경우 결과는 request handle에 의해 관리되고, fetch, getdata를 통해서 결과를 가져올 수 있다. *class_name*, *attr_name* 을 **LIKE** 절의 패턴 매칭에 의해서 검색해야 할 경우 *flag* 를 설정한다.    

    :param conn_handle: (IN) 연결 핸들
    :param type: (IN) 스키마 타입
    :param class_name: (IN) 클래스 이름 또는 NULL
    :param attr_name: (IN) 속성 이름 또는 NULL
    :param flag: (IN) 패턴 매칭 flag(**CCI_CLASS_NAME_PATTERN_MATCH** 또는 **CCI_ATTR_NAME_PATTERN_MATCH)** 
    :param err_buf: (OUT) 에러 버퍼
    :return: 성공 : 요청 핸들, 실패 : 에러 코드

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_CONNECT**

    **CCI_CLASS_NAME_PATTERN_MATCH**, **CCI_ATTR_NAME_PATTERN_MATCH** 두 개의 *flag* 가 사용되며, OR 연산자( | )로 둘 다 설정할 수 있다. 패턴 매칭을 사용할 경우 **LIKE** 절을 이용하여 검색한다. 예를 들어, *class_name* 이 "athlete"이고 *attr_name* 이 "code"로 끝나는 칼럼에 대한 정보를 검색하고 싶다면, *attr_name* 의 값을 "%code"로 하여 다음과 같이 입력할 수 있다. ::

        cci_schema_info(conn, CCI_SCH_ATTRIBUTE, "athlete", "%code", CCI_ATTR_NAME_PATTERN_MATCH, &error);

    각 *type* 에 대한 레코드의 구성은 아래 표와 같다.

    **타입에 대한 레코드 구성**

    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | 타입                                                                                                               | 칼럼 순서        | 칼럼 이름          | 칼럼 타입           |
    +====================================================================================================================+==================+====================+=====================+
    | CCI_SCH_CLASS                                                                                                      | 1                | NAME               | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 2                | TYPE               | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | * 0: system class   | 
    |                                                                                                                    |                  |                    | * 1: vclass         |
    |                                                                                                                    |                  |                    | * 2: class          |
    |                                                                                                                    |                  |                    | * 3: proxy          |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 3                | REMARKS            | char \*             |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_VCLASS                                                                                                     | 1                | NAME               | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 2                | TYPE               | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | * 1: vclass         |
    |                                                                                                                    |                  |                    | * 3: proxy          |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 3                | REMARKS            | char \*             |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_QUERY_SPEC                                                                                                 | 1                | QUERY_SPEC         | char \*             |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_ATTRIBUTE                                                                                                  | 1                | NAME               | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 2                | DOMAIN             | short               |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 3                | SCALE              | short               |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 4                | PRECISION          | int                 |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 5                | INDEXED            | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | 1: indexed          |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 6                | NON_NULL           | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | 1: non null         |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 7                | SHARED             | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | 1: shared           |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 8                | UNIQUE             | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | 1: unique           |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 9                | DEFAULT            | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 10               | ATTR_ORDER         | int                 |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | base = 1            |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 11               | CLASS_NAME         | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 12               | SOURCE_CLASS       | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 13               | IS_KEY             | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | 1: key              |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 14               | REMARKS            | char \*             |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_CLASS_ATTRIBUTE                                                                                            |                  |                    |                     |
    |                                                                                                                    |                  |                    |                     |
    | | CCI_SCH_CLASS_ATTRIBUTE 컬럼이 INSTANCE 또는 SHARED의 속성일 경우에                                              |                  |                    |                     | 
    | | 순서와 이름 값은 CCI_SCH_ATTRIBUTE의 컬럼과 동일하다.                                                            |                  |                    |                     |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_CLASS_METHOD                                                                                               | 1                | NAME               | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 2                | RET_DOMAIN         | short               |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 3                | ARG_DOMAIN         | char \*             |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_METHOD_FILE                                                                                                | 1                | METHOD_FILE        | char \*             |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_SUPERCLASS                                                                                                 | 1                | CLASS_NAME         | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 2                | TYPE               | short               |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_SUBCLASS                                                                                                   | 1                | CLASS_NAME         | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 2                | TYPE               | short               |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_CONSTRAINT                                                                                                 | 1                | TYPE               | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | * 0: unique         |
    |                                                                                                                    |                  |                    | * 1: index          |
    |                                                                                                                    |                  |                    | * 2: reverse unique |
    |                                                                                                                    |                  |                    | * 3: reverse index  |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 2                | NAME               | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 3                | ATTR_NAME          | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 4                | NUM_PAGES          | int                 |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 5                | NUM_KEYS           | int                 |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 6                | PRIMARY_KEY        | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | * 1: primary key    |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 7                | KEY_ORDER          | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | base = 1            |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 8                | ASC_DESC           | char \*             |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_TRIGGER                                                                                                    | 1                | NAME               | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 2                | STATUS             | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 3                | EVENT              | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 4                | TARGET_CLASS       | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 5                | TARGET_ATTR        | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 6                | ACTION_TIME        | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 7                | ACTION             | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 8                | PRIORITY           | float               |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 9                | CONDITION_TIME     | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 10               | CONDITION          | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 11               | REMARKS            | char \*             |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_CLASS_PRIVILEGE                                                                                            | 1                | CLASS_NAME         | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 2                | PRIVILEGE          | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 3                | GRANTABLE          | char \*             |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_ATTR_PRIVILEGE                                                                                             | 1                | ATTR_NAME          | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 2                | PRIVILEGE          | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 3                | GRANTABLE          | char \*             |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_DIRECT_SUPER_CLASS                                                                                         | 1                | CLASS_NAME         | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 2                | SUPER_CLASS_NAME   | char \*             |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_PRIMARY_KEY                                                                                                | 1                | CLASS_NAME         | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 2                | ATTR_NAME          | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 3                | KEY_SEQ            | int                 |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | base = 1            |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 4                | KEY_NAME           | char \*             |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_IMPORTED_KEYS                                                                                              | 1                | PKTABLE_NAME       | char \*             |
    |                                                                                                                    |                  |                    |                     |
    | | 주어진 테이블의 외래 키 칼럼들이 참조하고 있는 기본 키 칼럼들의 정보를 조회하며                                  |                  |                    |                     |
    | | 결과는 PKTABLE_NAME 및 KEY_SEQ 순서로 정렬된다.                                                                  |                  |                    |                     |
    |                                                                                                                    |                  |                    |                     |
    | | 이 타입을 인자로 지정하면, *class_name*\ 에는 외래 키 테이블,                                                    |                  |                    |                     |
    | | *attr_name*\ 에는 **NULL**\ 을 지정한다.                                                                         |                  |                    |                     |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 2                | PKCOLUMN_NAME      | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 3                | FKTABLE_NAME       | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 4                | FKCOLUMN_NAME      | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 5                | KEY_SEQ            | short               |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 6                | UPDATE_ACTION      | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | * 0: cascade        |
    |                                                                                                                    |                  |                    | * 1: restrict       |
    |                                                                                                                    |                  |                    | * 2: no action      |
    |                                                                                                                    |                  |                    | * 3: set null       |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 7                | DELETE_ACTION      | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | * 0: cascade        |
    |                                                                                                                    |                  |                    | * 1: restrict       |
    |                                                                                                                    |                  |                    | * 2: no action      |
    |                                                                                                                    |                  |                    | * 3: set null       |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 8                | FK_NAME            | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 9                | PK_NAME            | char \*             |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_EXPORTED_KEYS                                                                                              | 1                | PKTABLE_NAME       | char \*             |
    |                                                                                                                    |                  |                    |                     |
    | | 주어진 테이블의 기본 키 칼럼들을 참조하는 모든 외래 키 칼럼들의 정보를 조회하며,                                 |                  |                    |                     |
    | | 결과는 FKTABLE_NAME 및 KEY_SEQ 순서로 정렬된다.                                                                  |                  |                    |                     |
    | | 이 타입을 인자로 지정하면, *class_name*\ 에는 기본 키 테이블,                                                    |                  |                    |                     |
    | | *attr_name*\ 에는 **NULL**\ 을 지정한다.                                                                         |                  |                    |                     |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 2                | PKCOLUMN_NAME      | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 3                | FKTABLE_NAME       | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 4                | FKCOLUMN_NAME      | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 5                | KEY_SEQ            | short               |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 6                | UPDATE_ACTION      | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | * 0: cascade        |
    |                                                                                                                    |                  |                    | * 1: restrict       |
    |                                                                                                                    |                  |                    | * 2: no action      |
    |                                                                                                                    |                  |                    | * 3: set null       |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 7                | DELETE_ACTION      | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | * 0: cascade        |
    |                                                                                                                    |                  |                    | * 1: restrict       |
    |                                                                                                                    |                  |                    | * 2: no action      |
    |                                                                                                                    |                  |                    | * 3: set null       |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 8                | FK_NAME            | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 9                | PK_NAME            | char \*             |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | CCI_SCH_CROSS_REFERENCE                                                                                            | 1                | PKTABLE_NAME       | char \*             |
    |                                                                                                                    |                  |                    |                     |
    | | 주어진 테이블의 기본 키와 주어진 테이블의 외래 키가 상호 참조하는 경우,                                          |                  |                    |                     |
    | | 해당 외래 키 칼럼들의 정보를 조회하며, 결과는 FKTABLE_NAME 및 KEY_SEQ 순서로 정렬된다.                           |                  |                    |                     |
    | | 이 타입을 인자로 *class_name*\ 에는 기본 키 테이블,                                                              |                  |                    |                     |
    | | *attr_name*  에는 외래 키 테이블을 지정한다.                                                                     |                  |                    |                     |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 2                | PKCOLUMN_NAME      | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 3                | FKTABLE_NAME       | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 4                | FKCOLUMN_NAME      | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 5                | KEY_SEQ            | short               |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 6                | UPDATE_ACTION      | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | * 0: cascade        |
    |                                                                                                                    |                  |                    | * 1: restrict       |
    |                                                                                                                    |                  |                    | * 2: no action      |
    |                                                                                                                    |                  |                    | * 3: set null       |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 7                | DELETE_ACTION      | short               |
    |                                                                                                                    |                  |                    |                     |
    |                                                                                                                    |                  |                    | * 0: cascade        |
    |                                                                                                                    |                  |                    | * 1: restrict       |
    |                                                                                                                    |                  |                    | * 2: no action      |
    |                                                                                                                    |                  |                    | * 3: set null       |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 8                | FK_NAME            | char \*             |
    |                                                                                                                    +------------------+--------------------+---------------------+
    |                                                                                                                    | 9                | PK_NAME            | char \*             |
    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+

    :c:func:`cci_schema_info` 함수에서 인자 *type* 은 인자 *class_name*, *attr_name* 에 대해 **LIKE** 절의 패턴 매칭을 지원한다.

    **패턴 매칭을 지원하는 type 및 class_name(테이블 이름), attr_name(칼럼 이름) 입력**

    +-------------------------------------+-------------------+-------------------------------------------------------------------------------+
    | type                                | class_name        | attr_name                                                                     |
    +=====================================+===================+===============================================================================+
    | CCI_SCH_CLASS (VCLASS)              | 문자열 또는 NULL  | 항상 NULL                                                                     |
    +-------------------------------------+-------------------+-------------------------------------------------------------------------------+
    | CCI_SCH_ATTRIBUTE (CLASS ATTRIBUTE) | 문자열 또는 NULL  | 문자열 또는 NULL                                                              |
    +-------------------------------------+-------------------+-------------------------------------------------------------------------------+
    | CCI_SCH_CLASS_PRIVILEGE             | 문자열 또는 NULL  | 항상 NULL                                                                     |
    +-------------------------------------+-------------------+-------------------------------------------------------------------------------+
    | CCI_SCH_ATTR_PRIVILEGE              | 항상 NULL         | 문자열 또는 NULL                                                              |
    +-------------------------------------+-------------------+-------------------------------------------------------------------------------+
    | CCI_SCH_PRIMARY_KEY                 | 문자열 또는 NULL  | 항상 NULL                                                                     |
    +-------------------------------------+-------------------+-------------------------------------------------------------------------------+
    | CCI_SCH_TRIGGER                     | 문자열 또는 NULL  | 항상 NULL                                                                     |
    +-------------------------------------+-------------------+-------------------------------------------------------------------------------+

    *   class_name이 "항상 NULL"인 type은 테이블 이름에 대해 패턴 매칭을 지원하지 않는다.
    *   attr_name이 "항상 NULL"인 type은 칼럼 이름에 대해 패턴 매칭을 지원하지 않는다.
    *   패턴 *flag* 가 설정되지 않을 경우 주어진 테이블 이름 또는 칼럼 이름은 동등 조건(=) 매칭을 사용하므로 **NULL** 이 주어지면 결과는 없다. 
    *   패턴 *flag* 가 설정되어 있을 경우 테이블 이름 또는 칼럼 이름이 **NULL**\이면 **LIKE** 절의 "%"와 동일한 결과, 즉 모든 테이블 또는 칼럼에 대한 결과를 얻는다.

    .. note::

        **CCI_SCH_CLASS**, **CCI_SCH_VCLASS** 의 TYPE 칼럼 : proxy 타입이 추가됨. OLEDB, ODBC, PHP에서 사용할 경우 proxy와 vclass를 구분하지 않고 vclass로 보임.

    .. code-block:: c
    
        // gcc -o schema_info schema_info.c -m64 -I${CUBRID}/include -lnsl ${CUBRID}/lib/libcascci.so -lpthread

        #include <stdio.h>
        #include <stdlib.h>
        #include <string.h>

        #include "cas_cci.h"

        int
        main ()
        {
          int conn = 0, req = 0, col_count = 0, res = 0, i, ind;
          char *data, query_result_buffer[1024];
          T_CCI_ERROR cci_error;
          T_CCI_COL_INFO *col_info;
          T_CCI_CUBRID_STMT cmd_type;


          conn = cci_connect ("localhost", 33000, "demodb", "dba", "");
          // get all columns' information of table "athlete"
          // req = cci_schema_info(conn, CCI_SCH_ATTRIBUTE, "athlete", "code", 0, &cci_error);
          req =
            cci_schema_info (conn, CCI_SCH_ATTRIBUTE, "athlete", NULL,
                             CCI_ATTR_NAME_PATTERN_MATCH, &cci_error);

          if (req < 0)
          {
              fprintf (stdout, "(%s, %d) ERROR : %s [%d] \n\n", __FILE__, __LINE__,
                       cci_error.err_msg, cci_error.err_code);
              goto _END;
          }
          col_info = cci_get_result_info (req, &cmd_type, &col_count);
          if (!col_info && col_count == 0)
          {
              fprintf (stdout, "(%s, %d) ERROR : cci_get_result_info\n\n", __FILE__,
                       __LINE__);
              goto _END;
          }
          res = cci_cursor (req, 1, CCI_CURSOR_FIRST, &cci_error);
          if (res == CCI_ER_NO_MORE_DATA)
          {
              goto _END;
          }
          if (res < 0)
          {
              fprintf (stdout, "(%s, %d) ERROR : %s [%d] \n\n", __FILE__, __LINE__,
                       cci_error.err_msg, cci_error.err_code);
              goto _END;
          }


          while (1)
          {
              res = cci_fetch (req, &cci_error);
              if (res < 0)
              {
                  fprintf (stdout, "(%s, %d) ERROR : %s [%d] \n\n", __FILE__,
                           __LINE__, cci_error.err_msg, cci_error.err_code);
                  goto _END;
              }

              for (i = 1; i <= col_count; i++)
              {
                  if ((res = cci_get_data (req, i, CCI_A_TYPE_STR, &data, &ind)) < 0)
                  {
                      goto _END;
                  }
                  if (ind != -1)
                  {
                      strcat (query_result_buffer, data);
                      strcat (query_result_buffer, "|");
                  }
                  else
                  {
                      strcat (query_result_buffer, "NULL|");
                  }
              }
              strcat (query_result_buffer, "\n");

              res = cci_cursor (req, 1, CCI_CURSOR_CURRENT, &cci_error);
              if (res == CCI_ER_NO_MORE_DATA)
              {
                  goto _END;
              }
              if (res < 0)
                {
                  fprintf (stdout, "(%s, %d) ERROR : %s [%d] \n\n", __FILE__,
                           __LINE__, cci_error.err_msg, cci_error.err_code);
                  goto _END;
                }
          }

        _END:
          if (req > 0)
            cci_close_req_handle (req);
          if (conn > 0)
            res = cci_disconnect (conn, &cci_error);
          if (res < 0)
            fprintf (stdout, "(%s, %d) ERROR : %s [%d] \n\n", __FILE__, __LINE__,
                     cci_error.err_msg, cci_error.err_code);

          fprintf (stdout, "Result : %s\n", query_result_buffer);

          return 0;
        }
       
cci_set_allocators
------------------

.. c:function:: int cci_set_allocators(CCI_MALLOC_FUNCTION malloc_func, CCI_FREE_FUNCTION free_func, CCI_REALLOC_FUNCTION realloc_func, CCI_CALLOC_FUNCTION calloc_func)

    사용자가 정의한 메모리 할당 및 해제 함수들을 등록하여 사용할 수 있게 한다. 이 함수를 실행하면 CCI API 내부에서 사용되는 모든 메모리 할당 및 해제 작업은 사용자가 정의한 메모리 할당 함수들을 사용하게 된다. 이를 사용하지 않으면 기본적인 시스템 함수들(malloc, free, realloc, calloc)이 사용된다.

    .. note:: 이 함수는 Linux 전용으로, Windows에서는 사용할 수 없다.

    :param malloc_func: (IN) malloc에 해당하는 외부 정의 함수에 대한 포인터
    :param free_func: (IN) free에 해당하는 외부 정의 함수에 대한 포인터
    :param realloc_func: (IN) realloc에 해당하는 외부 정의 함수에 대한 포인터
    :param calloc_func: calloc에 해당하는 외부 정의 함수에 대한 포인터
    :return: 에러 코드(0: 성공)

        *   **CCI_ER_NOT_IMPLEMENTED**

    .. code-block:: c

        /*
               How to build: gcc -Wall -g -o test_cci test_cci.c -I${CUBRID}/include -L${CUBRID}/lib -lcascci
        */
         
        #include <stdio.h>
        #include <stdlib.h>
        #include "cas_cci.h"
         
        void *my_malloc(size_t size)
        {
            printf ("my malloc: size: %ld\n", size);
            return malloc (size);
        }
         
        void *my_calloc(size_t nm, size_t size)
        {
            printf ("my calloc: nm: %ld, size: %ld\n", nm, size);
            return calloc (nm, size);
        }
         
        void *my_realloc(void *ptr, size_t size)
        {
            printf ("my realloc: ptr: %p, size: %ld\n", ptr, size);
            return realloc (ptr, size);
        }
         
        void my_free(void *ptr)
        {
            printf ("my free: ptr: %p\n", ptr);
            return free (ptr);
        }
         
         
        int test_simple (int con)
        {
            int req = 0, col_count = 0, i, ind;
            int error;
            char *data;
            T_CCI_ERROR cci_error;
            T_CCI_COL_INFO *col_info;
            T_CCI_CUBRID_STMT stmt_type;
            char *query = "select * from db_class";
            
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
                printf ("get_result_info error\n");
                goto handle_error;
            }
         
            //Executing the prepared SQL statement
            error = cci_execute (req, 0, 0, &cci_error);
            if (error < 0)
            {
                printf ("execute error: %d, %s\n", cci_error.err_code, cci_error.err_msg);
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
                    printf ("cursor error: %d, %s\n", cci_error.err_code, cci_error.err_msg);
                    goto handle_error;
                }
            
                //Fetching the query result into a client buffer
                error = cci_fetch (req, &cci_error);
                if (error < 0)
                {
                    printf ("fetch error: %d, %s\n", cci_error.err_code, cci_error.err_msg);
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

            //Closing the query result
            error = cci_close_query_result(req, &cci_error);
             if (error < 0)
            {
                printf ("cci_close_query_result error: %d, %s\n", cci_error.err_code, cci_error.err_msg);
                goto handle_error;
            }

            //Closing the request handle
            error = cci_close_req_handle(req);
            if (error < 0)
            {
                printf ("cci_close_req_handle error\n");
                goto handle_error;
            }
            
            //Disconnecting with the server
            error = cci_disconnect (con, &cci_error);
            if (error < 0)
            {
                printf ("cci_disconnect error: %d, %s\n", cci_error.err_code, cci_error.err_msg);
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
        
        int main()
        {
            int con = 0;
            
            if (cci_set_allocators (my_malloc, my_free, my_realloc, my_calloc) != 0)
            {
                printf ("cannot register allocators\n");
                return 1;
            };
            
            //getting a connection handle for a connection with a server
            con = cci_connect ("localhost", 33000, "demodb", "dba", "");
            if (con < 0)
            {
                printf ("cannot connect to database\n");
                return 1;
            }
            
            test_simple (con);
            return 0;
        }

cci_set_autocommit
------------------

.. c:function:: int cci_set_autocommit (int conn_handle, CCI_AUTOCOMMIT_MODE  autocommit_mode)

    현재 데이터베이스 연결의 자동 커밋 모드 여부를 설정한다. 이 함수는 자동 커밋 모드를 설정하거나 해제하는 데에만 사용되며, 이 함수를 호출하면 진행 중이던 트랜잭션은 모드 설정과 상관없이 커밋된다.

    .. note:: **cubrid_broker.conf**\ 의 :ref:`CCI_DEFAULT_AUTOCOMMIT <cci_default_autocommit>`\ 은 프로그램 시작 시의 기본 자동 커밋 모드를 설정한다.

    :param conn_handle: (IN) 연결 핸들
    :param autocommit_mode: (IN) 자동 커밋모드 설정. CCI_AUTOCOMMIT_FALSE 또는 CCI_AUTOCOMMIT_TRUE 중 하나의 값을 가진다.
    :return: 에러 코드(0: 성공)

    .. note::

         **CCI_DEFAULT_AUTOCOMMIT**, a broker parameter configured in the **cubrid_broker.conf** file, determines whether it is in auto-commit mode upon program startup.

cci_set_db_parameter
---------------------

.. c:function:: int cci_set_db_parameter(int conn_handle, T_CCI_DB_PARAM param_name, void* value, T_CCI_ERROR *err_buf)

    시스템 파라미터를 설정한다. 설정할 수 있는 *param_name* 은 :c:func:`cci_get_db_parameter`\ 를 참조한다.

    :param conn_handle: (IN) 연결 핸들
    :param param_name: (IN) 시스템 파라미터 이름
    :param value: (IN) 파라미터 값
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드(0: 성공)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_PARAM_NAME**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CONNECT**

cci_set_element_type
--------------------

.. c:function:: int cci_set_element_type(T_CCI_SET set)

    **T_CCI_SET** 타입 값의 엘리먼트 타입을 가져온다.

    :param set: (IN) cci set 포인터
    :return: 타입

cci_set_free
------------

.. c:function:: void cci_set_free(T_CCI_SET set)

    :c:func:`cci_get_data`\ 에 대해 **CCI_A_TYPE_SET** 에 의해 가져온 **T_CCI_SET** 타입 값에 할당된 메모리를 제거한다. **T_CCI_SET** 타입 값은 :c:func:`cci_get_data` 함수를 통해 가져오거나 :c:func:`cci_set_make` 함수를 통해 생성할 수 있다.

    :param set: (IN) cci set 포인터

cci_set_get
-----------

.. c:function:: int cci_set_get(T_CCI_SET set, int index, T_CCI_A_TYPE a_type, void *value, int *indicator)

    **T_CCI_SET** 타입 값에 대해 *index* 번째의 데이터를 가져온다. 
    
    :param set: (IN) cci set 포인터
    :param index: (IN) set index (base: 1)
    :param a_type: (IN) 타입
    :param value: (OUT) 결과 버퍼
    :param indicator: (OUT) null 표시(indicator)
    :return: 에러 코드

        *   **CCI_ER_SET_INDEX**
        *   **CCI_ER_TYPE_CONVERSION**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_COMMUNICATION**
    
    *a_type*\ 에 대한 *value*\ 의 타입은 다음과 같다.

    +-----------------------+------------------------------+
    |   a_type              | value 타입                   |
    +=======================+==============================+
    | **CCI_A_TYPE_STR**    | char \*\*                    |
    +-----------------------+------------------------------+
    | **CCI_A_TYPE_INT**    | int \*                       |
    +-----------------------+------------------------------+
    | **CCI_A_TYPE_FLOAT**  | float \*                     |
    +-----------------------+------------------------------+
    | **CCI_A_TYPE_DOUBLE** | double \*                    |
    +-----------------------+------------------------------+
    | **CCI_A_TYPE_BIT**    | **T_CCI_BIT** \*             |
    +-----------------------+------------------------------+
    | **CCI_A_TYPE_DATE**   | **T_CCI_DATE** \*            |
    +-----------------------+------------------------------+
    | **CCI_A_TYPE_BIGINT** | int64_t \*                   |
    |                       | (Windows는 __int64 \*)       |
    +-----------------------+------------------------------+

cci_set_holdability
-------------------

.. c:function:: int cci_set_holdability(int conn_handle,int holdable)

    연결 수준에서 결과 셋의 커서 유지(cursor holdability) 여부를 설정한다. 값이 1이면 커밋 여부에 관계 없이 연결이 종료되거나 결과 셋을 의도적으로 닫기 전까지 커서를 유지(holdable)하고, 0이면 커밋될 때 결과 셋이 닫히면서 커서를 유지하지 않는다(not holdable). 커서 유지에 대한 자세한 설명은 :ref:`cursor-holding`\ 를 참고한다.

    :param conn_handle: (IN) 연결 핸들
    :param holdable: (IN) 커서 유지 여부 설정 값(0: not holdable, 1: holdable)
    :return: 에러 코드
    
        *   **CCI_ER_INVALID_HOLDABILITY**

cci_set_isolation_level
-----------------------

.. c:function:: int cci_set_isolation_level(int conn_handle, T_CCI_TRAN_ISOLATION  new_isolation_level, T_CCI_ERROR *err_buf)

    연결에 대한 트랜잭션 격리 수준(transaction isolation level)을 설정한다. 이후 주어진 연결에 대한 모든 트랜잭션은 *new_isolation_level* 로서 동작한다.

    :param conn_handle: (IN) 연결 핸들
    :param new_isolation_level: (IN) 격리 수준(isolation level)
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_ISOLATION_LEVEL**
        *   **CCI_ER_DBMS**
    
    cci_set_db_parameter()에 의해 트랜잭션 격리 수준이 설정된 경우에는 현재의 트랜잭션에 대해서만 영향을 주고 트랜잭션이 끝나면 CAS에서 설정된 트랜잭션 격리 수준으로 복귀된다. 연결 전체에 대해서 트랜잭션 격리 수준을 설정할 경우는 반드시 cci_set_isolation_level()을 사용해야 한다.

cci_set_lock_timeout
--------------------

.. c:function:: int cci_set_lock_timeout(int conn_handle, int locktimeout, T_CCI_ERROR * err_buf)

    연결에 대한 잠금 타임아웃을 밀리초 단위로 설정한다.
    cci_set_db_parameter(conn_id, CCI_PARAM_LOCK_TIMEOUT, &val, err_buf)를 호출한 것과 동일하며, :c:func:`cci_set_db_parameter` 함수를 참고한다.
    
    :param conn_handle: (IN) 연결 핸들
    :param locktimeout: (IN) 잠금 타임아웃 시간(단위: 밀리 초).
    :return: 에러 코드(0: 성공)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_PARAM_NAME**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CONNECT**

cci_set_login_timeout
---------------------

.. c:function:: int cci_set_login_timeout(int conn_handle, int timeout, T_CCI_ERROR *err_buf)
    
    로그인 타임아웃을 밀리초 단위로 설정한다. :c:func:`cci_prepare` 함수나 :c:func:`cci_execute` 함수를 호출하면서 내부적으로 재접속이 발생할 때 로그인 타임 아웃이 적용된다. 이 변경은 현재의 연결에만 적용된다.
   
    :param conn_handle: (IN) 연결 핸들
    :param timeout: (IN) 로그인 타임아웃(단위: 밀리초)
    :param err_buf: (OUT) 에러 버퍼
    :return: 에러 코드(0: 성공)
        
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_USED_CONNECTION**

cci_set_make
------------

.. c:function:: int cci_set_make(T_CCI_SET *set, T_CCI_U_TYPE u_type, int size, void *value, int *indicator)

    새로운 **CCI_A_TYPE_SET** 타입의 set을 만든다. 만들어진 set은 :c:func:`cci_bind_param`\ 을 통해 **CCI_A_TYPE_SET** 으로 서버에 전달된다. :c:func:`cci_set_make`\ 에 의해 만들어진 set은 반드시 :c:func:`cci_set_free`\ 를 통해 사용된 메모리를 제거해야 한다. *u_type* 에 따른 *value* 의 타입은 :c:func:`cci_bind_param`\ 을 참고한다.

    :param set: (OUT) cci set 포인터
    :param u_type: (IN) 엘리먼트 타입
    :param size: (IN) set 크기
    :param value: (IN) set 엘리먼트
    :param indicator: (IN) null 표시 배열(indicator array)
    :return: 에러 코드

cci_set_max_row
---------------

.. c:function:: int cci_set_max_row(int req_handle, int max)

    :c:func:`cci_execute`\ 에 의해 수행되는 **SELECT** 문의 결과에 대해 최대 레코드 수를 지정한다. *max* 값이 0일 경우 지정하지 않은 것과 동일하다.

    :param req_handle: (IN) 요청 핸들
    :param max: (IN) 최대 행
    :return: 에러 코드

    .. code-block:: c

        req = cci_prepare( ... );
        cci_set_max_row(req, 1);
        cci_execute( ... );

cci_set_query_timeout
---------------------

.. c:function:: int cci_set_query_timeout(int req_handle, int milli_sec)

    질의 수행의 타임아웃 시간을 설정한다.

    :param req_handle: (IN) 요청 핸들
    :param milli_sec:  타임아웃(timeout) 시간, 단위는 msec.
    :return: 성공 : 요청 핸들 ID, 실패 : 에러 코드

        *   **CCI_ER_REQ_HANDLE**

    **cci_set_query_timeout** 으로 설정된 타임아웃 시간은 :c:func:`cci_prepare`, :c:func:`cci_execute`, :c:func:`cci_execute_array`, :c:func:`cci_execute_batch` 함수들에 영향을 미친다. 각 함수에서 타임아웃이 발생했을 때 :c:func:`cci_connect_with_url` 연결 URL에 설정한 **disconnect_on_query_timeout** 의 값이 yes이면 **CCI_ER_QUERY_TIMEOUT** 에러를 반환한다.

    위 함수들은 :c:func:`cci_connect_with_url` 함수의 인자인 연결 URL에 **login_timeout** 이 설정되어 있는 경우에도 **CCI_ER_LOGIN_TIMEOUT** 에러를 반환할 수 있는데, 이는 응용 클라이언트와 브로커 응용 서버(CAS) 간 재연결 과정에서 로그인 타임아웃이 발생한 경우이다.

    응용 클라이언트와 CAS 간 재연결 과정은 CAS가 재시작하거나 재스케쥴되는 경우에 발생한다. 재스케쥴이란 CAS가 트랜잭션 단위로 응용 클라이언트를 선택하여 연결을 시작하고 종료하는 과정을 의미하는데, 브로커 파라미터인 **KEEP_CONNECTION** 이 AUTO이면 상황에 따라 발생한다. 보다 자세한 사항은 :ref:`parameter-by-broker`\의 **KEEP_CONNECTION** 설명을 참고한다.

cci_set_size
------------

.. c:function:: int cci_set_size(T_CCI_SET set)

    **T_CCI_SET** 타입 값에 대한 엘리먼트 개수를 가져온다.

    :param set: (IN) cci set 포인터
    :return: 크기
