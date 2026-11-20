
:meta-keywords: CCI driver, CCI api, cubrid cci
:meta-description: CUBRID CCI API Reference for your C-based back-end application.

CCI API Reference
=================

.. contents::

cci_bind_param
--------------

.. c:function::    int cci_bind_param (int req_handle, int index, T_CCI_A_TYPE a_type, void *value, T_CCI_U_TYPE u_type, char flag)

    The **cci_bind_param** function binds data in the *bind* variable of prepared statement. This function converts *value* of the given *a_type* to an actual binding type and stores it. Subsequently, whenever :c:func:`cci_execute` is called, the stored data is sent to the server. If **cci_bind_param** () is called multiple times for the same *index*, the latest configured value is valid.

    :param req_handle: (IN) Request handle of a prepared statement
    :param index: (IN) Location of binding; it starts with 1.
    :param a_type: (IN) Data type of *value*
    :param value: (IN) Data value to bind
    :param u_type: (IN) Data type to be applied to the database
    :param flag: (IN) bind_flag(:c:type:`CCI_BIND_PTR`).
    :return: Error code (0: success)
    
        *   **CCI_ER_BIND_INDEX**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_TYPE_CONVERSION**
        *   **CCI_ER_USED_CONNECTION**

    To bind **NULL** to the database, choose one of below settings.

    *   Set the value of *value* to a **NULL** pointer
    *   Set the value of *u_type* to :c:macro:`CCI_U_TYPE_NULL`

    The following shows a part of code to bind NULL.
    
    .. code-block:: c
    
        res = cci_bind_param (req, 2 /* binding index*/, CCI_A_TYPE_STR, NULL, CCI_U_TYPE_STRING, CCI_BIND_PTR);
        
    or
    
    .. code-block:: c
    
        res = cci_bind_param (req, 2 /* binding index*/, CCI_A_TYPE_STR, data, CCI_U_TYPE_NULL, CCI_BIND_PTR);

    can be used.
    
    If **CCI_BIND_PTR** is configured for *flag*, the pointer of *value* variable is copied (shallow copy), but no value is copied. 
    If it is not configured for *flag*, the value of *value* variable is copied (deep copy) by allocating memory. If multiple columns are bound by using the same memory buffer, **CCI_BIND_PTR** must not be configured for the *flag*.

    **T_CCI_A_TYPE** is a C language type that is used in CCI applications for data binding, and consists of primitive types such as int and float, and user-defined types defined by CCI such as **T_CCI_BIT** and **T_CCI_DATE**. The identifier for each type is defined as shown in the table below.

    +-----------------------------+-----------------------------+
    | a_type                      | value type                  |
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

    **T_CCI_U_TYPE** is a column type of database and data bound though the *value* argument is converted into this type. 
    The **cci_bind_param** () function uses two kinds of types to send information which is used to convert U-type data from A-type data; the U-type data can be interpreted by database language and the A-type data can be interpreted by C language.

    There are various A-type data that are allowed by U-type data. For example, **CCI_U_TYPE_INT** can receive **CCI_A_TYPE_STR** as A-type data including **CCI_A_TYPE_INT**. For information on type conversion, see :ref:`implicit-type-conversion`.

    Both **T_CCI_A_TYPE** and **T_CCI_U_TYPE** enum(s) are defined in the **cas_cci.h** file. The definition of each identifier is described in the table below.

    +--------------------------+------------------------------------+
    | u_type                   | Corresponding a_type (default)     |
    +==========================+====================================+
    | **CCI_U_TYPE_CHAR**      | **CCI_A_TYPE_STR**                 |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_STRING**    | **CCI_A_TYPE_STR**                 |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_BIT**       | **CCI_A_TYPE_BIT**                 |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_VARBIT**    | **CCI_A_TYPE_BIT**                 |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_NUMERIC**   | **CCI_A_TYPE_STR**                 |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_INT**       | **CCI_A_TYPE_INT**                 |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_SHORT**     | **CCI_A_TYPE_INT**                 |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_FLOAT**     | **CCI_A_TYPE_FLOAT**               |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_DOUBLE**    | **CCI_A_TYPE_DOUBLE**              |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_DATE**      | **CCI_A_TYPE_DATE**                |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_TIME**      | **CCI_A_TYPE_DATE**                |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_TIMESTAMP** | **CCI_A_TYPE_DATE**                |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_OBJECT**    | **CCI_A_TYPE_STR**                 |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_BIGINT**    | **CCI_A_TYPE_BIGINT**              |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_DATETIME**  | **CCI_A_TYPE_DATE**                |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_BLOB**      | **CCI_A_TYPE_BLOB**                |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_CLOB**      | **CCI_A_TYPE_CLOB**                |
    +--------------------------+------------------------------------+
    | **CCI_U_TYPE_ENUM**      | **CCI_A_TYPE_STR**                 |
    +--------------------------+------------------------------------+

    When the string including the date is used as an input parameter of **DATE**, **DATETIME**, or **TIMESTAMP**, "YYYY/MM/DD" or "YYYY-MM-DD" is allowed for the date string type. Therefore, "2012/01/31" or "2012-01-31" is valid, but "01/31/2012" is invalid. The following is an example of having the string that includes the date as an input parameter of the date type.

    .. code-block:: c

        // "CREATE TABLE tbl(aa date, bb datetime)";
         
        char *values[][3] =
        {
            {"1994/11/30", "1994/11/30 20:08:08"},
            {"2008-10-31", "2008-10-31 20:08:08"}
        };
        
        req = cci_prepare(conn, "insert into tbl (aa, bb) values ( ?, ?)", CCI_PREPARE_INCLUDE_OID, &error);
        
        for(i=0; i< 2; i++)
        {
            res = cci_bind_param(req, 1, CCI_A_TYPE_STR, values[i][0], CCI_U_TYPE_DATE, (char)NULL);
            res = cci_bind_param(req, 2, CCI_A_TYPE_STR, values[i][1], CCI_U_TYPE_DATETIME, (char)NULL);
            cci_execute(req, CCI_EXEC_QUERY_ALL, 0, err_buf);
        }

cci_bind_param_array
--------------------

.. c:function:: int cci_bind_param_array(int req_handle, int index, T_CCI_A_TYPE a_type, void *value, int *null_ind, T_CCI_U_TYPE u_type)

    The **cci_bind_param_array** function binds a parameter array for a prepared :c:func:`cci_execute_array` is called, data is sent to the server by the stored *value* pointer. If **cci_bind_param_array** () is called multiple times for the same *index*, the last configured value is valid. If **NULL** is bound to the data, a non-zero value is configured in *null_ind*. If *value* is a **NULL** pointer, or *u_type* is **CCI_U_TYPE_NULL**, all data are bound to **NULL** and the data buffer used by *value* cannot be reused. For the data type of *value* for *a_type*, see the :c:func:`cci_bind_param` function description.

    :param req_handle: (IN) Request handle of the prepared statement
    :param index: (IN) Binding location
    :param a_type: (IN) Data type of *value*
    :param value: (IN) Data value to be bound
    :param null_ind: (IN) **NULL** indicator array (0 : not **NULL**, 1 : **NULL**)
    :param u_type: (IN) Data type to be applied to the database.
    :return: Error code (0: success)
    
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

    The **cci_bind_param_array_size** function determines the size of the array to be used in :c:func:`cci_bind_param_array`. **cci_bind_param_array_size** () must be called first before :c:func:`cci_bind_param_array` is used.

    :param req_handle: Request handle of a prepared statement
    :param array_size: (IN) binding array size
    :return: Error code (0: success)
    
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_USED_CONNECTION**

cci_bind_param_ex
-----------------

.. c:function:: int cci_bind_param_ex (int req_handle, int index, T_CCI_A_TYPE a_type, void *value, int length, T_CCI_U_TYPE u_type, char flag) 
     
    The **cci_bind_param_ex** function works as the same with :c:func:`cci_bind_param`. However, it has an additional argument, *length*, which specifies the byte length of a string if bound data is a string.
     
    :param req_handle: (IN) Request handle of the prepared statement 
    :param index: (IN) Binding location, starting from 1
    :param a_type: (IN) Data type of *value* 
    :param value: (IN) Data value to be bound
    :param length: (IN) Byte length of a string to be bound
    :param u_type: (IN) Data type to be applied to the database.
    :param flag: (IN) bind_flag(:c:type:`CCI_BIND_PTR`). 
     
    :return: Error code(0: success) 
  
    The *length* argument can be used for binding a string which includes '\\0' as below.
     
    .. code-block:: c 
  
        cci_bind_param_ex(statement, 1, CCI_A_TYPE_STR, "aaa\0bbb", 7, CCI_U_TYPE_STRING, 0); 

cci_blob_free
-------------

.. c:function:: int cci_blob_free(T_CCI_BLOB blob)

    The **cci_blob_free** function frees memory of *blob* struct.

    :return: Error code (0: success)
    
        *   **CCI_ER_INVALID_LOB_HANDLE**

cci_blob_new
------------

.. c:function:: int cci_blob_new(int conn_handle, T_CCI_BLOB* blob, T_CCI_ERROR* error_buf)

    The **cci_blob_new** function creates an empty file where **LOB** data is stored and returns locator referring to the data to *blob* struct.

    :param conn_handle: (IN) Connection handle
    :param blob: (OUT) **LOB** locator
    :param error_buf: (OUT) Error buffer
    :return: Error code (0: success)
    
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

.. c:function:: int cci_blob_read(int conn_handle, T_CCI_BLOB blob, long start_pos, int length, char *buf, T_CCI_ERROR* error_buf)

    The **cci_blob_read** function reads as much as data from *start_pos* to *length* of the **LOB** data file specified in *blob*; then it stores it in *buf* and returns it.

    :param conn_handle: (IN) Connection handle
    :param blob: (OUT) **LOB** locator
    :param start_pos: (IN) Index location of **LOB** data file
    :param length: (IN) **LOB** data length from buffer
    :param buf: (IN) Data buffer to read
    :param error_buf: (OUT) Error buffer
    :return: Size of read value (>= 0: success), Error code (< 0: error)
    
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

.. c:function:: long long cci_blob_size(T_CCI_BLOB* blob)

    The **cci_blob_size** function returns data file size that is specified in *blob*.

    :param blob: (OUT) **LOB** locator
    :return: Size of **BLOB** data file (>= 0: success), Error code (< 0: error)

        *   **CCI_ER_INVALID_LOB_HANDLE**

cci_blob_write
--------------

.. c:function:: int cci_blob_write(int conn_handle, T_CCI_BLOB blob, long start_pos, int length, const char *buf, T_CCI_ERROR* error_buf)

    The **cci_blob_write** function reads as much as data from *buf* to *length* and stores it from *start_pos* of the **LOB** data file specified in *blob*.

    :param conn_handle: (IN) Connection handle
    :param blob: (OUT) **LOB** locator
    :param start_pos: (IN) Index location of **LOB** data file
    :param length: (IN) Data length from buffer
    :param buf: (OUT) Data buffer to write
    :param error_buf: (OUT) Error buffer
    :return: Size of written value (>= 0: success), Error code (< 0: error)

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

    Cancel the running query on the other thread. This function executes the same behavior as Statement.cancel() method in JAVA.
    
    :param conn_handle: (IN) Connection handle
    :return: Error code
        
        *    **CCI_ER_COMMUNICATION**
        *    **CCI_ER_CON_HANDLE**
        *    **CCI_ER_CONNECT**

    The following shows how to cancel the query execution of a thread.
    
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

    The **cci_clob_free** function frees memory of **CLOB** struct.

    :param clob: (IN) **LOB** locator
    :return: Error code (0: success)

        *   **CCI_ER_INVALID_LOB_HANDLE**

cci_clob_new
------------

.. c:function:: int cci_clob_new(int conn_handle, T_CCI_CLOB* clob, T_CCI_ERROR* error_buf)

    The **cci_clob_new** function creates an empty file where **LOB** data is stored and returns locator referring to the data to *clob* struct.

    :param conn_handle: ((IN) Connection handle
    :param clob: (OUT) **LOB** locator
    :param error_buf: (OUT) Error buffer
    :return: Error code (0: success)

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

    The **cci_clob_read** function reads as much as data from *start_pos* to *length* in the **LOB** data file specified in *clob*; then it stores it in *buf* and returns it.

    :param conn_handle: (IN) Connection handle
    :param clob: (IN) **LOB** locator
    :param start_pos: (IN) Index location of **LOB** data file
    :param length: (IN) **LOB** data length from buffer
    :param buf: (IN) Data buffer to read
    :param error_buf: (OUT) Error buffer
    :return: Size of read value (>= 0: success), Error code (< 0: Error)

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

    The **cci_clob_size** function returns data file size that is specified in *clob*.

    :param clob: (IN) **LOB** locator
    :return: Size of **CLOB** data file (>= 0: success), Error code (< 0: error)

        *   **CCI_ER_INVALID_LOB_HANDLE**

cci_clob_write
--------------

.. c:function:: int cci_clob_write(int conn_handle, T_CCI_CLOB clob, long start_pos, int length, const char *buf, T_CCI_ERROR* error_buf)

    The **cci_clob_write** function reads as much as data from *buf* to *length* and then stores the value from *start_pos* in **LOB** data file specified in *clob*.

    :param conn_handle: (IN) Connection handle
    :param clob: (IN) **LOB** locator
    :param start_pos: (IN) Index location of **LOB** data file
    :param length: (IN) Data length from buffer
    :param buf: (OUT) Data buffer to write
    :param error_buf: (OUT) Error buffer
    :return: Size of written value (>= 0: success), Error code (< 0: Error)

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

    The **cci_close_query_result** funciton closes the resultset returned by :c:func:`cci_execute`, :c:func:`cci_execute_array` or :c:func:`cci_execute_batch`.
    If you run :c:func:`cci_prepare` repeatedly without closing the request handle(req_handle), it is recommended to call this function before calling :c:func:`cci_close_req_handle`.
    
    :param req_handle: (IN) Request handle
    :param err_buf: (OUT) Error buffer
    :return: Error code (0: success)
    
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

    The **cci_close_req_handle** function closes the request handle obtained by :c:func:`cci_prepare`.

    :param req_handle: (IN) Request handle
    :return: Error code (0: success)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_USED_CONNECTION**

cci_col_get
-----------

.. c:function:: int cci_col_get (int conn_handle, char *oid_str, char *col_attr, int *col_size, int *col_type, T_CCI_ERROR *err_buf)

    The **cci_col_get** function gets an attribute value of collection type. If the name of the class is C, and the domain of *set_attr* is set (multiset, sequence), the query looks like as follows:
    
    .. code-block:: sql
    
        SELECT a FROM C, TABLE(set_attr) AS t(a) WHERE C = oid;

    That is, the number of members becomes the number of records.

    :param conn_handle: (IN) Connection handle
    :param oid_str: (IN) oid
    :param col_attr: (IN) Collection attribute name
    :param col_size: (OUT) Collection size (-1 : null)
    :param col_type: (OUT) Collection type (set, multiset, sequence: u_type)
    :param err_buf: (OUT) Database error buffer
    :return: Request handle

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_OBJECT**
        *   **CCI_ER_DBMS**

cci_col_seq_drop
----------------

.. c:function:: int cci_col_seq_drop (int conn_handle, char *oid_str, char *col_attr, int index, T_CCI_ERROR *err_buf)

    The **cci_col_seq_drop** function drops the index-th (base: 1) member of the sequence attribute values. The following example shows how to drop the first member of the sequence attribute values. ::

        cci_col_seq_drop(conn_handle, oid_str, seq_attr, 1, err_buf);

    :param conn_handle: (IN) Connection handle
    :param oid_str: (IN) oid
    :param col_attr: (IN) Collection attribute name
    :param index: (IN) Index
    :param err_buf: (OUT) Database error buffer
    :return: Error code

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_OBJECT**
        *   **CCI_ER_DBMS**

cci_col_seq_insert
------------------

.. c:function:: int cci_col_seq_insert (int conn_handle, char *oid_str, char *col_attr, int index, char *value, T_CCI_ERROR *err_buf)

    The **cci_col_seq_insert** function inserts one member at the index-th (base: 1) position of the sequence attribute values. The following example shows how to insert "a" at the first position of the sequence attribute values. ::

        cci_col_seq_insert(conn_handle, oid_str, seq_attr, 1, "a", err_buf);

    :param conn_handle: (IN) Connection handle
    :param oid_str: (IN) oid
    :param col_attr: (IN) Collection attribute name
    :param index: (IN) Index
    :param value: (IN) Sequential element (string)
    :param err_buf: (OUT) Database error buffer
    :return: Error code

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_OBJECT**
        *   **CCI_ER_DBMS**

cci_col_seq_put
---------------

.. c:function:: int cci_col_seq_put (int conn_handle, char *oid_str, char *col_attr, int index, char *value, T_CCI_ERROR *err_buf)

    The **cci_col_seq_put** function replaces the index-th (base: 1) member of the sequence attribute values with a new value. The following example shows how to replace the first member of the sequence attributes values with "a". ::

        cci_col_seq_put(conn_handle, oid_str, seq_attr, 1, "a", err_buf);

    :param conn_handle: (IN) Connection handle
    :param oid_str: (IN) oid
    :param col_attr: (IN) Collection attribute name
    :param index: (IN) Index
    :param value: (IN) Sequential value
    :param err_buf: (OUT) Database error buffer
    :return: Error code

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_OBJECT**
        *   **CCI_ER_DBMS**

cci_col_set_add
---------------

.. c:function:: int cci_col_set_add (int conn_handle, char *oid_str, char *col_attr, char *value, T_CCI_ERRROR *err_buf)

    The **cci_col_set_add** function adds one member to the set attribute values. The following example shows how to add "a" to the set attribute values. ::

        cci_col_set_add(conn_handle, oid_str, set_attr, "a", err_buf);

    :param conn_handle: (IN) Connection handle
    :param oid_str: (IN) oid
    :param col_attr: (IN) collection attribute name
    :param value: (IN) set element
    :param err_buf: (OUT) Database error buffer
    :return: Error code

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_OBJECT**
        *   **CCI_ER_DBMS**

cci_col_set_drop
----------------

.. c:function:: int cci_col_set_drop (int conn_handle, char *oid_str, char *col_attr, char *value, T_CCI_ERROR *err_buf)

    The **cci_col_set_drop** function drops one member from the set attribute values. The following example shows how to drop "a" from the set attribute values. ::

        cci_col_set_drop(conn_handle, oid_str, set_attr, "a", err_buf);

    :param conn_handle: (IN) Connection handle
    :param oid_str: (IN) oid
    :param col_attr: (IN) collection attribute name
    :param value: (IN) set element (string)
    :param err_buf: (OUT) Database error buffer
    :return: Error code

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_QUERY_TIMEOUT**
        *   **CCI_ER_LOGIN_TIMEOUT**
        *   **CCI_ER_COMMUNICATION**

cci_col_size
------------

.. c:function:: int cci_col_size (int conn_handle, char *oid_str, char *col_attr, int *col_size, T_CCI_ERROR *err_buf)

    The **cci_col_size** function gets the size of the set (seq) attribute.

    :param conn_handle: (IN) Connection handle
    :param oid_str: (IN) oid
    :param col_attr: (IN) Collection attribute name
    :param col_size: (OUT) Collection size (-1: NULL)
    :param err_buf: Database error buffer
    :return: Error code (0: success)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_OBJECT**
        *   **CCI_ER_DBMS**

cci_connect
-----------

.. c:function:: int cci_connect(char *ip, int port, char *db_name, char *db_user, char *db_password)

    A connection handle to the database server is assigned and it tries to connect to the server. If it has succeeded, the connection handle ID is returned; if fails, an error code is returned.

    :param ip: (IN) A string that represents the IP address of the server (host name)
    :param port: (IN) Broker port (The port configured in the **$CUBRID/conf/cubrid_broker.conf** file)
    :param db_name: (IN) Database name
    :param db_user: (IN) Database user name
    :param db_passwd: (IN) Database user password
    :return: Success: Connection handle ID (int), Failure: Error code

        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_HOSTNAME**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CONNECT**

cci_connect_ex
--------------

.. c:function:: int cci_connect_ex(char *ip, int port, char *db_name, char *db_user, char *db_password, T_CCI_ERROR * err_buf)

    The **cci_connect_ex** function returns **CCI_ER_DBMS** error and checks the error details in the database error buffer (*err_buf*) at the same time. In that point, it is different from :c:func:`cci_connect` and the others are the same as the :c:func:`cci_connect` function.

    :param ip: (IN) A string that represents the IP address of the server (host name)
    :param port: (IN) Broker port (The port configured in the **$CUBRID/conf/cubrid_broker.conf** file)
    :param db_name: (IN) Database name
    :param db_user: (IN) Database user name
    :param db_passwd: (IN) Database user password
    :param err_buf: Database error buffer
    :return: Success: Connection handle ID (int), Failure: Error code

        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_HOSTNAME**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CONNECT**

.. _cci_connect_with_url:

cci_connect_with_url
--------------------

.. c:function:: int cci_connect_with_url (char *url, char *db_user, char *db_password)

    The **cci_connect_with_url** function connects a database by using connection information passed with a *url* argument. If broker's HA feature is used in CCI, you must specify the connection information of the standby broker server with altHosts property, which is used for the failover, in the *url* argument of this function. It returns the ID of a connection handle on success; it returns an error code on failure. For details about HA features of broker, see :ref:`duplexing-brokers`.

    :param url: (IN) A string that contains server connection information. 
    :param db_user: (IN) Database user name. If this is NULL, it becomes <*db_user*> in *url*. If this is an empty string ("") or <*db_user*> in *url* is not specified, DB user name becomes **PUBLIC**.
    :param db_passwd: (IN) Database user password. If this is NULL, <*db_password*> in *url* is used. If <*db_password*> in *url* is not specified, DB password becomes an empty string ("").
    :return: Success: Connection handle ID (int), Failure: Error code

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
                     | useSSL=true|false
         
        <alternative_hosts> ::= <host>:<port> [,<host>:<port>]
         
        <host> := HOSTNAME | IP_ADDR
        <time> := SECOND
        <milli_sec> := MILLI SECOND    

    **altHosts** is the property related to connection target and **loginTimeout**, **queryTimeout**, and **disconnectOnQueryTimeout** are the properties related to timeout; **logSlowQueries**, **logTraceApi**, and **logTraceNetwork** are the properties related to log information configuration for debugging. Note that a property name which is a value to be entered in the *url* argument is not case sensitive.

    *   *host*: A host name or IP address of the master database
    *   *port*: A port number
    *   *db_name*: A name of the database
    *   *db_user*: A name of the database user
    *   *db_password*: A database user password. You cannot include ':' in the password of the *url* string.

    *   **altHosts** = *standby_broker1_host*, *standby_broker2_host*, ...: Specifies the broker information of the standby server, which is used for failover when it is impossible to connect to the active server. You can specify multiple brokers for failover, and the connection to the brokers is attempted in the order listed in **alhosts**.
    
        .. note:: Even if there are **RW** and **RO** together in *ACCESS_MODE** setting of brokers of main host and **altHosts**, application decides the target host to access without the relation for the setting of **ACCESS_MODE**. Therefore, you should define the main host and **altHosts** as considering **ACCESS_MODE** of target brokers.

    *   **rcTime**: After the failure occurred on the first connected broker, the application connects to the broker specified by **altHosts**\(broker failover). Then it attempts to reconnect to the first connected broker at every **rcTime**\(default value: 600 seconds).
    
    *    **loadBalance**: When this value is true, the applications try to connect to the main host and alternative hosts specified with the **altHosts** property as a random order. (default value: false).
    
    *   **login_timeout** | **loginTimeout**: Timeout value (unit: msec.) for database login. Upon timeout, a **CCI_ER_LOGIN_TIMEOUT** (-38) error is returned. The default value is  **30,000**\ (30 sec.). If this value is 0, it means infinite waiting. This value is also applied when internal reconnection occurs after the initial connection.

    *   **query_timeout** | **queryTimeout**: If time specified in these properties has expired when calling :c:func:`cci_prepare`, :c:func:`cci_execute`, etc. a cancellation message for query request which was sent to a server will be delivered and called function returns a **CCI_ER_QUERY_TIMEOUT** (-39) error. The default value is 0. If this value is 0, it means infinite waiting. The value returned upon timeout may vary depending on a value specified in **disconnect_on_query_timeout**. For details, see **disconnect_on_query_timeout**.

        .. note:: If you use :c:func:`cci_execute_batch` or :c:func:`cci_execute_array`\ function, or set **CCI_EXEC_QUERY_ALL** in :c:func:`cci_execute` function to run multiple queries at once, query timeout is applied to one function, not to one query. In other words, if query timeout occurs after the start of a function, a function running is quit.

    *   **disconnect_on_query_timeout** | **disconnectOnQueryTimeout**: Whether to disconnect socket immediately after time for query request has expired. It determines whether to terminate a socket connection immediately or wait for server response after sending cancellation message for query request to a server when calling :c:func:`cci_prepare`, :c:func:`cci_execute`, etc. The default value is **false**, meaning that it will wait for server response. It this value is true, a socket will be closed immediately after sending a cancellation message to a server upon timeout and returns the **CCI_ER_QUERY_TIMEOUT** (-39) error. (If an error occurs on database server side, not on broker side, it returns -1. If you want to view error details, see error codes in "database error buffer." You can get information how to check error codes in :ref:`CCI Error Codes and Error Messages <cci-error-codes>`.) Please note that there is a possibility that a database server does not get a cancellation message and execute a query even after an error is returned.

    *   **logFile**: A log file name for debugging (default value: **cci_** <*handle_id*> **.log**). <*handle_id*> indicates the ID of a connection handle returned by this function.
    
    *   **logBaseDir**: A directory where a debug log file is created. The file name including the path will be logBaseDir/logFile, and the relative path is possible.
    
    *   **logSlowQueries**: Whether to log slow query for debugging (default value: **false**)
    *   **slowQueryThresholdMillis**: Timeout for slow query logging if slow query logging is enabled (default value: **60000**, unit: milliseconds)
    *   **logTraceApi**: Whether to log the start and end of CCI functions
    *   **logTraceNetwork**: Whether to log network data content transferred of CCI functions

    *   **useSSL**: Packet Encryption mode (Default: false)

       *   Packet encryption: useSSL = true
       *   Plain text: useSSL = false

    **Example** ::

        --connection URL string when a property(altHosts) is specified for HA
        URL=cci:CUBRID:192.168.0.1:33000:demodb:::?altHosts=192.168.0.2:33000,192.168.0.3:33000
         
        --connection URL string when properties(altHosts,rcTime) is specified for HA
        URL=cci:CUBRID:192.168.0.1:33000:demodb:::?altHosts=192.168.0.2:33000,192.168.0.3:33000&rcTime=600
         
        --connection URL string when properties(logSlowQueries,slowQueryThresholdMills, logTraceApi, logTraceNetwork) are specified for interface debugging
        URL = "cci:cubrid:192.168.0.1:33000:demodb:::?logSlowQueries=true&slowQueryThresholdMillis=1000&logTraceApi=true&logTraceNetwork=true"

        --connection URL string when useSSL property specified for encrypted connection
        URL = "cci:cubrid:192.168.0.1:33000:demodb:::?useSSL=true

    .. warning::

        * The **useSSL** flag must match with the mode of the broker trying to connect. If encryption mode is different from the server that trying to connect, that connection request will be rejected. Please refer to the following cases that are not allowed.

           *   useSSL=true, connection request will be rejected when the broker is in 'normal mode' (**cubrid_broker.conf**: SSL = OFF)
           *   useSSL=false, connection request will be rejected when the broker is in 'encryption mode' (**cubrid_broker.conf**: SSL = ON)

cci_connect_with_url_ex
-----------------------

.. c:function:: int cci_connect_with_url_ex (char *url, char *db_user, char *db_password, T_CCI_ERROR * err_buf)

    The **cci_connect_with_url_ex** function returns **CCI_ER_DBMS** error and checks the error details in the database error buffer (*err_buf*) at the same time. In that point, it is different from :c:func:`cci_connect_with_url` and the others are the same as the :c:func:`cci_connect_with_url` function. 
    
    :param err_buf: Database error buffer    

.. c:function:: int cci_cursor(int req_handle, int offset, T_CCI_CURSOR_POS origin, T_CCI_ERROR *err_buf)

    The **cci_cursor** function moves the cursor specified in the request handle to access the specific record in the query result executed by :c:func:`cci_execute`. The position of cursor is moved by the values specified in the *origin* and *offset* values. If the position to be moved is not valid, **CCI_ER_NO_MORE_DATA** is returned.

    :param req_handle: (IN) Request handle
    :param offset: (IN) Offset to be moved
    :param origin: (IN) Variable to represent a position. The type is **T_CCI_CURSOR_POS**. **T_CCI_CURSOR_POS** enum consists of **CCI_CURSOR_FIRST**, **CCI_CURSOR_CURRENT** and **CCI_CURSOR_LAST**.
    :param err_buf: (OUT) Database error buffer
    :return: Error code (0: success)

        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_NO_MORE_DATA**
        *   **CCI_ER_COMMUNICATION**

    **Example**

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

    The **cci_cursor_update** function updates *cursor_pos* from the value of the *index* -th column to *value*. If the database is updated to **NULL**, *value* becomes **NULL**. For update conditions, see :c:func:`cci_prepare`. The data types of *value* for *a_type* are shown in the table below.
    
    :param req_handle: (IN) Request handle
    :param cursor_pos: (IN) Cursor position
    :param index: (IN) Column index
    :param a_type: (IN) *value* Type
    :param value: (IN) A new value
    :param err_buf: (OUT) Database error buffer
    :return:  Error code (0: success)

        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_TYPE_CONVERSION**
        *   **CCI_ER_ATYPE**

    Data types of *value* for *a_type* are as below.
        
    +-----------------------------+-----------------------------+
    | a_type                      | value type                  |
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

    The **cci_datasource_borrow** function obtains CCI connection to be used in **T_CCI_DATASOURCE** struct.

    :param datasource: (IN) **T_CCI_DATASOURCE** struct pointer in which CCI connection exists
    :param err_buf: (OUT) Error code and message returned upon error occurrence
    :return: Success: CCI connection handler identifier, Failure: -1

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
 
    A property name of a DATASOURCE is specified in *key*, a value in *val*. The changed property value by this function is applied to all connections in the *datasource*.
     
    :param datasource: (IN) T_CCI_DATASOURCE struct pointer to obtain CCI connections.
    :param key: (IN) A pointer to the string of a property name
    :param val: (IN) A pointer to the string of a property value
    :return: Error code(0: success)
    
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_NO_PROPERTY**
        *   **CCI_ER_PROPERTY_TYPE**
    
    The following shows names and values of changeable properties.

    ========================= =========== ============================== =========================================================================================================================
    Property name             Type        Value                          Description
    ========================= =========== ============================== =========================================================================================================================
    default_autocommit        bool        true/false                     Whether auto-commit or not. The default is CCI_DEFAULT_AUTOCOMMIT in cubrid_broker.conf; the default of this is ON(true)
    default_lock_timeout      msec        number                         lock timeout                                                      
    default_isolation         string      See the table of               isolation level. The default is isolation_level in cubrid.conf; the default of this is "READ COMMITTED".
                                          :c:func:`cci_property_set`                                                                     
    login_timeout             msec        number                         login timeout. The default is 0(infinite wait). It can also be used when you call prepare or execute functions; 
                                                                         at this time reconnection can happen.
    ========================= =========== ============================== =========================================================================================================================

    **Example**

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

.. c:function:: T_CCI_DATASOURCE * cci_datasource_create (T_CCI_PROPERTIES *properties, T_CCI_ERROR *err_buf)

    The **cci_datasource_create** function creates DATASOURCE of CCI.

    :param properties: (IN) **T_CCI_PROPERTIES** struct pointer in which configuration of struct pointer is stored. Values of properties will be set with :c:func:`cci_property_set`.
    :param err_buf: (OUT) Error buffer. Error code and message returned upon error occurrence
    :return: Success: **T_CCI_DATASOURCE** struct pointer created, Failure: **NULL**

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

    The **cci_datasource_destroy** function destroys DATASOURCE of CCI.

    :param datasource: (IN) **T_CCI_DATASOURCE** struct pointer to be deleted
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

    The **cci_datasource_release** function returns CCI connection released in **T_CCI_DATASOURCE** struct. If you want to reuse the connection after calling this function, recall :c:func:`cci_datasource_borrow`.

    :param datasource: (IN) **T_CCI_DATASOURCE** struct pointer which returns CCI connection
    :param conn: (IN) CCI connection handler identifier released
    :param err_buf: (OUT) Error buffer(returns error code and error message when an error occurs)
    :return: Success: 1, Failure: 0

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

    The **cci_disconnect** function disconnects all request handles created for *conn_handle*. If a transaction is being performed, the handles are disconnected after :c:func:`cci_end_tran` is executed.

    :param conn_handle: (IN) Connection handle
    :param err_buf: (OUT) Database error buffer
    :return: Error code (0: success)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**

cci_end_tran
------------

.. c:function:: int cci_end_tran(int conn_handle, char type, T_CCI_ERROR *err_buf)

    The **cci_end_tran** function performs commit or rollback on the current transaction. At this point, all open request handles are terminated and the connection to the database server is disabled. However, even after the connection to the server is disabled, the connection handle remains valid.

    :param conn_handle: (IN) Connection handle
    :param type: (IN) **CCI_TRAN_COMMIT** or **CCI_TRAN_ROLLBACK**
    :param err_buf: (OUT) Database error buffer
    :return: Error code (0: success)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_TRAN_TYPE**

    You can configure the default value of auto-commit mode by using :ref:`CCI_DEFAULT_AUTOCOMMIT <cci_default_autocommit>` broker parameter upon startup of an application. If configuration on broker parameter is omitted, the default value is **ON**; use the :c:func:`cci_set_autocommit` function to change auto-commit mode within an application. If auto-commit mode is **OFF**, you must explicitly commit or roll back transaction by using the :c:func:`cci_end_tran` function.

cci_escape_string
-----------------

.. c:function:: long cci_escape_string(int conn_handle, char *to, const char *from, unsigned long length, T_CCI_ERROR *err_buf)

    Converts the input string to a string that can be used in the CUBRID query. The following parameters are specified in this function: connection handle or **no_backslash_escapes** setting value, output string pointer, input string pointer, the length of the input string, and the address of the **T_CCI_ERROR** struct variable.

    :param conn_handle: (IN) connection handle or **no_backslash_escapes** setting value. When a connection handle is given, the **no_backslash_escapes** parameter value is read to determine how to convert. Instead of the connection handle, **CCI_NO_BACKSLASH_ESCAPES_TRUE** or **CCI_NO_BACKSLASH_ESCAPES_FALSE** value can be sent to determine how to convert.
    :param to: (OUT) Result string
    :param from: (IN) Input string
    :param length: (IN) Maximum byte length of the input string
    :param err_buf: (OUT) Database error buffer
    :return: Success: Byte length of the changed string, Failure: Error Code
    
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_COMMUNICATION**

    When the system parameter **no_backslash_escapes** is yes (default) or when the **CCI_NO_BACKSLASH_ESCAPES_TRUE** value is sent to the connection handle location, the string is converted to the following characters.

    *   ' (single quote) => ' + ' (escaped single quote)

    When the system parameter **no_backslash_escapes** is no or when the **CCI_NO_BACKSLASH_ESCAPES_FALSE** value is sent to the connection handle location, the string is converted to the following characters:

    *   \\n (new line character, ASCII 10) => \\ + n (backslash + Alphabet n)
    *   \\r (carriage return, ASCII 13) => \\ + r (backslash + Alphabet r)
    *   \\0 (ASCII 0) => \\ + 0 (backslash + 0(ASCII 48)
    *   \\  (backslash) => \\ + \\

    You can assign the space where the result string will be saved by using the *length* parameter. It will take as much as the byte length of the maximum input string * 2 + 1.

cci_execute
-----------

.. c:function:: int cci_execute(int req_handle, char flag, int max_col_size, T_CCI_ERROR *err_buf)

    The **cci_execute** function executes the SQL statement (prepared statement) that has executed :c:func:`cci_prepare`. A request handle, *flag*, the maximum length of a column to be fetched, and the address of a **T_CCI_ERROR** construct variable in which error information being stored are specified as arguments.

    :param req_handle: (IN) Request handle of the prepared statement
    :param flag: (IN) exec flag ( **CCI_EXEC_QUERY_ALL** )
    :param max_col_size: (IN) The maximum length of a column to be fetched when it is a string data type in bytes. If this value is 0, full length is fetched.
    :param err_buf: (OUT) Database error buffer
    :return: 
      * **SELECT** : Returns the number of results
      * **INSERT**, **UPDATE** : Returns the number of rows reflected
      * Others queries : 0
      * Failure : Error code
      
        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_BIND**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_QUERY_TIMEOUT**
        *   **CCI_ER_LOGIN_TIMEOUT**
  
    Through a *flag*, the way of query execution can be set as all queries or the first one.
    
    .. note::
    
        In 2008 R4.4 and from 9.2, CUBRID does not support setting the *flag* as **CCI_EXEC_ASYNC**, which brings the results as an asynchronous method.

    If the *flag* is set to **CCI_EXEC_QUERY_ALL**, all prepared queries(separated by semicolon) are executed. If not, only the first query is executed.
    
    If the *flag* is set to **CCI_EXEC_QUERY_ALL**, the following rules are applied.
    
    *   The return value is the result of the first query.
    *   If an error occurs in any query, the execution is processed as a failure.
    *   For a query composed of in a query composed of q1; q2; q3, even if an error occurs in q2 after q1 succeeds the execution, the result of q1 remains valid. That is, the previous successful query executions are not rolled back when an error occurs.
    *   If a query is executed successfully, the result of the second query can be obtained using :c:func:`cci_next_result`.

    *max_col_size* is a value that is used to determine the maximum length of a column to be sent to a client when the columns of the prepared statement are **CHAR**, **VARCHAR**, **BIT** or **VARBIT**. If this value is 0, full length is fetched.

cci_execute_array
-----------------

.. c:function:: int cci_execute_array(int req_handle, T_CCI_QUERY_RESULT **query_result, T_CCI_ERROR *err_buf)

    If more than one value is bound to the prepared statement, this gets the values of the variables to be bound and executes the query by binding each value to the variable.

    :param req_handle: (IN) Request handle of the prepared statement
    :param query_result: (OUT) Query results
    :param err_buf: (OUT) Database error buffer
    :return: 
         * Success: The number of executed queries (it has nothing to do with query execution success/failure.)
         * Failure: Negative number
    
            *   **CCI_ER_REQ_HANDLE**
            *   **CCI_ER_BIND**
            *   **CCI_ER_DBMS**
            *   **CCI_ER_COMMUNICATION**
            *   **CCI_ER_QUERY_TIMEOUT**
            *   **CCI_ER_LOGIN_TIMEOUT**
       
    To bind the data, call the :c:func:`cci_bind_param_array_size` function to specify the size of the array, bind each value to the variable by using the :c:func:`cci_bind_param_array` function, and execute the query by calling the :c:func:`cci_execute_array` function. The query result will be stored on the array of **T_CCI_QUERY_RESULT** structure.

    :c:func:`cci_execute_array` function returns the results of queries to the *query_result* variable. You can use below macros to get the result of each query. In the macro, note that the validation check for each parameter entered is not performed. 
    
    After using the query_result variable, you must delete the query_result by using the :c:func:`cci_query_result_free` function.

    +---------------------------------------+---------------------------------+---------------------------------+
    | Macro                                 | Return Type                     | Description                     |
    +=======================================+=================================+=================================+
    | :c:macro:`CCI_QUERY_RESULT_RESULT`    | int                             | the number of affected rows     |
    |                                       |                                 | or error identifier             |
    |                                       |                                 | (-1: CAS error, -2: DBMS error) |    
    +---------------------------------------+---------------------------------+---------------------------------+
    | :c:macro:`CCI_QUERY_RESULT_ERR_NO`    | int                             | error number about a query      |
    +---------------------------------------+---------------------------------+---------------------------------+
    | :c:macro:`CCI_QUERY_RESULT_ERR_MSG`   | char \*                         | error message about a query     |
    +---------------------------------------+---------------------------------+---------------------------------+
    | :c:macro:`CCI_QUERY_RESULT_STMT_TYPE` | int(**T_CCI_CUBRID_STMT** enum) | type of a query statement       |
    +---------------------------------------+---------------------------------+---------------------------------+
    
    If autocommit mode is on, each query in the array is committed after executing.
    
    .. note:: 
    
        *   In the previous version of 2008 R4.3, if the autocommit mode is on, all queries in the array were committed after all of them are executed.  From 2008 R4.3 version, the transaction is committed every time when a query is executed.
        *   In autocommit mode off, if the general error occurs during executing one of the queries in the array on the cci_execute_array function which does a batch processing of the queries, the query with an error is ignored and the next query is executed continuously. But if the deadlock occurs, the error occurs as rolling back the transaction. 
    
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

    In CCI, multiple jobs can be processed simultaneously when using DML queries such as **INSERT** / **UPDATE** / **DELETE**. :c:macro:`CCI_QUERY_RESULT_RESULT` and :c:func:`cci_execute_batch` functions can be used to execute such batch jobs. Note that prepared statements cannot be used in the :c:func:`cci_execute_batch` function. The query result will be stored on the array of **T_CCI_QUERY_RESULT** structure.

    :param conn_handle: (IN) Connection handle
    :param num_sql_stmt: (IN)  The number of *sql_stmt*
    :param sql_stmt: (IN) SQL statement array
    :param query_result: (OUT) The results of *sql_stmt*
    :param err_buf: (OUT) Database error buffer
    :return: 
         * Success: The number of executed queries (it has nothing to do with query execution success/failure.)
         * Failure: Negative number

            *   **CCI_ER_CON_HANDLE**
            *   **CCI_ER_DBMS**
            *   **CCI_ER_COMMUNICATION**
            *   **CCI_ER_NO_MORE_MEMORY**
            *   **CCI_ER_CONNECT**
            *   **CCI_ER_QUERY_TIMEOUT**
            *   **CCI_ER_LOGIN_TIMEOUT**

    Executes *sql_stmt* as many times as *num_sql_stmt* specified as a parameter and returns the number of queries executed with the query_result variable. You can use :c:macro:`CCI_QUERY_RESULT_RESULT`, c:macro:`CCI_QUERY_RESULT_ERR_NO`, :c:macro:`CCI_QUERY_RESULT_ERR_MSG` and :c:macro:`CCI_QUERY_RESULT_STMT_TYPE` macros to get the result of each query. Regarding the summary of these macros, see the :c:func:`cci_execute_array` function.
    
    Note that the validity check is not performed for each parameter entered in the macro.
    
    After using the *query_result* variable, you must delete the query result by using the :c:func:`cci_query_result_free` function.
    
    If autocommit mode is on, each query in the array is committed after executing.
    
    .. note:: 
    
        *   In the previous version of 2008 R4.3, if the autocommit is on, all queries in the array were committed after all of them are executed. From 2008 R4.3 version, each query in the array is committed right after each running.
        *   If autocommit mode is off, after the general error occurs during executing one of the queries in the array on the cci_execute_batch function which does a batch processing of the queries, the query with an error is ignored and the next query is executed. But if the deadlock occurs, the error occurs as rolling back the transaction. 

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

    The **cci_execute_result** function stores the execution results (e.g. statement type, result count) performed by :c:func:`cci_execute` to the array of **T_CCI_QUERY_RESULT** structure. You can use :c:macro:`CCI_QUERY_RESULT_RESULT`, :c:macro:`CCI_QUERY_RESULT_ERR_NO`, :c:macro:`CCI_QUERY_RESULT_ERR_MSG`, :c:macro:`CCI_QUERY_RESULT_STMT_TYPE` macros to get the results of each query. Regarding the summary of these macros, see the :c:func:`cci_execute_array` function.

    Note that the validity check is not performed for each parameter entered in the macro.
    
    The memory of used query results must be released by the :c:func:`cci_query_result_free` function.

    :param req_handle: (IN) Request handle of the prepared statement
    :param query_result: (OUT) Query results
    :param err_buf: (OUT) Database error buffer
    :return: Success: The number of queries, Failure: Negative number

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

    The **cci_fetch** function fetches the query result executed by :c:func:`cci_execute` from the server-side CAS and stores it to the client buffer. The :c:func:`cci_get_data` function can be used to identify the data of a specific column from the fetched query result.

    :param req_handle: (IN) Request handle
    :param err_buf: (OUT) Database error buffer
    :return: Error code (0: success)

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

    The **cci_fetch_buffer_clear** function clears the records temporarily stored in the client buffer.

    :param req_handle: Request handle
    :return: Error code (0: success)

        *   **CCI_ER_REQ_HANDLE**

cci_fetch_sensitive
-------------------

.. c:function:: int cci_fetch_sensitive(int req_handle, T_CCI_ERROR *err_buf)

    The **cci_fetch_sensitive** function sends changed values for sensitive column when the **SELECT** query result is delivered. If the results by *req_handle* are not sensitive, they are same as the ones by :c:func:`cci_fetch`. The return value of **CCI_ER_DELETED_TUPLE** means that the given row has been deleted. 
    
    :param req_handle: (IN) Request handle
    :param err_buf: (OUT) Database error buffer
    :return: Error code (0: success)

        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_NO_MORE_DATA**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_DELETED_TUPLE**

    sensitive column means the item that can provide updated value in the **SELECT** list when you re-request the results. For example, if a column is directly used as an item of the **SELECT** list without aggregation operation, this column can be called a sensitive column. 
    
    When you fetch the result again, the sensitive result receive the data from the server, not from the client buffer.

cci_fetch_size
--------------

.. c:function:: int cci_fetch_size(int req_handle, int fetch_size)

    This function is deprecated. Even if it's called, there will be ignored.

cci_get_autocommit
------------------

.. c:function:: CCI_AUTOCOMMIT_MODE cci_get_autocommit(int conn_handle)

    The **cci_get_autocommit** function returns the auto-commit mode currently configured.

    :param conn_handle: (IN) Connection handle
    :return: 
    
        *   **CCI_AUTOCOMMIT_TRUE**: Auto-commit mode is ON
        *   **CCI_AUTOCOMMIT_FALSE**: Auto-commit mode is OFF
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_USED_CONNECTION**

cci_get_bind_num
----------------

.. c:function:: int cci_get_bind_num(int req_handle)

    The **cci_get_bind_num** function gets the number of input bindings. If the SQL statement used during preparation is composed of multiple queries, it represents the number of input bindings used in all queries.

    :param req_handle: (IN) Request handle for the prepared statement
    :return: The number of input bindings

        *   **CCI_ER_REQ_HANDLE**

cci_get_cas_info
----------------

.. c:function:: int cci_get_cas_info (int conn_handle, char *info_buf, int buf_length, T_CCI_ERROR * err_buf) 
  
    Retrieve CAS information which is connected to conn_handle. The string of the below format is returned to the info_buf.
  
    :: 
  
        <host>:<port>,<cas id>,<cas process id> 

    The below is an example.
    
    ::
    
        127.0.0.1:33000,1,12916 
  
    Through CAS ID, you can check the SQL log file of this CAS easily.
  
    For details, see :ref:`sql-log-check`.
         
    :param conn_handle: (IN) connection handle
    :param info_buf: (OUT) connection information buffer
    :param buf_length: (IN) buffer length of the connection information
    :param err_buf: (OUT) Error buffer
    :return: error code
     
        * **CCI_ER_INVALID_ARGS** 
        * **CCI_ER_CON_HANDLE** 

cci_get_class_num_objs
----------------------

.. c:function:: int cci_get_class_num_objs(int conn_handle, char *class_name, int flag, int *num_objs, int *num_pages, T_CCI_ERROR *err_buf)

    The **cci_get_class_num_objs** function gets the number of objects of the *class_name* class and the number of pages being used. If the flag is configured to 1, an approximate value is fetched; if it is configured to 0, an exact value is fetched.

    :param conn_handle: (IN) Connection handle
    :param class_name: (IN) Class name
    :param flag: (IN)  0 or 1
    :param num_objs: (OUT) The number of objects
    :param num_pages: (OUT) The number of pages
    :param err_buf: (OUT) Database error buffer
    :return: Error code (0: success)
    
        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CONNECT**

CCI_GET_COLLECTION_DOMAIN
-------------------------

.. c:macro:: #define CCI_GET_COLLECTION_DOMAIN(u_type)

    If *u_type* is set, multiset, or sequence type, this macro gets the domain of the set, multiset or sequence. If *u_type* is not a set type, the return value is the same as *u_type*.

    :return: Type (CCI_U_TYPE)

cci_get_cur_oid
---------------

.. c:function:: int cci_get_cur_oid(int req_handle, char *oid_str_buf)

    The **cci_get_cur_oid** function gets OID of the currently fetched records if **CCI_INCLUDE_OID** is configured in execution. The OID is represented in string for a page, slot, or volume.

    :param conn_handle: (IN) Request handle
    :param oid_str_buf: (OUT) OID string
    :return: Error code (0: success)

        *   **CCI_ER_REQ_HANDLE**

cci_get_data
------------

.. c:function:: int cci_get_data(int req_handle, int col_no, int type, void *value, int *indicator)

    The **cci_get_data** function gets the *col_no* -th value from the currently fetched result. 
    
    :param req_handle: (IN) Request handle
    :param col_no: (IN) One-based column index. It starts with 1.
    :param type: (IN) Data type (defined in the **T_CCI_A_TYPE**) of *value* variable
    :param value: (OUT) Variable address for data to be stored. If *type* is one of (CCI_A_TYPE_STR, CCI_A_TYPE_SET, CCI_A_TYPE_BLOB or CCI_A_TYPE_CLOB) and the value of a column is NULL, the *value* will be NULL, too.
    :param indicator: (OUT) **NULL** indicator. (-1 : **NULL**)

     *   If *type* is **CCI_A_TYPE_STR** : -1 is returned in case of **NULL**; the length of string stored in *value* is returned, otherwise.
     *   If *type* is not **CCI_A_TYPE_STR** : -1 is returned in case of **NULL**, 0 is returned, otherwise.

    :return: Error code (0: success)
    
        *   **CCI_ER_REQ_HANDLE**
        *   **CCI_ER_TYPE_CONVERSION**
        *   **CCI_ER_COLUMN_INDEX**
        *   **CCI_ER_ATYPE**

    The *type* of the *value* variable is determined based on the given *type* argument, and the value or the pointer is copied to the *value* variable accordingly. For a value to be copied, the memory for the address to be transferred to the *value* variable must have been previously assigned. Note that if a pointer is copied, a pointer in the application client library is returned, so the value becomes invalid next time the :c:func:`cci_get_data` function is called.

    In addition, the pointer returned by the pointer copy must not be freed. However, if the type is **CCI_A_TYPE_SET**, the memory must be freed by using the :c:func:`cci_set_free` function after using the set because the set is returned after the **T_CCI_SET** type memory is allocated. The following table shows the summary of *type* arguments and data types of their corresponding *value* values.

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

    **Remark**

    *   For **LOB** type, if the :c:func:`cci_get_data` function is called, meta data with the **LOB** type column (locator) is displayed. To call data of the **LOB** type column, the :c:func:`cci_blob_read` function should be called.

    The below example shows a part of a code to print out the fetched result with :c:func:`cci_get_data`.
        
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

    The **cci_get_db_parameter** function gets a parameter value specified in the database.

    :param conn_handle: (IN) Connection handle
    :param param_name: (IN) System parameter name
    :param value: (OUT) Parameter value
    :param err_buf: (OUT) Database error buffer
    :return: Error code (0: success)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_PARAM_NAME**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CONNECT**

    The data type of *value* for *param_name* is shown in the table below.
    
    +---------------------------------+--------------+----------+
    | param_name                      | value Type   | note     |
    +=================================+==============+==========+
    | **CCI_PARAM_ISOLATION_LEVEL**   | int \*       | get/set  |
    +---------------------------------+--------------+----------+
    | **CCI_PARAM_LOCK_TIMEOUT**      | int \*       | get/set  |
    +---------------------------------+--------------+----------+
    | **CCI_PARAM_MAX_STRING_LENGTH** | int \*       | get only |
    +---------------------------------+--------------+----------+
    | **CCI_PARAM_AUTO_COMMIT**       | int \*       | get only |
    +---------------------------------+--------------+----------+  
    
    In :c:func:`cci_get_db_parameter` and :c:func:`cci_set_db_parameter`, the input/output unit of **CCI_PARAM_LOCK_TIMEOUT** is milliseconds.

    .. warning:: In the earlier version of CUBRID 9.0, you should be careful that the output unit of **CCI_PARAM_LOCK_TIMEOUT** is second.

    **CCI_PARAM_MAX_STRING_LENGTH** is measured in bytes and it gets a value defined in the **MAX_STRING_LENGTH** broker parameter.

cci_get_db_version
------------------

.. c:function:: int cci_get_db_version(int conn_handle, char *out_buf, int out_buf_size)

    The **cci_get_db_version** function gets the Database Management System (DBMS) version.

    :param conn_handle: (IN) Connection handle
    :param out_buf: (OUT) Result buffer
    :param out_buf_size: (IN) *out_buf* size
    :return: Error code (0: success)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CONNECT**

cci_get_err_msg
---------------

.. c:function:: int cci_get_err_msg(int err_code, char *msg_buf, int msg_buf_size)

    The **cci_get_err_msg** function stores error messages corresponding to the error code in the error message buffer. For details on error codes and error messages, see :ref:`CCI Error Codes and Error Messages <cci-error-codes>`.

    :param err_code: (IN) Error code
    :param msg_buf: (OUT) Error message buffer
    :param msg_buf_size: (IN) *msg_buf* size
    :return: 0: Success, -1: Failure

    note:: From CUBRID 9.1, CUBRID ensures that all functions which have err_buf parameter store an error value into err_buf parameter, so you don't need to use cci_get_err_msg function if a function has err_buf parameter.
    
        .. code-block:: c
        
            req = cci_prepare (con, query, 0, &err_buf);
            if (req < 0)
            {
                printf ("error: %d, %s\n", err_buf.err_code, err_buf.err_msg);
                goto handle_error;
            }
    
        On the previous version of 9.1, an err_buf value was stored only when CCI_ER_DBMS error occurred. So printing an error message should have been separated by CCI_ER_DBMS error.
        
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
        
        From 9.1, you can simplify the branch of the above code by using cci_get_error_msg function.
        
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

    Saves the error messages corresponding to the CCI error codes in the message buffer. If the value of CCI error code is **CCI_ER_DBMS**, the database error buffer (*err_buf*) receives the error message sent from the data server and saves it in the message buffer(*msg_buf*). For details on error codes and messages, see :ref:`CCI Error Codes and Error Messages <cci-error-codes>`.

    :param err_code: (IN) Error code
    :param err_buf: (IN) Database error buffer
    :param msg_buf: (OUT) Error message buffer
    :param msg_buf_size: (IN) *msg_buf* size
    :return: 0: Success, -1: Failure

cci_get_holdability
-------------------

.. c:function:: int cci_get_holdability(int conn_handle)

    Returns the cursor holdability setting value about the result set from the connection handle. When it is 1, the connection is disconnected or the cursor is holdable until the result set is intentionally closed regardless of commit. When it is 0, the result set is closed when committed and the cursor is not holdable. For more details on cursor holdability, see Cursor Holdability.

    :param conn_handle: (IN) Connection handle
    :return: 0 (not holdable), 1 (holdable)
    
        *   **CCI_ER_CON_HANDLE**

cci_get_last_insert_id
----------------------

.. c:function:: int cci_get_last_insert_id(int conn_handle, void *value, T_CCI_ERROR *err_buf)

    Gets the primary key of the INSERT statement which executed at the last time.
    
    :param conn_handle: (IN) Request handle
    :param value: (OUT) The pointer of the result buffer pointer(char \*\*). It stores the last primary key of INSERT statement executed on the last time. The memory which this pointer indicates doesn't need to be released, because it is the fixed buffer inside the connection handle.
    :param err_buf: (OUT) Error buffer

    :return: Error code (0: success)

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
 
    Return login timeout value to *timeout*.
    
    :param conn_handle: (IN) Connection handle
    :param timeout: (OUT) A pointer to login timeout value(unit: millisecond)
    :param err_buf: (OUT) Error buffer
    :return: Error code (0: success)
 
        *   **CCI_ER_INVALID_ARGS**
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_USED_CONNECTION**
        
cci_get_query_plan
------------------

.. c:function:: int cci_get_query_plan(int req_handle, char **out_buf_p)

    Saves the query plan to the result buffer; the query plan about the request handle which the cci_prepare function returned.
    You can call this function whether to call the cci_execute function or not. 

    After calling the cci_get_query_plan function, if the use of a result buffer ends, you should call the :c:func:`cci_query_info_free` function to release the result buffer created by cci_get_query_plan function.

    ::
        
        char *out_buf;
        ...
        req = cci_prepare (con, query, 0, &cci_error);
        ...
        ret = cci_get_query_plan(req, &out_buf); 
        ...
        printf("plan = %s", out_buf);
        cci_query_info_free(out_buf);
        
    :param req_handle: (IN) Request handle
    :param out_buf_p: (OUT) The pointer of a result buffer pointer
    :return: Error code
        
        *    **CCI_ER_REQ_HANDLE**
        *    **CCI_ER_CON_HANDLE**
        *    **CCI_ER_USED_CONNECTION**
    
    .. seealso:: 
    
        :c:func:`cci_query_info_free`

cci_query_info_free
-------------------

.. c:function:: int cci_query_info_free(char *out_buf)

    Releases the result buffer memory allocated from the :c:func:`cci_get_query_plan` function.

    :param req_handle: (IN) Request handle
    :param out_buf: (OUT) Result buffer pointer
    :return: Error code
    
        *    **CCI_ER_NO_MORE_MEMORY**
        
    .. seealso:: 
    
        :c:func:`cci_get_query_plan`

cci_get_query_timeout
---------------------

.. c:function:: int cci_get_query_timeout (int req_handle)

    The **cci_get_query_timeout** function returns timeout configured for query execution.

    :param req_handle: (IN) Request handle
    :return: Success: Timeout value configured in current request handle (unit: msec.), Failure: Error code
    
        *   CCI_ER_REQ_HANDLE

cci_get_result_info
-------------------

.. c:function:: T_CCI_COL_INFO* cci_get_result_info(int req_handle, T_CCI_CUBRID_STMT *stmt_type, int *num)

    If the prepared statement is **SELECT**, the **T_CCI_COL_INFO** struct that stores the column information about the execution result can be obtained by using this function. If it is not **SELECT**, **NULL** is returned and the *num* value becomes 0.

    :param req_handle: (IN) Request handle for the prepared statement
    :param stmt_type: (OUT) Command type
    :param num: (OUT) he number of columns in the **SELECT** statement (if *stmt_type* is **CUBRID_STMT_SELECT**)
    :return: Success: Result info pointer, Failure: **NULL**

    You can access the **T_CCI_COL_INFO** struct directly to get the column information from the struct, but you can also use a macro to get the information, which is defined as follows. The address of the **T_CCI_COL_INFO** struct and the column index are specified as parameters for each macro. The macro can be called only for the **SELECT** query. Note that the validity check is not performed for each parameter entered in each macro. If the return type of the macro is char*, do not free the memory pointer.

    +--------------------------------------------+------------------+---------------------------+
    | Macro                                      | Return Type      | Meaning                   |
    +============================================+==================+===========================+
    | :c:macro:`CCI_GET_RESULT_INFO_TYPE`        | **T_CCI_U_TYPE** | column type               |
    +--------------------------------------------+------------------+---------------------------+
    | :c:macro:`CCI_GET_RESULT_INFO_SCALE`       | short            | column scale              |
    +--------------------------------------------+------------------+---------------------------+
    | :c:macro:`CCI_GET_RESULT_INFO_PRECISION`   | int              | column precision          |
    +--------------------------------------------+------------------+---------------------------+
    | :c:macro:`CCI_GET_RESULT_INFO_NAME`        | char \*          | column name               |
    +--------------------------------------------+------------------+---------------------------+
    | :c:macro:`CCI_GET_RESULT_INFO_ATTR_NAME`   | char \*          | column attribute name     |
    +--------------------------------------------+------------------+---------------------------+
    | :c:macro:`CCI_GET_RESULT_INFO_CLASS_NAME`  | char \*          | column class name         |
    +--------------------------------------------+------------------+---------------------------+
    | :c:macro:`CCI_GET_RESULT_INFO_IS_NON_NULL` | char (0 or 1)    | whether a column is NULL  |
    +--------------------------------------------+------------------+---------------------------+

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

    The **CCI_GET_RESULT_INFO_ATTR_NAME** macro gets the actual attribute name of the *index*-th column of a prepared **SELECT** list. If there is no name for the attribute (constant, function, etc), " " (empty string) is returned. It does not check whether the specified argument, *res_info*, is **NULL** and whether *index* is valid. You cannot delete the returned memory pointer with **free**\ ().

    :param res_info: (IN) A pointer to the column information fetched by :c:func:`cci_get_result_info`
    :param index: (IN) Column index
    :return: Attribute name (char \*)

CCI_GET_RESULT_INFO_CLASS_NAME
------------------------------

.. c:macro:: #define CCI_GET_RESULT_INFO_CLASS_NAME(T_CCI_COL_INFO* res_info, int index)

    The **CCI_GET_RESULT_INFO_CLASS_NAME** macro gets the *index*-th column's class name of a prepared **SELECT** list. It does not check whether the specified argument, *res_info*, is **NULL** and whether *index* is valid. You cannot delete the returned memory pointer with **free** (). The return value can be **NULL**.

    :param res_info: (IN) Column info pointer by :c:func:`cci_get_result_info`
    :param index: (IN) Column index
    :return: Class name (char \*)

CCI_GET_RESULT_INFO_IS_NON_NULL
-------------------------------

.. c:macro:: #define CCI_GET_RESULT_INFO_IS_NON_NULL(T_CCI_COL_INFO* res_info, int index)

    The **CCI_GET_RESULT_INFO_IS_NON_NULL** macro gets a value indicating whether the *index*-th column of a prepared **SELECT** list is nullable. It does not check whether the specified argument, *res_info*, is **NULL** and whether *index* is valid.

    When a column of a **SELECT** list is not a column but an expression, CUBRID cannot judge it's NON_NULL or not; therefore, CCI_GET_RESULT_INFO_IS_NON_NULL macro always returns 0.

    :param res_info: (IN) Column info pointer by :c:func:`cci_get_result_info`
    :param index: (IN) Column index
    :return: 0: nullable, 1: non **NULL**

CCI_GET_RESULT_INFO_NAME
------------------------

.. c:macro:: #define CCI_GET_RESULT_INFO_NAME(T_CCI_COL_INFO* res_info, int index)

    The **CCI_GET_RESULT_INFO_NAME** macro gets the *index*-th column's name of a prepared **SELECT** list. It does not check whether the specified argument, *res_info*, is **NULL** and whether *index* is valid. You cannot delete the returned memory pointer with **free** ().

    :param res_info: (IN) Column info pointer to :c:func:`cci_get_result_info`
    :param index: (IN) Column index
    :return: Column name (char \*)

CCI_GET_RESULT_INFO_PRECISION
-----------------------------

.. c:macro:: #define CCI_GET_RESULT_INFO_PRECISION(T_CCI_COL_INFO* res_info, int index)

    The **CCI_GET_RESULT_INFO_PRECISION** macro gets the *index*-th column's precision of a prepared **SELECT** list. It does not check whether the specified argument, *res_info*, is **NULL** and whether *index* is valid.

    :param res_info: (IN) Column info pointer by :c:func:`cci_get_result_info`
    :param index: (IN) Column index
    :return: precision (int)

CCI_GET_RESULT_INFO_SCALE
-------------------------

.. c:macro:: #define CCI_GET_RESULT_INFO_SCALE(T_CCI_COL_INFO* res_info, int index)

    The **CCI_GET_RESULT_INFO_SCALE** macro gets the *index*-th column's scale of a prepared **SELECT** list. It does not check whether the specified argument, *res_info*, is **NULL** and whether *index* is valid. 
    
    :param res_info: (IN) Column info pointer by :c:func:`cci_get_result_info`
    :param index: (IN) Column index
    :return: scale (int)

CCI_GET_RESULT_INFO_TYPE
------------------------

.. c:macro:: #define CCI_GET_RESULT_INFO_TYPE(T_CCI_COL_INFO* res_info, int index)

    The **CCI_GET_RESULT_INFO_TYPE** macro gets the *index*-th column's type of a prepared **SELECT** list. It does not check whether the specified argument, *res_info*, is **NULL** and whether *index* is valid.

    If you want to check which column is a SET type or not, use :c:macro:`CCI_IS_SET_TYPE`.

    :param res_info: (IN) pointer to the column information fetched by :c:func:`cci_get_result_info`
    :param index: (IN) Column index
    :return: Column type (**T_CCI_U_TYPE**)

CCI_IS_SET_TYPE
---------------

.. c:macro:: #define CCI_IS_SET_TYPE(u_type)

    The CCI_IS_SET_TYPE macro check whether *u_type* is set type.

    :param u_type: (IN)
    :return: 1 : set, 0 : not set

CCI_IS_MULTISET_TYPE
--------------------
.. c:macro:: #define CCI_IS_MULTISET_TYPE(u_type)

    The CCI_IS_SET_TYPE macro check whether *u_type* is multiset type.

    :param u_type: (IN)
    :return: 1 : multiset, 0 : not multiset

CCI_IS_SEQUENCE_TYPE
--------------------

.. c:macro:: #define CCI_IS_SEQUENCE_TYPE(u_type)

    The CCI_IS_SET_TYPE macro check whether *u_type* is sequence type.

    :param u_type: (IN)
    :return: 1 : sequence, 0 : not sequence

CCI_IS_COLLECTION_TYPE
----------------------

.. c:macro:: #define CCI_IS_COLLECTION_TYPE(u_type)

    The CCI_IS_SET_TYPE macro check whether *u_type* is collection (set, multiset, sequence) type.

    :param u_type: (IN)
    :return: 1 : collection (set, multiset, sequence), 0 : not collection

cci_get_version
---------------

.. c:function:: int cci_get_version(int *major, int *minor, int *patch)

    The cci_get_version function gets the version of CCI library. In case of version "9.2.0.0001", 9 is the major version, 2 is the minor version, and 0 is the patch version.

    :param major: (OUT) major version
    :param minor: (OUT) minor version
    :param patch: (OUT) patch version
    :return: Zero without exception (success)

    .. note:: 
    
        In CUBRID for Linux, you can check the file version of CCI library by using the **strings** command. ::

            $ strings /home/usr1/CUBRID/lib/libcascci.so | grep VERSION
            VERSION=9.2.0.0001

cci_init
--------

.. c:function::  void cci_init()

    If you compile the CCI application program for Windows with static linking library(.lib), this function should be called always. In the other cases, this does not need to be called. 

cci_is_holdable
---------------

.. c:function:: int cci_is_holdable(int req_handle)

    The **cci_is_holdable** function returns whether the request handle(req_handle) is holdable or not.

    :param req_handle: (IN) Request handle for the prepared statement
    :return: 
    
        *   1: holdable
        *   0: not holdable
        *   **CCI_ER_REQ_HANDLE**

    .. seealso:: 
    
        :c:func:`cci_prepare`

cci_is_updatable
----------------

.. c:function:: int cci_is_updatable(int req_handle)

    The **cci_is_updatable** function checks the SQL statement executing :c:func:`cci_prepare` can make updatable result set (which means CCI_PREPARE_UPDATABLE is configured in *flag* when executing :c:func:`cci_prepare`).

    :param req_handle: (IN) Request handle for the prepared statement
    :return: 
    
        *   1 : updatable
        *   0 : not updatable
        *   **CCI_ER_REQ_HANDLE**

cci_next_result
---------------

.. c:function:: int cci_next_result(int req_handle, T_CCI_ERROR *err_buf)

    The **cci_next_result** function gets results of next query if **CCI_EXEC_QUERY_ALL** *flag* is set upon :c:func:`cci_execute`. The information about the query fetched by next_result can be obtained with :c:func:`cci_get_result_info`. If next_result is executed successfully, the database is updated with the information of the current query.

    You can execute multiple queries in the written order when **CCI_EXEC_QUERY_ALL** *flag* is set on :c:func:`cci_execute`. At this time, CUBRID brings the first query's result after calling :c:func:`cci_execute`; CUBRID brings the second and the other queries' results after calling **cci_next_result** function. At this time, the column information about the query result can be brought by calling :c:func:`cci_get_result_info` function whenever :c:func:`cci_execute` function or **cci_next_result** function is called.
    
    In other words, when you run Q1, Q2, and Q3 at once with calling :c:func:`cci_prepare` function, the result of Q1 is brought by calling :c:func:`cci_execute`; the result of Q2 or Q3 is brought by calling **cci_next_result**.
    The result of Q1, Q2 or Q3 is brought by calling :c:func:`cci_get_result_info` function for each time.

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

    :param req_handle: (IN) Request handle of a prepared statement
    :param err_buf: (OUT) Database error buffer        
    :return: 

      *     **SELECT** : The number of results
      *     **INSERT**, **UPDATE** : The number of records reflected
      *     Others : 0
      *     Failure : Error code
    
            *   **CCI_ER_REQ_HANDLE**
            *   **CCI_ER_DBMS**
            *   **CCI_ER_COMMUNICATION**

    The error code **CAS_ER_NO_MORE_RESULT_SET** means that no more result set exists.

cci_oid
-------

.. c:function:: int cci_oid(int conn_handle, T_CCI_OID_CMD cmd, char *oid_str, T_CCI_ERROR *err_buf)

    By the value of `cmd` argument, it executes the following behavior.

    *   CCI_OID_DROP: Deletes the given oid.
    *   CCI_OID_IS_INSTANCE: Checks whether the given oid is an instance oid.
    *   CCI_OID_LOCK_READ: Sets read lock on the given oid.
    *   CCI_OID_LOCK_WRITE: Sets write lock on the given oid.

    :param conn_handle: (IN) Connection handle
    :param cmd: (IN) CCI_OID_DROP, CCI_OID_IS_INSTANCE, CCI_OID_LOCK_READ, CCI_OID_LOCK_WRITE
    :param oid_str:  (IN) oid    
    :param err_buf: (OUT) Database error buffer        
    :return: 

        *   when `cmd` is CCI_OID_IS_INSTANCE
      
            *   0 : Non-instance
            *   1 : Instance
            *   < 0 : error

        *   when `cmd` is CCI_OID_DROP, CCI_OID_LOCK_READ or CCI_OID_LOCK_WRITE
      
            Error code (0: success)
        
            *   **CCI_ER_CON_HANDLE**
            *   **CCI_ER_CONNECT**
            *   **CCI_ER_OID_CMD**
            *   **CCI_ER_OBJECT**
            *   **CCI_ER_DBMS**

cci_oid_get
-----------

.. c:function:: int cci_oid_get(int conn_handle, char *oid_str, char **attr_name, T_CCI_ERROR *err_buf)

    The **cci_oid_get** function gets the attribute values of the given oid. *attr_name* is an array of the attributes, and it must end with **NULL**. If *attr_name* is NULL, the information of all attributes is fetched. The request handle has the same form as when the SQL statement "SELECT attr_name FROM oid_class WHERE oid_class = oid" is executed.

    :param conn_handle: (IN) Connection handle
    :param oid_str: (IN) oid    
    :param attr_name: (IN) A list of attributes    
    :param err_buf: (OUT) Database error buffer
    :return: Success: Request handle, Failure: Error code

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_CONNECT**

cci_oid_get_class_name
----------------------

.. c:function:: int cci_oid_get_class_name(int conn_handle, char *oid_str, char *out_buf, int out_buf_len, T_CCI_ERROR *err_buf)

    The **cci_oid_get_class_name** function gets the class name of the given oid.

    :param conn_handle: (IN) Connection handle
    :param oid_str: (IN) oid    
    :param out_buf: (OUT) Out buffer
    :param out_buf_len: (IN) *out_buf* length
    :param err_buf: (OUT) Database error buffer    
    :return: Error code

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_OBJECT**
        *   **CCI_ER_DBMS**

cci_oid_put
-----------

.. c:function:: int cci_oid_put(int conn_handle, char *oid_str, char **attr_name, char **new_val_str, T_CCI_ERROR *err_buf)

    The **cci_oid_put** function configures the *attr_name* attribute values of the given oid to *new_val_str*. The last value of *attr_name* must be **NULL**. Any value of any type must be represented as a string. The value represented as a string is applied to the database after being converted depending on the attribute type on the server. To insert a **NULL** value, configure the value of *new_val_str* [i] to **NULL**.

    :param conn_handle: (IN) Connection handle
    :param oid_str: (IN) oid    
    :param attr_name: (IN) The list of attribute names
    :param new_val_str: (IN) The list of new values
    :param err_buf: (OUT) Database error buffer        
    :return: Error code (0: success)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**

cci_oid_put2
------------

.. c:function:: int cci_oid_put2(int conn_handle, char *oidstr, char **attr_name, void **new_val, int *a_type, T_CCI_ERROR *err_buf)

    The **cci_oid_put2** function sets the *attr_name* attribute values of the given oid to *new_val*. The last value of *attr_name* must be **NULL**. To insert a **NULL** value, set the value of *new_val* [i] to **NULL**.

    :param conn_handle: (IN) Connection handle
    :param oidstr: (IN) oid    
    :param attr_name: (IN) A list of attribute names
    :param new_val: (IN) A new value array
    :param a_type: (IN) *new_val* type array
    :param err_buf: (OUT) Database error buffer        
    :return: Error code (0: success)
    
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**

    The type of *new_val* [i] for *a_type* is shown in the table below.

    **Type of new_val[i] for a_type**

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
    |                       | (For Windows: __int64 \*)    |
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

    The :c:func:`cci_prepare` function prepares SQL execution by acquiring request handle for SQL statements. If a SQL statement consists of multiple queries, the preparation is performed only for the first query. With the parameter of this function, an address to **T_CCI_ERROR** where connection handle, SQL statement, *flag*, and error information are stored.

    :param conn_handle: (IN) Connection handle
    :param sql_stmt: (IN) SQL statement
    :param flag: (IN) prepare flag (CCI_PREPARE_UPDATABLE, CCI_PREPARE_INCLUDE_OID, CCI_PREPARE_CALL or CCI_PREPARE_HOLDABLE)
    :param err_buf: (OUT) Database error buffer        
    :return: Success: Request handle ID (int), Failure: Error code
    
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_STR_PARAM**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_QUERY_TIMEOUT**
        *   **CCI_ER_LOGIN_TIMEOUT**

    **CCI_PREPARE_UPDATABLE**, **CCI_PREPARE_INCLUDE_OID**, **CCI_PREPARE_CALL** or **CCI_PREPARE_HOLDABLE** can be configured in *flag*. If **CCI_PREPARE_UPDATABLE** is configured, updatable resultset is created and **CCI_PREPARE_INCLUDE_OID** is automatically configured. **CCI_PREPARE_UPDATABLE** and **CCI_PREPARE_HOLDABLE** cannot be used simultaneously in *flag*.
    If you want to call the Java Stored Procedure, specify **CCI_PREPARE_CALL** flag into the **cci_prepare** function. You can see an related example on :c:func:`cci_register_out_param`.
    
    The default value of whether to keep result set after commit is cursor holdability. Thus, if you want to configure **CCI_PREPARE_UPDATABLE** in *flag* of :c:func:`cci_prepare`, you should call :c:func:`cci_set_holdability` first before calling :c:func:`cci_prepare` so that cursor cannot be maintained.

    However, not all updatable resultsets are created even though **CCI_PREPARE_UPDATABLE** is configured. So you need to check if the results are updatable by using :c:func:`cci_is_updatable` after preparation. You can use :c:func:`cci_oid_put` or :c:func:`cci_oid_put2` to update result sets.

    The conditions of updatable queries are as follows:
    
    *   Must be **SELECT**.
    *   OID can be included in the query result.
    *   The column to be updated must be the one that belongs to the table specified in the **FROM** clause.

    If **CCI_PREPARE_HOLDABLE** is set, a cursor is held as long as result set is closed or connection is disconnected after the statement is committed(see :ref:`cursor-holding`).

cci_prepare_and_execute
-----------------------

.. c:function:: int cci_prepare_and_execute(int conn_handle, char *sql_stmt, int max_col_size, int *exec_retval, T_CCI_ERROR *err_buf)

    The **cci_prepare_and_execute** function executes the SQL statement immediately and returns a request handle for the SQL statement. A request handle, SQL statement, the maximum length of a column to be fetched, error code, and the address of a **T_CCI_ERROR** construct variable in which error information being stored are specified as arguments. *max_col_size* is a value to configure the maximum length of a column to be sent to a client when the column of a SQL statement is **CHAR**, **VARCHAR**, **BIT**, or **VARBIT**. If this value is 0, full length is fetched.

    :param conn_handle: (IN) Connection handle
    :param sql_stmt: (IN) SQL statement
    :param max_col_size: (IN) The maximum length of a column to be fetched when it is a string data type in bytes. If this value is 0, full length is fetched.
    :param exec_retval: (OUT) Success: Affected rows, Failure: Error code
    :param err_buf: (OUT) Database error buffer        
    :return: Success: Request handle ID (int), Failure: Error code

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

    The **cci_property_create** function creates **T_CCI_PROPERTIES** struct to configure DATASOURCE of CCI.

    :return: Success: **T_CCI_PROPERTIES** struct pointer in which memory is allocated, Failure: **NULL**

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

    The **cci_property_destroy** function destroys **T_CCI_PROPERTIES** struct.

    :param properties: **T_CCI_PROPERTIES** struct pointer to be destroyed

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

.. c:function:: char *cci_property_get (T_CCI_PROPERTIES * properties, char *key)

    The **cci_property_get** function retrieves the property value configured in the **T_CCI_PROPERTIES** struct.

    :param properties: **T_CCI_PROPERTIES** struct pointer which gets value corresponding to *key*
    :param key: Name of property to be retrieved (For name and description available properties, see the :c:func:`cci_property_set`)
    :return: Success: String pointer of value corresponding to *key*, Failure: NULL

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

    It configures a property value in **T_CCI_PROPERTIES** struct.
    
    :param properties: **T_CCI_PROPERTIES** struct pointer in which *key* and *value* are stored
    :param key: String pointer of property name
    :param value: String pointer of property value
    :return: Success: 1, Failure: 0

    The property names and its meanings that can be configured in the struct are as follows:

    ============================= =========== =============================================== ==========================================================================================================
    Property name                 Type        Default                                         Description
    ============================= =========== =============================================== ==========================================================================================================
    user                          string                                                      DB user name.
    password                      string                                                      DB user password.
    url                           string                                                      Connection URL. For specifying connection URL string, see :c:func:`cci_connect_with_url`.
    pool_size                     int         10                                              Maximum number of connections which a connection pool can have.
    max_pool_size                 int         pool_size                                       Total number of connections to create when creating an initial datasource. maximum value is INT_MAX.
    max_wait                      msec        1000                                            Maximum waiting time to get connection.
    pool_prepared_statement       bool        false                                           Whether to enable statement pooling. true or false.
    max_open_prepared_statement   int         1000                                            The maximum number of prepared statements to maintain in the statement pool.
    login_timeout                 msec        0(inf.)                                         Login timeout applied when you create a datasource with :c:func:`cci_datasource_create` function        
                                                                                              or internal timeout occurs in prepare/execute function.    
    query_timeout                 msec        0(inf.)                                         Query timeout.
    disconnect_on_query_timeout   bool        no                                              Whether to terminate connection when execution is discarded due to query execution timeout. yes or no.
    default_autocommit            bool        CCI_DEFAULT_AUTOCOMMIT in cubrid_broker.conf    Auto-commit mode specified when a datasource is created by :c:func:`cci_datasource_create` function.
                                                                                              true or false.
    default_isolation             string      isolation_level in cubrid.conf                  Transaction isolation level specified when a datasource is created by :c:func:`cci_datasource_create`  
                                                                                              function. See the below table.
    default_lock_timeout          msec        lock_timeout in cubrid.conf                     lock timeout specified when a datasource is created by :c:func:`cci_datasource_create` function.
    ============================= =========== =============================================== ==========================================================================================================
    
    If the number of prepared statements exceeds **max_open_prepared_statement** value, the oldest prepared statement is released from the statement pool. If you reuse it later, it is added to the statement pool again.
    
    A number of connections which a connection pool can contain is changable by using :c:func:`cci_datasource_change_property` function when it's required, but it cannot exceed **max_pool_size**. You can change this number when you want to handle the limitation of the number of connections. For example, usually set the number less than **max_pool_size**; raise the number when more connections are required than expected; shrink the number again when many connections are not required.
    
    The number of connections which a connection pool can contain is limited until **pool_size**; it's maximum value is **max_pool_size**.
        
    When you set **login_timeout**, **default_autocommit**, **default_isolation** or **default_lock_timeout**, these specified values are used in a connection which is returend when calling :c:func:`cci_datasource_borrow` function.
 
    **login_timeout**, **default_autocommit**, **default_isolation** or **default_lock_timeout** can be changed even after calling :c:func:`cci_datasource_borrow` function. Regarding this, see :c:func:`cci_set_login_timeout`, :c:func:`cci_set_autocommit`, :c:func:`cci_set_isolation_level`, :c:func:`cci_set_lock_timeout`. Changed values by functions which the name starts with **cci_set_** are only applied to the changed connection objects, and after the connection is released, they are restored as the specified values by :c:func:`cci_property_set`. It is restored as the default value when there is no value specified by :c:func:`cci_property_set` function.
    
    .. note:: 
    
        *   When specifying values together in :c:func:`cci_property_set` function and URL string, values specified in :c:func:`cci_property_set` function have the first priority.

        *   **login_timeout** is applied when you create a DATASOURCE object and internal reconnection occurs in :c:func:`cci_prepare` or :c:func:`cci_execute` function. Internal reconnection occurs when you get the connection object from DATASOURCE and from :c:func:`cci_connect`/:c:func:`cci_connect_with_url` function.
        
        *   Creating time of a DATASOURCE object can take more than an internal reconnection time; therefore, you can consider to apply  **login_timeout** differently on these two cases. For example, if you want to set 5000(5 sec.) in the former and 2000(2 sec.) in the later, for the later, use :c:func:`cci_set_login_timeout` after creating a DATASOURCE.

        .. CUBRIDSUS-12567
    
    **default_isolation** has one of the following configuration values. For details on isolation level, see :ref:`set-transaction-isolation-level`.

    +----------------------------+---------------------------------------+
    | isolation_level            | Configuration Value                   |
    +============================+=======================================+
    | SERIALIZABLE               | "TRAN_SERIALIZABLE"                   |
    +----------------------------+---------------------------------------+
    | REPEATABLE READ            | "TRAN_REP_READ"                       |
    +----------------------------+---------------------------------------+
    | READ COMMITTED             | "TRAN_READ_COMMITTED"                 |
    +----------------------------+---------------------------------------+

    DB user's name and password can be specified by the setting of **user** and **password** directly, or by the setting of **user** and **password** in **url**.

    The following shows how to work as the first priority if both are specified.
    
    *   If both are specified, the value of direct setting will be first.
    *   If one of them is NULL, the value which is not NULL is used.
    *   If both are NULL, they are used as NULL value.
    *   If the direct setting value of DB user is NULL then "public" is used, else if the direct setting value of the password is NULL then NULL is used.
    *   If the direct setting value of the password is NULL, it follows the setting of URL.
    
    The following shows that the DB user's name becomes "dba" and the password becomes "cubridpwd".
    
    ::
    
        cci_property_set(ps, "user", "dba");
        cci_property_set(ps, "password", "cubridpwd");
          ...
        cci_property_set(ps, "url", "cci:cubrid:192.168.0.1:33000:demodb:public:mypwd:?logSlowQueries=true&slowQueryThresholdMillis=1000&logTraceApi=true&logTraceNetwork=true");

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

    The **cci_query_result_free** function releases query results created by :c:func:`cci_execute_batch`, :c:func:`cci_execute_array` or :c:func:`cci_execute_result` function from memory.

    :param query_result: (IN) Query results to release from memory
    :param num_query: (IN) The number of arrays in *query_result*
    :return: 0: success

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

    Since query results performed by :c:func:`cci_execute_batch`, :c:func:`cci_execute_array`, or :c:func:`cci_execute_result` function are stored as an array of **T_CCI_QUERY_RESULT** structure, you need to check the query result for each item of the array.
    
    **CCI_QUERY_RESULT_ERR_NO** fetches the error number for the array item specified as *index*, if it is not an error, it returns 0. 

    :param query_result: (IN) Query result to retrieve
    :param index: (IN) Index of the result array(base :1). It represents a specific location of the result array.
    
    :return: Error number

CCI_QUERY_RESULT_ERR_MSG
------------------------

.. c:macro:: #define CCI_QUERY_RESULT_ERR_MSG(T_CCI_QUERY_RESULT* query_result, int index)

    The **CCI_QUERY_RESULT_ERR_MSG** macro gets error messages about query results executed by :c:func:`cci_execute_batch`, :c:func:`cci_execute_array` or :c:func:`cci_execute_result` function. If there is no error message, this macro returns ""(empty string). It does not check whether the specified argument, *query_result*, is **NULL**, and whether *index* is valid.

    :param query_result: (IN) Query results of to be executed
    :param index: (IN) Column index (base: 1)
    :return: Error message

CCI_QUERY_RESULT_RESULT
-----------------------

.. c:macro:: #define CCI_QUERY_RESULT_RESULT(T_CCI_QUERY_RESULT* query_result, int index)

    The **CCI_QUERY_RESULT_RESULT** macro gets the result count executed by :c:func:`cci_execute_batch`, :c:func:`cci_execute_array` or :c:func:`cci_execute_result` function. It does not check whether the specified argument, *query_result*, is **NULL** and whether *index* is valid.

    :param query_result: (IN) Query results to be retrieved
    :param index: (IN) Column index (base: 1)
    :return: result count

CCI_QUERY_RESULT_STMT_TYPE
--------------------------

.. c:macro:: #define CCI_QUERY_RESULT_STMT_TYPE(T_CCI_QUERY_RESULT* query_result, int index)

    Since query results performed by :c:func:`cci_execute_batch`, :c:func:`cci_execute_array` or :c:func:`cci_execute_result` fuction are stored as an array of T_CCI_QUERY_RESULT  type, you need to check the query result for each item of the array. 
    
    The **CCI_QUERY_RESULT_STMT_TYPE** macro gets the statement type for the array items specified as *index*.
    
    It does not check whether the specified argument, *query_result*, is **NULL** and whether *index* is valid.

    :param query_result: (IN) Query results to be retrieved
    :param index: (IN) Column index (base: 1)
    :return: statement type (**T_CCI_CUBRID_STMT**)

cci_register_out_param
----------------------

.. c:function:: int cci_register_out_param(int req_handle, int index)

    The **cci_register_out_param** function is used to bind the parameters as outbind in Java Stored Procedure. The index value begins from 1.
    To call this function, **CCI_PREPARE_CALL** flag of **cci_prepare** function should be specified.

    :param req_handle: (IN) Request handle
    :return: Error code
            
        *    **CCI_ER_BIND_INDEX**
        *    **CCI_ER_REQ_HANDLE**
        *    **CCI_ER_CON_HANDLE**
        *    **CCI_ER_USED_CONNECTION**
    
    The following shows to print out "Hello, CUBRID" string with Java Stored Procedure.
    
    To use Java Stored Procedure, firstly specify **java_stored_procedure** parameter in cubrid.conf as **yes**, then start the database.
    
    ::
    
        $ vi cubrid.conf
        java_stored_procedure=yes
    
        $ cubrid service start
        $ cubrid server start demodb
    
    Implement and compile the class to be used as Java Stored Procedure.
    
    .. code-block:: java
    
        public class SpCubrid{
            public static void outTest(String[] o) {
                o[0] = "Hello, CUBRID";
            }
        }
        
        %javac SpCubrid.java
    
    Load the compiled Java class into CUBRID.
    
    ::
        
        $ loadjava demodb SpCubrid.class
        
    Register the loaded Java class.
    
    .. code-block:: sql
        
        -- csql> 
        
        CREATE PROCEDURE test_out(x OUT STRING)
        AS LANGUAGE JAVA
        NAME 'SpCubrid.outTest(java.lang.String[] o)';
        
     On the CCI application program, specify **CCI_PREPARE_CALL** flag into the **cci_prepare** function, and bring the fetching result after setting the position of the outbind parameter by calling **cci_register_out_param**.
    
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

    The **cci_row_count** function gets the number of rows affected by the last executed query.
    
    :param conn_handle: (IN) Connection handle
    :param row_count: (OUT)  The number of rows affected by the last executed query
    :param err_buf: (OUT) Error buffer
    
    :return: Error code
    
        *    **CCI_ER_COMMUNICATION**
        *    **CCI_ER_LOGIN_TIMEOUT**
        *    **CCI_ER_NO_MORE_MEMORY**
        *    **CCI_ER_DBMS**

cci_savepoint
-------------

.. c:function:: int cci_savepoint(int conn_handle, T_CCI_SAVEPOINT_CMD cmd, char* savepoint_name, T_CCI_ERROR *err_buf)

    The **cci_savepoint** function configures savepoint or performs transaction rollback to a specified savepoint. If *cmd* is set to **CCI_SP_SET**, it configures savepoint and if it is set to **CCI_SP_ROLLBACK**, it rolls back transaction to specified savepoint.

    :param conn_handle: (IN) Connection handle
    :param cmd: (IN) CCI_SP_SET or CCI_SP_ROLLBACK
    :param savepoint_name: (IN) Savepoint name
    :param err_buf: (OUT) Database error buffer
    :return: Error code

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_QUERY_TIMEOUT**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_DBMS**

    .. code-block:: c

        con = cci_connect( ... );
        ... /* query execute */

        /* Sets the savepoint named "savepoint1" */
        cci_savepoint(con, CCI_SP_SET, "savepoint1", err_buf);

        ... /* query execute */

        /* Rolls back to specified savepoint,"savepoint1" */
        cci_savepoint(con, CCI_SP_ROLLBACK, "savepoint1", err_buf);

cci_schema_info
---------------

.. c:function:: int cci_schema_info(int conn_handle, T_CCI_SCHEMA_TYPE type, char *class_name, char *attr_name, char flag, T_CCI_ERROR *err_buf)

    The **cci_schema_info** function gets schema information. If it is performed successfully, the results are managed by the request handle and can be fetched by fetch and getdata. If you want to retrieve a *class_name* and *attr_name* by using pattern matching of the **LIKE** statement, you should configure *flag*.

    :param conn_handle: (IN) Connection handle
    :param type: (IN) Schema type
    :param class_name: (IN) Class name or NULL
    :param attr_name: (IN) Attribute name or NULL
    :param flag: (IN) Pattern matching flag (**CCI_CLASS_NAME_PATTERN_MATCH** or **CCI_ATTR_NAME_PATTERN_MATCH**)
    :param err_buf: (OUT) Database error buffer
    :return: Success: Request handle, Failure: Error code

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_CONNECT**

    Two types of *flag* s, **CCI_CLASS_NAME_PATTERN_MATCH**, and **CCI_ATTR_NAME_PATTERN_MATCH**, are used for pattern matching; you can configure these two *flag* s by using the OR operator ( | ). To use pattern matching, search by using the **LIKE** statement. For example, to search the information on a column whose *class_name* is "athlete" and *attr_name* is "code," you can enter as follows (in the example, "%code" is entered in the value of *attr_name*). ::

        cci_schema_info(conn, CCI_SCH_ATTRIBUTE, "athlete", "%code", CCI_ATTR_NAME_PATTERN_MATCH, &error);
    
    The table below shows record type for each *type*.

    **Record for Each Type**

    +--------------------------------------------------------------------------------------------------------------------+------------------+--------------------+---------------------+
    | Type                                                                                                               | Column Order     | Column Name        | Column Type         |
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
    | When the attribute of the CCI_SCH_CLASS_ATTRIBUTE column is INSTANCE or SHARED,                                    |                  |                    |                     |
    | the order and the name values are identical to those of the column of CCI_SCH_ATTRIBUTE.                           |                  |                    |                     |
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
    | Used to retrieve primary key columns that are referred by a foreign key column in a given table.                   |                  |                    |                     |
    | The results are sorted by PKTABLE_NAME and KEY_SEQ.                                                                |                  |                    |                     |
    |                                                                                                                    |                  |                    |                     |
    | If this type is specified as a parameter, a foreign key table is specified for                                     |                  |                    |                     |
    | class_name                                                                                                         |                  |                    |                     |
    | , and                                                                                                              |                  |                    |                     |
    | NULL                                                                                                               |                  |                    |                     |
    | is specified for                                                                                                   |                  |                    |                     |
    | attr_name.                                                                                                         |                  |                    |                     |
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
    | Used to retrieve primary key columns that are referred by all foreign key columns.                                 |                  |                    |                     |
    | The results are sorted by FKTABLE_NAME and KEY_SEQ.                                                                |                  |                    |                     |
    | If this type is specified as a parameter, a primary key table is specified for                                     |                  |                    |                     |
    | class_name                                                                                                         |                  |                    |                     |
    | , and                                                                                                              |                  |                    |                     |
    | NULL                                                                                                               |                  |                    |                     |
    | is specified for                                                                                                   |                  |                    |                     |
    | attr_name.                                                                                                         |                  |                    |                     |
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
    | Used to retrieve foreign key information when primary keys and foreign keys in a given table are cross referenced. |                  |                    |                     |
    | The results are sorted by FKTABLE_NAME and KEY_SEQ.                                                                |                  |                    |                     |
    |                                                                                                                    |                  |                    |                     |
    | If this type is specified as a parameter, a primary key is specified for                                           |                  |                    |                     |
    | class_name                                                                                                         |                  |                    |                     |
    | , and a foreign key table is specified for                                                                         |                  |                    |                     |
    | attr_name.                                                                                                         |                  |                    |                     |
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

    In the **cci_schema_info** function, the *type* argument supports the pattern matching of the **LIKE** statement for the *class_name* and *attr_name*.

    **type, class_name(table name), and attr_name(column name) That Supports Pattern Matching**

    +-------------------------------------+----------------+------------------------------------------------------------------------+
    | type                                | class_name     | attr_name                                                              |
    +=====================================+================+========================================================================+
    | CCI_SCH_CLASS (VCLASS)              | string or NULL | always NULL                                                            |
    +-------------------------------------+----------------+------------------------------------------------------------------------+
    | CCI_SCH_ATTRIBUTE (CLASS ATTRIBUTE) | string or NULL | string or NULL                                                         |
    +-------------------------------------+----------------+------------------------------------------------------------------------+
    | CCI_SCH_CLASS_PRIVILEGE             | string or NULL | always NULL                                                            |
    +-------------------------------------+----------------+------------------------------------------------------------------------+
    | CCI_SCH_ATTR_PRIVILEGE              | always NULL    | string or NULL                                                         |
    +-------------------------------------+----------------+------------------------------------------------------------------------+
    | CCI_SCH_PRIMARY_KEY                 | string or NULL | always NULL                                                            |
    +-------------------------------------+----------------+------------------------------------------------------------------------+
    | CCI_SCH_TRIGGER                     | string or NULL | always NULL                                                            |
    +-------------------------------------+----------------+------------------------------------------------------------------------+

    *   The type with "NULL" in class_name doesn't support the pattern matching about the table name.
    *   The type with "NULL" in attr_name doesn't support the pattern matching about the column name.
    *   If the pattern *flag* is not configured, exact matching will be used for the given table and column names; in this case, no result will be returned if the value is **NULL**.
    *   If *flag* is configured and the value is **NULL**, the result will be the same as when "%" is given in the **LIKE** statement. In other words, the result about all tables or all columns will be returned.

    .. note::
    
        TYPE column of **CCI_SCH_CLASS** and **CCI_SCH_VCLASS**: The proxy type is added. When used in OLEDB, ODBC or PHP, vclass is represented without distinguishing between proxy and vclass.

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

    The **cci_set_allocators** function registers the memory allocation/release functions used by users. By executing this function, you can use user-defined functions for every memory allocation/release jobs being processed in CCI API. If you do not use this function, system functions (malloc, free, realloc, and calloc) are used.

    .. note:: This function can be used only on Linux, so cannot be used on Windows.

    :param malloc_func: (IN) Pointer of externally defined function corresponding to malloc
    :param free_func: (IN) Pointer of externally defined function corresponding to free
    :param realloc_func: (IN) Pointer of externally defined function corresponding to realloc
    :param calloc_func: Pointer of externally defined function corresponding to calloc
    :return: Error code (0: success)

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

    The **cci_set_autocommit** function configures the auto-commit mode of current database connection. It is only used to turn ON/OFF of auto-commit mode. When this function is called, every transaction being processed is committed regardless of configured mode.

    .. note:: :ref:`CCI_DEFAULT_AUTOCOMMIT <cci_default_autocommit>` in **cubrid_broker.conf** determines the default autocommit mode upon program startup.

    :param conn_handle: (IN) Connection handle
    :param autocommit_mode: (IN) Configures the auto-commit mode. It has one of the following value: CCI_AUTOCOMMIT_FALSE or CCI_AUTOCOMMIT_TRUE
    :return: Error code (0: success)

    .. note::
    
        **CCI_DEFAULT_AUTOCOMMIT**, a broker parameter configured in the **cubrid_broker.conf** file, determines whether it is in auto-commit mode upon program startup.

cci_set_db_parameter
---------------------

.. c:function:: int cci_set_db_parameter(int conn_handle, T_CCI_DB_PARAM param_name, void* value, T_CCI_ERROR *err_buf)

    The **cci_set_db_parameter** function configures a system parameter. For the type of *value* for *param_name*, see :c:func:`cci_get_db_parameter`.

    :param conn_handle: (IN) Connection handle
    :param param_name: (IN) System parameter name
    :param value: (IN) Parameter value
    :param err_buf: (OUT) Database error buffer
    :return: Error code (0: success)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_PARAM_NAME**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CONNECT**

cci_set_element_type
--------------------

.. c:function:: int cci_set_element_type(T_CCI_SET set)

    The **cci_set_element_type** function gets the element type of the **T_CCI_SET** type value.

    :param set: (IN) cci set pointer
    :return: Type

cci_set_free
------------

.. c:function:: void cci_set_free(T_CCI_SET set)

    The **cci_set_free** function releases the memory allocated to the type value of **T_CCI_SET** fetched by **CCI_A_TYPE_SET** with :c:func:`cci_get_data`. The **T_CCI_SET** type value can be created through fetching :c:func:`cci_get_data` or :c:func:`cci_set_make` function.

    :param set: (IN) cci set pointer

cci_set_get
-----------

.. c:function:: int cci_set_get(T_CCI_SET set, int index, T_CCI_A_TYPE a_type, void *value, int *indicator)

    The **cci_set_get** function gets the *index* -th data for the type value of **T_CCI_SET**.
    
    :param set: (IN) cci set pointer
    :param index: (IN) set index (base: 1)
    :param a_type: (IN) Type
    :param value: (OUT) Result buffer
    :param indicator: (OUT) null indicator
    :return: Error code

        *   **CCI_ER_SET_INDEX**
        *   **CCI_ER_TYPE_CONVERSION**
        *   **CCI_ER_NO_MORE_MEMORY**
        *   **CCI_ER_COMMUNICATION**

    The data type of *value* for *a_type* is shown in the table below.

    +-----------------------+------------------------------+
    |   a_type              | value type                   |
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
    |                       | (For Windows: __int64 \*)    |
    +-----------------------+------------------------------+

cci_set_holdability
-------------------

.. c:function:: int cci_set_holdability(int conn_handle,int holdable)

    Sets whether to enable or disable cursor holdability of the result set from the connection level. When it is 1, the connection is disconnected or the cursor is holdable until the result set is intentionally closed regardless of commit. When it is 0, the result set is closed when committed and the cursor is not holdable. For more details on cursor holdability, see :ref:`cursor-holding`.

    :param conn_handle: (IN) Connection handle
    :param holdable: (IN) Cursor holdability setting value (0: not holdable, 1: holdable)
    :return: Error Code
    
        *   **CCI_ER_INVALID_HOLDABILITY**

cci_set_isolation_level
-----------------------

.. c:function:: int cci_set_isolation_level(int conn_handle, T_CCI_TRAN_ISOLATION  new_isolation_level, T_CCI_ERROR *err_buf)

    The **cci_set_isolation_level** function sets the transaction isolation level of connections. All further transactions for the given connections work as *new_isolation_level*.

    :param conn_handle: (IN) Connection handle
    :param new_isolation_level: (IN) Transaction isolation level
    :param err_buf: (OUT) Database error buffer
    :return: Error code

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_CONNECT**
        *   **CCI_ER_ISOLATION_LEVEL**
        *   **CCI_ER_DBMS**

    **Note** If the transaction isolation level is set by cci_set_db_parameter(), only the current transaction is affected. When the transaction is complete, the transaction isolation level returns to the one set by CAS. You must use **cci_set_isolation_level** () to set the isolation level for the entire connection.

cci_set_lock_timeout
--------------------

.. c:function:: int cci_set_lock_timeout(int conn_handle, int locktimeout, T_CCI_ERROR * err_buf)

    The **cci_set_lock_timeout** function specifies the connection lock timeout as milliseconds.
    This is the same with calling **cci_set_db_parameter** (conn_id, CCI_PARAM_LOCK_TIMEOUT, &val, err_buf). See :c:func:`cci_set_db_parameter`.
    
    :param conn_handle: (IN) Connection handle
    :param locktimeout: (IN) lock timeout value(Unit: milliseconds).
    :return: Error code(0: success)

        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_PARAM_NAME**
        *   **CCI_ER_DBMS**
        *   **CCI_ER_COMMUNICATION**
        *   **CCI_ER_CONNECT**

cci_set_login_timeout
---------------------

.. c:function:: int cci_set_login_timeout(int conn_handle, int timeout, T_CCI_ERROR *err_buf)
    
    The **cci_set_login_timeout** function specifies the login timeout as milliseconds. The login timeout is applied when an internal reconnection occurs as :c:func:`cci_prepare` or :c:func:`cci_execute` function is called. This change is only applied to the current connection.
   
    :param conn_handle: (IN) Connection handle
    :param timeout: (IN) Login timeout(unit: milliseconds)
    :param err_buf: (OUT) Error buffer
    :return: Error code(0: success)
        
        *   **CCI_ER_CON_HANDLE**
        *   **CCI_ER_USED_CONNECTION**

cci_set_make
------------

.. c:function:: int cci_set_make(T_CCI_SET *set, T_CCI_U_TYPE u_type, int size, void *value, int *indicator)

    The **cci_set_make** function makes a set of a new **CCI_A_TYPE_SET** type. The created set is sent to the server as **CCI_A_TYPE_SET** by :c:func:`cci_bind_param`. The memory for the set created by **cci_set_make()** must be freed by :c:func:`cci_set_free`. The type of *value* for *u_type* is shown in the table below.

    :param set: (OUT) cci set pointer
    :param u_type: (IN) Element type
    :param size: (IN) set size
    :param value: (IN) set element
    :param indicator: (IN) null indicator array
    :return: Error code

cci_set_max_row
---------------

.. c:function:: int cci_set_max_row(int req_handle, int max)

    The **cci_set_max_row** function configures the maximum number of records for the results of the **SELECT** statement executed by :c:func:`cci_execute`. If the *max* value is 0, it is the same as not setting the value.

    :param req_handle: (IN) Connection handle
    :param max: (IN) The maximum number of rows
    :return: Error code

    .. code-block:: c

        req = cci_prepare( ... );
        cci_set_max_row(req, 1);
        cci_execute( ... );

cci_set_query_timeout
---------------------

.. c:function:: int cci_set_query_timeout(int req_handle, int milli_sec)

    The **cci_set_query_timeout** function configures timeout value for query execution.

    :param req_handle: (IN) Request handle
    :param milli_sec:  Timeout (unit: msec.)
    :return: Success: Request handle ID (int), Failure: Error code

        *   CCI_ER_REQ_HANDLE

    The timeout value configured by **cci_set_query_timeout** affects :c:func:`cci_prepare`, :c:func:`cci_execute`, :c:func:`cci_execute_array`, :c:func:`cci_execute_batch` functions. When timeout occurs in the function and if the **disconnect_on_query_timeout** value configured in :c:func:`cci_connect_with_url` connection URL is yes, it returns the **CCI_ER_QUERY_TIMEOUT** error.

    These functions can return the **CCI_ER_LOGIN_TIMEOUT** error if **login_timeout** is configured in the connection URL, which is an argument of :c:func:`cci_connect_with_url` function; this means that login timeout happens between application client and CAS during re-connection.

    It is going through the process of re-connection between application client and CAS when an application restarts or it is re-scheduled. Re-scheduling is a process that CAS chooses an application client, and starts and stops connection in the unit of transaction. If **KEEP_CONNECTION**, broker parameter, is OFF, it always happens; if AUTO, it can happen depending on its situation. For details, see the description of **KEEP_CONNECTION** in the :ref:`parameter-by-broker`

cci_set_size
------------

.. c:function:: int cci_set_size(T_CCI_SET set)

    The **cci_set_size** function gets the number of elements for the type value of **T_CCI_SET**.

    :param set: (IN) cci set pointer
    :return: Size
