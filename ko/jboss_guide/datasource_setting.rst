

.. _datasource_setting:

CUBRID DataSource 설정
======================

본문에 들어가기에 앞서 DataSource를 설정하기 위해서는 JDBC 드라이버가 설정이 되어있어야 한다.

JDBC 드라이버 설치: :ref:`install_jboss`

DataSource 역시 CUBRID JDBC 드라이버 등록과 마찬가지로 운영체제별로 큰 차이는 존재하지 않는다. 사용자가 편한 방법을 통해 진행하는 것을 추천한다.

이 장에서는 non-XA DataSource를 설치하는 과정을 다루고 있다.

DataSource 설정
----------------

JBoss-cli를 사용하여 설치
^^^^^^^^^^^^^^^^^^^^^^^^^^

#. jboss-cli.sh(Windows는 jboss-cli.bat)을 실행한다.

#. connect를 입력하여 서버에 접속하고 다음 명령어를 입력한다 ::

    /subsystem=datasources/data-source=cubridDS:add(jndi-name="java:/cubridDS",connection-url="[CUBRID_URL]",driver-name=cubrid,user-name=[USER_NAME],password=[PASSWORD])

#. 아래 명령어를 입력하여 테스트를 실행한다. ::

    /subsystem=datasources/data-source=cubridDS:test-connection-in-pool

    {
        "outcome" => "success",
        "result" => [true]
    }

#. 위처럼 메시지가 출력되면 등록에 성공한 것이다.

웹 콘솔을 이용하여 설치
^^^^^^^^^^^^^^^^^^^^^^^

#. 브라우저를 통해 http://[SERVER_IP]:9990 로 접속한다.

#. configuration -> SubSystems -> Datasources & Drivers -> DataSource 에서 추가 버튼을 선택한다.

#. Custom을 선택한 뒤 Next를 눌러 진행한다.

#. Attribute에서 아래와 같이 입력한다. ::

    Name: [DATASOURCE_NAME]
    JNDI Name: java:/[JNDI_NAME]

#. JDBC Driver에서 아래와 같이 입력한다. ::

    Driver Name: cubrid
    Driver Module Name: com.cubrid
    Driver Class Name: cubrid.jdbc.driver.CUBRIDDriver

#. Connection에서 아래와 같이 입력한다. ::

    Connection URL: [CONNECTION URL]
    User Name: [USER_NAME]
    Password: [PASSWORD]
    Security Domain: [SECURITY_DOMAIN]

#. Test Connection을 수행하여 결과를 확인한다.

#. 성공하면 등록이 완료된 것이다.

실패한 경우 Back 버튼을 눌러 설정에 문제가 있는지 검토한다.

XML 파일을 직접 수정하여 설치
^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^

#. $[JBOSS_HOME]\\standalone\\configuration으로 이동한다.

#. standalone.xml 파일을 편집기로 실행한다.

#. datasources 태그를 찾는다.

#. 다음과 같이 내용을 추가한다. ::

    <datasource jndi-name="java:/cubridDS" pool-name="cubrid">
        <connection-url>[CONNECTION_URL]</connection-url>
        <driver-class>cubrid.jdbc.driver.CUBRIDDriver</driver-class>
        <driver>cubrid</driver>
        <security>
            <user-name>[USER_NAME]</user-name>
            <password>[PASSWORD]</password>
        </security>
    </datasource>

#. 변경된 내용을 저장 후 종료한다.

.. note ::

    모든 설정 방법에서 사용되는 attribute는 jboss에 존재하는 attribute 중에 극히 일부만 기재되어있다.

    사용자가 필요한 경우 환경에 맞게 property를 추가하여 사용하면 된다. 더 많은 옵션이 필요하면 이곳을 참고한다.: 

    https://access.redhat.com/documentation/en-us/red_hat_jboss_enterprise_application_platform/7.3/html-single/configuration_guide/index#datasource_parameters