Enhancements
------------

* `[965] <https://github.com/CUBRID/cubrid/pull/965>`_ `[CBRD-21558] <http://jira.cubrid.org/browse/CBRD-21558>`_ inline functions for string compression
* `[964] <https://github.com/CUBRID/cubrid/pull/964>`_ `[CBRD-21558] <http://jira.cubrid.org/browse/CBRD-21558>`_ Set HFID in heap_get_context when it is already available in the scan_cache

Bug Fixes
---------

* `[978] <https://github.com/CUBRID/cubrid/pull/978>`_ `[CBRD-21540] <http://jira.cubrid.org/browse/CBRD-21540>`_ order by list might be already expanded
* `[975] <https://github.com/CUBRID/cubrid/pull/975>`_ `[CBRD-21839] <http://jira.cubrid.org/browse/CBRD-21839>`_ fixes bound read of parsing timezone offset
* `[973] <https://github.com/CUBRID/cubrid/pull/973>`_ `[CBRD-21497] <http://jira.cubrid.org/browse/CBRD-21497>`_ assigns an idle query id
* `[963] <https://github.com/CUBRID/cubrid/pull/963>`_ `[CBRD-21818] <http://jira.cubrid.org/browse/CBRD-21818>`_ Removed an assert
* `[962] <https://github.com/CUBRID/cubrid/pull/962>`_ `[CBRD-21807] <http://jira.cubrid.org/browse/CBRD-21807>`_ Fix retrieving records with desc property and NULL fields
* `[930] <https://github.com/CUBRID/cubrid/pull/930>`_ `[CBRD-21645] <http://jira.cubrid.org/browse/CBRD-21645>`_ restore uses the current log files if exist
* `[918] <https://github.com/CUBRID/cubrid/pull/918>`_ `[CBRD-21727] <http://jira.cubrid.org/browse/CBRD-21727>`_ Fix crash of WIN32 CUBRID Manager Server
* `[915] <https://github.com/CUBRID/cubrid/pull/915>`_ `[CBRD-21721] <http://jira.cubrid.org/browse/CBRD-21721>`_ Fix buffer overflow for timezone generation
* `[914] <https://github.com/CUBRID/cubrid/pull/914>`_ `[CBRD-21253] <http://jira.cubrid.org/browse/CBRD-21253>`_ Sync after flushing log
* `[908] <https://github.com/CUBRID/cubrid/pull/908>`_ `[CBRD-21643] <http://jira.cubrid.org/browse/CBRD-21643>`_ Fix to build with Visual Studio 2017
* `[896] <https://github.com/CUBRID/cubrid/pull/896>`_ `[CBRD-21704] <http://jira.cubrid.org/browse/CBRD-21704>`_ fix log_header initializer
* `[894] <https://github.com/CUBRID/cubrid/pull/894>`_ `[CBRD-21704] <http://jira.cubrid.org/browse/CBRD-21704>`_ Fix log_Gl.hdr.vacuum_last_blockid
* `[893] <https://github.com/CUBRID/cubrid/pull/893>`_ `[CBRD-21707] <http://jira.cubrid.org/browse/CBRD-21707>`_ push and pop error of lock_event_log_lock_info to keep existing error if any
* `[889] <https://github.com/CUBRID/cubrid/pull/889>`_ `[CBRD-21645] <http://jira.cubrid.org/browse/CBRD-21645>`_ keep archive when empty data and mvcc_op_log_sa not null
* `[886] <https://github.com/CUBRID/cubrid/pull/886>`_ `[CBRD-21679] <http://jira.cubrid.org/browse/CBRD-21679>`_ fix vacuum last_blockid after restoredb
* `[876] <https://github.com/CUBRID/cubrid/pull/876>`_ `[CBRD-21662] <http://jira.cubrid.org/browse/CBRD-21662>`_ fixes legacy typos
* `[875] <https://github.com/CUBRID/cubrid/pull/875>`_ `[CBRD-21646] <http://jira.cubrid.org/browse/CBRD-21646>`_ fixes a race condition between dismounting log archive and accessing it
* `[874] <https://github.com/CUBRID/cubrid/pull/874>`_ `[CBRD-21620] <http://jira.cubrid.org/browse/CBRD-21620>`_ Fix data volume expansion recovery
* `[873] <https://github.com/CUBRID/cubrid/pull/873>`_ `[CBRD-21658] <http://jira.cubrid.org/browse/CBRD-21658>`_ Merge log archive and vacuum fixes (includes `[834] <https://github.com/CUBRID/cubrid/pull/834>`_ `[833] <https://github.com/CUBRID/cubrid/pull/833>`_ `[836] <https://github.com/CUBRID/cubrid/pull/836>`_ `[847] <https://github.com/CUBRID/cubrid/pull/847>`_ `[837] <https://github.com/CUBRID/cubrid/pull/837>`_ `[860] <https://github.com/CUBRID/cubrid/pull/860>`_ `[863] <https://github.com/CUBRID/cubrid/pull/863>`_ `[869] <https://github.com/CUBRID/cubrid/pull/869>`_\)
* `[856] <https://github.com/CUBRID/cubrid/pull/856>`_ `[CBRD-21579] <http://jira.cubrid.org/browse/CBRD-21579>`_ alternative fix: generate one eval structure for an analytic function when optimization fails
* `[813] <https://github.com/CUBRID/cubrid/pull/813>`_ `[CBRD-21517] <http://jira.cubrid.org/browse/CBRD-21517>`_ fixes potential FMR overflow columns for grouping by iscan
* `[813] <https://github.com/CUBRID/cubrid/pull/813>`_ `[CBRD-21517] <http://jira.cubrid.org/browse/CBRD-21517>`_ fixes potential FMR overflow columns for grouping by iscan
* `[799] <https://github.com/CUBRID/cubrid/pull/799>`_ `[CBRD-21503] <http://jira.cubrid.org/browse/CBRD-21503>`_ allow archive purging in SA_MODE
* `[794] <https://github.com/CUBRID/cubrid/pull/794>`_ `[CBRD-21498] <http://jira.cubrid.org/browse/CBRD-21498>`_ fix missing error code from dbi_compat.h

