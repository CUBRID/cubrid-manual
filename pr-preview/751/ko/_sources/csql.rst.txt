
:meta-keywords: csql, cubrid csql, cubrid commands, executing csql, csql options
:meta-description: CUBRID CSQL is an application that allows users to use SQL statements through a command-driven interface. This section briefly explains how to use the CSQL Interpreter and associated commands.

***************
CSQL 인터프리터
***************

CUBRID에서 SQL 질의문을 수행하려면 그래픽 사용자 인터페이스(GUI) 기반 CUBRID Manager나 콘솔 기반 CSQL 인터프리터를 사용해야 한다.

CSQL은 CUBRID에서 명령어 방식으로 SQL 문을 사용할 수 있는 프로그램이다. 여기에서는 CSQL 인터프리터의 간단한 사용법과 관련 명령어를 설명한다.

.. _csql-intro:

CSQL 인터프리터 소개
====================

**SQL 사용을 위한 도구**

CSQL 인터프리터는 CUBRID와 함께 설치되며, 대화형(interactive) 방식과 일괄 수행(batch) 방식으로 SQL 질의를 수행하고 수행 결과를 조회할 수 있는 프로그램이다. CSQL 인터프리터는 명령어 라인 입력 방식의 인터페이스를 제공하며, 입력된 SQL 문장과 그 결과는 나중에 사용하기 위해서 파일에 저장할 수도 있다.

CSQL 인터프리터는 CUBRID를 사용하는 가장 기본적이고 손쉬운 방법이다. CUBRID를 사용하는데 제공되는 다양한 API(JDBC, ODBC, PHP, CCI 등)를 활용하여 데이터베이스 응용 프로그램을 작성할 수 있다. 또한, CUBRID에서 제공하는 관리 및 질의 도구인 CUBRID 매니저를 사용할 수도 있다. 사용자는 CSQL 인터프리터가 제공하는 터미널 기반의 환경에서 SQL 질의를 생성하고, 수행 결과를 조회할 수 있다.

CSQL 인터프리터는 CUBRID 데이터베이스에 접속하여 SQL 문을 통해 다양한 작업을 수행한다. CSQL 인터프리터를 이용해 다음과 같은 작업을 수행할 수 있다.

* SQL 문을 이용하여 데이터베이스 조회, 갱신, 삭제 등의 작업
* 외부 셸 명령 실행
* 조회 결과의 저장 혹은 출력
* SQL 스크립트 파일의 작성 및 실행
* 테이블 스키마 조회
* 데이터베이스 서버 시스템 파라미터의 조회 및 변경
* 다양한 데이터베이스 정보(스키마, 트리거, 지연 트리거, workspace, 잠금, 통계) 조회

**DBA를 위한 도구**

**DBA** (Database Administrator)는 일상적인 많은 관리 업무를 수행하기 위해서 CUBRID가 설치된 시스템에 접속해서 CUBRID가 제공하는 다양한 관리 유틸리티를 이용해서 작업을 수행한다. 따라서, 터미널 기반의 인터페이스를 제공하는 CSQL 인터프리터는 **DBA** 가 데이터베이스 관리 업무를 수행하는데 유용하게 사용된다. 또한, CSQL 인터프리터는 **DBA** 에게 필요한 다양한 데이터베이스 정보를 제공한다.

CSQL 인터프리터는 독립 모드(Standalone Mode)로 실행될 수도 있다. 독립 실행 모드는 CSQL 인터프리터가 서버 프로세스의 기능을 포함하여 직접 데이터베이스 파일에 접근하여 수행하는 방식이다. 즉 별도의 데이터베이스 서버 프로세스가 구동되어 있지 않은 상태에서 해당 데이터베이스를 대상으로 SQL 문을 실행할 수 있다. CSQL 인터프리터는 데이터베이스 서버나 브로커 등 어떠한 다른 프로그램의 도움 없이 **csql** 유틸리티 하나로 데이터베이스를 이용할 수 있는 강력한 수단이다.

CSQL 실행
=========

.. _csql-exec-mode:

CSQL 실행 모드
--------------

**대화형 모드(Interactive Mode)**

CSQL 인터프리터는 데이터베이스에서 스키마 또는 데이터를 다루기 위한 SQL 문을 입력하고 수행할 수 있다. **csql** 유틸리티를 실행하면 나타나는 프롬프트에 사용자는 구문을 입력한다. 구문을 입력한 후 실행하면 다음 라인에 결과가 표시되는데, 이를 대화형 모드라고 한다.

**일괄 수행 모드(Batch Mode)**

사용자는 원하는 SQL 문을 임의의 파일에 저장한 후 **csql** 유틸리티가 해당 파일을 읽도록 구문을 실행할 수 있다. 이를 일괄 수행(배치형) 모드라고 한다.

**독립 모드(Standalone Mode)**

독립 실행 모드는 CSQL 인터프리터가 서버 프로세스의 기능을 포함하여 직접 데이터베이스 파일에 접근하여 수행하는 방식이다. 즉 별도의 데이터베이스 서버 프로세스가 구동되어 있지 않은 상태에서 해당 데이터베이스를 대상으로 SQL 문을 실행할 수 있다. 독립 모드는 동시에 한 사용자만이 접근이 가능하므로 **DBA** (Database Administrator)가 관리 작업을 위해 수행하는데 적합한 모드이다.

**클라이언트/서버 모드(Client/Server Mode)**

클라이언트/서버 모드는 일반적으로 해당 CSQL 인터프리터가 클라이언트 프로세스로 동작하여 데이터베이스 서버 프로세스에 접속하는 방식으로 사용되는 모드이다.

**시스템 관리자 모드**

시스템 관리자 모드는 CSQL 인터프리터를 통해 특별한 관리 작업을 수행하기 위해 사용되는 모드이다. 서버 접속 개수가 시스템 파라미터 **max_clients**\ 의 값을 초과하더라도 CSQL 인터프리터에서 시스템 관리자 모드로 접속하면 추가로 단 하나의 연결을 허용한다. 체크 포인트를 수행하거나 트랜잭션 모니터링을 종료 등의 작업을 수행할 수 있다.

::

    csql -u dba --sysadm demodb 

CSQL 사용 방법
--------------

**로컬 호스트 접속**

**csql** 유틸리티를 사용하여 CSQL 인터프리터를 실행한다. 이 때, 필요에 따라 옵션을 설정할 수 있으며, 옵션을 설정하려면 접속하려는 데이터베이스 이름을 인수로 지정한다. 다음은 로컬 서버에 위치한 데이터베이스에 접속하는 **csql** 유틸리티 구문이다. ::

    csql [options] database_name

**원격 호스트 접속**

다음은 원격 호스트에 위치한 데이터베이스에 접속하는 **csql** 유틸리티 구문이다. ::

    csql [options] database_name@remote_host_name

단, 원격 호스트에서 CSQL 인터프리터를 실행하려면 다음 조건을 만족해야 한다.

* 원격 호스트와 로컬 호스트에 설치된 CUBRID는 동일한 버전이어야 한다.
* 원격 호스트와 로컬 호스트의 마스터 프로세스가 사용하는 포트 번호가 동일해야 한다.
* **-C** 옵션을 사용하여 클라이언트/서버 모드로 원격 호스트에 접속해야 한다.

**예제**

다음은 192.168.1.3 위치의 원격 호스트에 존재하는 **demodb** 에 접속하여 **csql** 유틸리티를 호출하는 예제이다. ::

    csql -C demodb@192.168.1.3

CSQL 시작 옵션
--------------

프롬프트 상에서 옵션 목록을 보려면, 다음과 같이 옵션을 적용할 데이터베이스를 지정하지 않고 **csql** 유틸리티를 실행한다. ::

    $ csql
    A database-name is missing.
    interactive SQL utility, version 10.1
    usage: csql [OPTION] database-name[@host]

    valid options:
      -S, --SA-mode                standalone mode execution
      -C, --CS-mode                client-server mode execution
      -u, --user=ARG               alternate user name
      -p, --password=ARG           password string, give "" for none
      -e, --error-continue         don't exit on statement error
      -i, --input-file=ARG         input-file-name
      -o, --output-file=ARG        output-file-name
      -s, --single-line            single line oriented execution
      -c, --command=ARG            CSQL-commands
      -l, --line-output            display each value in a line
      -r, --read-only              read-only mode
      -t, --plain-output           display results in a script-friendly format (only works with -c and -i)
      -N, --skip-column-names      do not display column names in results (only works with -c and -i)
          --string-width           display each column which is a string type in this width
          --no-auto-commit         disable auto commit mode execution
          --no-pager               do not use pager
          --no-single-line         turn off single line oriented execution
          --no-trigger-action      disable trigger action

    For additional information, see http://www.cubrid.org

**옵션**

.. program:: csql

.. option:: -S, --SA-mode

    **-S** 옵션을 이용하여 독립 모드로 데이터베이스에 접속하여 **csql**\ 을 실행한다. 데이터베이스를 독점적으로 사용하고자 할 때 **-S** 옵션을 이용한다. **csql**\ 이 독립 모드로 실행중이면 또 다른 **csql** 또는 유틸리티의 사용이 불가능하다. **-S** 옵션과 **-C** 옵션을 둘 다 생략하면 **-C** 옵션으로 동작한다. ::

        csql -S demodb

.. option:: -C, --CS-mode

    **-C** 옵션을 이용하여 클라이언트/서버 모드로 데이터베이스에 접속하여 **csql** 유틸리티를 실행한다. 데이터베이스에 여러 클라이언트가 동시 접속하는 환경에서 **-C** 옵션을 이용한다. 만약 클라이언트/서버 모드로 원격 호스트의 데이터베이스에 접속한 경우라도 **csql** 유틸리티를 실행하는 도중에 발생한 에러 로그는 로컬 호스트의 **csql.err** 파일에 기록된다. ::

        csql -C demodb

.. option:: -i, --input-file=ARG

    **-i** 옵션을 이용하여 배치 모드에서 사용할 입력 파일의 이름을 지정한다. **infile** 파일에는 하나 이상의 SQL 문이 저장되어 있으며, **-i** 옵션이 지정되지 않으면 CSQL 인터프리터는 대화형 모드로 실행된다. ::

        csql -i infile demodb

.. option:: -o, --output-file=ARG

    **-o** 옵션을 이용하여 질의 수행 결과를 화면에 출력하지 않고 지정된 파일에 저장한다. 이는 CSQL 인터프리터에 의한 질의 수행 결과를 추후 조회하고자 할 때 유용하게 사용될 수 있다. ::

        csql -o outfile demodb

.. option:: -u, --user=ARG

    **-u** 옵션을 이용하여 지정된 데이터베이스에 접속하려는 사용자 이름을 지정한다. 만약 **-u** 옵션이 지정되지 않으면 가장 낮은 사용자 권한을 가지는 **PUBLIC** 이 사용자로 지정된다. 또한 사용자 이름이 유효하지 않은 경우에는 오류가 출력되고 **csql** 유틸리티는 종료된다. 암호가 설정된 사용자 이름이 지정된 경우에는 암호를 입력받기 위한 프롬프트가 출력된다. ::

        csql -u DBA demodb

.. option:: -p, --password=ARG

    **-p** 옵션을 이용하여 지정된 사용자의 암호를 입력한다. 특히, 배치 모드에서는 지정한 사용자에 대한 암호 입력을 요청하는 프롬프트가 출력되지 않으므로 **-p** 옵션을 이용하여 암호를 입력해야 한다. 잘못된 암호를 입력하면, 오류가 출력되고 **csql** 유틸리티는 종료된다. ::

        csql -u DBA -p *** demodb

.. option:: -s, --single-line

    **-i** 옵션과 함께 사용하는 옵션으로, **-s** 옵션을 지정하면 파일에 입력된 여러 개의 SQL 문을 하나씩 나누어 수행한다. 이 옵션은 질의 수행에 메모리를 적게 할당하고 싶을 때 유용하게 이용할 수 있다. 각 SQL 문은 세미콜론(;)으로 구분한다. 옵션을 생략하면 여러 개의 SQL 문을 한꺼번에 읽어들인 후 수행한다. ::

        csql -s -i infile demodb

.. option:: -c, --command=ARG

    **-c** 옵션을 이용하여 셸 상에서 하나 이상의 SQL 문을 직접 수행한다. 이 때, 각 문장은 세미콜론(;)으로 구분한다. ::

        csql -c 'select * from olympic;select * from stadium' demodb

.. option:: -l, --line-output

    **-l** 옵션을 이용하여 SQL 문을 실행한 결과 레코드의 SELECT 리스트 값들을 라인 단위로 나누어서 출력한다. **-l** 옵션을 지정하지 않으면 결과 레코드의 모든 SELECT 리스트 값들을 한 라인에 출력한다. ::

        csql -l demodb

.. option:: -e, --error-continue 

    SQL 문 여러 개를 연속으로 나열하여 실행할 때 **-e** 옵션을 이용하면 SQL 문 중간에 의미상(semantic) 오류 또는 런타임 에러가 발생하여도 이를 무시하고 계속 SQL 문을 실행한다. 이때 SQL 문에 문법상(syntax) 오류가 있다면 **-e** 옵션이 지정되어 있어도 오류가 발생한 후의 질의를 실행하지 않는다. ::

        $ csql -e demodb

        csql> SELECT * FROM aaa;SELECT * FROM athlete WHERE code=10000;

        In line 1, column 1,

        ERROR: before ' ;SELECT * FROM athlete WHERE code=10000; '
        Unknown class "aaa".


        === <Result of SELECT Command in Line 1> ===

                 code  name                  gender                nation_code           event               
        =====================================================================================================
                10000  'Aardewijn Pepijn'    'M'                   'NED'                 'Rowing'            


        1 rows selected. (0.006433 sec) Committed.

.. option:: -r, --read-only

    **-r** 옵션을 이용하여 읽기 전용으로 데이터베이스에 접속한다. 데이터베이스에 읽기 전용으로 접속하면 테이블을 만들거나 데이터를 입력할 수 없고 데이터를 조회만 할 수 있다. ::

        csql -r demodb

.. option:: -t, --plain-output
 
    컬럼명과 값만 표시되며 **-c** 또는 **-i** 옵션과 함께 작동된다. 각 컬럼과 값이 탭과 줄 바꿈으로 구분되며, 결과에 포함된 탭과 백슬래시는 '\n', '\t' 및 '\\'으로 각각 대체된다. 이 옵션은 **-l** 옵션과 함께 지정된 경우에는 무시된다.

    ::
    
        $ csql testdb@localhost -c "select * from test_tbl" -t
 
        col1 col2 col3
        string1 12:16:10.090 PM 10/23/2014
        string2 12:16:10.090 PM 10/23/2014
        string3 12:16:10.090 PM 10/23/2014
        string4 12:16:10.090 PM 10/23/2014
        string5 12:16:10.090 PM 10/23/2014
        string6 12:16:10.090 PM 10/23/2014
        string7 12:16:10.090 PM 10/23/2014
        string8 12:16:10.090 PM 10/23/2014
        string9 12:16:10.090 PM 10/23/2014
        string10 12:16:10.090 PM 10/23/2014
 
.. option:: -N, --skip-column-names
 
    결과에서 컬럼명을 숨긴다. **-c** 또는 **-i** 옵션을 사용하는 경우에만 작동하며 일반적으로 **-t** 옵션과 함께 사용된다. 이 옵션은 **-l** 옵션과 함께 지정된 경우에는 무시된다.
 
    ::
 
        $ csql testdb@localhost -c "select * from test_tbl" -t -N
 
        string1 12:16:10.090 PM 10/23/2014
        string2 12:16:10.090 PM 10/23/2014
        string3 12:16:10.090 PM 10/23/2014
        string4 12:16:10.090 PM 10/23/2014
        string5 12:16:10.090 PM 10/23/2014
        string6 12:16:10.090 PM 10/23/2014
        string7 12:16:10.090 PM 10/23/2014
        string8 12:16:10.090 PM 10/23/2014
        string9 12:16:10.090 PM 10/23/2014
        string10 12:16:10.090 PM 10/23/2014

.. option:: --no-auto-commit

    **--no-auto-commit** 옵션을 이용하여 자동 커밋 모드를 중지한다. **--no-auto-commit** 옵션을 지정하지 않으면 기본적으로 CSQL 인터프리터는 자동 커밋 모드로 작동되고, 입력된 SQL 문이 실행될 때마다 자동으로 커밋된다. 또한, CSQL 인터프리터를 시작한 후 **;AUtocommit** 세션 명령을 수행해도 동일한 결과를 얻을 수 있다. ::

        csql --no-auto-commit demodb

.. option:: --no-pager

    **--no-pager** 옵션을 이용하여 CSQL 인터프리터에서 수행한 질의 결과를 페이지 단위로 출력하지 않고, 일괄적으로 출력한다. **--no-pager** 옵션을 지정하지 않으면 페이지 단위로 질의 수행 결과를 출력한다. ::

        csql --no-pager demodb

.. option:: --no-single-line

    **--no-single-line** 옵션을 이용하면 SQL 문 여러 개를 저장해 두었다가 **;xr** 혹은 **;r** 세션 명령어로 한꺼번에 수행한다. 이 옵션을 지정하지 않으면 **;xr** 혹은 **;r** 세션 명령어 없이 SQL 문이 바로 실행된다. ::

        csql --no-single-line demodb

.. option::  --sysadm

    이 옵션은 **-u dba**\와 같이 사용해야 하며, 시스템 관리자 모드로 실행하고자 할 때 지정한다.

    ::

        csql -u dba --sysadm demodb

.. option::  --write-on-standby

    이 옵션은 시스템 관리자 모드 옵션(**--sysadm--**)과 함께 사용해야 한다. 이 옵션으로 CSQL을 실행한 dba는 standby 상태의 DB 즉, 슬레이브 DB 또는 레플리카 DB에 직접 접속하여 쓰기 작업을 수행할 수 있다. 단, 레플리카에 직접 쓰는 데이터는 복제되지 않는다.
    
    :: 

         csql --sysadm --write-on-standby -u dba testdb@localhost 

    .. note::
    
        레플리카에 직접 데이터를 쓰는 경우 복제 불일치가 발생함에 주의해야 한다.
        
.. option::  --no-trigger-action

    이 옵션을 지정하면 해당 CSQL에서 수행되는 질의문의 트리거는 동작하지 않는다.
      
.. _csql-session-commands:

세션 명령어
===========

CSQL 인터프리터에는 SQL 문 이외에 CSQL 인터프리터를 제어하는 특별한 명령어가 있으며 이를 세션 명령어라고 한다. 모든 세션 명령어는 반드시 세미콜론(;)으로 시작해야 한다.

**;help** 를 입력하여 CSQL 인터프리터에서 지원되는 세션 명령어를 확인할 수 있다. 세션 명령어를 전부 입력하지 않고 대문자로 표시된 글자까지만 입력해도 CSQL 인터프리터는 세션 명령어를 인식한다. 세션 명령어는 대소문자를 구분하지 않는다. 

"질의 버퍼"는 질의문을 실행하기 전까지 질의문을 저장하는 버퍼이다. **--no-single-line** 옵션을 부여하여 CSQL을 실행하는 경우 **;xr** 명령으로 질의를 실행하기 전까지는 질의문을 버퍼에 유지한다.

**파일에서 질의 읽기(;REAd)**

**;REAd** 명령어는 파일의 내용을 질의 버퍼로 읽는 세션 명령어로, 지정된 입력 파일에 저장된 질의문들을 실행하는데 사용할 수 있다. 질의 버퍼에 올려진 파일 내용을 보기 위해서는 **;List** 명령어를 사용한다. ::

    csql> ;read nation.sql
    The file has been read into the command buffer.
    csql> ;list
    insert into "sport_event" ("event_code", "event_name", "gender_type", "num_player") values
    (20001, 'Archery Individual', 'M', 1);
    insert into "sport_event" ("event_code", "event_name", "gender_type", "num_player") values
    20002, 'Archery Individual', 'W', 1);
    ....

**파일에 질의 저장(;Write)**

**;Write** 는 질의 버퍼의 내용을 파일에 저장하는 세션 명령어로 사용자가 CSQL 인터프리터에서 입력 혹은 수정한 질의문을 파일에 저장할 때 사용된다. ::

    csql> ;write outfile
    Command buffer has been saved.

**파일에 덧붙이기(;APpend)**

현재 질의 버퍼의 내용을 출력 파일인 **outfile**\ 에 추가한다. ::

    csql> ;append outfile
    Command buffer has been saved.

**셸 명령어를 실행(;SHELL)**

**;SHELL** 세션 명령어로 외부 셸을 호출할 수 있다. CSQL 인터프리터가 실행된 환경에서 새로운 셸이 시작되고, 셸을 마치면 다시 CSQL 인터프리터로 돌아온다. 만약에 **;SHELL_Cmd** 명령어로 수행할 셸 명령어가 지정되어 있다면 셸을 구동하여 지정된 명령어를 실행하고 CSQL 인터프리터로 복귀하게 된다. ::

    csql> ;shell
    % ls -al
    total 2088
    drwxr-xr-x 16 DBA cubrid   4096 Jul 29 16:51 .
    drwxr-xr-x  6 DBA cubrid   4096 Jul 29 16:17 ..
    drwxr-xr-x  2 DBA cubrid   4096 Jul 29 02:49 audit
    drwxr-xr-x  2 DBA cubrid   4096 Jul 29 16:17 bin
    drwxr-xr-x  2 DBA cubrid   4096 Jul 29 16:17 conf
    drwxr-xr-x  4 DBA cubrid   4096 Jul 29 16:14 cubridmanager
    % exit
    csql>

**셸 명령어 등록(;SHELL_Cmd)**

;SHELL_Cmd를 사용하여 ;SHELL 세션 명령어로 실행할 셸 명령어를 등록한다. 등록된 명령어를 실행하기 위해서는 예제와 같이 ;shell 명령어를 입력한다. ::

    csql> ;shell_c ls -la
    csql> ;shell
    total 2088
    drwxr-xr-x 16 DBA cubrid   4096 Jul 29 16:51 .
    drwxr-xr-x  6 DBA cubrid   4096 Jul 29 16:17 ..
    drwxr-xr-x  2 DBA cubrid   4096 Jul 29 02:49 audit
    drwxr-xr-x  2 DBA cubrid   4096 Jul 29 16:17 bin
    drwxr-xr-x  2 DBA cubrid   4096 Jul 29 16:17 conf
    drwxr-xr-x  4 DBA cubrid   4096 Jul 29 16:14 cubridmanager
    csql>

**페이저 명령어 등록(;PAger_cmd)**

;PAger_cmd를 사용하여 질의 실행 결과를 출력하는 페이저 명령어를 등록한다. 등록되는 명령어에 따라 출력되는 방식이 결정된다. 기본 명령어는 **more** 이며, **cat**, **less** 등이 사용될 수 있다. 단, 이 명령어는 Linux에서만 정상 동작한다.

페이저 명령어를 **more** 로 등록하는 경우 질의 결과를 페이지 단위로 출력하고, 스페이스 키가 눌려질 때까지 다음 페이지의 출력을 대기한다. ::

    csql>;pager more
    
페이저 명령어를 cat으로 등록하는 경우 페이징 없이 질의 결과 전체를 출력한다. ::

    csql>;pager cat

output.txt로 출력을 리다이렉션하면 질의 결과 전체를 output.txt에 기록한다. ::

    csql>;pager cat > output.txt

페이저 명령어를 **less** 로 등록하는 경우 질의 결과에 대해 포워딩, 백워딩을 할 수 있고 패턴 검색도 할 수 있다. ::

    csql>;pager less
    
**less** 에서 사용하는 키보드 명령은 다음과 같다.

* Page UP, b: 한 페이지 뒤로 가기(백워딩)

* Page Down, Space: 한 페이지 앞으로 가기(포워딩)

* /문자열: 질의 결과에서 문자열 찾기

* n: 다음 문자열 찾기

* N: 앞의 문자열 찾기

* q: 질의 결과 보기 종료하기
    
**현재 작업 디렉터리 변경(;CD)**

CSQL 인터프리터를 실행한 현재 작업 디렉터리를 지정된 디렉터리로 변경한다. 경로를 지정하지 않으면 홈 디렉터리로 변경된다. ::

    csql> ;cd /home1/DBA/CUBRID
    Current directory changed to  /home1/DBA/CUBRID.

**CSQL 인터프리터 종료(;EXit)**

CSQL 인터프리터를 종료한다. ::

    csql> ;ex

**질의 버퍼 초기화(;CLear)**

**;CLear** 세션 명령어는 질의 버퍼의 내용을 초기화한다. ::

    csql> ;clear
    csql> ;list

**질의 버퍼의 내용 보여주기(;List)**

현재까지 입력 수정된 질의 버퍼의 내용을 화면에 출력하기 위해서는 **;List** 세션 명령어를 사용한다. 질의 버퍼는 사용자의 SQL 입력, **;REAd** 명령어, **;EDIT** 명령어 등으로 수정될 수 있다. ::

    csql> ;list

**SQL 문 실행(;RUn)**

질의 버퍼에 있는 SQL 문을 실행하는 명령어이다. 다음에서 설명하는 **;Xrun** 세션 명령어와 달리 질의 실행 후에도 버퍼는 초기화되지 않는다. ::

    csql> ;run

**SQL 문 실행 후 질의 버퍼 초기화(;Xrun)**

질의 버퍼에 있는 SQL 문을 실행하는 명령어이다. 질의 실행 후 질의 버퍼는 초기화된다. ::

    csql> ;xrun

**트랜잭션 커밋(;COmmit)**

현재 수행되고 있는 트랜잭션을 커밋(commit)하는 세션 명령어이다. 자동 커밋(auto-commit) 모드가 아닌 경우, 명시적으로 커밋 명령어를 입력해야 CSQL 인터프리터에서 수행 중이던 트랜잭션이 커밋된다. 자동 커밋(auto-commit) 모드인 경우는 SQL을 실행할 때마다 트랜잭션이 자동으로 커밋된다. ::

    csql> ;commit
    Execute OK. (0.000192 sec)
    
**트랜잭션 롤백(;ROllback)**

현재 수행되고 있는 트랜잭션을 롤백(rollback)하는 세션 명령어이다. **;COmmit** 과 마찬가지로 자동 커밋(auto-commit) 모드가 아닐 경우(OFF)에만 의미가 있다. ::

    csql> ;rollback
    Execute OK. (0.000166 sec)

**자동 커밋 모드 설정(;AUtocommit)**

자동 커밋(auto-commit) 모드를 **ON** 또는 **OFF** 로 설정하는 명령어이다. 만약, **ON** 또는 **OFF** 를 지정하지 않으면 현재 설정된 값을 보여준다. 참고로 CSQL 인터프리터는 기본값이 **ON** 이다. ::

    csql> ;autocommit off
    AUTOCOMMIT IS OFF

**체크포인트 수행(;CHeckpoint)**

CSQL 세션 내에서 체크포인트 수행을 지시하는 명령어이다. CSQL 인터프리터 접속 시 사용자 지정 옵션(**-u** *user_name*)에 **DBA** 그룹 멤버가 지정되고 시스템 관리자 모드(**--sysadm**)로 접속한 경우에만 수행할 수 있다.

체크포인트는 현재 데이터 버퍼에 존재하는 모든 더티 페이지를 디스크로 내려쓰기(flush)하는 작업이며, CSQL 세션 내에서 파라미터 값을 설정하는 명령어(**;set** *parameter_name value*)를 통해서도 체크포인트 주기를 변경할 수 있다. 체크포인트 수행 주기와 관련된 파라미터는 **checkpoint_interval**\ 과 **checkpoint_every_size** 가 있다. 이에 대한 자세한 내용은 :ref:`logging-parameters` 를 참고한다. ::

    sysadm> ;checkpoint
    Checkpoint has been issued.

**트랜잭션 모니터링 또는 종료(;Killtran)**

CSQL 세션 내에서 트랜잭션 상태 정보를 확인하거나 특정 트랜잭션을 종료시키는 명령어이다. CSQL 인터프리터 접속 시 사용자 지정 옵션(**-u** *user_name*)에 **DBA** 그룹 멤버가 지정되고 시스템 관리자 모드(**--sysadm**)로 접속한 경우에만 수행할 수 있다. 인자가 생략되면 모든 트랜잭션 상태 정보를 화면 출력하고, 인자로 특정 트랜잭션 ID가 지정되면 해당 트랜잭션을 종료시킨다. ::

    sysadm> ;killtran
    Tran index      User name      Host name      Process id      Program name
    -------------------------------------------------------------------------------
          1(+)            dba      myhost             664           cub_cas
          2(+)            dba      myhost            6700              csql
          3(+)            dba      myhost            2188           cub_cas
          4(+)            dba      myhost             696              csql
          5(+)         public      myhost            6944              csql
     
    sysadm> ;killtran 3
    The specified transaction has been killed.

**데이터베이스 재접속(;REStart)**

CSQL 세션 내에서 대상 데이터베이스에 재접속을 시도하는 명령어이다. CSQL 인터프리터를 클라이언트/서버 모드(CS 모드)로 수행하는 경우에는 서버와의 접속이 해제되므로 유의한다. 이 명령어는 HA 환경에서 장애로 인해 다른 서버로 절체가 이루어짐에 따라 도중에 서버와의 연결이 해제되는 경우, 세션을 유지하면서 절체된 서버로 재접속할 때 유용하게 사용할 수 있다. ::

    csql> ;restart
    The database has been restarted.

**현재 날짜 출력(;DATE)**

**;DATE** 는 CSQL 인터프리터에서 현재 날짜 및 시간 정보를 출력한다. ::

    csql> ;date
         Tue July 29 18:58:12 KST 2008

**대상 데이터베이스 정보 출력(;DATAbase)**

CSQL 인터프리터에서 작업 중인 데이터베이스 이름 및 호스트 이름을 출력한다. 만약, 대상 데이터베이스가 HA모드로 동작 중이라면 현재 HA모드(active, standby, 또는 maintenance)도 함께 출력될 것이다. ::

    csql> ;database
         demodb@cubridhost (active)

**지정한 테이블의 스키마 정보 출력(;SChema)**

**;SChema** 세션 명령어로 지정한 테이블의 스키마 정보를 확인할 수 있다. 해당 테이블의 이름, 칼럼 명, 제약 사항 등의 정보가 출력된다. ::

    csql> ;schema event
    === <Help: Schema of a Class> ===
     <Class Name>
         event
     <Attributes>
         code           INTEGER NOT NULL
         sports         CHARACTER VARYING(50)
         name           CHARACTER VARYING(50)
         gender         CHARACTER(1)
         players        INTEGER
     <Constraints>
         PRIMARY KEY pk_event_event_code ON event (code)

**트리거 출력(;TRigger)**

지정한 트리거 명을 검색하여 출력하는 명령어이다. 트리거 명을 지정하지 않으면 정의된 모든 트리거를 보여준다. ::

    csql> ;trigger
    === <Help: All Triggers> ===
        trig_delete_contents

**파라미터 값 확인(;Get)**

**;Get** 세션 명령어를 이용해 현재 CSQL 인터프리터에 설정된 파라미터 값을 확인할 수 있다. 지정된 파라미터 명이 정확하지 않으면 오류가 발생한다. ::

    csql> ;get isolation_level
    === Get Param Input ===
    isolation_level=4

**파라미터 값 설정(;SEt)**

특정 파라미터의 값을 설정하기 위해서는 **;Set** 세션 명령어를 사용한다. 동적 변경이 가능한 파라미터만 값을 변경할 수 있으며, 서버 파라미터는 DBA 권한이 있어야만 값을 변경할 수 있다. 동적 변경이 가능한 파라미터 목록은 :ref:`broker-configuration` 를 참고한다. ::

    csql> ;set block_ddl_statement=1
    === Set Param Input ===
    block_ddl_statement=1

    -- dba 계정으로 실행한 csql에서 log_max_archives 값을 동적으로 변경
    csql> ;set log_max_archives=5

**문자열 타입과 비트 타입 칼럼의 출력 길이 지정(;STring-width)** 

문자열 타입과 비트 타입 칼럼의 출력 길이를 제한하기 위해서 사용할 수 있다. 

**;string-width** 뒤에 값을 주지 않으면 현재의 출력 길이를 보여준다. 값이 0이면, 해당 칼럼의 값을 모두 출력한다. 값이 0보다 크다면, 해당 길이만큼 칼럼의 값을 출력한다.

::

    csql> SELECT name FROM NATION WHERE NAME LIKE 'Ar%';
      'Arab Republic of Egypt'
      'Aruba'
      'Armenia'
      'Argentina'

    csql> ;string-width 5
    csql>  SELECT name FROM NATION WHERE NAME LIKE 'Ar%';
      'Arab '
      'Aruba'
      'Armen'
      'Argen'

    csql> ;string-width
    STRING-WIDTH : 5

**지정한 칼럼의 출력 길이 지정(;COLumn-width)**

타입과 상관없이 특정 칼럼의 출력 길이를 제한하기 위해서 사용할 수 있다. 
;COL 뒤에 값을 주지 않으면 현재 설정된 칼럼의 출력 길이를 보여준다.  뒤에 값이 0이면 해당 칼럼의 값을 모두 출력하며, 값이 0보다 크다면 해당 길이만큼 칼럼의 값을 출력한다.  ::

    csql> CREATE TABLE tbl(a BIGINT, b BIGINT);
    csql> INSERT INTO tbl VALUES(12345678890, 1234567890)
    csql> ;column-width a=5
    csql> SELECT * FROM tbl;
          12345            1234567890
    csql> ;column-width
    COLUMN-WIDTH a : 5

**질의 실행 계획 보기 수준 설정(;PLan)**

**;PLan** 세션 명령어는 질의 실행 계획 보기의 수준을 설정한다. 수준은 **simple**, **detail**, **off** 로 지정한다. 각 설정값의 의미는 다음과 같다.

*   **off**: 질의 실행 계획을 출력하지 않음
*   **simple**: 질의 실행 계획을 단순하게 출력함. (OPT LEVEL=257)
*   **detail**: 질의 실행 계획을 자세하게 출력함. (OPT LEVEL=513)

.. _set-autotrace:
 
**SQL 트레이스 설정(;trace)**

질의 결과를 출력할 때 SQL 트레이스 결과를 항상 함께 출력할 것인지 여부를 설정한다. 
이 명령을 사용하여 SQL 트레이스를 ON으로 설정하면, "**SHOW TRACE**" 구문을 실행하지 않아도 질의 결과를 출력한 다음에 질의 프로파일링(profiling) 결과를 자동으로 출력한다.
 
보다 자세한 설명은 :ref:`query-profiling`\ 를 참고한다.
 
명령 형식은 다음과 같다.
 
::
 
    ;trace {on | off} [{text | json}]
 
*   **on**: SQL 트레이스를 on한다.
*   **off**: SQL 트레이스를 off한다.
*   **text**: 일반 TEXT 형식으로 출력한다. OUTPUT 이하 절을 생략하면 일반 TEXT 형식으로 출력한다.
*   **json**: JSON 형식으로 출력한다.

.. note:: 독립 모드(-S 옵션 사용)로 실행한 CSQL 인터프리터는 SQL 트레이스 기능을 지원하지 않는다.

**정보 출력(;Info)**

**;Info** 세션 명령어는 스키마, 트리거, 작업 환경, 잠금, 통계 등의 정보를 확인할 수 있는 명령어이다. ::

    csql> ;info lock
    *** Lock Table Dump ***
     Lock Escalation at = 100000, Run Deadlock interval = 1
    Transaction (index  0, unknown, unknown@unknown|-1)
    Isolation COMMITTED READ 
    State TRAN_ACTIVE
    Timeout_period -1
    ......

.. _csql-execution-statistics:

**CSQL 실행 통계 정보 출력(;.Hist)**

**;.Hist** 세션 명령어는 CSQL에서 질의 실행 통계 정보의 수집을 시작하기 위한 CSQL 세션 명령어로서, 이 정보는 "**;.Hist on**"이  입력된 이후부터 현재 연결된 CSQL에 대해서만 수집된다. 다음은 이 세션 명령어의 **;.Hist** 세션 명령어의 옵션으로 **on**, **off**\ 를 제공하며, 각 옵션의 의미는 다음과 같다.

*   **on**: 해당 연결에 대한 서버 실행 통계 정보 수집을 시작.
*   **off**: 서버 실행 통계 정보 수집을 종료.

단, **cubrid.conf** 파일에서 관련 파라미터(**communication_histogram**)를 **yes**\ 로 설정하거나, csql에서 ";se communication_histogram=yes"를 실행해야만 이 명령어를 사용할 수 있다.

"**;.Hist on**" 이후 서버 실행 통계 정보를 화면에 출력하기 위해서는 **;.dump_hist** 또는 **;.x**\ 와 같은 실행 명령어를 입력해야 한다. **;.dump_hist** 또는 **;.x**\ 를 수행할 때마다, 축적된 값이 출력된 후 모든 값이 초기화된다.

참고로, DB 서버의 모든 질의 실행 통계 정보를 확인하기 위해서는 **cubrid statdump** 유틸리티를 사용해야 한다.

다음 예제는 현재 연결에 대한 서버 실행 통계 정보를 확인하는 예제이다.  출력되는 통계 정보 항목 또는 **cubrid statdump**\ 에 대한 설명은 :ref:`statdump`\ 을 참고한다.

::

    csql> ;.hist on
    csql> ;.x
    Histogram of client requests:
    Name                            Rcount   Sent size  Recv size , Server time
     No server requests made
     
     *** CLIENT EXECUTION STATISTICS ***
    System CPU (sec)              =          0
    User CPU (sec)                =          0
    Elapsed (sec)                 =         20
     
     *** SERVER EXECUTION STATISTICS ***
    Num_file_creates              =          0
    Num_file_removes              =          0
    Num_file_ioreads              =          0
    Num_file_iowrites             =          0
    Num_file_iosynches            =          0
    Num_data_page_fetches         =         56
    Num_data_page_dirties         =         14
    Num_data_page_ioreads         =          0
    Num_data_page_iowrites        =          0
    Num_data_page_victims         =          0
    Num_data_page_iowrites_for_replacement =          0
    Num_log_page_ioreads          =          0
    Num_log_page_iowrites         =          0
    Num_log_append_records        =          0
    Num_log_archives              =          0
    Num_log_checkpoints           =          0
    Num_log_wals                  =          0
    Num_page_locks_acquired       =          2
    Num_object_locks_acquired     =          2
    Num_page_locks_converted      =          0
    Num_object_locks_converted    =          0
    Num_page_locks_re-requested   =          0
    Num_object_locks_re-requested =          1
    Num_page_locks_waits          =          0
    Num_object_locks_waits        =          0
    Num_tran_commits              =          1
    Num_tran_rollbacks            =          0
    Num_tran_savepoints           =          0
    Num_tran_start_topops         =          3
    Num_tran_end_topops           =          3
    Num_tran_interrupts           =          0
    Num_btree_inserts             =          0
    Num_btree_deletes             =          0
    Num_btree_updates             =          0
    Num_btree_covered             =          0
    Num_btree_noncovered          =          0
    Num_btree_resumes             =          0
    Num_query_selects             =          1
    Num_query_inserts             =          0
    Num_query_deletes             =          0
    Num_query_updates             =          0
    Num_query_sscans              =          1
    Num_query_iscans              =          0
    Num_query_lscans              =          0
    Num_query_setscans            =          0
    Num_query_methscans           =          0
    Num_query_nljoins             =          0
    Num_query_mjoins              =          0
    Num_query_objfetches          =          0
    Num_network_requests          =          8
    Num_adaptive_flush_pages      =          0
    Num_adaptive_flush_log_pages  =          0
    Num_adaptive_flush_max_pages  =          0
     
     *** OTHER STATISTICS ***
    Data_page_buffer_hit_ratio    =     100.00
    csql> ;.hist off

**질의 수행 시간을 출력(;TIme)**

**;TIme** 세션 명령어로 질의를 수행한 시간을 출력하도록 설정할 수 있다. **ON** 혹은 **OFF** 로 지정하며, 인자가 없으면 현재 설정값을 보여준다. 기본값은 **ON** 이다.

**SELECT** 질의에서는 페치(fetch)한 레코드를 출력하는 시간까지 포함한다. 따라서, **SELECT** 질의에서 모든 레코드의 출력이 한 번에 끝난 수행 시간을 보려면 CSQL 인터프리터 수행 시 **--no-pager** 옵션을 사용해야 한다. ::

    $ csql -u dba --no-pager demodb
    csql> ;time ON
    csql> ;time
    TIME IS ON

**질의 결과를 칼럼 당 한 라인으로 출력(;LINe-output)**

이 값을 **ON** 으로 설정하면 질의 결과 레코드를 칼럼 당 한 라인으로 출력한다. 기본 설정은 OFF로서, 한 레코드는 한 라인으로 출력한다. ::

    csql> ;line-output ON
    csql> select * from athlete;

    === <Result of SELECT Command in Line 1> ===

    <00001> code       : 10999
            name       : 'Fernandez Jesus'
            gender     : 'M'
            nation_code: 'ESP'
            event      : 'Handball'
    <00002> code       : 10998
            name       : 'Fernandez Jaime'
            gender     : 'M'
            nation_code: 'AUS'
            event      : 'Rowing'
    ...

**질의 수행 이력 확인(;HISTORYList)**

앞서 수행된 명령어(입력 내용)를 수행 번호를 포함한 리스트로 보여준다. ::

    csql> ;historylist
    ----< 1 >----
    select * from nation;
    ----< 2 >----
    select * from athlete;

**지정된 수행 번호에 해당하는 입력 내용을 버퍼로 불러오기(;HISTORYRead)**

**;HISTORYRead** 세션 명령어를 사용해 지정된 **;HISTORYList** 에서 확인한 수행 번호에 해당하는 내용을 명령어 버퍼로 불러올 수 있다. 해당 SQL 문을 직접 입력한 것과 같은 상태이므로 바로 **;run** 또는 **;xrun** 를 입력할 수 있다. ::

    csql> ;historyread 1

**기본 편집기를 호출(;EDIT)**

지정된 편집기를 호출하는 세션 명령어이다. 기본 편집기는 Linux에서는 vi이고, Windows에서는 메모장이다. 다른 편집기로 지정하려면 **;EDITOR_Cmd** 명령어를 이용한다. ::

    csql> ;edit

**편집기 설정(;EDITOR_Cmd)**

**;EDIT** 세션 명령어에서 사용될 편집기를 지정한다. 예제와 같이 기본 편집기인 vi 대신에 해당 시스템에 설치된 다른 편집기(예: emacs)를 설정할 수 있다. ::

    csql> ;editor_cmd emacs
    csql> ;edit
