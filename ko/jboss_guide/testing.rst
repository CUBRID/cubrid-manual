

.. _test_datasource:

JBoss DataSource 테스트
========================

이 장에서는 등록한 DataSource 를 테스트용으로 작성한 jsp파일을 통해 테스트 하는 방법을 설명한다.

테스트 방식은 운영체제와 큰 관계가 없어서 설명에 구분을 두지 않는다. 이 장에 기록된 방법을 제외하고 사용자가 직접 코드를 작성해 테스트하는 것도 가능하다.

테스트 방법
-----------

#. test.jsp파일을 생성하여 내용을 아래와 같이 작성한다. ::

	<%@ page import="java.sql.Connection" %>
	<%@ page import="java.sql.ResultSet" %>
	<%@ page import="java.sql.Statement" %>
	<%@ page import="java.sql.SQLException" %>>

	<%@ page import="javax.sql.DataSource" %>
	<%@ page import="javax.naming.InitialContext" %>

	<%
		Connection conn = null;
		Statement stmt = null;
		ResultSet rs = null;

		try {
			InitialContext ic = new InitialContext();
			DataSource ds = (DataSource) ic.lookup("[JNDI_NAME]");
			conn = ds.getConnection();
			stmt = conn.createStatement();
			rs = stmt.executeQuery("select SYSDATETIME");

			while(rs.next()){
				out.println(rs.getString("SYS_DATETIME"));
			}
		} catch (Exception e) {
			e.printStackTrace();
		} finally {
			try {
				if (rs != null) {
					rs.close();
				}
			} catch (SQLException e) {}

			try {
				if (stmt != null){
					stmt.close();
				}
			} catch (SQLException e) {}

			try {
				if (conn != null) {
					conn.close();
				}
			} catch (SQLException e) {}
		}
	%>

#. 생성한 파일을 아래의 경로로 이동시킨다. ::

	$[JBOSS_HOME]\standalone\deployments\test.war

#. standalone.sh(또는 standalone.bat)를 통해 서버를 실행시킨 후 브라우저에 아래의 경로를 입력하여 결과를 확인한다. ::

	http://[SERVER_ADDRESS]:8080/test/test.jsp

현재 년, 월, 일, 시, 분, 초, 밀리초 결과가 예외 없이 맞게 출력된다면 DataSource 설정이 성공적으로 마무리되었다.