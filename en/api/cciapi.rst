****************
CCI API 레퍼런스
****************

**CCI API**

**cci_bind_param**

**Description**

The
**cci_bind_param**
function binds data in the
*bind*
variable of prepared statement. This function converts
*value*
of the given
*a_type*
to an actual binding type and stores it. Subsequently, whenever
`cci_execute <#api_api_cci_execute_htm>`_
() is called, the stored data is sent to the server. If
**cci_bind_param**
() is called multiple times for the same
*index*
, the latest configured value is valid.

If
**NULL**
is bound to the database, there can be two scenarios.

*   When the value of
    *value*
    is a
    **NULL**
    pointer:



*   When the value of
    *u_type*
    is
    **CCI_U_TYPE_NULL**
    :



If
**CCI_BIND_PTR**
is configured for
*flag*
, the pointer of
*value*
variable is copied (shallow copy), but no value is copied. If it is not configured for
*flag*
, the value of
*value*
variable is copied (deep copy) by allocating memory. If multiple columns are bound by using the same memory buffer,
**CCI_BIND_PTR**
must not be configured for the
*flag*
.

**T_CCI_A_TYPE**
is a C language type that is used in CCI applications for data binding, and consists of primitive types such as int and float, and user-defined types defined by CCI such as
**T_CCI_BIT**
and
**T_CCI_DATE**
. The identifier for each type is defined as shown in the table below.

+-----------------------+-------------------------+
| **a_type**            | **value type**          |
|                       |                         |
+-----------------------+-------------------------+
| **CCI_A_TYPE_STR**    | char*                   |
|                       |                         |
+-----------------------+-------------------------+
| **CCI_A_TYPE_INT**    | int*                    |
|                       |                         |
+-----------------------+-------------------------+
| **CCI_A_TYPE_FLOAT**  | float*                  |
|                       |                         |
+-----------------------+-------------------------+
| **CCI_A_TYPE_DOUBLE** | double*                 |
|                       |                         |
+-----------------------+-------------------------+
| **CCI_A_TYPE_BIT**    | **T_CCI_BIT**           |
|                       | *                       |
|                       |                         |
+-----------------------+-------------------------+
| **CCI_A_TYPE_SET**    | **T_CCI_SET**           |
|                       | *                       |
|                       |                         |
+-----------------------+-------------------------+
| **CCI_A_TYPE_DATE**   | **T_CCI_DATE**          |
|                       | *                       |
|                       |                         |
+-----------------------+-------------------------+
| **CCI_A_TYPE_BIGINT** | int64_t*                |
|                       | (For Windows: __int64*) |
|                       |                         |
+-----------------------+-------------------------+
| **CCI_A_TYPE_BLOB**   | **T_CCI_BLOB**          |
|                       |                         |
+-----------------------+-------------------------+
| **CCI_A_TYPE_CLOB**   | **T_CCI_CLOB**          |
|                       |                         |
+-----------------------+-------------------------+

**T_CCI_U_TYPE**
is a column type of database and data bound though the
*value*
argument is converted into this type. The
**cci_bind_param**
() function uses two kinds of types to send information which is used to convert U-type data from A-type data; the U-type data can be interpreted by database language and the A-type data can be interpreted by C language.

There are various A-type data that are allowed by U-type data. For example,
**CCI_U_TYPE_INT**
can receive
**CCI_A_TYPE_STR**
as A-type data including
**CCI_A_TYPE_INT**
. For information on type conversion, see "CUBRID SQL Guide > Data Types > Implicit Type Conversion > Rules."

Both
**T_CCI_A_TYPE**
and
**T_CCI_U_TYPE**
enum(s) are defined in the
**cas_cci.h**
file. The definition of each identifier is described in the table below.

+--------------------------+------------------------------------+
| **u_type**               | **Corresponding a_type (default)** |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_CHAR**      | **CCI_A_TYPE_STR**                 |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_STRING**    | **CCI_A_TYPE_STR**                 |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_NCHAR**     | **CCI_A_TYPE_STR**                 |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_VARNCHAR**  | **CCI_A_TYPE_STR**                 |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_BIT**       | **CCI_A_TYPE_BIT**                 |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_VARBIT**    | **CCI_A_TYPE_BIT**                 |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_NUMERIC**   | **CCI_A_TYPE_STR**                 |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_INT**       | **CCI_A_TYPE_INT**                 |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_SHORT**     | **CCI_A_TYPE_INT**                 |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_MONETARY**  | **CCI_A_TYPE_DOUBLE**              |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_FLOAT**     | **CCI_A_TYPE_FLOAT**               |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_DOUBLE**    | **CCI_A_TYPE_DOUBLE**              |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_DATE**      | **CCI_A_TYPE_DATE**                |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_TIME**      | **CCI_A_TYPE_DATE**                |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_TIMESTAMP** | **CCI_A_TYPE_DATE**                |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_OBJECT**    | **CCI_A_TYPE_STR**                 |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_BIGINT**    | **CCI_A_TYPE_BIGINT**              |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_DATETIME**  | **CCI_A_TYPE_DATE**                |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_BLOB**      | **CCI_A_TYPE_BLOB**                |
|                          |                                    |
+--------------------------+------------------------------------+
| **CCI_U_TYPE_CLOB**      | **CCI_A_TYPE_CLOB**                |
|                          |                                    |
+--------------------------+------------------------------------+

When the string including the date is used as an input parameter of
**DATE**
,
**DATETIME**
, or
**TIMESTAMP**
, only "YYYY/MM/DD" is allowed for the date string type. Therefore, "2012/01/31" is valid, but "01/31/2012" or "2012-01-31" is invalid. The following is an example of having the string that includes the date as an input parameter of the date type.

// "CREATE TABLE tbl(aa date, bb datetime)";

 

char *values[][3] =

{

    {"1994/11/30", "1994/11/30 20:08:08"},

    {"2008/10/31", "2008/10/31 20:08:08"}

};

req = cci_prepare(conn, "insert into tbl (aa, bb) values ( ?, ?)", CCI_PREPARE_INCLUDE_OID, &error);

for(i=0; i< 2; i++)

{

    res = cci_bind_param(req, 1, CCI_A_TYPE_STR, values[i][0], CCI_U_TYPE_DATE, (char)NULL);

    res = cci_bind_param(req, 2, CCI_A_TYPE_STR, values[i][1], CCI_U_TYPE_DATETIME, (char)NULL);

    cci_execute(req, CCI_EXEC_QUERY_ALL, 0, err_buf);

…

**Syntax**

int
**cci_bind_param**
(int
*req_handle*
, int
*index*
,
**T_CCI_A_TYPE**
*a_type*
, void *
*value*
,
**T_CCI_U_TYPE**
*u_type*
, char
*flag*
)

*   *req_handle*
    : (IN) Request handle of a prepared statement



*   *index*
    : (IN) Location of binding; it starts with 1.



*   *a_type*
    : (IN) Data type of
    *value*



*   *value*
    : (IN) Data value to bind



*   *u_type*
    : (IN) Data type to be applied to the database



*   *flag*
    : (IN) bind_flag (
    **CCI_BIND_PTR**
    )



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_REQ_HANDLE**



*   **CCI_ER_TYPE_CONVERSION**



*   **CCI_ER_BIND_INDEX**



*   **CCI_ER_ATYPE**



*   **CCI_ER_NO_MORE_MEMORY**



**cci_bind_param_array**

**Description**

The
**cci_bind_param_array**
function binds a parameter array for a prepared
`cci_execute_array <#api_api_cci_executearray_htm>`_
() occurs, data is sent to the server by the stored
*value*
pointer. If
**cci_bind_param_array**
() is called multiple times for the same
*index*
, the last configured value is valid. If
**NULL**
is bound to the data, a non-zero value is configured in
*null_ind*
.

If
*value*
is a
**NULL**
pointer, or
*u_type*
is
**CCI_U_TYPE_NULL**
, all data are bound to
**NULL**
and the data buffer used by
*value*
cannot be reused.

For the data type of
*value*
for
*a_type*
, see the
`cci_bind_param <#api_api_cci_bindparam_htm>`_
() function description.

**Syntax**

int
**cci_bind_param_array**
(int
*req_handle*
, int
*index*
,
**T_CCI_A_TYPE**
*a_type*
, void *
*value*
, int *
*null_ind*
,
**T_CCI_U_TYPE**
*u_type*
)

*   *req_handle*
    : (IN) Request handle of the prepared statement



*   *index*
    : (IN) Binding location



*   *a_type*
    : (IN) Data type of
    *value*



*   *value*
    : (IN) Data value to be bound



*   *null_ind*
    : (IN)
    **NULL**
    indicator array (0: not
    **NULL**
    , 1 :
    **NULL**
    )



*   *u_type*
    : (IN) Data type to be applied to the database.



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_REQ_HANDLE**



*   **CCI_ER_TYPE_CONVERSION**



*   **CCI_ER_BIND_INDEX**



*   **CCI_ER_ATYPE**



*   **CCI_ER_BIND_ARRAY_SIZE**



**cci_bind_param_array_size**

**Description**

The
**cci_bind_param_array_size**
function determines the size of the array to be used in
`cci_bind_param_array <#api_api_cci_bindparamarray_htm>`_
().
**cci_bind_param_array_size**
() must be called first before
`cci_bind_param_array <#api_api_cci_bindparamarray_htm>`_
() is used.

**Syntax**

int
**cci_bind_param_array_size**
(int
*req_handle*
, int
*array_size*
)

*   *req_handle*
    : (IN) Request handle of a prepared statement



*   *array_size*
    : (IN) Binding array size



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_REQ_HANDLE**



**cci_blob_free**

**Description**

The
**cci_blob_free**
function frees memory of
*blob*
struct.

**Syntax**

**int**
**cci_blob_free**
(
**T_CCI_BLOB**
*blob*
)

**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_INVALID_LOB_HANDLE**



**cci_blob_new**

**Description**

The
**cci_blob_new**
function creates an empty file where
**LOB**
data is stored and returns Locator referring to the data to
*blob*
struct.

**Syntax**

**int**
**cci_blob_new**
(
**int**
*conn_handle*
,
**T_CCI_BLOB**
*
*blob*
,
**T_CCI_ERROR**
*
*error_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *blob*
    : (OUT)
    **LOB**
    Locator



*   *error_buf*
    : (OUT) Error buffer



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_CONNECT**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_NO_MORE_MEMORY**



*   **CCI_ER_DBMS**



*   **CCI_ER_INVALID_LOB_HANDLE**



**cci_blob_read**

**Description**

The
**cci_blob_read**
function reads as much as data from
*start_pos*
to
*length*
of the
**LOB**
data file specified in
*blob*
; then it stores it in
*buf*
and returns it.

**Syntax**

**int**
**cci_blob_read**
(
**int**
*conn_handle*
,
**T_CCI_BLOB**
*blob*
,
**long**
*start_pos*
**, int**
*length*
**, const char**
**buf*
**, T_CCI_ERROR***
*error_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *blob*
    : (IN)
    **LOB**
    Locator



*   *start_pos*
    : (IN) Index location of
    **LOB**
    data file



*   *length*
    : (IN)
    **LOB**
    data length from buffer



*   *error_buf*
    : (OUT) Error buffer



**Return Value**

*   Size of read value (>= 0: success)



*   Error code (< 0: error)



**Error Code**

*   **CCI_ER_INVALID_LOB_READ_POS**



*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_NO_MORE_MEMORY**



*   **CCI_ER_DBMS**



*   **CCI_ER_INVALID_LOB_HANDLE**



**cci_blob_size**

**Description**

The
**cci_blob_size**
function returns data file size that is specified in
*blob*
.

**Syntax**

**long long**
**cci_blob_size**
(
**T_CCI_BLOB***
*blob*
)

*   *blob*
    : (IN)
    **LOB**
    Locator



**Return Value**

*   Size of
    **BLOB**
    data file (>= 0: success)



*   Error code (< 0: error)



**Error Code**

*   **CCI_ER_INVALID_LOB_HANDLE**



**cci_blob_write**

**Description**

The
**cci_blob_write**
function reads as much as data from
*buf*
to
*length*
and stores it from
*start_pos*
of the
**LOB**
data file specified in
*blob*
.

**Syntax**

**int**
**cci_blob_write**
(
**int**
*conn_handle*
,
**T_CCI_BLOB**
*blob*
,
**long**
*start_pos*
**, int**
*length*
**, const char**
**buf*
**, T_CCI_ERROR***
*error_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *blob*
    : (IN)
    **LOB**
    Locator



*   *start_pos*
    : (IN) Index location of
    **LOB**
    data file



*   *length*
    : (IN) Data length from buffer



*   *error_buf*
    : (OUT) Error buffer



**Return Value**

*   Size of written value (>= 0: success)



*   Error code (< 0: error)



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_NO_MORE_MEMORY**



*   **CCI_ER_DBMS**



*   **CCI_ER_INVALID_LOB_HANDLE**



**cci_clob_free**

**Description**

The
**cci_clob_free**
function frees memory of
**CLOB**
struct.

**Syntax**

**int**
**cci_clob_free**
(
**T_CCI_CLOB**
*clob*
)

**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_INVALID_LOB_HANDLE**



**cci_clob_new**

**Description**

The
**cci_clob_new**
 function creates an empty file where
**LOB**
data is stored and returns Locator referring to the data to
*clob*
struct.

**Syntax**

**int**
**cci_clob_new**
(
**int**
*conn_handle*
,
**T_CCI_CLOB**
*
*clob*
,
**T_CCI_ERROR**
*
*error_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *clob*
    : (OUT)
    **LOB**
    Locator



*   *error_buf*
    : (OUT) Error buffer



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_CONNECT**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_NO_MORE_MEMORY**



*   **CCI_ER_DBMS**



*   **CCI_ER_INVALID_LOB_HANDLE**



**cci_clob_read**

**Description**

The
**cci_clob_read**
 function reads as much as data from
*start_pos*
to
*length*
in the
**LOB**
data file specified in
*clob*
; then it stores it in
*buf*
and returns it.

**Syntax**

**int**
**cci_clob_read**
(
**int**
*conn_handle*
,
**T_CCI_CLOB**
*clob*
,
**long**
*start_pos*
**, int**
*length*
**, const char**
**buf*
**, T_CCI_ERROR***
*error_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *clob*
    : (IN)
    **LOB**
    Locator



*   *start_pos*
    : (IN) Index location of
    **LOB**
    data file



*   *length*
    : (IN)
    **LOB**
    data length from buffer



*   *error_buf*
    : (OUT) Error buffer



**Return Value**

*   Size of read value (>= 0: success)



*   Error code (< 0: Error)



**Error Code**

*   **CCI_ER_INVALID_LOB_READ_POS**



*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_NO_MORE_MEMORY**



*   **CCI_ER_DBMS**



*   **CCI_ER_INVALID_LOB_HANDLE**



**cci_clob_size**

**Description**

The
**cci_clob_size**
function returns data file size that is specified in
*clob*
.

**Syntax**

**long long**
**cci_clob_size**
(
**T_CCI_CLOB***
*clob*
)

*   *clob*
    : (IN)
    **LOB**
    Locator



**Return Value**

*   Size of
    **CLOB**
    data file (>= 0: success)



*   Error code (< 0: error)



**Error Code**

*   **CCI_ER_INVALID_LOB_HANDLE**



**cci_clob_write**

**Description**

The
**cci_clob_write**
 function reads as much as data from
*buf*
to
*length*
and then stores the value from
*start_pos*
in
**LOB**
data file specified in
*clob*
.

**Syntax**

**int**
**cci_clob_write**
(
**int**
*conn_handle*
,
**T_CCI_CLOB**
*clob*
,
**long**
*start_pos*
**, int**
*length*
**, const char**
**buf*
**, T_CCI_ERROR***
*error_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *clob*
    : (IN)
    **LOB**
    Locator



*   *start_pos*
    : (IN) Index location of
    **LOB**
    data file



*   *length*
    : (IN) Data length from buffer



*   *error_buf*
    : (OUT) Error buffer



**Return Value**

*   Size of written value (>= 0: success)



*   Error code (< 0: Error)



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_NO_MORE_MEMORY**



*   **CCI_ER_DBMS**



*   **CCI_ER_INVALID_LOB_HANDLE**



**cci_close_req_handle**

**Description**

The
**cci_close_req_handle**
function closes the request handle obtained by
`cci_prepare <#api_api_cci_prepare_htm>`_
().

**Syntax**

int
**cci_close_req_handle**
(int
*req_handle*
)

*   *req_handle*
    : (IN) Request handle



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_REQ_HANDLE**



*   **CCI_ER_COMMUNICATION**



**cci_col_get**

**Description**

The
**cci_col_get**
 function gets an attribute value of collection type. If the name of the class is C, and the domain of
*set_attr*
is set (multiset, sequence), the query looks like as follows:

SELECT a FROM C, TABLE(set_attr) AS t(a) WHERE C = oid;

That is, the number of members becomes the number of records.

**Syntax**

int
**cci_col_get**
(int
*conn_handle*
, char *
*oid_str*
, char *
*col_attr*
, int *
*col_size*
, int *
*col_type*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *oid_str*
    : (IN) oid



*   *col_attr*
    : (IN) Collection attribute name



*   *col_size*
    : (OUT) Collection size (-1 : null)



*   *col_type*
    : (OUT) Collection type (set, multiset, sequence: u_type)



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Request handle



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



*   **CCI_ER_OBJECT**



*   **CCI_ER_DBMS**



**cci_col_seq_drop**

**Description**

The
**cci_col_seq_drop**
function drops the index-th (base: 1) member of the sequence attribute values. The following example shows how to drop the first member of the sequence attribute values.

cci_col_seq_drop(con_id,
*oid_str*
, seq_attr, 1,
*err_buf*
);

**Syntax**

int
**cci_col_seq_drop**
(int
*conn_handle*
, char *
*oid_str*
, char *
*col_attr*
, int
*index*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *oid_str*
    : (IN) oid



*   *col_attr*
    : (IN) Collection attribute name



*   *index*
    : (IN) Index



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



*   **CCI_ER_OBJECT**



*   **CCI_ER_DBMS**



**cci_col_seq_insert**

**Description**

The
**cci_col_seq_insert**
function inserts one member at the index-th (base: 1) position of the sequence attribute values. The following example shows how to insert "a" at the first position of the sequence attribute values.

cci_col_seq_insert(con_id,
*oid_str*
, seq_attr, 1, "a",
*err_buf*
);

**Syntax**

int
**cci_col_seq_insert**
(int
*conn_handle*
, char *
*oid_str*
, char *
*col_attr*
, int
*index*
, char *
*value*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *oid_str*
    : (IN) oid



*   *col_attr*
    : (IN) Collection attribute name



*   *index*
    : (IN) Index



*   *value*
    : (IN) Sequential element (string)



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



*   **CCI_ER_OBJECT**



*   **CCI_ER_DBMS**



**cci_col_seq_put**

**Description**

The
**cci_col_seq_put**
function replaces the index-th (base: 1) member of the sequence attribute values with a new value. The following example shows how to replace the first member of the sequence attributes values with "a".

cci_col_seq_put(con_id,
*oid_str*
, seq_attr, 1, "a", err_buf);

**Syntax**

int
**cci_col_seq_put**
(int
*conn_handle*
, char *
*oid_str*
, char *
*col_attr*
, int
*index*
, char *
*value*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *oid_str*
    : (IN) oid



*   col_attr: (IN) Collection attribute name



*   index: (IN) Index



*   value: (IN) Sequential value



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



*   **CCI_ER_OBJECT**



*   **CCI_ER_DBMS**



**cci_col_set_add**

**Description**

The
**cci_col_set_add**
function adds one member to the set attribute values. The following example shows how to add "a" to the set attribute values.

cci_col_set_add(con_id,
*oid_str*
, set_attr, "a",
*err_buf*
);

**Syntax**

int
**cci_col_set_add**
( int
*conn_handle*
, char *
*oid_str*
, char *
*col_attr*
, char *
*value*
,
**T_CCI_ERRROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *oid_str*
    : (IN) oid



*   *col_attr*
    : (IN) collection attribute name



*   *value*
    : (IN) set element



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



*   **CCI_ER_OBJECT**



*   **CCI_ER_DBMS**



**cci_col_set_drop**

**Description**

The
**cci_col_set_drop**
function drops one member from the set attribute values. The following example shows how to drop "a" from the set attribute values.

cci_col_set_drop(con_id,
*oid_str*
, set_attr, "a",
*err_buf*
);

**Syntax**

int
**cci_col_set_drop**
(int
*conn_handle*
, char *
*oid_str*
, char *
*col_attr*
, char *
*value*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *oid_str*
    : (IN) oid



*   *col_attr*
    : (IN) collection attribute name



*   *value*
    : (IN) set element (string)



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_QUERY_TIMEOUT**



*   **CCI_ER_LOGIN_TIMEOUT**



*   **CCI_ER_COMMUNICATION**



**cci_col_size**

**Description**

The
**cci_col_size**
function gets the size of the set (seq) attribute.

**Syntax**

int
**cci_col_size**
(int
*conn_handle*
, char *
*oid_str*
, char *col_attr, int *col_size,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *oid_str*
    : (IN) oid



*   *col_attr*
    : (IN) Collection attribute name



*   *col_size*
    : (OUT) Collection size (-1: NULL)



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



*   **CCI_ER_OBJECT**



*   **CCI_ER_DBMS**



**cci_connect**

**Description**

A connection handle to the database server is assigned and it tries to connect to the server. If it has succeeded, the connection handle ID is returned; if fails, an error code is returned.

**Syntax**

int
**cci_connect**
(char *
*ip*
, int
*port*
, char *
*db_name*
, char *
*db_user*
, char *
*db_password*
)

*   *ip*
    : (IN) A string that represents the IP address of the server (host name)



*   *port*
    : (IN) Broker port (The port configured in the
    **$CUBRID/conf/cubrid_broker.conf**
    file)



*   *db_name*
    : (IN) Database name



*   *db_user*
    : (IN) Database user name



*   *db_passwd*
    : (IN) Database user password



**Return Value**

*   Success: Connection handle ID (int)



*   Failure: Error code



**Error Code**

*   **CCI_ER_NO_MORE_MEMORY**



*   **CCI_ER_HOSTNAME**



*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_DBMS**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_CONNECT**



**cci_connect_ex**

**Description**

The
**cci_connect_ex**
function returns
**CCI_ER_DBMS**
error and checks the error details in the database error buffer (
*err_buf*
) at the same time. In that point, it is different from
`cci_connect <#api_api_cci_connect_htm>`_
() and the others are the same as the
`cci_connect <#api_api_cci_connect_htm>`_
() function. For more information, see
`cci_connect <#api_api_cci_connect_htm>`_
().

**Syntax**

int
**cci_connect_ex**
(char *
*ip*
, int
*port*
, char *
*db_name*
, char *
*db_user*
, char *
*db_password*
, T_CCI_ERROR *
*err_buf*
)

*   *err_buf*
    : (OUT) Database error buffer



**cci_connect_with_url**

**Description**

The
**cci_connect_with_url**
function connects a database by using connection information passed with a
*url*
argument. If CUBRID HA of broker is enabled in CCI, you must specify the connection information of the standby broker server, which is used for failover in althost property when failure occurs, in the
*url*
argument of this function. If it has succeeded, the ID of connection handle is returned; it returns an error code on failure. For details about HA features of broker, see Administrator Guide > CUBRID HA > CUBRID Features > Duplexing Brokers.

**althosts**
is the property related to connection target and
**loginTimeout**
,
**queryTimeout**
, and
**disconnectOnQueryTimeout**
are the properties related to timeout;
**logSlowQueries**
,
**logTraceApi**
, and 
**logTraceNetwork**
are the properties related to log information configuration for debugging.

Note that a property name which is a value to be entered in the
*url*
argument is not case sensitive.

**Syntax**

int
**cci_connect_with_url**
(char *
*url*
, char *
*db_user*
, char *
*db_password*
)

 

<
*url*
> ::=

**cci**
:
**CUBRID**
:<
*host*
>:<
*port*
>:<
*db_name*
>:<
*db_user*
>:<
*db_password*
>:[?<
*properties*
>]

 

<
*properties*
> ::= <
*property*
> [&<
*property*
>]

<
*property*
> ::=
**althosts**
=<
*alternative_hosts*
> [ &
**rctime**
=<
*time*
>]

             |{
**login_timeout**
|
**loginTimeout**
}=<
*milli_sec*
>

             |{
**query_timeout**
|
**queryTimeout**
}=<
*milli_sec*
>

             |{
**disconnect_on_query_timeout**
|
**disconnectOnQueryTimeout**
}=
**true|false**

             |
**logFile**
=<
*file_name*
>

             |
**logBaseDir**
=<
*dir_name*
>

             |
**logSlowQueries**
=
**true**
|
**false**
[&
**slowQueryThresholdMillis**
=<
*milli_sec*
>]

             |
**logTraceApi**
=
**true**
|
**false**

             |
**logTraceNetwork**
=
**true**
|
**false**

 

<
*alternative_hosts*
> ::= <
*host*
>:<
*port*
> [,<
*host*
>:<
*port*
>]

 

<
*host*
> :=
**HOSTNAME**
|
**IP_ADDR**

<
*time*
> :=
**SECOND**

<
*milli_sec*
> :=
**MILLI SECOND**

*   *url*
    : (IN) A string that contains server connection information

    *   *host*
        : A host name or IP address of the master database



    *   *port*
        : A port number



    *   *db_name*
        : A name of the database



    *   *db_user*
        : A name of the database user



    *   *db_password*
        : A database user password





*   **althosts**
    =
    *standby_broker1_host*
    ,
    *standby_broker2_host,*
    . . .: Specifies the broker information of the standby server, which is used for failover when it is impossible to connect to the active server. You can specify multiple brokers for failover, and the connection to the brokers is attempted in the order listed in
    **alhosts**
    .



*   **rctime**
    : An interval between the attempts to connect to the active broker in which failure occurred. After a failure occurs, the system connects to the broker specified by
    **althosts**
    (failover), terminates the transaction, and then attempts to connect to the active broker of the master database at every
    **rctime**
    . The default value is 600 seconds.



*   **login_timeout**
    |
    **loginTimeout**
    : Timeout value (unit: msec.) for database login. Upon timeout, a 
    **CCI_ER_LOGIN_TIMEOUT**
    (-38) error is returned. The default value is 0, which means infinite postponement. 

    *   **query_timeout**
        |
        **queryTimeout**
        : If time specified in these properties has expired when calling
        `cci_prepare <#api_api_cci_prepare_htm>`_
        (),
        `cci_execute <#api_api_cci_execute_htm>`_
        (), etc. a cancellation message for query request which was sent to a server will be delivered and called function returns a
        **CCI_ER_QUERY_TIMEOUT**
        (-39) error. The value returned upon timeout may vary depending on a value specified in
        **disconnect_on_query_timeout**
        . For details, see
        **disconnect_on_query_timeout**
        .



    *   **disconnect_on_query_timeout**
        |
        **disconnectOnQueryTimeout**
        : Whether to disconnect socket immediately after time for query request has expired. It determines whether to terminate a socket connection immediately or wait for server response after sending cancellation message for query request to a server when calling
        `cci_prepare <#api_api_cci_prepare_htm>`_
        (),
        `cci_execute <#api_api_cci_execute_htm>`_
        (), etc. The default value is
        **false**
        , meaning that it will wait for server response. It this value is true, a socket will be closed immediately after sending a cancellation message to a server upon timeout and returns the
        **CCI_ER_QUERY_TIMEOUT**
        (-39) error. (If an error occurs on database server side, not on broker side, it returns -1. If you want to view error details, see error codes in "database error buffer." You can get information how to check error codes in
        `CCI Error Codes and Error Messages <#api_api_cci_programming_htm_err>`_
        .) In this case, you must explicitly close the database connection handle by using the
        **cci_disconnect**
        function. Please note that there is a possibility that a database server does not get a cancellation message and execute a query even after an error is returned.



    *   **logFile**
        : A log file name for debugging (default value:
        **cci_**
        <
        *handle_id*
        >
        **.log**
        ). <
        *handle_id*
        > indicates the ID of a connection handle returned by this function.



    *   **logBaseDir**
        : A directory where a debug log file is created



    *   **logSlowQueries**
        : Whether to log slow query for debugging (default value:
        **false**
        )



    *   **slowQueryThresholdMillis**
        : Timeout for slow query logging if slow query logging is enabled (default value:
        **60000**
        , unit: milliseconds)



    *   **logTraceApi**
        : Whether to log the start and end of CCI functions



    *   **logTraceNetwork**
        : Whether to log network data content transferred of CCI functions





*   *db_user*
    : (IN) Database user name. If it is NULL or an empty string, use <
    *db_user*
    > in
    *url*
    .



*   *db_passwd*
    : (IN) Database user password. If it is NULL or an empty string, use <
    *db_password*
    > in
    *url*
    .



**Return Value**

*   Success: Connection handle ID (int)



*   Failure: Error code



**Error Code**

*   **CCI_ER_NO_MORE_MEMORY**



*   **CCI_ER_HOSTNAME**



*   **CCI_ER_INVALID_URL**



*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



*   **CCI_ER_DBMS**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_LOGIN_TIMEOUT**



**Example**

--connection URL string when a property(althosts) is specified for HA

URL=cci:CUBRID:192.168.0.1:33000:demodb:::?althosts=192.168.0.2:33000,192.168.0.3:33000

 

--connection URL string when properties(althosts,rctime) is specified for HA

URL=cci:CUBRID:192.168.0.1:33000:demodb:::?althosts=192.168.0.2:33000,192.168.0.3:33000&rctime=600

 

--connection URL string when properties(logSlowQueries,slowQueryThresholdMills, logTraceApi, logTraceNetwork) are specified for interface debugging

URL = "cci:cubrid:192.168.0.1:33000:demodb:::?logSlowQueries=true&slowQueryThresholdMillis=1000&logTraceApi=true&logTraceNetwork=true"

**Remark**

*   Because a colon (:) and a question mark (?) are used as a separator in URL string, it is not allowed to include them for password of URL string. To use them, you must specify a user name (
    *db_user*
    ) and a password (
    *db_passwd*
    ) as a separate parameter.



**cci_connect_with_url_ex**

**Description**

The
**cci_connect_with_url_ex**
function returns
**CCI_ER_DBMS**
error and checks the error details in the database error buffer (
*err_buf*
) at the same time. In that point, it is different from
`cci_connect_with_url <#api_api_cci_connectwithurl_htm>`_
() and the others are the same as the
`cci_connect_with_url <#api_api_cci_connectwithurl_htm>`_
() function. For more information, see
`cci_connect_with_url <#api_api_cci_connectwithurl_htm>`_
().

is different from
`cci_connect_with_url <#api_api_cci_connectwithurl_htm>`_
() in terms that the detailed description of the error can be checked through the database error buffer (
*err_buf*
) by returning
**CCI_ER_DBMS**
error. For more information, see
`cci_connect_with_url <#api_api_cci_connectwithurl_htm>`_
().

**Syntax**

int
**cci_connect_with_url_ex**
(char *
*url*
, char *
*db_user*
, char *
*db_password*
, T_CCI_ERROR *
*err_buf*
)

*   *err_buf*
    : (OUT) Database error buffer



**cci_cursor**

**Description**

The
**cci_cursor**
function moves the cursor specified in the request handle to access the specific record in the query result executed by
`cci_execute <#api_api_cci_execute_htm>`_
(). The position of cursor is moved by the values specified in the
*origin*
and
*offset*
values. If the position to be moved is not valid,
**CCI_ER_NO_MORE_DATA**
is returned.

**Syntax**

int
**cci_cursor**
(int
*req_handle*
, int
*offset*
,
**T_CCI_CURSOR_POS**
*origin*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *req_handle*
    : (IN) Request handle



*   *offset*
    : (IN) Offset to be moved



*   *origin*
    : (IN) Variable to represent a position. The type is
    **T_CCI_CURSOR_POS**
    .
    **T_CCI_CURSOR_POS**
    enum consists of
    **CCI_CURSOR_FIRST**
    ,
    **CCI_CURSOR_CURRENT**
    , and
    **CCI_CURSOR_LAST**
    .



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_REQ_HANDLE**



*   **CCI_ER_NO_MORE_DATA**



*   **CCI_ER_COMMUNICATION**



**Example**

//the cursor moves to the first record

cci_cursor(req, 1, CCI_CURSOR_FIRST, &err_buf);

 

//the cursor moves to the next record

cci_cursor(req, 1, CCI_CURSOR_CURRENT, &err_buf);

 

//the cursor moves to the last record

cci_cursor(req, 1, CCI_CURSOR_LAST, &err_buf);

 

//the cursor moves to the previous record

cci_cursor(req, -1, CCI_CURSOR_CURRENT, &err_buf);

**cci_cursor_update**

**Description**

The
**cci_cursor_update**
 function updates
*cursor_pos*
from the value of the
*index*
-th column to
*value*
. If the database is updated to
**NULL**
,
*value*
becomes
**NULL**
. For update conditions, see
`cci_prepare <#api_api_cci_prepare_htm>`_
(). The data types of
*value*
for
*a_type*
are shown in the table below.

+-----------------------+--------------------------------+
| **a_type**            | **value Type**                 |
|                       |                                |
+-----------------------+--------------------------------+
| CCI_A_TYPE_STR        | char*                          |
|                       |                                |
+-----------------------+--------------------------------+
| CCI_A_TYPE_INT        | int*                           |
|                       |                                |
+-----------------------+--------------------------------+
| CCI_A_TYPE_FLOAT      | float*                         |
|                       |                                |
+-----------------------+--------------------------------+
| CCI_A_TYPE_DOUBLE     | double*                        |
|                       |                                |
+-----------------------+--------------------------------+
| CCI_A_TYPE_BIT        | **T_CCI_BIT**                  |
|                       | *                              |
|                       |                                |
+-----------------------+--------------------------------+
| CCI_A_TYPE_SET        | **T_CCI_SET**                  |
|                       |                                |
+-----------------------+--------------------------------+
| CCI_A_TYPE_DATE       | **T_CCI_DATE**                 |
|                       | *                              |
|                       |                                |
+-----------------------+--------------------------------+
| **CCI_A_TYPE_BIGINT** | int64_t (For Windows: __int64) |
|                       |                                |
+-----------------------+--------------------------------+
| **CCI_A_TYPE_BLOB**   | **T_CCI_BLOB**                 |
|                       |                                |
+-----------------------+--------------------------------+
| **CCI_A_TYPE_CLOB**   | **T_CCI_CLOB**                 |
|                       |                                |
+-----------------------+--------------------------------+

**Syntax**

int
**cci_cursor_update**
(int
*req_handle*
, int
*cursor_pos*
, int
*index*
,
**T_CCI_A_TYPE**
*a_type*
, void *
*value*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *req_handle*
    : (IN) Request handle



*   *cursor_pos*
    : (IN) Cursor position



*   *index*
    : (IN) Column index



*   *a_type*
    : (IN)
    *value*
    Type



*   *value*
    : (IN) A new value



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_REQ_HANDLE**



*   **CCI_ER_TYPE_CONVERSION**



*   **CCI_ER_ATYPE**



**cci_datasource_borrow**

**Description**

The
**cci_datasource_borrow**
function obtains CCI connection to be used in
**T_CCI_DATASOURCE**
struct.

**Syntax**

**T_CCI_CONN**
**cci_datasource_borrow**
(
**T_CCI_DATASOURCE**
*
*datesource*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *datasource*
    :
    **T_CCI_DATASOURCE**
    struct pointer in which CCI connection exists



*   *err_buf*
    : Error code and message returned upon error occurrence



**Return Value**

*   Success: CCI connection handler identifier



*   Failure: -1



**See Also**

*   `cci_property_create <#api_api_cci_propertycreate_htm>`_



*   `cci_property_destroy <#api_api_cci_propertydestroy_htm>`_



*   `cci_property_get <#api_api_cci_propertyget_htm>`_



*   `cci_property_set <#api_api_cci_propertyset_htm>`_



*   `cci_datasource_create <#api_api_cci_datasourcecreate_htm>`_



*   `cci_datasource_destroy <#api_api_cci_datasourcedestroy_ht_5686>`_



*   `cci_datasource_release <#api_api_cci_datasourcerelease_ht_568>`_



**cci_datasource_create**

**Description**

The
**cci_datasource_create**
function creates DATASOURCE of CCI.

**Syntax**

**T_CCI_DATASOURCE**
*
**cci_datasource_create**
(
**T_CCI_PROPERTIES**
*
*properties*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *properties*
    :
    **T_CCI_PROPERTIES**
    struct pointer in which configuration of struct pointer is stored



*   *err_buf*
    : Error code and message returned upon error occurrence



**Return Value**

*   Success:
    **T_CCI_DATASOURCE**
    struct pointer created



*   Failure:
    **NULL**



**See Also**

*   `cci_property_create <#api_api_cci_propertycreate_htm>`_



*   `cci_property_destroy <#api_api_cci_propertydestroy_htm>`_



*   `cci_property_get <#api_api_cci_propertyget_htm>`_



*   `cci_property_set <#api_api_cci_propertyset_htm>`_



*   `cci_datasource_borrow <#api_api_cci_datasourceborrow_htm>`_



*   `cci_datasource_destroy <#api_api_cci_datasourcedestroy_ht_5686>`_



*   `cci_datasource_release <#api_api_cci_datasourcerelease_ht_568>`_



**cci_datasource_destroy**

**Description**

The
**cci_datasource_destroy**
function destroys DATASOURCE of CCI.

**Syntax**

void
**cci_datasource_destroy**
(
**T_CCI_DATASOURCE**
*
*datasource*
)

*   *datasource*
    :
    **T_CCI_DATASOURCE**
    struct pointer to be deleted



**Return Value**

None

**See Also**

*   `cci_property_create <#api_api_cci_propertycreate_htm>`_



*   `cci_property_destroy <#api_api_cci_propertydestroy_htm>`_



*   `cci_property_get <#api_api_cci_propertyget_htm>`_



*   `cci_property_set <#api_api_cci_propertyset_htm>`_



*   `cci_datasource_borrow <#api_api_cci_datasourceborrow_htm>`_



*   `cci_datasource_create <#api_api_cci_datasourcecreate_htm>`_



*   `cci_datasource_release <#api_api_cci_datasourcerelease_ht_568>`_



**cci_datasource_release**

**Description**

The
**cci_datasource_release**
function returns CCI connection released in
**T_CCI_DATASOURCE**
struct.

**Syntax**

int
**cci_datasource_release**
(
**T_CCI_DATASOURCE**
*
*date_source*
,
**T_CCI_CONN**
*conn*
)

*   *datasource*
    :
    **T_CCI_DATASOURCE**
    struct pointer which returns CCI connection



*   *conn*
    : CCI connection handler identifier released



**Return Value**

*   Success: 1



*   Failure: 0



**See Also**

*   `cci_property_create <#api_api_cci_propertycreate_htm>`_



*   `cci_property_destroy <#api_api_cci_propertydestroy_htm>`_



*   `cci_property_get <#api_api_cci_propertyget_htm>`_



*   `cci_property_set <#api_api_cci_propertyset_htm>`_



*   `cci_datasource_borrow <#api_api_cci_datasourceborrow_htm>`_



*   `cci_datasource_create <#api_api_cci_datasourcecreate_htm>`_



*   `cci_datasource_destroy <#api_api_cci_datasourcedestroy_ht_5686>`_



**cci_disconnect**

**Description**

The
**cci_disconnect**
function disconnects all request handles created for
*conn_handle*
. If a transaction is being performed, the handles are disconnected after
`cci_end_tran <#api_api_cci_endtran_htm>`_
() is executed.

**Syntax**

int
**cci_disconnect**
(int
*conn_handle*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_DBMS**



*   **CCI_ER_COMMUNICATION**



**cci_end_tran**

**Description**

The
**cci_end_tran**
 function performs commit or rollback on the current transaction. At this point, all open request handles are terminated and the connection to the database server is disabled. However, even after the connection to the server is disabled, the connection handle remains valid.

You can configure the default value of auto-commit mode by using
**CCI_DEFAULT_AUTOCOMMIT**
(broker parameter) upon startup of an application. If configuration on broker parameter is omitted, the default value is
**ON**
; use the
`cci_set_autocommit <#api_api_cci_setautocommit_htm>`_
() function to change auto-commit mode within an application. If auto-commit mode is
**OFF**
, you must explicitly commit or roll back transaction by using the
`cci_end_tran <#api_api_cci_endtran_htm>`_
() function.

**Syntax**

int
**cci_end_tran**
(int
*conn_handle*
, char
*type*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *type*
    : (IN)
    **CCI_TRAN_COMMIT**
    or
    **CCI_TRAN_ROLLBACK**



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_DBMS**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_TRAN_TYPE**



**cci_escape_string**

**Description**

Converts the input string to a string that can be used in the CUBRID query. The following parameters are specified in this function: connection handle or
**no_backslash_escapes**
setting value, output string pointer, input string pointer, the length of the input string, and the address of the
**T_CCI_ERROR**
struct variable.

When the system parameter
**no_backslash_escapes**
**의**
is yes (default) or when the
**CCI_NO_BACKSLASH_ESCAPES_TRUE**
value is sent to the connection handle location, the string is converted to the following characters.

*   ' (single quote) => ' + ' (escaped single quote)



When the system parameter
**no_backslash_escapes**
**의**
is no or when the
**CCI_NO_BACKSLASH_ESCAPES_FALSE**
value is sent to the connection handle location, the string is converted to the following characters:

*   \n (new line character, ASCII 10) => \ + n (backslash + Alphabet n)



*   \r (carrage return, ASCII 13) => \ + r (backslash + Alphabet r)



*   \0 (ASCII 0) => \ + 0 (backslash + 0(ASCII 48)



*   \  (backslash) => \ + \



You can assign the space where the result string will be saved by using the
*length*
parameter. It will take as much as the byte length of the maximum input string * 2 + 1.

**Syntax**

long
**cci_escape_string**
(int
*conn_handle*
, char *
*to*
, const char *
*from*
, unsigned long
*length*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) connection handle or
    **no_backslash_escapes**
    setting value. When a connection handle is given, the
    **no_backslash_escapes**
    parameter value is read to determine how to convert. Instead of the connection handle,
    **CCI_NO_BACKSLASH_ESCAPES_TRUE**
    or
    **CCI_NO_BACKSLASH_ESCAPES_FALSE**
    value can be sent to determine how to convert.



*   *to*
    : (OUT) Result string



*   *from*
    : (IN) Input string



*   *length*
    : (IN) Maximum byte length of the input string



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Success: Byte length of the changed string



*   Failure: Error Code



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_COMMUNICATION**



**cci_execute**

**Description**

The
**cci_execute**
function executes the SQL statement (prepared statement) that has executed
`cci_prepare <#api_api_cci_prepare_htm>`_
(). A request handle,
*flag*
, the maximum length of a column to be fetched, and the address of a
**T_CCI_ERROR**
construct variable in which error information being stored are specified as arguments.

The function of retrieving the query result from the server by configuring
*flag*
can be classified as synchronous or asynchronous. Or it can be determined whether to execute multiple queries or one query. If the flag is set to
**CCI_EXEC_QUERY_ALL**
, a synchronous mode (sync_mode) is used to retrieve query results immediately after executing prepared queries if it is set to
**CCI_EXEC_ASYNC**
, an asynchronous mode (async_mode) is used to retrieve the result immediately each time a query result is created. The
*flag*
is set to
**CCI_EXEC_QUERY_ALL**
by default, and in such cases the following rules are applied.

*   The return value is the result of the first query.



*   If an error occurs in any query, the execution is processed as a failure.



*   For a query composed of in a query composed of q1; q2; q3 if an error occurs in q2 after q1 succeeds the execution, the result of q1 remains valid. That is, the previous successful query executions are not rolled back when an error occurs.



*   If a query is executed successfully, the result of the second query can be obtained using
    `cci_next_result <#api_api_cci_nextresult_htm>`_
    ().



*max_col_size*
is a value that is used to determine the maximum length of a column to be sent to a client when the columns of the prepared statement are
**CHAR**
,
**VARCHAR**
,
**NCHAR**
,
**VARNCHAR**
,
**BIT**
or
**VARBIT**
. If this value is 0, full length is fetched.

**Syntax**

int
**cci_execute**
(int
*req_handle*
, char
*flag*
, int
*max_col_size*
,
**T_CCI_ERROR**
**err_buf*
)

*   *req_handle*
    : (IN) Request handle of the prepared statement



*   *flag*
    : (IN) Exec flag (
    **CCI_EXEC_ASYNC**
    or
    **CCI_EXEC_QUERY_ALL**
    )



*   *max_col_size*
    : (IN) The maximum length of a column to be fetched when it is a string data type in bytes. If this value is 0, full length is fetched.



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Success



*   **SELECT**
    : Returns the number of results in sync mode returns 0 in async mode.



*   **INSERT**
    ,
    **UPDATE**
    : Returns the number of rows reflected.



*   Others queries: 0



*   Failure: Error code



**Error Code**

*   **CCI_ER_REQ_HANDLE**



*   **CCI_ER_BIND**



*   **CCI_ER_DBMS**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_QUERY_TIMEOUT**



*   **CCI_ER_LOGIN_TIMEOUT**



**cci_execute_array**

**Description**

If more than one value is bound to the prepared statement, this gets the values of the variables to be bound and executes the query by binding each value to the variable.

To bind the data, call the
`cci_bind_param_array_size <#api_api_cci_bindparamarraysize_h_5618>`_
() function to specify the size of the array, bind each value to the variable by using the
`cci_bind_param_array <#api_api_cci_bindparamarray_htm>`_
() function, and execute the query by calling the
**cci_execute_array**
() function.

You can get three execution results by calling the
`cci_execute <#api_api_cci_execute_htm>`_
() function. However, the
**cci_execute_array**
() function returns the number of queries executed by the query_result variable. You can use the following macro to get the information about the execution result. However, note that the validity check is not performed for each parameter entered in the macro. After using the query_result variable, you must delete the query_result by using the
`cci_query_result_free <#api_api_cci_queryresultfree_htm>`_
() function.

+-----------------------------+-----------------------------+---------------------------+
| **Marco**                   | **Return Type**             | **Meaning**               |
|                             |                             |                           |
+-----------------------------+-----------------------------+---------------------------+
| **CCI_QUERY_RESULT_RESULT** | int                         | the number of results     |
|                             |                             |                           |
+-----------------------------+-----------------------------+---------------------------+
| CCI_QUERY_RESULT_ERR_MSG    | char*                       | error message about query |
|                             |                             |                           |
+-----------------------------+-----------------------------+---------------------------+
| CCI_QUERY_RESULT_STMT_TYPE  | int(T_CCI_CUBRID_STMT enum) | type of query statement   |
|                             |                             |                           |
+-----------------------------+-----------------------------+---------------------------+

**Syntax**

int
**cci_execute_array**
(int
*req_handle*
,
**T_CCI_QUERY_RESULT**
**
*query_result*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *req_handle*
    : (IN) Request handle of the prepared statement



*   *query_result*
    : (OUT) Query results (the number of executed queries)



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Success: The number of executed queries



*   Failure: Negative number



**Error Code**

*   **CCI_ER_REQ_HANDLE**



*   **CCI_ER_BIND**



*   **CCI_ER_DBMS**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_QUERY_TIMEOUT**



*   **CCI_ER_LOGIN_TIMEOUT**



**Example**

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

  printf ("execute error: %d, %s\n", cci_error.err_code,

            cci_error.err_msg);

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

**cci_execute_batch**

**Description**

In CCI, multiple jobs can be processed simultaneously when using DML queries such as
**INSERT**
/
**UPDATE**
/
**DELETE**
.
`CCI_QUERY_RESULT_RESULT <#api_api_cci_queryresultresult_ht_1623>`_
() and
**cci_execute_batch**
() functions can be used to execute such batch jobs. Note that prepared statements cannot be used in the
**cci_execute_batch**
() function.

Executes
*sql_stmt*
as many times as
*num_sql_stmt*
specified as a parameter and returns the number of queries executed with the query_result variable. You can use the macro (
`CCI_QUERY_RESULT_RESULT <#api_api_cci_queryresultresult_ht_1623>`_
,
`CCI_QUERY_RESULT_ERR_MSG <#api_api_cci_queryresulterrmsg_ht_870>`_
,
`CCI_QUERY_RESULT_STMT_TYPE <#api_api_cci_queryresultstmttype__9124>`_
) available in the
`cci_execute_array <#api_api_cci_executearray_htm>`_
() function to get the information about the execution result. For more information about each macro, see the
`cci_execute_array <#api_api_cci_executearray_htm>`_
() function. However, note that the validity check is not performed for each parameter entered in the macro. After using the
*query_result*
variable, you must delete the query result by using the
`cci_query_result_free <#api_api_cci_queryresultfree_htm>`_
() function.

**Syntax**

int
**cci_execute_batch**
(int
*conn_handle*
, int
*num_sql_stmt*
, char **
*sql_stmt*
,
**T_CCI_QUERY_RESULT**
**
*query_result*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *num_sql_stmt*
    : (IN) The number of
    *sql_stmt*



*   *sql_stmt*
    : (IN) SQL statement array



*   *query_result*
    : (OUT) The results of
    *sql_stmt*



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Success: The number of executed queries



*   Failure: Negative number



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_DBMS**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_NO_MORE_MEMORY**



*   **CCI_ER_CONNECT**



*   **CCI_ER_QUERY_TIMEOUT**



*   **CCI_ER_LOGIN_TIMEOUT**



**Example**

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

    "select * from athlete order by code desc for orderby_num() < 3";

//calling cci_execute_batch()

  n_executed = cci_execute_batch (con, count, queries, &result, &cci_error);

  if (n_executed < 0)

    {

      printf ("execute_batch: %d, %s\n", cci_error.err_code,

              cci_error.err_msg);

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

**cci_execute_result**

**Description**

The
**cci_execute_result**
 function gets the execution results (e.g. statement type, result count) performed by
`cci_execute <#api_api_cci_execute_htm>`_
(). The results of each query are retrieved by
`CCI_QUERY_RESULT_STMT_TYPE <#api_api_cci_queryresultstmttype__9124>`_
and
`CCI_QUERY_RESULT_RESULT <#api_api_cci_queryresultresult_ht_1623>`_
. The query results used must be deleted by
`cci_query_result_free <#api_api_cci_queryresultfree_htm>`_
.

**Syntax**

int
**cci_execute_result**
(int
*req_handle*
,
**T_CCI_QUERY_RESULT**
**
*query_result*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *req_handle*
    : (IN) Request handle of the prepared statement



*   *query_result*
    : (OUT) Query results



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Suceess: The number of queries



*   Failure: Negative number



**Error Code**

*   **CCI_ER_REQ_HANDLE**



*   **CCI_ER_COMMUNICATION**



**Example**

T_CCI_QUERY_RESULT *qr;

…

 

cci_execute( … );

res = cci_execute_result(req_h, &qr, &err_buf);

if (res < 0) {

  /* error */

}

else {

  for (i=1 ; i <= res ; i++) {

    result_count = CCI_QUERY_RESULT_RESULT(qr, i);

    stmt_type = CCI_QUERY_RESULT_STMT_TYPE(qr, i);

  }

  cci_query_result_free(qr, res);

}

**cci_fetch**

**Description**

The
**cci_fetch**
function fetches the query result executed by
`cci_execute <#api_api_cci_execute_htm>`_
() from the server-side CAS and stores it to the client buffer. The
`cci_get_data <#api_api_cci_getdata_htm>`_
() function can be used to identify the data of a specific column from the fetched query result.

**Syntax**

int
**cci_fetch**
(int
*req_handle*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *req_handle*
    : (IN) Request handle



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_REQ_HANDLE**



*   **CAS_ER_HOLDABLE_NOT_ALLOWED**



*   **CCI_ER_NO_MORE_DATA**



*   **CCI_ER_RESULT_SET_CLOSED**



*   **CCI_ER_DELETED_TUPLE**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_NO_MORE_MEMORY**



**cci_fetch_buffer_clear**

**Description**

The
**cci_fetch_buffer_clear**
function clears the records temporarily stored in the client buffer.

**Syntax**

int
**cci_fetch_buffer_clear**
(int
*req_handle*
)

*   *req_handle*
    : (IN) Request handle



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_REQ_HANDLE**



**cci_fetch_sensitive**

**Description**

The
**cci_fetch_sensitive**
function sends changed values for sensitive column. If the results by
*req_handle*
are not sensitive (**), they are same as the ones by
`cci_fetch <#api_api_cci_fetch_htm>`_
(). The return value of
**CCI_ER_DELETED_TUPLE**
means that the given row has been deleted.

sensitive column means items that can provide updated values in the
**SELECT**
list upon the re-request of results. For example, a column is directly used as an item of the
**SELECT**
list without aggregation operation, the colum can be called sensitive column.

sensitive result does not receive from the server, not records stored in the client buffer when it is fetch is again.

**Syntax**

int
**cci_fetch_sensitive**
(int
*req_handle*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *req_handle*
    : (IN) Request handle



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_REQ_HANDLE**



*   **CCI_ER_NO_MORE_DATA**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_DBMS**



*   **CCI_ER_DELETED_TUPLE**



**cci_fetch_size**

**Description**

The
**cci_fetch_size**
function determines the number of records sent by
`cci_fetch <#api_api_cci_fetch_htm>`_
() from the server to the client.

**Syntax**

int
**cci_fetch_size**
(int
*req_handle*
, int
*fetch_size*
)

*   *req_handle*
    : (IN) Request handle



*   *fetch_size*
    : (IN) Fetch size



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_REQ_HANDLE**



**cci_get_autocommit**

**Description**

The
**cci_get_autocommit**
function returns the auto-commit mode currently configured.

**Syntax**

CCI_AUTOCOMMIT_MODE
**cci_get_autocommit**
(int
*conn_handle*
)

*   *conn_handle*
    : Connection handle



**Return Value**

*   CCI_AUTOCOMMIT_TRUE: Auto-commit mode is ON



*   CCI_AUTOCOMMIT_FALSE: Auto-commit mode is OFF



**Error Code**

*   None



**cci_get_bind_num**

**Description**

The
**cci_get_bind_num**
function gets the number of input bindings. If the SQL statement used during preparation is composed of multiple queries, it represents the number of input bindings used in all queries.

**Syntax**

int
**cci_get_bind_num**
(int
*req_handle*
)

*   *req_handle*
    : (IN) Request handle for the prepared statement



**Return Value**

*   The number of input bindings



**Error Code**

*   **CCI_ER_REQ_HANDLE**



**cci_get_class_num_objs**

**Description**

The
**cci_get_class_num_objs**
function gets the number of objects of the
*class_name*
class and the number of pages being used. If the flag is configured to 1, an approximate value is fetched; if it is configured to 0, an exact value is fetched.

**Syntax**

int
**cci_get_class_num_objs**
(int
*conn_handle*
, char *
*class_name*
, int
*flag*
, int *
*num_objs*
, int *
*num_pages*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *class_name*
    : (IN) Class name



*   *flag*
    : (IN) 0 or 1



*   *num_objs*
    : (OUT) The number of objects



*   *num_pages*
    : (OUT) The number of pages



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_REQ_HANDLE**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_CONNECT**



**CCI_GET_COLLECTION_DOMAIN**

**Description**

If
*u_type*
is set, multiset, or sequence type, this macro gets the domain of the set, multiset or sequence. If
*u_type*
is not a set type, the return value is the same as
*u_type*
.

**Syntax**

**#define CCI_GET_COLLECTION_DOMAIN**
(
*u_type*
)

**Return Value**

*   Type (CCI_U_TYPE)



**cci_get_cur_oid**

**Description**

The
**cci_get_cur_oid**
function gets OID of the currently fetched records if
**CCI_INCLUDE_OID**
is configured in execution. The OID is represented in string for a page, slot, or volume.

**Syntax**

int
**cci_get_cur_oid**
(int
*req_handle*
, char *
*oid_str_buf*
)

*   *conn_handle*
    : (IN) Request handle



*   *oid_str_buf*
    : (OUT) OID string



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_REQ_HANDLE**



**cci_get_data**

**Description**

The
**cci_get_data**
function gets the
*col_no*
-th value from the currently fetched result. The
*type*
of the
*value*
variable is determined based on the given
*type*
argument, and the value or the pointer is copied to the
*value*
variable accordingly.

For a value to be copied, the memory for the address to be transferred to the
*value*
variable must have been previously assigned. Note that if a pointer is copied, a pointer in the application client library is returned, so the value becomes invalid next time the
**cci_get_data**
() function is called.

In addition, the pointer returned by the pointer copy must not be freed. However, if the type is
**CCI_A_TYPE_SET**
, the memory must be freed by using the
`cci_set_free <#api_api_cci_setfree_htm>`_
() function after using the set because the set is returned after the
**T_CCI_SET**
type memory is allocated. The following table shows the summary of
*type*
arguments and data types of their corresponding
*value*
values.

+---------------------+-------------------------+-------------------------------------------+
| **type**            | **value Type**          | **Meaning**                               |
|                     |                         |                                           |
+---------------------+-------------------------+-------------------------------------------+
| CCI_A_TYPE_STR      | char**                  | pointer copy                              |
|                     |                         |                                           |
+---------------------+-------------------------+-------------------------------------------+
| CCI_A_TYPE_INT      | int*                    | value copy                                |
|                     |                         |                                           |
+---------------------+-------------------------+-------------------------------------------+
| CCI_A_TYPE_FLOAT    | float*                  | value copy                                |
|                     |                         |                                           |
+---------------------+-------------------------+-------------------------------------------+
| CCI_A_TYPE_DOUBLE   | double*                 | value copy                                |
|                     |                         |                                           |
+---------------------+-------------------------+-------------------------------------------+
| CCI_A_TYPE_BIT      | **T_CCI_BIT**           | value copy (pointer copy for each member) |
|                     | *                       |                                           |
|                     |                         |                                           |
+---------------------+-------------------------+-------------------------------------------+
| CCI_A_TYPE_SET      | **T_CCI_SET**           | memory alloc and value copy               |
|                     | *                       |                                           |
|                     |                         |                                           |
+---------------------+-------------------------+-------------------------------------------+
| CCI_A_TYPE_DATE     | **T_CCI_DATE**          | value copy                                |
|                     | *                       |                                           |
|                     |                         |                                           |
+---------------------+-------------------------+-------------------------------------------+
| CCI_A_TYPE_BIGINT   | int64_t*                | value copy                                |
|                     | (For Windows: __int64*) |                                           |
|                     |                         |                                           |
+---------------------+-------------------------+-------------------------------------------+
| **CCI_A_TYPE_BLOB** | **T_CCI_BLOB**          | memory alloc and value copy               |
|                     |                         |                                           |
+---------------------+-------------------------+-------------------------------------------+
| **CCI_A_TYPE_CLOB** | **T_CCI_CLOB**          | memory alloc and value copy               |
|                     |                         |                                           |
+---------------------+-------------------------+-------------------------------------------+

**Syntax**

int
**cci_get_data**
(int
*req_handle*
, int
*col_no*
, int
*type*
, void *
*value*
, int *
*indicator*
)

*   *req_handle*
    : (IN) Request handle



*   *col_no*
    : (IN) One-based column index. It starts with 1.



*   *type*
    : (IN) Data type (defined in the
    **T_CCI_A_TYPE**
    ) of
    *value*
    variable



*   *value*
    : (OUT) Variable address for data to be stored



*   *indicator*
    : (OUT)
    **NULL**
    indicator (-1:
    **NULL**
    )



*   If
    *type*
    is
    **CCI_A_TYPE_STR**
    : -1 is returned in case of
    **NULL**
    ; the length of string stored in
    *value*
    is returned, otherwise.



*   If
    *type*
    is
    **CCI_A_TYPE_STR**
    : -1 is returned in case of
    **NULL**
    , 0 is returned, otherwise.



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_REQ_HANDLE**



*   **CCI_ER_TYPE_CONVERSION**



*   **CCI_ER_COLUMN_INDEX**



*   **CCI_ER_ATYPE**



**Remark**

*   For
    **LOB**
    type, if the
    **cci_get_data**
    () function is called, meta data with the
    **LOB**
    type column (Locator) is displayed. To call data of the
    **LOB**
    type column, the
    `cci_blob_read <#api_api_cci_blobread_htm>`_
    () function should be called.



**cci_get_db_parameter**

**Description**

The
**cci_get_db_parameter**
function gets a parameter value specified in the database. The data type of
*value*
for
*param_name*
is shown in the table below.

+---------------------------------+----------------+----------+
| **param_name**                  | **value Type** | **note** |
|                                 |                |          |
+---------------------------------+----------------+----------+
| **CCI_PARAM_ISOLATION_LEVEL**   | int*           | get/set  |
|                                 |                |          |
+---------------------------------+----------------+----------+
| **CCI_PARAM_LOCK_TIMEOUT**      | int*           | get/set  |
|                                 |                |          |
+---------------------------------+----------------+----------+
| **CCI_PARAM_MAX_STRING_LENGTH** | int*           | get only |
|                                 |                |          |
+---------------------------------+----------------+----------+

In
**cci_set_db_parameter**
and
**cci_get_db_parameter**
, the input/output of
**CCI_PARAM_LOCK_TIMEOUT**
is in milliseconds.

**Note**
**cci_get_db_parameter**
 in the earlier version of CUBRID 9.0, you should be careful because the output unit of
**CCI_PARAM_LOCK_TIMEOUT**
is second.

**CCI_PARAM_MAX_STRING_LENGTH**
is measured in bytes and it gets a value defined in the
**MAX_STRING_LENGTH**
broker parameter.

**Syntax**

int
**cci_get_db_parameter**
(int
*conn_handle*
,
**T_CCI_DB_PARAM**
*param_name*
, void *
*value*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *param_name*
    : (IN) System parameter name



*   *value*
    : (OUT) Parameter value



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_PARAM_NAME**



*   **CCI_ER_DBMS**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_CONNECT**



**cci_get_db_version**

**Description**

The
**cci_get_db_version**
function gets the Database Management System (DBMS) version.

**Syntax**

int
**cci_get_db_version**
(int
*conn_handle*
, char *
*out_buf*
, int
*out_buf_size*
)

*   *conn_handle*
    : (IN) Connection handle



*   *out_buf*
    : (OUT) Result buffer



*   *out_buf_size*
    : (IN)
    *oub_buf*
    size



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_CONNECT**



**cci_get_err_msg**

**Description**

The
**cci_get_err_msg**
function stores error messages in the error message buffer. For details on error codes and error messages, see
`CCI Error Codes and Error Messages <#api_api_cci_programming_htm_err>`_
.

**Syntax**

int
**cci_get_err_msg**
(int
*err_code*
, char *
*msg_buf*
, int
*msg_buf_size*
)

*   *err_code*
    : (IN) Error code



*   *msg_buf*
    : (OUT) Error message buffer



*   *msg_buf_size*
    : (IN)
    *msg_buf*
    size



**Return Value**

*   0: Success



*   -1: Failure



**cci_get_error_msg**

**Description**

Saves the error messages corresponding to the CCI error codes in the message buffer. If the value of CCI error code is
**CCI_ER_DBMS**
, the database error buffer (
*err_buf*
) receives the error message sent from the data server and saves it in the message buffer. For details on error codes and messages, see
`CCI Error Codes and Error Messages <#api_api_cci_programming_htm_err>`_
.

**Syntax**

int
**cci_get_error_msg**
(int
*err_code*
,
**T_CCI_ERROR**
*
*err_buf*
, char *
*msg_buf*
, int
*msg_buf_size*
)

*   *err_code*
    : (IN) Connection handle



*   *err_buf*
    : (IN) Database error buffer



*   *msg_buf*
    : (OUT) Error message buffer



*   *msg_buf_size*
    : (IN)
    *msg_buf*
    size



**Return Value**

*   0: Success



*   -1: Failure



**cci_get_holdability**

**Description**

Returns the cursor holdability setting value about the result set from the connection handle. When it is 1, the connection is disconnected or the cursor is holdable until the result set is intentionally closed regardless of commit. When it is 0, the result set is closed when committed and the cursor is not holdable. For more details on cursor holdability, see Cursor Holdability.

**Syntax**

int
**cci_get_holdability**
(int
*conn_handle*
)

*   *conn_handle*
    : (IN) Connection handle



**Return Value**

*   0 : not holdable



*   1 : holdable



*   Error Code



**Error Code**

*   **CCI_ER_CON_HANDLE**



**cci_get_query_timeout**

**Description**

The
**cci_get_query_timeout**
function returns timeout configured for query execution.

**Syntax**

int
**cci_get_query_timeout**
(int
*req_handle*
)

*   *conn_handle*
    : (IN) Request handle



**Return Value**

*   Success: Timeout value configured in current request handle (unit: msec.)



*   Failure: Error code



**Error Code**

*   **CCI_ER_REQ_HANDLE**



**cci_get_result_info**

**Description**

If the prepared statement is
**SELECT**
, the
**T_CCI_COL_INFO**
struct that stores the column information about the execution result can be obtained by using this function. If it is not
**SELECT**
,
**NULL**
is returned and the
*num*
value becomes 0.

You can access the
**T_CCI_COL_INFO**
struct directly to get the column information from the struct, but you can also use a macro to get the information, which is defined as follows. The address of the
**T_CCI_COL_INFO**
struct and the column index are specified as parameters for each macro. The macro can be called only for the
**SELECT**
query. Note that the validity check is not performed for each parameter entered in each macro. If the return type of the macro is char*, do not free the memory pointer.

+-------------------------------------+-----------------+--------------------------+
| **Macro**                           | **Return Type** | **Meaning**              |
|                                     |                 |                          |
+-------------------------------------+-----------------+--------------------------+
| **CCI_GET_RESULT_INFO_TYPE**        | T_CCI_U_TYPE    | column type              |
|                                     |                 |                          |
+-------------------------------------+-----------------+--------------------------+
| **CCI_GET_RESULT_INFO_SCALE**       | short           | column scale             |
|                                     |                 |                          |
+-------------------------------------+-----------------+--------------------------+
| **CCI_GET_RESULT_INFO_PRECISION**   | int             | column precision         |
|                                     |                 |                          |
+-------------------------------------+-----------------+--------------------------+
| **CCI_GET_RESULT_INFO_NAME**        | char*           | column name              |
|                                     |                 |                          |
+-------------------------------------+-----------------+--------------------------+
| **CCI_GET_RESULT_INFO_ATTR_NAME**   | char*           | column attribute name    |
|                                     |                 |                          |
+-------------------------------------+-----------------+--------------------------+
| **CCI_GET_RESULT_INFO_CLASS_NAME**  | char*           | column class name        |
|                                     |                 |                          |
+-------------------------------------+-----------------+--------------------------+
| **CCI_GET_RESULT_INFO_IN_NON_NULL** | char(0 or 1)    | whether a column is NULL |
|                                     |                 |                          |
+-------------------------------------+-----------------+--------------------------+

**Syntax**

**T_CCI_COL_INFO**
*
**cci_get_result_info**
(int
*req_handle*
,
**T_CCI_CUBRID_STMT**
*stmt
*_type*
, int *
*num*
)

*   *req_handle*
    : (IN) Request handle for the prepared statement



*   *stmt_type*
    : (OUT) Command type



*   *num*
    : (OUT) The number of columns in the
    **SELECT**
    statement (if
    *stmt_type*
    is
    **CUBRID_STMT_SELECT**
    )



**Return Value**

*   Success: Result info pointer



*   Failure:
    **NULL**



**Example**

col_info = cci_get_result_info (req, &stmt_type, &col_count);

  if (col_info == NULL)

    {

      printf ("get_result_info error: %d, %s\n", cci_error.err_code,

              cci_error.err_msg);

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

**CCI_GET_RESULT_INFO_ATTR_NAME**

**Description**

The
**CCI_GET_RESULT_INFO_ATTR_NAME**
macro gets the actual attribute name of the
*index*
-th column of a prepared
**SELECT**
statement. If there is no name for the attribute (constant, function, etc), " " (empty string) is returned. It does not check whether the specified argument,
*res_info*
, is
**NULL**
and whether
*index*
is valid. You cannot delete the returned memory pointer with
**free**
().

**Syntax**

#define
**CCI_GET_RESULT_INFO_ATTR_NAME**
(
**T_CCI_COL_INFO**
*
*res_info*
, int
*index*
)

*   *res_info*
    : (IN) A pointer to the column information fetched by
    `cci_get_result_info <#api_api_cci_getresultinfo_htm>`_



*   *index*
    : (IN) Column index



**Return Value**

*   Attribute name (char*)



**CCI_GET_RESULT_INFO_CLASS_NAME**

**Description**

The
**CCI_GET_RESULT_INFO_CLASS_NAME**
macro gets the
*index*
-th class name of a prepared
**SELECT**
statement. It does not check whether the specified argument,
*res_info*
, is
**NULL**
and whether
*index*
is valid. You cannot delete the returned memory pointer with
**free**
(). The return value can be
**NULL**
.

**Syntax**

#define
**CCI_GET_RESULT_INFO_CLASS_NAME**
(
**T_CCI_COL_INFO**
*
*res_info*
, int
*index*
)

*   *res_info*
    : (IN) Column info pointer by
    `cci_get_result_info <#api_api_cci_getresultinfo_htm>`_



*   *index*
    : (IN) Column index



**Return Value**

*   Class name (char*)



**CCI_GET_RESULT_INFO_IS_NON_NULL**

**Description**

The
**CCI_GET_RESULT_INFO_IS_NON_NULL**
macro gets a value indicating whether the
*index*
-th column of a prepared
**SELECT**
statement is nullable. It does not check whether the specified argument,
*res_info*
, is
**NULL**
and whether
*index*
is valid.

**Syntax**

#define
**CCI_GET_RESULT_INFO_IS_NON_NULL**
(
**T_CCI_COL_INFO**
*
*res_info*
, int
*index*
)

*   *res_info*
    : (IN) Column info pointer by
    `cci_get_result_info <#api_api_cci_getresultinfo_htm>`_



*   *index*
    : (IN) Column index



**Return Value**

*   0: nullable



*   1: non
    **NULL**



**CCI_GET_RESULT_INFO_NAME**

**Description**

The
**CCI_GET_RESULT_INFO_NAME**
macro gets the
*index*
-th column name of a prepared
**SELECT**
statement. It does not check whether the specified argument,
*res_info*
, is
**NULL**
and whether
*index*
is valid. You cannot delete the returned memory pointer with
**free**
().

**Syntax**

#define
**CCI_GET_RESULT_INFO_NAME**
(
**T_CCI_COL_INFO**
*
*res_info*
, int
*index*
)

*   *res_info*
    : (IN) Column info pointer to
    `cci_get_result_info <#api_api_cci_getresultinfo_htm>`_



*   *index*
    : (IN) Column index



**Return Value**

*   Column name (char*)



**CCI_GET_RESULT_INFO_PRECISION**

**Description**

The
**CCI_GET_RESULT_INFO_PRECISION**
macro gets the
*index*
-th precision of a prepared
**SELECT**
statement. It does not check whether the specified argument,
*res_info*
, is
**NULL**
and whether
*index*
is valid.

**Syntax**

#define
**CCI_GET_RESULT_INFO_PRECISION**
(
**T_CCI_COL_INFO**
*
*res_info*
, int
*index*
)

*   *res_info*
    : (IN) Column info pointer by
    `cci_get_result_info <#api_api_cci_getresultinfo_htm>`_



*   *index*
    : (IN) Column index



**Return Value**

*   Precision (int)



**CCI_GET_RESULT_INFO_SCALE**

**Description**

The
**CCI_GET_RESULT_INFO_SCALE**
macro gets the
*index*
-th column's scale of a prepared
**SELECT**
statement. It does not check whether the specified argument,
*res_info*
, is
**NULL**
and whether
* index*
is valid.

**Syntax**

#define
**CCI_GET_RESULT_INFO_SCALE**
(
**T_CCI_COL_INFO**
*
*res_info*
, int
*index*
)

*   *res_info*
    : (IN) Column info pointer by
    `cci_get_result_info <#api_api_cci_getresultinfo_htm>`_



*   *index*
    : (IN) Column index



**Return Value**

*   scale (int)



**CCI_GET_RESULT_INFO_TYPE**

**Description**

The
**CCI_GET_RESULT_INFO_TYPE**
macro gets the
*index*
-th column type of a prepared
**SELECT**
statement. It does not check whether the specified argument,
*res_info*
, is
**NULL**
and whether
*index*
is valid.

**Syntax**

#define
**CCI_GET_RESULT_INFO_TYPE**
(
**T_CCI_COL_INFO**
*
*res_info*
, int
*index*
)

*   *res_info*
    : (IN) pointer to the column information fetched by
    `cci_get_result_info <#api_api_cci_getresultinfo_htm>`_



*   *index*
    : (IN) Column index



**Return Value**

*   Column type (
    **T_CCI_U_TYPE**
    )



**CCI_IS_SET_TYPE, CCI_IS_MULTISET_TYPE, CCI_IS_SEQUENCE_TYPE, CCI_IS_COLLECTION_TYPE**

**Description**

The
**CCI_IS_SET_TYPE**
,
**CCI_IS_MULTISET_TYPE**
,
**CCI_IS_SEQUENCE_TYPE**
, and
**CCI_IS_COLLECTION_TYPE**
macros check whether
*u_type*
is set, multiset or sequence type.

**Syntax**

**#define CCI_IS_SET_TYPE**
(
*u_type*
)
**#define CCI_IS_MULTISET_TYPE**
(
*u_type*
)
**#define CCI_IS_SEQUENCE_TYPE**
(
*u_type*
)
**#define CCI_IS_COLLECTION_TYPE**
(
*u_type*
)

**Return Value**

*   **CCI_IS_SET_TYPE**



*   1: set



*   0: not set



*   **CCI_IS_MULTISET_TYPE**



*   1: multiset



*   0: not multiset



*   **CCI_IS_SEQUENCE_TYPE**



*   1: sequence



*   0: not sequence



*   **CCI_IS_SET_TYPE**



*   1: collection (set, multiset, sequence)



*   0: not collection



**cci_get_version**

**Description**

The cci_get_version function gets the version of CCI library. In case of version "9.0.0.0001" 9 is the major version, 4 is the minor version, and 0 is the patch version.

**Syntax**

int
**cci_get_version**
(int *
*major*
, int *
*minor*
, int *
*patch*
)

*   *major*
    : (OUT) major version



*   *minor*
    : (OUT) minor version



*   *patch*
    : (OUT) patch version



**Return Value**

*   Zero without exception (success)



**Remark**

In CUBRID for Linux, you can check the file version of CCI library by using the
**strings**
command.

$ strings /home/usr1/CUBRID/lib/libcascci.so | grep VERSION

VERSION=9.0.0.1

**cci_is_updatable**

**Description**

The
**cci_is_updatable**
function checks the SQL statement executing
`cci_prepare <#api_api_cci_prepare_htm>`_
() can make updatable result set (which means CCI_PREPARE_UPDATABLE is configued in
*flag*
when executing
`cci_prepare <#api_api_cci_prepare_htm>`_
()).

**Syntax**

int
**cci_is_updatable**
(int
*req_handle*
)

*   *req_handle*
    : (IN) Request handle for the prepared statement



**Return Value**

*   1: updatable



*   0: not updatable



**Error Code**

*   **CCI_ER_REQ_HANDLE**



**cci_next_result**

**Description**

The
**cci_next_result**
function gets results of next query if
**CCI_EXEC_QUERY_ALL**
*flag*
is set upon
`cci_execute <#api_api_cci_execute_htm>`_
(). The information about the query fetched by next_result can be obtained with
`cci_get_result_info <#api_api_cci_getresultinfo_htm>`_
. If next_result is executed successfully, the database is updated with the information of the current query.

The error code
**CAS_ER_NO_MORE_RESULT_SET**
means that no more result set exists.

**Syntax**

int
**cci_next_result**
(int
*req_handle*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *req_handle*
    : (IN) Request handle of a prepared statement



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Success



*   **SELECT**
    (sync mode): The number of results, (async mode) : 0



*   **INSERT**
    ,
    **UPDATE**
    : The number of records reflected



*   Others: 0



*   Failure: Error code



**Error Code**

*   **CCI_ER_REQ_HANDLE**



*   **CCI_ER_DBMS**



*   **CCI_ER_COMMUNICATION**



**cci_oid**

**Description**

CCI_OID_DROP: Deletes the given oid.

CCI_OID_IS_INSTANCE: Checks whether the given oid is an instance oid.

CCI_OID_LOCK_READ: Sets read lock on the given oid.

CCI_OID_LOCK_WRITE: Sets write lock on the given oid.

**Syntax**

int
**cci_oid**
(int
*conn_handle*
,
**T_CCI_OID_CMD**
*cmd*
, char *
*oid_str*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *cmd*
    : (IN) CCI_OID_DROP, CCI_OID_IS_INSTANCE, CCI_OID_LOCK_READ, CCI_OID_LOCK_WRITE



*   *oid_str*
    : (IN) oid



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   CCI_OID_IS_INSTANCE



*   0: Non-instance



*   1: Instance



*   0: error



*   CCI_OID_DROP, CCI_OID_LOCK_READ, CCI_OID_LOCK_WRITE



*   Error code (0: success)



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



*   **CCI_ER_OID_CMD**



*   **CCI_ER_OBJECT**



*   **CCI_ER_DBMS**



**cci_oid_get**

**Description**

The
**cci_oid_get**
function gets the attribute values of the given oid.
*attr_name*
is an array of the attributes, and it must end with
**NULL**
. If
*attr_name*
is NULL, the information of all attributes is fetched. The request handle has the same form as when the SQL statement "SELECT attr_name FROM oid_class WHERE oid_class = oid" is executed.

**Syntax**

int
**cci_oid_get**
(int
*conn_handle*
, char *
*oid_str*
, char **
*attr_name*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *oid_str*
    : (IN) oid



*   *attr_name*
    : (IN) A list of attributes



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Success: Request handle



*   Failure: Error code



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_NO_MORE_MEMORY**



*   **CCI_ER_CONNECT**



**cci_oid_get_class_name**

**Description**

The
**cci_oid_get_class_name**
function gets the class name of the given oid.

**Syntax**

int
**cci_oid_get_class_name**
(int
*conn_handle*
, char *
*oid_str*
, char *
*out_buf*
, int
*out_buf_len*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *oid_str*
    : (IN) oid



*   *out_buf*
    : (OUT) Out buffer



*   *out_buf_len*
    : (IN)
    *out_buf*
    length



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



*   **CCI_ER_OBJECT**



*   **CCI_ER_DBMS**



**cci_oid_put**

**Description**

The
**cci_oid_put**
function configures the
*attr_name*
attribute values of the given oid to
*new_val_str*
. The last value of
*attr_name*
must be
**NULL**
. Any value of any type must be represented as a string. The value represented as a string is applied to the database after being converted depending on the attribute type on the server. To insert a
**NULL**
value, configure the value of
*new_val_str*
[i] to
**NULL**
.

**Syntax**

int
**cci_oid_put**
(int conn_handle, char *oid_str, char **attr_name, char **new_val_str,
**T_CCI_ERROR**
*err_buf)

*   *conn_handle*
    : (IN) Connection handle



*   *oid_str*
    : (IN) oid



*   *attr_name*
    : (IN) The list of attribute names



*   *new_val_str*
    : (IN) The list of new values



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



**cci_oid_put2**

**Description**

The
**cci_oid_put2**
function sets the
*attr_name*
attribute values of the given oid to
*new_val*
. The last value of
*attr_name*
must be
**NULL**
. To insert a
**NULL**
value, set the value of
*new_val*
[i] to
**NULL**
.

The type of
*new_val*
[i] for
*a_type*
is shown in the table below.

**Type of new_val[i] for a_type**

+-----------------------+--------------------------------+
| **Type**              | **value type**                 |
|                       |                                |
+-----------------------+--------------------------------+
| **CCI_A_TYPE_STR**    | char*                          |
|                       |                                |
+-----------------------+--------------------------------+
| **CCI_A_TYPE_INT**    | int*                           |
|                       |                                |
+-----------------------+--------------------------------+
| **CCI_A_TYPE_FLOAT**  | float*                         |
|                       |                                |
+-----------------------+--------------------------------+
| **CCI_A_TYPE_DOUBLE** | double*                        |
|                       |                                |
+-----------------------+--------------------------------+
| **CCI_A_TYPE_BIT**    | **T_CCI_BIT***                 |
|                       |                                |
+-----------------------+--------------------------------+
| **CCI_A_TYPE_SET**    | **T_CCI_SET**                  |
|                       |                                |
+-----------------------+--------------------------------+
| **CCI_A_TYPE_DATE**   | **T_CCI_DATE***                |
|                       |                                |
+-----------------------+--------------------------------+
| **CCI_A_TYPE_BIGINT** | int64_t (For Windows: __int64) |
|                       |                                |
+-----------------------+--------------------------------+

**Syntax**

int
**cci_oid_put2**
(int
*conn_handle*
, char *
*oidstr*
, char **
*attr_name*
, void **
*new_val*
, int *
*a_type*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *oid_str*
    : (IN) oid



*   *attr_name*
    : (IN) A list of attribute names



*   *new_val*
    : (IN) A new value array



*   *a_type*
    : (IN)
    *new_val*
    type array



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



**Example**

char *attr_name[array_size]

void *attr_val[array_size]

int a_type[array_size]

int int_val

 

…

attr_name[0] = "attr_name0"

attr_val[0] = &int_val

a_type[0] = CCI_A_TYPE_INT

attr_name[1] = "attr_name1"

attr_val[1] = "attr_val1"

a_type[1] = CCI_A_TYPE_STR

 

…

attr_name[num_attr] = NULL

 

res = cci_put2(con_h,
*oid_str*
, attr_name, attr_val, a_type, &error)

…

**cci_prepare**

**Description**

The
**cci_prepare**
function prepares SQL execution by acquiring request handle for SQL statements. If a SQL statement consists of multiple queries, the preparation is performed only for the first query. With the parameter of this function, an address to
**T_CCI_ERROR**
where connection handle, SQL statement,
*flag*
, and error information are stored.

**CCI_PREPARE_UPDATABLE**
,
**CCI_PREPARE_INCLUDE_OID**
, or
**CCI_PREPARE_HOLDABLE**
can be configured in
*flag*
. If
**CCI_PREPARE_UPDATABLE**
is configured, updatable resultset is created and
**CCI_PREPARE_INCLUDE_OID**
is automatically configured.
**CCI_PREPARE_UPDATABLE**
and
**CCI_PREPARE_HOLDABLE**
can not be used simultaneously in
*flag.*

The default value of whether to keep result set after commit is cursor holdability. Thus, if you want to configure
**CCI_PREPARE_UPDATABLE**
in
*flag*
of
**ci_prepare**
(), you should call
**cci_set_holdable**
(conn, 0) first before calling
**cci_prepare**
() so that cursor cannot be maintained.

However, not all updatable resultsets are created even though
**CCI_PREPARE_UPDATABLE**
is configured. So you need to check if the results are updatable by using
`cci_is_updatable <#api_api_cci_isupdatable_htm>`_
after preparation. You can use
`cci_oid_put <#api_api_cci_oidput_htm>`_
or
`cci_oid_put2 <#api_api_cci_oidput2_htm>`_
to update result sets.

The conditions of updatable queries are as follows:

*   Must be
    **SELECT**
    .



*   OID can be included in the query result.



*   The column to be updated must be the one that belongs to the table specified in the
    **FROM**
    clause.



If
**CCI_PREPARE_HOLDABLE**
is set, a cursor is holded as long as result set is closed or connection is disconnected after the statement is committed (see CUBRID SQL Guide > Transaction Lock >Cursor Holdability).

**Syntax**

int
**cci_prepare**
(int
*conn_handle*
, char *
*sql_stmt*
, char
*flag*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *sql_stmt*
    : (IN) SQL statement



*   *flag*
    : (IN) prepare flag (
    **CCI_PREPARE_UPDATABLE**
    ,
    **CCI_PREPARE_INCLUDE_OID**
    , or
    **CCI_PREPARE_HOLDABLE**
    )



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Success: Request handle ID (int)



*   Failure: Error code



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_DBMS**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_STR_PARAM**



*   **CCI_ER_NO_MORE_MEMORY**



*   **CCI_ER_CONNECT**



*   **CCI_ER_QUERY_TIMEOUT**



*   **CCI_ER_LOGIN_TIMEOUT**



**cci_prepare_and_execute**

**Description**

The
**cci_prepare_and_execute**
function executes the SQL statement immediately and returns a request handle for the SQL statement. A request handle, SQL statement, the maximum length of a column to be fetched, error code, and the address of a
**T_CCI_ERROR**
construct variable in which error information being stored are specified as arguments.
*max_col_size*
is a value to configure the maximum length of a column to be sent to a client when the column of a SQL statement is
**CHAR**
,
**VARCHAR**
,
**NCHAR**
,
**VARNCHAR**
,
**BIT**
, or 
**VARBIT**
. If this value is 0, full length is fetched.

**Syntax**

int
**cci_prepare_and_execute**
(int
*conn_handle*
, char *
*sql_stmt*
, int
*max_col_size*
, int *
*exec_retval*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Request handle



*   *sql_stmt*
    : (IN) SQL statement



*   *max_col_size*
    : (IN) The maximum length of a column to be fetched when it is a string data type in bytes. If this value is 0, full length is fetched.



*   *exec_retval*
    : (OUT) Error code



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Success: Request handle ID (int)



*   Failure: Error code



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_DBMS**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_STR_PARAM**



*   **CCI_ER_NO_MORE_MEMORY**



*   **CCI_ER_CONNECT**



*   **CCI_ER_QUERY_TIMEOUT**



**cci_property_create**

**Description**

The
**cci_property_create**
function creates
**T_CCI_PROPERTIES**
struct to configure DATASOURCE of CCI.

**Syntax**

**T_CCI_PROPERTIES**
*
**cci_property_create**
()

**Return Value**

*   Success:
    **T_CCI_PROPERTIES**
    struct pointer in which memory is allocated



*   Failure:
    **NULL**



**See Also**

*   `cci_property_destroy <#api_api_cci_propertydestroy_htm>`_



*   `cci_property_get <#api_api_cci_propertyget_htm>`_



*   `cci_property_set <#api_api_cci_propertyset_htm>`_



*   `cci_datasource_borrow <#api_api_cci_datasourceborrow_htm>`_



*   `cci_datasource_create <#api_api_cci_datasourcecreate_htm>`_



*   `cci_datasource_destroy <#api_api_cci_datasourcedestroy_ht_5686>`_



*   `cci_datasource_release <#api_api_cci_datasourcerelease_ht_568>`_



**cci_property_destroy**

**Description**

The
**cci_property_destroy**
function destroys
**T_CCI_PROPERTIES**
struct.

**Syntax**

void
**cci_property_destroy**
(
**T_CCI_PROPERTIES**
*
*properties*
)

*   *properties*
    :
    **T_CCI_PROPERTIES**
    struct pointer to be destroyed



**Return Value**

None

**See Also**

*   `cci_property_create <#api_api_cci_propertycreate_htm>`_



*   `cci_property_get <#api_api_cci_propertyget_htm>`_



*   `cci_property_set <#api_api_cci_propertyset_htm>`_



*   `cci_datasource_borrow <#api_api_cci_datasourceborrow_htm>`_



*   `cci_datasource_create <#api_api_cci_datasourcecreate_htm>`_



*   `cci_datasource_destroy <#api_api_cci_datasourcedestroy_ht_5686>`_



*   `cci_datasource_release <#api_api_cci_datasourcerelease_ht_568>`_



**cci_property_get**

**Description**

The
**cci_property_get**
function retrieves the property value configured in the
**T_CCI_PROPERTIES**
struct.

**Syntax**

char *
**cci_property_get**
(
**T_CCI_PROPERTIES**
*
*properties*
, char *
*key*
)

*   *properties*
    :
    **T_CCI_PROPERTIES**
    struct pointer which gets value corresponding to
    *key*



*   *key*
    : Name of property to be retrieved (For name and description available properties, see the
    `cci_property_set <#api_api_cci_propertyset_htm>`_
    function)



**Return Value**

*   Success: String pointer of value corresponding to
    *key*



*   Failure: NULL



**See Also**

*   `cci_property_create <#api_api_cci_propertycreate_htm>`_



*   `cci_property_destroy <#api_api_cci_propertydestroy_htm>`_



*   `cci_property_set <#api_api_cci_propertyset_htm>`_



*   `cci_datasource_borrow <#api_api_cci_datasourceborrow_htm>`_



*   `cci_datasource_create <#api_api_cci_datasourcecreate_htm>`_



*   `cci_datasource_destroy <#api_api_cci_datasourcedestroy_ht_5686>`_



*   `cci_datasource_release <#api_api_cci_datasourcerelease_ht_568>`_



**cci_property_set**

**Description**

It configures a property value in
**T_CCI_PROPERTIES**
struct. The property names and its meanings that can be configured in the struct are as follows:

*   **pool_size**
    : Maximum number of connection (default value: 10)



*   **max_wait**
    : Maximum waiting time to get connection (default value: 1000 msec.)



*   **pool_prepared_statement**
    : Whether to enable statement pooling (default value: false)



*   **login_timeout**
    : Login timeout time (default value: 0 (unlimited))



*   **query_timeout**
    : Query timeout time (default value: 0 (unlimited))



*   **disconnect_on_query_timeout**
    : Whether to terminate connection when execution is discarded due to query execution timeout (default value: no)



*   **default_autocommit**
    : Auto-commit mode refreshed whenever
    **cci_datasource_borrow**
    is called; true or false



*   **default_isolation**
    : Transaction isolation level refreshed whenever
    **cci_datasource_borrow**
    is called



*   **default_lock_timeout**
    : lock_timeout refreshed whenever
    **cci_datasource_borrow**
    is called



If you configure
**default_autocommit**
,
**default_isolation**
, or
**default_lock_timeout**
value, connection for autocommit, isolation, or lock_timeout based on current configured value is returned when
**cci_datasource_borrow**
is called. If you do not configure it, connection for autocommit, isolation, or lock_timeout is returned with keeping the value that a user changed before.

**default_isolation**
has one of the following configuration values. For details on isolation level, see "CUBRID SQL Guide > Transaction and Lock > Transaction Isolation Level."

+----------------------------+---------------------------------------+
| **isolation_level**        | **Configuration Value**               |
|                            |                                       |
+----------------------------+---------------------------------------+
| SERIALIZABLE               | "TRAN_SERIALIZABLE"                   |
|                            |                                       |
+----------------------------+---------------------------------------+
| REPEATABLE READ CLASS with | "TRAN_REP_CLASS_REP_INSTANCE"         |
| REPEATABLE READ INSTANCES  | or "TRAN_REP_READ"                    |
|                            |                                       |
+----------------------------+---------------------------------------+
| REPEATABLE READ CLASS with | "TRAN_REP_CLASS_COMMIT_INSTANCE"      |
| READ COMMITTED INSTANCES   | or "TRAN_READ_COMMITTED"              |
|                            | or "TRAN_CURSOR_STABILITY"            |
|                            |                                       |
+----------------------------+---------------------------------------+
| REPEATABLE READ CLASS with | "TRAN_REP_CLASS_UNCOMMIT_INSTANCE"    |
| READ UNCOMMITTED INSTANCES | or "TRAN_READ_UNCOMMITTED"            |
|                            |                                       |
+----------------------------+---------------------------------------+
| READ COMMITTED CLASS with  | "TRAN_COMMIT_CLASS_COMMIT_INSTANCE"   |
| READ COMMITTED INSTANCES   |                                       |
|                            |                                       |
+----------------------------+---------------------------------------+
| READ COMMITTED CLASS with  | "TRAN_COMMIT_CLASS_UNCOMMIT_INSTANCE" |
| READ UNCOMMITTED INSTANCES |                                       |
|                            |                                       |
+----------------------------+---------------------------------------+

**Syntax**

int
**cci_property_set**
(
**T_CCI_PROPERTIES**
*
*properties*
, char *
*key*
, char *
*value*
)

*   *properties*
    :
    **T_CCI_PROPERTIES**
    struct pointer in which
    *key*
    and
    *value*
    are stored



*   *key*
    : String pointer of property name



*   *value*
    : String pointer of property value



**Return Value**

*   Success: 1



*   Failure: 0



**See Also**

*   `cci_property_create <#api_api_cci_propertycreate_htm>`_



*   `cci_property_destroy <#api_api_cci_propertydestroy_htm>`_



*   `cci_property_get <#api_api_cci_propertyget_htm>`_



*   `cci_datasource_borrow <#api_api_cci_datasourceborrow_htm>`_



*   `cci_datasource_create <#api_api_cci_datasourcecreate_htm>`_



*   `cci_datasource_destroy <#api_api_cci_datasourcedestroy_ht_5686>`_



*   `cci_datasource_release <#api_api_cci_datasourcerelease_ht_568>`_



**CCI_QUERY_RESULT_ERR_MSG**

**Description**

The
**CCI_QUERY_RESULT_ERR_MSG**
macro gets error messages about query results executed by
`cci_execute_batch <#api_api_cci_executebatch_htm>`_
,
`cci_execute_array <#api_api_cci_executearray_htm>`_
 or
`cci_execute_result <#api_api_cci_executeresult_htm>`_
function. If there is non error message, this macro returns ""(empty string). It does not check whether the specified argument,
*query_result*
, is
**NULL**
, and whether
*index*
is valid.

**Syntax**

#define
**CCI_QUERY_RESULT_ERR_MSG**
(
**T_CCI_QUERY_RESULT**
*
*query_result*
, int
*index*
)

*   *query_result*
    : (IN) Query results of to be executed



*   *index*
    : (IN) Column index (base: 1)



**Return Value**

*   Error message



**cci_query_result_free**

**Description**

The
**cci_query_result_free**
function deletes query results created by
`cci_execute_batch <#api_api_cci_executebatch_htm>`_
,
`cci_execute_array <#api_api_cci_executearray_htm>`_
or
`cci_execute_result <#api_api_cci_executeresult_htm>`_
function.

**Syntax**

int
**cci_query_result_free**
(
**T_CCI_QUERY_RESULT**
*
*query_result*
, int
*num_query*
)

*   *query_result*
    : (IN) Query results to be deleted



*   *num_query*
    : (IN) The number of arrays in
    *query_result*



**Return Value**

*   0: success



**Example**

T_CCI_QUERY_RESULT *qr;

char **sql_stmt;

 

 

res = cci_execute_array(conn, &qr, &err_buf);

 

 

cci_query_result_free(qr, res);

**CCI_QUERY_RESULT_RESULT**

**Description**

The
**CCI_QUERY_RESULT_RESULT**
macro gets the result count executed by
`cci_execute_batch <#api_api_cci_executebatch_htm>`_
,
`cci_execute_arra <#api_api_cci_executearray_htm>`_
, or
`cci_execute_result <#api_api_cci_executeresult_htm>`_
 function. It does not check whether the specified argument,
*query_result*
, is
**NULL**
and whether
*index*
is valid.

**Syntax**

#define
**CCI_QUERY_RESULT_RESULT**
(
**T_CCI_QUERY_RESULT**
*
*query_result*
, int
*index*
)

*   *query_result*
    : (IN) Query results to be retrieved



*   *index*
    : (IN) Column index (base: 1)



**Return Value**

*   result count



**CCI_QUERY_RESULT_STMT_TYPE**

**Description**

The
**CCI_QUERY_RESULT_STMT_TYPE**
macro gets the statement type executed by
`cci_execute_batch <#api_api_cci_executebatch_htm>`_
,
`cci_execute_array <#api_api_cci_executearray_htm>`_
or
`cci_execute_result <#api_api_cci_executeresult_htm>`_
function. It does not check whether the specified argument,
*query_result*
, is
**NULL**
and whether
*index*
is valid.

**Syntax**

#define
**CCI_QUERY_RESULT_STMT_TYPE**
(
**T_CCI_QUERY_RESULT**
*
*query_result*
, int
*index*
)

*   *query_result*
    : (IN) Query results to be retrieved



*   *index*
    : (IN) Column index (base: 1)



**Return Value**

*   statement type (
    **T_CCI_CUBRID_STMT**
    )



**cci_savepoint**

**Description**

The
**cci_savepoint**
 function configures savepoint or performs transaction rollback to a specified savepoint. If
*cmd*
is set to
**CCI_SP_SET**
, it configures savepoint and if it is set to
**CCI_SP_ROLLBACK**
, it rolls back transaction to specified savepoint.

**Syntax**

int
**cci_savepoint**
(int
*conn_handle*
,
**T_CCI_SAVEPOINT_CMD**
*cmd*
, char*
*savepoint_name*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *cmd*
    : (IN) CCI_SP_SET or CCI_SP_ROLLBACK



*   savepoint_name: (IN) Savepoint name



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_QUERY_TIMEOUT**



*   **CCI_ER_NO_MORE_MEMORY**



*   **CCI_ER_DBMS**



**Example**

con = cci_connect( … );
… /* query execute */
/* Sets the savepoint named "savepoint1"
cci_savepoint(con, CCI_SP_SET, "savepoint1",
*err_buf*
);
… /* query execute */
/* Rolls back to specified savepoint,"savepoint1" */
cci_savepoint(con, CCI_SP_ROLLBACK, "savepoint1",
*err_buf*
);

**cci_schema_info**

**Description**

The
**cci_schema_info**
function gets schema information. If it is performed successfully, the results are managed by the request handle and can be fetched by fetch and getdata. If you want to retrieve a
*class_name*
and
*attr_name*
by using pattern matching of the
**LIKE**
statement, you should configure
*flag*
.

Two types of
*flag*
s,
**CCI_CLASS_NAME_PATTERN_MATCH**
, and
**CCI_ATTR_NAME_PATTERN_MATCH**
, are used for pattern matching; you can configure these two
*flag*
s by using the OR operator ( | ). To use pattern matching, search by using the
**LIKE**
statement. For example, to search the information on a column of which
*class_name*
is "athlete" and
*attr_name*
is "code," you can enter as follows (in the example, "%code" is entered in the value of
*attr_name*
).

cci_schema_info(conn, CCI_SCH_ATTRIBUTE, "athlete", "%code", CCI_ATTR_NAME_PATTERN_MATCH, &error);The following table shows records composition of each
*type*
.

The table below shows record type for each
*type*
.

**Record for Each Type**

+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| **Type**                                                                                                                                                                 | **Column Order** | **Column Name**    | **Column Type**  |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_CLASS                                                                                                                                                            | 1                | NAME               | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 2                | TYPE               | short            |
|                                                                                                                                                                          |                  |                    | 0 : system class |
|                                                                                                                                                                          |                  |                    | 1 : vclass       |
|                                                                                                                                                                          |                  |                    | 2 : class        |
|                                                                                                                                                                          |                  |                    | 3 : proxy        |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_VCLASS                                                                                                                                                           | 1                | NAME               | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 2                | TYPE               | short            |
|                                                                                                                                                                          |                  |                    | 1 : vclass       |
|                                                                                                                                                                          |                  |                    | 3 : proxy        |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_QUERY_SPEC                                                                                                                                                       | 1                | QUERY_SPEC         | char*            |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_ATTRIBUTE                                                                                                                                                        | 1                | NAME               | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 2                | DOMAIN             | int              |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 3                | SCALE              | int              |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 4                | PRECISION          | int              |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 5                | INDEXED            | int              |
|                                                                                                                                                                          |                  |                    | 1 : indexed      |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 6                | NON_NULL           | int              |
|                                                                                                                                                                          |                  |                    | 1 : non null     |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 7                | SHARED             | int              |
|                                                                                                                                                                          |                  |                    | 1 : shared       |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 8                | UNIQUE             | int              |
|                                                                                                                                                                          |                  |                    | 1 : unique       |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 9                | DEFAULT            | void*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 10               | ATTR_ORDER         | int              |
|                                                                                                                                                                          |                  |                    | base : 1         |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 11               | CLASS_NAME         | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 12               | SOURCE_CLASS       | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 13               | IS_KEY             | short            |
|                                                                                                                                                                          |                  |                    | 1 : key          |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| When the attribute of the CCI_SCH_CLASS_ATTRIBUTE column is INSTANCE or SHARED, the order and the name values are identical to those of the column of CCI_SCH_ATTRIBUTE. |                  |                    |                  |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_CLASS_METHOD                                                                                                                                                     | 1                | NAME               | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 2                | RET_DOMAIN         | int              |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 3                | ARG_DOMAIN         | char*            |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_METHOD_FILE                                                                                                                                                      | 1                | METHOD_FILE        | char*            |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_SUPERCLASS                                                                                                                                                       | 1                | CLASS_NAME         | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 2                | TYPE               | short            |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_SUBCLASS                                                                                                                                                         | 1                | CLASS_NAME         | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 2                | TYPE               | short            |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_CONSTRAINT                                                                                                                                                       | 1                | TYPE               | int              |
|                                                                                                                                                                          |                  | 0 : unique         |                  |
|                                                                                                                                                                          |                  | 1 : index          |                  |
|                                                                                                                                                                          |                  | 2 : reverse unique |                  |
|                                                                                                                                                                          |                  | 3 : reverse index  |                  |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 2                | NAME               | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 3                | ATTR_NAME          | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 4                | NUM_PAGES          | int              |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 5                | NUM_KEYS           | int              |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 6                | PRIMARY_KEY        | short            |
|                                                                                                                                                                          |                  | 1 : primary key    |                  |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 7                | KEY_ORDER          | short            |
|                                                                                                                                                                          |                  |                    | base : 1         |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_TRIGGER                                                                                                                                                          | 1                | NAME               | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 2                | STATUS             | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 3                | EVENT              | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 4                | TARGET_CLASS       | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 5                | TARGET_ATTR        | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 6                | ACTION_TIME        | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 7                | ACTION             | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 8                | PRIORITY           | float            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 9                | CONDITION_TIME     | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 10               | CONDITION          | char*            |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_CLASS_PRIVILEGE                                                                                                                                                  | 1                | CLASS_NAME         | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 2                | PRIVELEGE          | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 3                | GRANTABLE          | char*            |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_ATTR_PRIVILEGE                                                                                                                                                   | 1                | ATTR_NAME          | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 2                | PRIVILEGE          | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 3                | GRANTABLE          | char*            |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_DIRECT_SUPER_CLASS                                                                                                                                               | 1                | CLASS_NAME         | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 2                | SUPER_CLASS_NAME   | char*            |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_PRIMARY_KEY                                                                                                                                                      | 1                | CLASS_NAME         | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 2                | ATTR_NAME          | char*            |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 3                | KEY_SEQ            | short            |
|                                                                                                                                                                          |                  |                    | base : 1         |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 4                | KEY_NAME           | char*            |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_IMPORTED_KEYS                                                                                                                                                    | 1                | PKTABLE_NAME       | char**           |
|                                                                                                                                                                          |                  |                    |                  |
| Used to retrieve primary key columns that are referred by a foreign key column in a given table. The results are sorted by PKTABLE_NAME and KEY_SEQ.                     |                  |                    |                  |
|                                                                                                                                                                          |                  |                    |                  |
| If this type is specified as a parameter, a foreign key table is specified for                                                                                           |                  |                    |                  |
| class_name                                                                                                                                                               |                  |                    |                  |
| , and                                                                                                                                                                    |                  |                    |                  |
| NULL                                                                                                                                                                     |                  |                    |                  |
| is specified for                                                                                                                                                         |                  |                    |                  |
| attr_name                                                                                                                                                                |                  |                    |                  |
| .                                                                                                                                                                        |                  |                    |                  |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 2                | PKCOLUMN_NAME      | char**           |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 3                | FKTABLE_NAME       | char**           |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 4                | FKCOLUMN_NAME      | char**           |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 5                | KEY_SEQ            | char**           |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 6                | UPDATE_ACTION      | int*             |
|                                                                                                                                                                          |                  | cascade=0          |                  |
|                                                                                                                                                                          |                  | restrict=1         |                  |
|                                                                                                                                                                          |                  | no action=2        |                  |
|                                                                                                                                                                          |                  | set null=3         |                  |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 7                | DELETE_ACTION      | int*             |
|                                                                                                                                                                          |                  | cascade=0          |                  |
|                                                                                                                                                                          |                  | restrict=1         |                  |
|                                                                                                                                                                          |                  | no action=2        |                  |
|                                                                                                                                                                          |                  | set null=3         |                  |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 8                | FK_NAME            | char**           |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 9                | PK_NAME            | char**           |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_EXPORTED_KEYS                                                                                                                                                    | 1                | PKTABLE_NAME       | char**           |
| Used to retrieve primary key columns that are referred by all foreign key columns. The results are sorted by FKTABLE_NAME and KEY_SEQ.                                   |                  |                    |                  |
|                                                                                                                                                                          |                  |                    |                  |
| If this type is specified as a parameter, a primary key table is specified for                                                                                           |                  |                    |                  |
| class_name                                                                                                                                                               |                  |                    |                  |
| , and                                                                                                                                                                    |                  |                    |                  |
| NULL                                                                                                                                                                     |                  |                    |                  |
| is specified for                                                                                                                                                         |                  |                    |                  |
| attr_name                                                                                                                                                                |                  |                    |                  |
| .                                                                                                                                                                        |                  |                    |                  |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 2                | PKCOLUMN_NAME      | char**           |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 3                | FKTABLE_NAME       | char**           |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 4                | FKCOLUMN_NAME      | char**           |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 5                | KEY_SEQ            | char**           |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 6                | UPDATE_ACTION      | int*             |
|                                                                                                                                                                          |                  | cascade=0          |                  |
|                                                                                                                                                                          |                  | restrict=1         |                  |
|                                                                                                                                                                          |                  | no action=2        |                  |
|                                                                                                                                                                          |                  | set null=3         |                  |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 7                | DELETE_ACTION      | int*             |
|                                                                                                                                                                          |                  | cascade=0          |                  |
|                                                                                                                                                                          |                  | restrict=1         |                  |
|                                                                                                                                                                          |                  | no action=2        |                  |
|                                                                                                                                                                          |                  | set null=3         |                  |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 8                | FK_NAME            | char**           |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 9                | PK_NAME            | char**           |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+
| CCI_SCH_CROSS_REFERENCE                                                                                                                                                  | 1                | PKTABLE_NAME       | char**           |
| Used to retrieve foreign key information when primary keys and foreign keys in a given table are cross referenced. The results are sorted by FKTABLE_NAME and KEY_SEQ.   |                  |                    |                  |
|                                                                                                                                                                          |                  |                    |                  |
| If this type is specified as a parameter, a primary key is specified for                                                                                                 |                  |                    |                  |
| class_name                                                                                                                                                               |                  |                    |                  |
| , and a foreign key table is specified for                                                                                                                               |                  |                    |                  |
| attr_name                                                                                                                                                                |                  |                    |                  |
| .                                                                                                                                                                        |                  |                    |                  |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 2                | PKCOLUMN_NAME      | char**           |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 3                | FKTABLE_NAME       | char**           |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 4                | FKCOLUMN_NAME      | char**           |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 5                | KEY_SEQ            | char**           |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 6                | UPDATE_ACTION      | int*             |
|                                                                                                                                                                          |                  | cascade=0          |                  |
|                                                                                                                                                                          |                  | restrict=1         |                  |
|                                                                                                                                                                          |                  | no action=2        |                  |
|                                                                                                                                                                          |                  | set null=3         |                  |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 7                | DELETE_ACTION      | int*             |
|                                                                                                                                                                          |                  | cascade=0          |                  |
|                                                                                                                                                                          |                  | restrict=1         |                  |
|                                                                                                                                                                          |                  | no action=2        |                  |
|                                                                                                                                                                          |                  | set null=3         |                  |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 8                | FK_NAME            | char**           |
|                                                                                                                                                                          |                  |                    |                  |
|                                                                                                                                                                          +------------------+--------------------+------------------+
|                                                                                                                                                                          | 9                | PK_NAME            | char**           |
|                                                                                                                                                                          |                  |                    |                  |
+--------------------------------------------------------------------------------------------------------------------------------------------------------------------------+------------------+--------------------+------------------+

In the
**cci_schema_info**
function, the
*type*
argument supports the pattern matching of the
**LIKE**
statement for the
*class_name*
and
*attr_name*
.

**type, class_name, and attr_name That Supports Pattern Matching**

+-------------------------------------+----------------+--------------------------------------------------------------------------------------------------------------------------------+
| **type**                            | **class_name** | **attr_name**                                                                                                                  |
|                                     |                |                                                                                                                                |
+-------------------------------------+----------------+--------------------------------------------------------------------------------------------------------------------------------+
| CCI_SCH_CLASS (VCLASS)              | string         | NULL                                                                                                                           |
|                                     |                |                                                                                                                                |
+-------------------------------------+----------------+--------------------------------------------------------------------------------------------------------------------------------+
| CCI_SCH_ATTRIBUTE (CLASS ATTRIBUTE) | string         | string or NULL                                                                                                                 |
|                                     |                | (If the value is NULL and the flag is CCI_ATTR_NAME_PATTERN_MATCH, information on all columns of the table will be displayed.) |
|                                     |                |                                                                                                                                |
+-------------------------------------+----------------+--------------------------------------------------------------------------------------------------------------------------------+
| CCI_SCH_CLASS_PRIVILEGE             | string         | NULL                                                                                                                           |
|                                     |                |                                                                                                                                |
+-------------------------------------+----------------+--------------------------------------------------------------------------------------------------------------------------------+
| CCI_SCH_ATTR_PRIVILEGE              | NULL           | string                                                                                                                         |
|                                     |                |                                                                                                                                |
+-------------------------------------+----------------+--------------------------------------------------------------------------------------------------------------------------------+
| CCI_SCH_PRIMARY_KEY                 | string         | NULL                                                                                                                           |
|                                     |                |                                                                                                                                |
+-------------------------------------+----------------+--------------------------------------------------------------------------------------------------------------------------------+
| CCI_SCH_TRIGGER                     | string         | NULL                                                                                                                           |
|                                     |                |                                                                                                                                |
+-------------------------------------+----------------+--------------------------------------------------------------------------------------------------------------------------------+

If the pattern
*flag*
is not configured, exact matching will be used for the given table and column names; in this case, no result will be returned if the value is
**NULL**
. If
*flag*
is configured and the value is
**NULL**
, the result will be the same as when "%" is given in the
**LIKE**
statement

**Note**
TYPE column of
**CCI_SCH_CLASS**
and
**CCI_SCH_VCLASS**
: The proxy type is added. When used in OLEDB, ODBC or PHP, vclass is represented without distinguishing between proxy and vclass.

**Syntax**

**int**
**cci_schema_info**
(int
*conn_handle*
,
**T_CCI_SCHEMA_TYPE**
*type*
,
**char**
*
*class_name*
,
**char**
*
*attr_name*
,
**char**
*flag*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *type*
    : (IN) Schema type



*   c
    *lass_name*
    : (IN) Class name or NULL



*   *attr_name*
    : (IN) Attribute name or NULL



*   *flag*
    : (IN) Pattern matching flag (
    **CCI_CLASS_NAME_PATTERN_MATCH**
    or
    **CCI_ATTR_NAME_PATTERN_MATCH**
    )



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Success: Request handle



*   Failure: Error code



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_DBMS**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_NO_MORE_MEMORY**



*   **CCI_ER_CONNECT**



**Example**

    conn = cci_connect ("localhost", 33000, "demodb", "dba", "");

 

// get all columns’ information of table "athlete"

    req = cci_schema_info(conn, CCI_SCH_ATTRIBUTE, "athlete", NULL, CCI_ATTR_NAME_PATTERN_MATCH, &error);

 

// get info. of table "athlete"’s column "code".

//req = cci_schema_info(conn, CCI_SCH_ATTRIBUTE, "athlete", "code", 0, &error);

    if (req < 0 )

    {

        fprintf(stdout, "(%s, %d) ERROR : %s [%d] \n\n", __FILE__, __LINE__,error.err_msg, error.err_code );

        goto _END;

    }

res_col_info = cci_get_result_info(req, &cmd_type, &col_count);

    if (!res_col_info && col_count == 0)

    {

        fprintf(stdout, "(%s, %d) ERROR : cci_get_result_info\n\n", __FILE__, __LINE__);

        goto _END;

    }

    res = cci_cursor(req, 1, CCI_CURSOR_FIRST, &error);

    if (res == CCI_ER_NO_MORE_DATA)

    {

        goto _END_WHILE;

    }

    if (res < 0)

    {

        fprintf(stdout, "(%s, %d) ERROR : %s [%d] \n\n", __FILE__, __LINE__,error.err_msg, error.err_code );

        goto _END_WHILE;

    }

 

 

    while (1)

    {

        res = cci_fetch(req, &error);

        if (res <  0)

        {

            fprintf(stdout, "(%s, %d) ERROR : %s [%d] \n\n", __FILE__, __LINE__,error.err_msg, error.err_code );

            goto _END_WHILE;

        }

 

        for (i = 1; i <= col_count; i++)

        {

            if ((res = cci_get_data(req, i, CCI_A_TYPE_STR, &buffer, &ind))<0)

            {

                goto _END_WHILE;

            }

            strcat(query_result_buffer, buffer);

            strcat(query_result_buffer, "|");

        }

        strcat(query_result_buffer, "\n");

 

        res = cci_cursor(req, 1, CCI_CURSOR_CURRENT, &error);

        if (res == CCI_ER_NO_MORE_DATA)

        {

            goto _END_WHILE;

        }

        if (res < 0)

        {

            fprintf(stdout, "(%s, %d) ERROR : %s [%d] \n\n", __FILE__, __LINE__,error.err_msg, error.err_code );

            goto _END_WHILE;

        }

    }

_END_WHILE:

    res = cci_close_req_handle(req);

    if (res <  0)

        goto _END;

 

_END:

    if (req > 0)

        cci_close_req_handle(req);

    if ( conn > 0)

        res = cci_disconnect(conn, &error);

    if (res < 0)

        fprintf(stdout, "(%s, %d) ERROR : %s [%d] \n\n", __FILE__, __LINE__,error.err_msg, error.err_code );

 

        fprintf(stdout, "Result : %s\n", query_result_buffer);

 

    return 0;

**cci_set_allocators**

**Description**

The
**cci_set_allocators**
function registers the memory allocation/release functions used by users. By executing this function, you can use user-defined functions for every memory allocation/release jobs being processed in CCI API. If you do not use this function, system functions (malloc, free, realloc, and calloc) are used.

**Syntax**

int
**cci_set_allocators**
(
**CCI_MALLOC_FUNCTION**
*malloc_func*
,
**CCI_FREE_FUNCTION**
*free_func*
,
**CCI_REALLOC_FUNCTION**
*realloc_func*
,
**CCI_CALLOC_FUNCTION**
*calloc_func*
)

*   *malloc_func*
    : (IN) Pointer of externally defined function corresponding to malloc



*   *free_func*
    : (IN) Pointer of externally defined function corresponding to free



*   *realloc_func*
    : (IN) Pointer of externally defined function corresponding to realloc



*   *calloc_func*
    : (IN) Pointer of externally defined function corresponding to calloc



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_NOT_IMPLEMENTED**



**Example**

/*

       How to build: gcc -Wall -g -o test_cci test_cci.c -I${CUBRID}/include -L${CUBRID}/lib -lcascci

*/

 

#include <stdio.h>

#include <stdlib.h>

#include "cas_cci.h"

 

void *

my_malloc(size_t size)

{

  printf ("my malloc: size: %ld\n", size);

  return malloc (size);

}

 

void *

my_calloc(size_t nm, size_t size)

{

  printf ("my calloc: nm: %ld, size: %ld\n", nm, size);

  return calloc (nm, size);

}

 

void *

my_realloc(void *ptr, size_t size)

{

  printf ("my realloc: ptr: %p, size: %ld\n", ptr, size);

  return realloc (ptr, size);

}

 

void

my_free(void *ptr)

{

  printf ("my free: ptr: %p\n", ptr);

  return free (ptr);

}

 

 

int

test_simple (int con)

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

**cci_set_autocommit**

**Description**

The
**cci_set_autocommit**
function configures the auto-commit mode of current database connection. It is only used to turn ON/OFF of auto-commit mode. When this function is called, every transaction being processed is committed regardless of configured mode.

Note that
**CCI_DEFAULT_AUTOCOMMIT**
, broker parameter configured in the
**cubrid_broker.conf**
file, determines whether it is in auto-commit mode upon program startup.

**Syntax**

int
**cci_set_autocommit**
(int
*conn_handle*
, CCI_AUTOCOMMIT_MODE 
*autocommit_mode*
)

*   *conn_handle*
    : (IN) Connection handle



*   *autocommit_mode*
    : (IN) Configures the auto-commit mode. It has one of the following value:
    **CCI_AUTOCOMMIT_FALSE**
    or
    **CCI_AUTOCOMMIT_TRUE**
    .



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_CON_HANDLE**



**cci_set_db_parameter**

**Description**

The
**cci_set_db_parameter**
function configures a system parameter. For the type of
*value*
for
*param_name*
, see
`cci_get_db_parameter <#api_api_cci_getdbparam_htm>`_
().

**Syntax**

int
**cci_set_db_parameter**
(int
*conn_handle*
,
**T_CCI_DB_PARAM**
*param_name*
, void*
*value*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *param_name*
    : (IN) System parameter name



*   *value*
    : (IN) Parameter value



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code (0: success)



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_PARAM_NAME**



*   **CCI_ER_DBMS**



*   **CCI_ER_COMMUNICATION**



*   **CCI_ER_CONNECT**



**cci_set_element_type**

**Description**

The
**cci_set_element_type**
function gets the element type of the
**T_CCI_SET**
type value.

**Syntax**

int
**cci_set_element_type**
(
**T_CCI_SET**
*set*
)

*   *set*
    : (IN) cci set pointer



**Return Value**

*   Type



**cci_set_free**

**Description**

The
**cci_set_free**
function releases the memory allocated to the type value of
**T_CCI_SET**
fetched by
**CCI_A_TYPE_SET**
with cci_get_data(). The
**T_CCI_SET**
type value can be created through fetching
`cci_get_data <#api_api_cci_getdata_htm>`_
() or
`cci_set_make <#api_api_cci_setmake_htm>`_
() function.

**Syntax**

void
**cci_set_free**
(
**T_CCI_SET**
*set*
)

*   *set*
    : (IN) cci set pointer



**Return Value**

*   None



**cci_set_get**

**Description**

The
**cci_set_get**
function gets the
*index*
-th data for the type value of
**T_CCI_SET**
. The data type of
*value*
for
*a_type*
is shown in the table below.

+-----------------------+----------------------------------+
| **a_type**            | **value Type**                   |
|                       |                                  |
+-----------------------+----------------------------------+
| **CCI_A_TYPE_STR**    | char**                           |
|                       |                                  |
+-----------------------+----------------------------------+
| **CCI_A_TYPE_INT**    | int*                             |
|                       |                                  |
+-----------------------+----------------------------------+
| **CCI_A_TYPE_FLOAT**  | float*                           |
|                       |                                  |
+-----------------------+----------------------------------+
| **CCI_A_TYPE_DOUBLE** | double*                          |
|                       |                                  |
+-----------------------+----------------------------------+
| **CCI_A_TYPE_BIT**    | **T_CCI_BIT***                   |
|                       |                                  |
+-----------------------+----------------------------------+
| **CCI_A_TYPE_DATE**   | **T_CCI_DATE***                  |
|                       |                                  |
+-----------------------+----------------------------------+
| **CCI_A_TYPE_BIGINT** | int64_t* (For Windows: __int64*) |
|                       |                                  |
+-----------------------+----------------------------------+

**Syntax**

int
**cci_set_get**
(
**T_CCI_SET**
*set*
, int
*index*
,
**T_CCI_A_TYPE**
*a_type*
, void *
*value*
, int *
*indicator*
)

*   *set*
    : (IN) cci set pointer



*   *index*
    : (IN) set index (base: 1)



*   *a_type*
    : (IN) Type



*   *value*
    : (OUT) Result buffer



*   *indicator*
    : (OUT) null indicator



**Return Value**

*   Error code



**Error Code**

*   **CCI_ER_SET_INDEX**



*   **CCI_ER_TYPE_CONVERSION**



*   **CCI_ER_NO_MORE_MEMORY**



*   **CCI_ER_COMMUNICATION**



**cci_set_holdability**

**Description**

Sets whether to enable or disable cursor holdability of the result set from the connection level. When it is 1, the connection is disconnected or the cursor is holdable until the result set is intentionally closed regardless of commit. When it is 0, the result set is closed when committed and the cursor is not holdable. For more details on cursor holdability, see Cursor Holdability.

**Syntax**

int
**cci_set_holdability**
(int
*conn_handle*
,int
*holdable*
)

*   *conn_handle*
    : (IN) Connection handle



*   *holdable*
    : (IN) Cursor holdability setting value (0: not holdable, 1: holdable)



**Return Value**

*   Error Code



**Error Code**

*   **CCI_ER_INVALID_HOLDABILITY**



**cci_set_isolation_level**

**Description**

The
**cci_set_isolation_level**
function sets the transaction isolation level of connections. All further transactions for the given connections work as
*new_isolation_level*
.

**Note**
If the transaction isolation level is set by cci_set_db_parameter(), only the current transaction is affected. When the transaction is complete, the transaction isolation level returns to the one set by CAS. You must use
**cci_set_isolation_level**
() to set the isolation level for the entire connection.

**Syntax**

int
**cci_set_isolation_level**
(int
*conn_handle*
,
**T_CCI_TRAN_ISOLATION**
 
*new_isolation_level*
,
**T_CCI_ERROR**
*
*err_buf*
)

*   *conn_handle*
    : (IN) Connection handle



*   *new_isolation_level*
    : (IN) Transaction isolation level



*   *err_buf*
    : (OUT) Database error buffer



**Return Value**

*   Error code



**Error Code**

*   **CCI_ER_CON_HANDLE**



*   **CCI_ER_CONNECT**



*   **CCI_ER_ISOLATION_LEVEL**



*   **CCI_ER_DBMS**



**cci_set_make**

**Description**

The
**cci_set_make**
function makes a set of a new
**CCI_A_TYPE_SET**
type. The created set is sent to the server as
**CCI_A_TYPE_SET**
by
`cci_bind_param <#api_api_cci_bindparam_htm>`_
(). The memory for the set created by
**cci_set_make()**
must be freed by
**cci_set_free()**
. The type of
*value*
for
*u_type*
is shown in the table below.

**Syntax**

int
**cci_set_make**
(
**T_CCI_SET**
*
*set*
,
**T_CCI_U_TYPE**
*u_type*
, int
*size*
, void *
*value*
, int *
*indicator*
)

*   *set*
    : (OUT) cci set pointer



*   *u_type*
    : (IN) Element type



*   *size*
    : (IN) set size



*   *value*
    : (IN) set element



*   *indicator*
    : (IN) null indicator array



**Return Value**

*   Error code



**cci_set_max_row**

**Description**

The
**cci_set_max_row**
function configures the maximum number of records for the results of the
**SELECT**
statement executed by
`cci_execute <#api_api_cci_execute_htm>`_
. If the
*max*
value is 0, it is the same as not setting the value.

**Syntax**

int
**cci_set_max_row**
(int
*req_handle*
, int
*max*
)

*   *req_handle*
    : (IN) Connection handle



*   *max*
    : (IN) The maximum number of rows



**Return Value**

*   Error code



**Example**

req = cci_prepare( … );

cci_set_max_row(req, 1);

cci_execute( … );

**cci_set_query_timeout**

**Description**

The
**cci_set_query_timeout**
function configures timeout value for query execution.

The timeout value configured by
**cci_set_query_timeout**
affects
**cci_prepare**
,
**cci_execute**
,
**cci_execute_array**
, the
**cci_execute_batch**
functions. When timeout occurs in the function and if the
**disconnect_on_query_timeout**
value configured in
**cci_connect_with_url**
connection URL is yes, it returns the
**CCI_ER_QUERY_TIMEOUT**
error.

These functions can return the
**CCI_ER_LOGIN_TIMEOUT**
error in case that
**login_timeout**
is configured in the connection URL, which is an argument of
**cci_connect_with_url**
function; this means that login timeout happens between application client and CAS during re-connection.

It is going through the process of re-connection between application client and CAS when an application restarts or it is re-scheduled. Re-scheduling is a process that CAS chooses an application client, and starts and stops connection in the unit of transaction. If
**KEEP_CONNECTION**
, broker parameter, is OFF, it always happens; if AUTO, it can happen depending on its situation. For details, see the description of
**KEEP_CONNECTION**
in the "Performance Tuning > Broker Configuration > Parameter by Broker."

**Syntax**

int
**cci_set_query_timeout**
(int
*req_handle*
, int
*milli_sec*
);

*   *req_handle*
    : (IN) Request handle



*   *milli_sec*
    : (IN) Timeout (unit: msec.)



**Return Value**

*   Success: Request handle ID (int)



*   Failure: Error code



**Error Code**

*   **CCI_ER_REQ_HANDLE**



**cci_set_size**

**Description**

The
**cci_set_size**
function gets the number of elements for the type value of
**T_CCI_SET**
.

**Syntax**

int
**cci_set_size**
(
**T_CCI_SET**
*set*
)

*   *set*
    : (IN) cci set pointer



**Return Value**

*   Size

