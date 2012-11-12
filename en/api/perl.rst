*************
Perl 드라이버
*************

**Perl Driver**

**Perl Overview**

**DBD::cubrid**
is a CUBRID Perl driver that implements Perl5 Database Interface (DBI) to enable access to CUBRID database server. It provides full API support.

CUBRID Perl driver is written based on CCI API so affected by CCI configurations such as
**CCI_DEFAULT_AUTOCOMMIT**
.

To download Perl driver or get the latest information, click
`http://www.cubrid.org/wiki_apis/entry/cubrid-perl-driver. <http://www.cubrid.org/wiki_apis/entry/cubrid-perl-driver.>`_

**Remark**

*   The database connection in thread-based programming must be used independently each other.



**Installing and Configuring Perl**

**Requirements**

*   Perl: It is recommended to use an appropriate version of Perl based on your system environment. For example, all Linux and FreeBSD distributions come with Perl. For Windows, ActivePerl is recommended. For details, see
    `http://www.activestate.com/activeperl <http://www.activestate.com/activeperl>`_
    .



*   CUBRID: You don't have to install CUBRID to build CUBRID Perl driver. You can download from
    `http://www.cubrid.org/downloads <http://www.cubrid.org/downloads>`_
    .



*   DBI:
    `http://code.activestate.com/ppm/DBI/ <http://code.activestate.com/ppm/DBI/>`_
    .



*   C compiler: In most cases, there are binary distributions of
    **DBD::cubrid**
    (
    `http://www.cubrid.org/?mid=downloads&item=perl_driver <http://www.cubrid.org/?mid=downloads&item=perl_driver>`_
    ) available. However, if you want to build the driver from source code, a C compiler is required. Make sure to use the same C compiler that was used for compiling Perl and CUBRID. Otherwise, you will encounter problems because of differences in the underlying C runtime libraries.



**Comprehensive Perl Archive Network (CPAN) Installation**

You can automatically install the driver from source code by using the CPAN module.

cpan

install DBD::cubrid

If you are using the CPAN module for the first time, it is recommended to accept default settings.

If you are using an older version, you might enter the command line below, instead of command line above.

perl -MCPAN -e shell

install DBD::cubrid

**Manual Installation**

If you cannot get the CPAN module, you should download the
**DBD::cubrid**
source code. The latest version is always available below:

`http://www.cubrid.org/?mid=downloads&item=perl_driver <http://www.cubrid.org/?mid=downloads&item=perl_driver>`_

The file name is typically something like this:
**DBD-cubrid-X.X.X.tar.gz**
. After extracting the archive, enter the command line below under the
**DBD-cubrid-X.X.X**
directory. (On Windows, you may need to replace
**make**
with
**nmake**
or
**dmake**
.)

Perl Makefile.PL

make

make test

If test seems to look fine, execute the command to build a driver.

make install

**Note**
To get the latest information about Perl driver, click
`http://www.cubrid.org/wiki_apis/entry/cubrid-perl-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-perl-driver>`_
.

**Perl API**

Currently, CUBRID Perl driver provides only basic features and does not support LOB type and column information verification.

If you want to get details about CUBRID Perl driver API, see
`http://search.cpan.org/~cubrid/ <http://search.cpan.org/~cubrid/DBD-cubrid-8.4.0.0002/cubrid.pm>`_
.

**Note**
To get the latest information about Perl driver, click
`http://www.cubrid.org/wiki_apis/entry/cubrid-perl-driver <http://www.cubrid.org/wiki_apis/entry/cubrid-perl-driver>`_
.
