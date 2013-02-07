****************************************************
Configuring Environment Variable and Starting CUBRID
****************************************************

Configuring the Environment Variable
====================================

The following environment variables need to be set in order to use the CUBRID. The necessary environment variables are automatically set when the CUBRID system is installed or can be changed, as needed, by the user.

**CUBRID Environment Variables**

*   **CUBRID** : The default environment variable that designates the location where the CUBRID is installed. This variable must be set accurately since all programs included in the CUBRID system uses this environment variable as reference.

*   **CUBRID_DATABASES** : The environment variable that designates the location of the **databases.txt** file. The CUBRID system stores the absolute path of database volumes in the **$CUBRID_DATABASES/databases.txt** file. See :ref:`databases-txt-file`.

*   **CUBRID_CHARSET** : The environment variable that specifies database locale (language+character set) in CUBRID. The initial value upon CUBRID installation is **en_US**. If character set is omitted after language name, ISO-8859-1(.iso88591) will be specified by default. For more information, see :ref:`Language Setting <language-setting>`.

*   **CUBRID_MSG_LANG** : The environment variable that specifies usage messages and error messages in CUBRID. The initial value upon start is not defined. If it's not defined, it follows the value of **CUBRID_CHARESET**. If character set is omitted after **en_US**, ISO-8859-1(.iso88591) will be specified by default. For more information, see :ref:`Language Setting <language-setting>`.

*   **CUBRID_TMP** : The environment variable that specifies the location where the cub_master process and the cub_broker process store the UNIX domain socket file in CUBRID for Linux. If it is not specified, the cub_master process stores the UNIX domain socket file under the **/tmp** directory and the cub_broker process stores the UNIX domain socket file under the **$CUBRID/var/CUBRID_SOCK** directory (not used in CUBRID for Windows).

**CUBRID_TMP** value has some constraints, which are as follows:

* Since the maximum length of the UNIX socket path is 108, when a path longer than 108 is entered in **$CUBRID_TMP**, an error is displayed. ::

	$ export CUBRID_TMP=/home1/siwankim/cubrid=/tmp/123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789

	$ cubrid server start apricot

	The $CUBRID_TMP is too long. (/home1/siwankim/cubrid=/tmp/123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789)

* When the relative path is entered, an error is displayed. ::

	$ export CUBRID_TMP=./var $ cubrid server start apricot

	The $CUBRID_TMP should be an absolute path. (./var)

**CUBRID_TMP** can be used to avoid the following problems that can occur at the default path of the UNIX domain socket that CUBRID uses.

* **/tmp** is used to store the temporary files in Linux. If the system administrator periodically and voluntarily cleans the space, the UNIX domain socket may be removed. In this case, configure **$CUBRID_TMP** to another path, not **/tmp**.
* The maximum length of the UNIX socket path is 108. When the installation path of CUBRID is too long and the **$CUBRID/var/CUBRID_SOCK** path that store the UNIX socket path for cub_broker exceeds 108 characters, the broker cannot be executed. Therefore, the path of **$CUBRID_TMP** must not exceed 1008 characters.

The above mentioned environment variables are set when the CUBRID is installed. However, the following commands can be used to verify the setting.

For Linux : ::

	% printenv CUBRID
	% printenv CUBRID_DATABASES
	% printenv CUBRID_CHARSET
	% printenv CUBRID TMP

In Windows : ::

	C:\> set CUBRID

**OS Environment and Java Environment Variables**

*   PATH: In the Linux environment, the directory **$CUBRID/bin**, which includes a CUBRID system executable file, must be included in the PATH environment variable.

*   LD_LIBRARY_PATH: In the Linux environment, **$CUBRID/lib**, which is the CUBRID system’s dynamic library file (libjvm.so), must be included in the **LD_LIBRARY_PATH** (or **SHLIB_PATH** or **LIBPATH**) environment variable.

*   Path: In the Windows environment, the **%CUBRID%\bin**, which is a directory that contains CUBRID system’s execution file, must be included in the **Path** environment variable.

*   JAVA_HOME: To use the Java stored procedure in the CUBRID system, the Java Virtual Machine (JVM) version 1.6 or later must be installed, and the **JAVA_HOME** environment variable must designate the concerned directory.
    See the :ref:`jsp_environment-configuration`.

**Configuring the Environment Variable**

**For Windows**

If the CUBRID system has been installed on Windows, then the installation program automatically sets the necessary environment variable. Select [Systems Properties] in [My Computer] and select the [Advanced] tab. Click the [Environment Variable] button and check the setting in the [System Variable]. The settings can be changed by clicking on the [Edit] button. See the Windows help for more information on how to change the environment variable on Windows.

.. image:: /images/image4.png

**For Linux**

If the CUBRID system has been installed on Linux, the installation program automatically creates the **.cubrid.sh** or **.cubrid.csh** file and makes configurations so that the files are automatically called from the installation account’s
shell log-in script. The following is the contents of . **cubrid.sh** environment variable configuration that was created in an environment that uses sh, bash, etc. ::

	CUBRID=/home1/cub_user/CUBRID
	CUBRID_DATABASES=/home1/cub_user/CUBRID/databases
	CUBRID_CHARSET=en_US
	ld_lib_path=`printenv LD_LIBRARY_PATH`
	
	if [ "$ld_lib_path" = "" ]
	then
	    LD_LIBRARY_PATH=$CUBRID/lib
	else
	    LD_LIBRARY_PATH=$CUBRID/lib:$LD_LIBRARY_PATH
	fi
	
	SHLIB_PATH=$LD_LIBRARY_PATH
	LIBPATH=$LD_LIBRARY_PATH
	PATH=$CUBRID/bin:$CUBRID/cubridmanager:$PATH
	
	export CUBRID
	export CUBRID_DATABASES
	export CUBRID_CHARSET
	export LD_LIBRARY_PATH
	export SHLIB_PATH
	export LIBPATH
	export PATH

.. _language-setting:

**Language Setting**

The language that will be used in the CUBRID DBMS can be designated with the **CUBRID_CHARSET** environment variable. The following are examples of values that can currently be set in the **CUBRID_CHARSET** environment variable.

*   **en_US** : English (Default value)
*   **ko_KR.euckr** : Korean EUC-KR encoding
*   **ko_KR.utf8** : Korean UTF-8 encoding
*   **de_DE.utf8** : German UTF-8 encoding
*   **es_ES.utf8** : Spanish UTF-8 encoding
*   **fr_FR.utf8** : French UTF-8 encoding
*   **it_IT.utf8** : Italian UTF-8 encoding
*   **ja_JP.utf8** : Japanese UTF-8 encoding
*   **km_KH.utf8** : Cambodian UTF-8 encoding
*   **tr_TR.utf8** : Turkish UTF-8 encoding
*   **vi_VN.utf8** : Vietnames UTF-8 encoding
*   **zh_CN.utf8** : Chinese UTF-8 encoding

Language and charset setting of CUBRID affects read and write data. The language is used for messages displayed by the program. The default value of **CUBRID_CHARSET** is **en_US** while installing the product.

For more details related to charset, locale and collation settings, see :doc:`admin/i18n`.

Starting the CUBRID Service
---------------------------

Configure environment variables and language, and then start the CUBRID service. For more information on configuring environment variables and language, see :ref:`control-cubrid-services`.

**Shell Command**

The following shell command can be used to start the CUBRID service and the *demodb* included in the installation package. ::

	% cubrid service start

	@ cubrid master start
	++ cubrid master start: success

	@ cubrid broker start
	++ cubrid broker start: success

	@ cubrid manager server start
	++ cubrid manager server start: success

	% cubrid server start demodb

	@ cubrid server start: demodb

	This may take a long time depending on the amount of recovery works to do.

	CUBRID 9.0

	++ cubrid server start: success

	@ cubrid server status

	Server demodb (rel 9.0, pid 31322)

**CUBRIDService or CUBRID Service Tray**

On the Windows environment, you can start or stop a service as follows:

*   Go to [Control Panel] > [Performance and Maintenance] > [Administrator Tools] > [Services] and select the CUBRIDService to start or stop the service.

	.. image:: /images/image5.png

*   In the system tray, right-click the CUBRID Service Tray. To start CUBRID, select [Service Start]; to stop it, select [Service Stop]. Selecting [Service Start] or [Service Stop] menu would be like executing cubrid service start or cubrid service stop in a command prompt; this command runs or stops the processes configured in service parameters of cubrid.conf.

*   If you click [Exit] while CUBRID is running, all the services and process in the server stop.

.. note::

	An administrator level (SYSTEM) authorization is required to start/stop CUBRID processes through the CUBRID Service tray; a login level user authorization is required to start/stop them with shell commands. If you cannot control the CUBRID processes on the Windows Vista or later version environment, select [Execute as an administrator (A)] in the [Start] > [All Programs] > [Accessories] > [Command Prompt]) or execute it by using the CUBRID Service Tray. When all processes of CUBRID Server stops, an icon on the CUBRID Service tray turns out gray.

**Creating Databases**

You can create databases by using the **cubrid createdb** utility and execute it where database volumes and log volumes are located. If you do not specify additional options such as **--db-volume-size** or **--log-volume-size**, 1.5 GB volume files are created by default (generic volume is set to 512 MB, active log is set to 512 MB, and background archive log is set to 512 MB). ::

	% cd testdb
	% cubrid createdb testdb
	% ls -l

	-rw------- 1 cubrid dbms 536870912 Jan 11 15:04 testdb
	-rw------- 1 cubrid dbms 536870912 Jan 11 15:04 testdb_lgar_t
	-rw------- 1 cubrid dbms 536870912 Jan 11 15:04 testdb_lgat
	-rw------- 1 cubrid dbms       176 Jan 11 15:04 testdb_lginf
	-rw------- 1 cubrid dbms       183 Jan 11 15:04 testdb_vinf

In the above, *testdb* represents a generic volume file, testdb_lgar_t represents a background archive log file, testdb_lgat represents an active log file, testdb_lginf reoresents a log information file, and testdb_vinf represents a volume information file.

For details on volumes, see :ref:`database-volume-structure` . For details on creating volumes, see :ref:`creating-database`. It is recommended to classify and add volumes based on its purpose by using the **cubrid addvoldb** utility. For details, see :ref:`adding-database-volume`.

**Starting Database**

You can start a database process by using the **cubrid server** utility. ::

	% cubrid server start testdb

To have *testdb* started upon startup of the CUBRID service (cubrid service start), configure *testdb* in the **server**  parameter of the **cubrid.conf**  file. ::

	% vi cubrid.conf

	[service]

	service=server,broker,manager
	server=testdb

	...
