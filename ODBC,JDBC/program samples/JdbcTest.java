
import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;

public class JdbcTest {

	// ORACLE JDBC Driver
	String DB_DRV = "oracle.jdbc.driver.OracleDriver";
	// DB 연결정보
	String DB_IP   = "localhost";
	String DB_PORT = "1521";
	String DB_SVCNAME  = "XEPDB1"; // DB service name
	String DB_ID   = "HR";    // 접속할 사용자 ID
	String DB_PWD  = "tiger"; // 접속할 사용자 password
	String DB_URL  = "jdbc:oracle:thin:@" + DB_IP + ":" + DB_PORT + "/" + DB_SVCNAME; 
	
	Connection conn = null;  // DB 접속
	Statement  stmt = null;  // SQL Statement
	ResultSet  rs = null;    // SQL 실행 결과
	
	// main 함수
	public static void main(String[] args) 
	{
		JdbcTest test = new JdbcTest();
		test.connect();
		
		System.out.println("\n### Before Insertion - All Jobs ###");
		test.executeSelect(false);
		
		
		System.out.println("\n### Before Insertion - CP_EN Job ###");
		test.executeSelect(true);
		test.executeInsert();
		
		System.out.println("\n### After Insertion ###");
		test.executeSelect(true);
		test.executeDelete();
		
		System.out.println("\n### After Deletion ###");
		test.executeSelect(true);
		
		test.disconnect();
	}
	
	// DB 접속
	public void connect()
	{
		try
		{
			System.out.println("======================================");
			System.out.println("DB_DRV : " + DB_DRV);
			System.out.println("DB_URL : " + DB_URL);
			System.out.println("DB_ID : " + DB_ID);
			System.out.println("DB_PWD : " + DB_PWD);
			System.out.println("--------------------------------------");
			Class.forName(DB_DRV);
			conn = DriverManager.getConnection(DB_URL, DB_ID, DB_PWD);
			System.out.println("Connection Success");
			System.out.println("======================================");
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
		}
	}
	
	// DB 접속 종료
	public void disconnect()
	{
		try
		{
			if (rs != null) rs.close();
			if (stmt != null) stmt.close();
			if (conn != null) conn.close();
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
		}
		finally
		{
			if (rs != null) try { rs.close(); } catch(Exception e) {}
			if (stmt != null) try { stmt.close(); } catch(Exception e) {}
			if (conn != null) try { conn.close(); } catch(Exception e) {}
		}
	}
	
	// SQL Select 문 수행
	public void executeSelect(boolean condFlag)
	{
		if (conn == null) return;
		try 
		{
			String strSQL = "select job_id, job_title, min_salary from jobs" +
					(condFlag ? " where job_id = 'CP_EN'":"");
			stmt = conn.createStatement();
			rs = stmt.executeQuery(strSQL);
			System.out.println();
			System.out.println("=========================================");
			System.out.println("SQL : " + strSQL);
			System.out.println("-----------------------------------------");
			if (!rs.next())
				System.out.println("no tuples in the table");
			else
			{
				do
				{
					System.out.println("job_id : " + rs.getString(1));
					System.out.println("job_title : " + rs.getString(2));			
					System.out.println("min_salary : " + rs.getInt(3));			
					System.out.println("-------------------------------------");
				} while (rs.next());
			}
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
			disconnect();
		}
	}
	
	// SQL Insert 문 수행
	public void executeInsert()
	{
		if (conn == null) return;
		try
		{
			String strSQL = "insert into jobs values('CP_EN', 'Computer Engineer', 20000, 60000)";
			stmt = conn.createStatement();
			rs = stmt.executeQuery(strSQL);
			System.out.println();
			System.out.println("=====================================");
			System.out.println("SQL : " + strSQL);
			System.out.println("-------------------------------------");
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
			disconnect();
		}		
	}
	
	// SQL Delete 문 수행
	public void executeDelete()
	{
		if (conn == null) return;
		try
		{
			String strSQL = "delete from jobs where job_title = 'Computer Engineer'";
			stmt = conn.createStatement();
			rs = stmt.executeQuery(strSQL);
			System.out.println();
			System.out.println("=====================================");
			System.out.println("SQL : " + strSQL);
			System.out.println("-------------------------------------");
		}
		catch (Exception ex)
		{
			ex.printStackTrace();
			disconnect();
		}		
	}

}
