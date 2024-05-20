
:meta-keywords: cubrid service, cubrid shell, cubrid create database, cubrid start database
:meta-description: CUBRID quick start guide. How to launch CUBRID service, create and start a database.

Starting the CUBRID Service
===========================

Configure environment variables and language, and then start the CUBRID service. For more information on configuring environment variables and language, see :ref:`control-cubrid-services`.

Shell Command
-------------

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

    CUBRID 11.3

    ++ cubrid server start: success

    @ cubrid server status

    Server demodb (rel 11.3, pid 31322)

CUBRIDService or CUBRID Service Tray
------------------------------------

On the Windows environment, you can start or stop a service as follows:

*   Go to [Control Panel] > [Performance and Maintenance] > [Administrator Tools] > [Services] and select the CUBRIDService to start or stop the service.

    .. image:: /images/image5.png

*   In the system tray, right-click the CUBRID Service Tray. To start CUBRID, select [Service Start]; to stop it, select [Service Stop].

    Selecting [Service Start] or [Service Stop] menu would be like executing cubrid service start or cubrid service stop in a command prompt; this command runs or stops the processes configured in service parameters of cubrid.conf.

*   If you click [Exit] while CUBRID is running, all the services and process in the server stop.

.. note::

    An administrator level (SYSTEM) authorization is required to start/stop CUBRID processes through the CUBRID Service tray; a login level user authorization is required to start/stop them with shell commands. If you cannot control the CUBRID processes on the Windows Vista or later version environment, select [Execute as an administrator (A)] in the [Start] > [All Programs] > [Accessories] > [Command Prompt]) or execute it by using the CUBRID Service Tray. When all processes of CUBRID Server stops, an icon on the CUBRID Service tray turns out gray.

Creating Databases
------------------

You can create databases by using the **cubrid createdb** utility and execute it where database volumes and log volumes are located. If you do not specify additional options such as **\-\-db-volume-size** or **\-\-log-volume-size**, 1.5 GB volume files are created by default (data volume is set to 512 MB, active log is set to 512 MB, and background archive log is set to 512 MB). ::

    % cd testdb
    % cubrid createdb testdb en_US
    % ls -l

    -rw------- 1 cubrid dbms 536870912 Jan 11 15:04 testdb
    -rw------- 1 cubrid dbms 536870912 Jan 11 15:04 testdb_lgar_t
    -rw------- 1 cubrid dbms 536870912 Jan 11 15:04 testdb_lgat
    -rw------- 1 cubrid dbms       176 Jan 11 15:04 testdb_lginf
    -rw------- 1 cubrid dbms       183 Jan 11 15:04 testdb_vinf

In the above, *testdb* represents a data volume file, testdb_lgar_t represents a background archive log file, testdb_lgat represents an active log file, testdb_lginf represents a log information file, and testdb_vinf represents a volume information file.

For details on volumes, see :ref:`database-volume-structure` . For details on creating volumes, see :ref:`creating-database`. It is recommended to classify and add volumes based on purpose by using the **cubrid addvoldb** utility. For details, see :ref:`adding-database-volume`.

Starting Database
-----------------

You can start a database process by using the **cubrid server** utility. ::

    % cubrid server start testdb

To have *testdb* started upon startup of the CUBRID service (cubrid service start), configure *testdb* in the **server**  parameter of the **cubrid.conf**  file. ::

    % vi cubrid.conf

    [service]

    service=server,broker,manager
    server=testdb

    ...
