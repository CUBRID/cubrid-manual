
:meta-keywords: cubrid perl driver, cubrid perl api
:meta-description: DBD::cubrid is a CUBRID Perl driver that implements Perl5 Database Interface (DBI) to enable access to CUBRID database server. It provides full API support. CUBRID Perl driver is written based on CCI API.

*************
Perl 드라이버
*************

**DBD::cubrid** 는 Perl5 DBI(Database Interface)에서 CUBRID 데이터베이스에 접근하기 위해 사용하는 CUBRID Perl 드라이버로, Perl5 DBI의 모든 API를 지원한다.

CUBRID Perl 드라이버는 CCI API를 기반으로 작성되었으므로, CCI API 및 CCI에 적용되는 **CCI_DEFAULT_AUTOCOMMIT** 과 같은 설정 파라미터에 영향을 받는다.

.. FIXME: 별도로 Perl 드라이버를 다운로드하거나 Perl 드라이버에 대한 최신 정보를 확인하려면 http://www.cubrid.org/wiki_apis/entry/cubrid-perl-driver\에 접속한다.

.. note::

    *   스레드 기반 프로그램에서 데이터베이스 연결은 각 스레드마다 독립적으로 사용해야 한다.
    *   자동 커밋 모드에서 SELECT 문 수행 이후 모든 결과 셋이 fetch되지 않으면 커밋이 되지 않는다. 따라서, 자동 커밋 모드라 하더라도 프로그램 내에서 결과 셋에 대한 fetch 도중 어떠한 오류가 발생한다면 반드시 커밋 또는 롤백을 수행하여 트랜잭션을 종료 처리하도록 한다. 

Perl 설치 및 설정
=================

**기본 환경**

*   Perl: 시스템에 적합한 버전의 Perl을 사용하는 것을 권장한다. 모든 Linux와 FreeBSD에는 Perl이 포함되어 있으며, Windows에서는 ActivePerl을 권장한다. Active Perl에 대한 자세한 내용은 https://www.activestate.com/products/perl\ 을 참고한다.

*   CUBRID: Perl 드라이버를 빌드하기 위해 CCI 드라이버가 필요하며, 이를 위해 CUBRID Engin을 설치해야 한다. https://www.cubrid.org/downloads\ 에서 필요한 버전의 CUBRID Engine을 다운로드 하여 설치하면 된다. CUBRID Engine의 설치후 CCI 드라이버는 CUBRID 기본 경로 $CUBRID/cci에서 확인할 수 있다.

*   DBI: https://metacpan.org/dist/DBI/

*   C 컴파일러: 대부분의 경우에는 **DBD::cubrid** 바이너리(https://www.cubrid.org/downloads#perl)를 사용할 수 있으나, 만약 소스코드에서 드라이버를 빌드하려면 C 컴파일러가 필요하다. C 컴파일러를 사용하려면 Perl과 CUBRID를 컴파일한 컴파일러와 같은 컴파일러를 사용해야 한다. 그렇지 않으면 C 런타임 라이브러리 차이 때문에 문제가 발생할 수 있다.

*   Linux에서는 CCI Driver를 빌드하기 위해 GNU Developer Toolset 8 또는 그 이상이 필요하다.

**CPAN을 이용한 설치**

다음과 같이 **CPAN** (Comprehensive Perl Archive Network)을 사용하면 자동으로 소스코드에서 드라이버를 설치할 수 있다. ::

    cpan
    install DBD::cubrid

만약 **CPAN** 모듈을 처음으로 사용한다면 기본 설정에 따르는 것을 권장한다.

최신 버전의 Perl을 사용하지 않는다면 위 명령어 대신 다음 명령어를 사용해야 할 수도 있다. ::

    perl -MCPAN -e shell
    install DBD::cubrid

**수동 설치**

**CPAN** 을 이용해서 설치할 수 없다면 **DBD::cubrid** 소스코드를 다운로드해야 한다. 최신 버전은 아래 주소에서 다운로드할 수 있다.

https://www.cubrid.org/downloads#perl

파일 이름은 일반적으로 **DBD-cubrid-X.X.X.tar.gz** 와 같은 형식이다. 압축을 해제한 후 **DBD-cubrid-X.X.X** 디렉터리로 이동하여 다음 명령어를 실행한다. ::

    Perl Makefile.PL
    make
    make test

Windows에서는 **make** 대신 **nmake** 또는 **dmake** 를 사용해야 할 수도 있다. 테스트 결과가 성공적이면 다음 명령어를 실행하여 드라이버를 빌드한다. ::

    make install

Perl API
========

CUBRID Perl 드라이버는 현재에는 기본 기능만을 제공하고 있다. LOB 타입이나 칼럼 정보 확인 등의 기능은 현재 지원하지 않는다.

CUBRID Perl API는 http://ftp.cubrid.org/CUBRID_Docs/Drivers/Perl/\ 을 참고한다.
