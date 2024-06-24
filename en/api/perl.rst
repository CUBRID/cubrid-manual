
:meta-keywords: cubrid perl driver, cubrid perl api
:meta-description: DBD::cubrid is a CUBRID Perl driver that implements Perl5 Database Interface (DBI) to enable access to CUBRID database server. It provides full API support. CUBRID Perl driver is written based on CCI API.

***********
Perl Driver
***********

**DBD::cubrid** is a CUBRID Perl driver that implements Perl5 Database Interface (DBI) to enable access to CUBRID database server. It provides full API support.

CUBRID Perl driver is written based on CCI API so affected by CCI configurations such as **CCI_DEFAULT_AUTOCOMMIT**.

.. FIXME: To download Perl driver or get the latest information, click http://www.cubrid.org/wiki_apis/entry/cubrid-perl-driver . 

.. note::

    *   The database connection in thread-based programming must be used independently each other.
    *   In autocommit mode, the transaction is not committed if all results are not fetched after running the SELECT statement. Therefore, although in autocommit mode, you should end the transaction by executing COMMIT or ROLLBACK if some error occurs during fetching for the resultset. 

Installing and Configuring Perl
===============================

**Requirements**

*   Perl: It is recommended to use an appropriate version of Perl based on your system environment. For example, all Linux and FreeBSD distributions come with Perl. For Windows, ActivePerl is recommended. For details, see https://www.activestate.com/products/perl/ .

*   CUBRID: To build the CUBRID Perl driver, the CCI driver is needed. You can obtain it by installing the CUBRID Engine, which can be downloaded from https://www.cubrid.org/downloads. After downloading and installing a proper version of the CUBRID Engine, you can find the required CCI driver in the installed CUBRID Engine directory, $CUBRID/cci.

*   DBI: https://metacpan.org/dist/DBI/

*   C compiler: In most cases, there are binary distributions of **DBD::cubrid** ( https://www.cubrid.org/downloads#perl ) available. However, if you want to build the driver from source code, a C compiler is required. Make sure to use the same C compiler that was used for compiling Perl and CUBRID. Otherwise, you will encounter problems because of differences in the underlying C runtime libraries.

*   For building CCI Driver In Linux, GNU Developer Toolset 8 or higher is required.

**Comprehensive Perl Archive Network (CPAN) Installation**

You can automatically install the driver from source code by using the CPAN module. ::

    cpan
    install DBD::cubrid

If you are using the CPAN module for the first time, it is recommended to accept default settings.

If you are using an older version, you might enter the command line below, instead of command line above. ::

    perl -MCPAN -e shell
    install DBD::cubrid

**Manual Installation**

If you cannot get the CPAN module, you should download the **DBD::cubrid** source code. The latest version is always available below:

https://www.cubrid.org/downloads#perl

The file name is typically something like this: **DBD-cubrid-X.X.X.tar.gz**. After extracting the archive, enter the command line below under the **DBD-cubrid-X.X.X** directory. (On Windows, you may need to replace **make** with **nmake** or **dmake**.) ::

    Perl Makefile.PL
    make
    make test

If test seems to look fine, execute the command to build a driver. ::

    make install

Perl API
========

Currently, CUBRID Perl driver provides only basic features and does not support LOB type and column information verification.

If you want to get details about CUBRID Perl driver API, see http://ftp.cubrid.org/CUBRID_Docs/Drivers/Perl/.
