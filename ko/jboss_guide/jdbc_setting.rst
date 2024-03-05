

.. _jdbc_setting:

CUBRID JDBC 설치
================

이 장에서는 JBoss에 큐브리드 JDBC를 설치하는 과정을 설명한다. CUBRID 11.0 버전이 설치되어 있다는 가정하에 진행되므로 CUBRID를 설치하고 본 매뉴얼을 읽는 것을 권장한다.

설치 방법은 다음 문서를 참고한다: :ref:`install-execute`\

JDBC 파일을 다운받는 경로는 다음과 같다: https://ftp.cubrid.org/CUBRID_Drivers/JDBC_Driver/JDBC-11.0-latest-cubrid.jar

JDBC를 설치하는 방법은 운영체제에 영향을 크게 받지 않으므로 구분을 하지 않고 설명한다. 단 등록하는 방법의 차이가 있으므로 자신의 환경에서 편한 방법을 사용하는 것을 추천한다.

JDBC 설치 방법
---------------

#. $[JBOSS_HOME]\\modules\\system\\layers\\base\\com\\cubrid\\main 로 이동한다.

#. 해당 폴더로 CUBRID JDBC 파일을 이동시킨다.

#. 같은 위치에 module.xml 파일을 생성한다.

#. module.xml 파일의 내용을 다음과 같이 작성한다. ::

    <?xml version="1.0" encoding="UTF-8"?>
    <module xmlns="urn:jboss:module:1.5" name="com.cubrid">
        <resources>
            <resource-root path="JDBC-11.0-latest-cubrid.jar"/>
        </resources>
        <dependencies>
            <module name="javax.api"/>
            <module name="javax.transaction.api"/>
        </dependencies>
    </module>

.. note::
    base 디렉터리 밑에 사용자가 원하는 이름의 폴더를 생성하여 CUBRID JDBC 드라이버를 위치해도 된다. 단 경로의 끝에는 반드시 main 폴더가 있어야 하며 그 안에 JDBC 드라이버와 module.xml 파일이 있어야 JBoss가 드라이버를 인식한다.

    예를 들어, 만약 사용자가 아래의 경로에 JDBC 드라이버를 위치한다면

    $[JBOSS_HOME] \\ modules \\ system \\ layers \\ base \\ **driver \\ cubridjdbc \\ main**

    module.xml 파일의 name 부분에도 수정된 경로를 반영해야 한다.

    ...
    <module xmls="urn:jboss:module:1.5" **name="driver.cubridjdbc"**>
    ...

jboss-cli를 사용하여 설치
^^^^^^^^^^^^^^^^^^^^^^^^^^

#. standalone.sh(Windows의 경우 standalone.bat)을 실행한 뒤 jboss-cli를 실행한다.

#. connect를 입력하여 서버에 연결한다.

#. 아래의 명령어를 입력한다. ::

    /subsystem=datasources/jdbc-driver=cubrid:add(driver-name=cubrid,driver-module-name=com.cubrid,driver-datasource-class-name=cubrid.jdbc.driver.CUBRIDDataSource,driver-xa-datasource-class-name=cubrid.jdbc.driver.CUBRIDXADataSource)

#. 아래의 메시지가 출력되면 설치가 완료된 것이다. ::

    {"outcome" => "success"}

#. exit를 입력하여 jboss-cli를 종료할 수 있다.

만약 에러 메시지가 표시될 경우 위의 JDBC 설치 경로 내용을 참고한다.

웹 콘솔을 이용하여 설치
^^^^^^^^^^^^^^^^^^^^^^^

#. 브라우저를 통해 http://[SERVER_IP]:9990 로 접속한다.

#. configuration -> SubSystems -> Datasources & Drivers -> JDBC Drivers에서 추가 버튼을 선택한다.

#. 아래와 같이 내용을 입력한 뒤 **Add** 를 선택한다.

    .. image:: ../images/jboss_jdbc_setting.png

#. 등록이 성공적으로 완료되면 다음과 같은 내용을 확인 할 수 있다.

    .. image:: ../images/jboss_jdbc_info.png


XML 파일을 직접 수정하여 설치
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. $[JBOSS_HOME]\\standalone\\configuration으로 이동한다.

#. standalone.xml 파일을 편집기로 실행한다.

#. datasources 태그 밑의 drivers 태그를 찾는다.

#. 다음과 같이 내용을 추가한다. ::

    <drivers>
        <driver name="h2" module="com.h2database.h2">
            <xa-datasource-class>org.h2.jdbcx.JdbcDataSource</xa-datasource-class>
        </driver>
        <driver name="cubrid" module="com.cubrid">
            <driver-class>cubrid.jdbc.driver.CUBRIDDriver</driver-class>
            <xa-datasource-class>cubrid.jdbc.driver.CUBRIDXADataSource</xa-datasource-class>
            <datasource-class>cubrid.jdbc.driver.CUBRIDDataSource</datasource-class>
        </driver>
    </drivers>

#. 변경된 내용을 저장 후 종료한다.

기타
^^^^^

설치를 완료하면 즉시 적용이 되지만 혹시 모를 오류를 대비하여 서버를 reboot 하는 것을 추천한다.