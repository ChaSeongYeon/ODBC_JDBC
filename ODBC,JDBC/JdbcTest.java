package jdbc_lab;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.Scanner;

public class JdbcTest {

	// ORACLE JDBC Driver
	String DB_DRV = "oracle.jdbc.driver.OracleDriver";
	// DB 연결정보
	String DB_IP   = "localhost";
	String DB_PORT = "1521";
	String DB_SVCNAME  = "XEPDB1"; // DB service name
	String DB_ID   = "HR";    // 접속할 사용자 ID
	String DB_PWD  = "1234"; // 접속할 사용자 password
	String DB_URL  = "jdbc:oracle:thin:@" + DB_IP + ":" + DB_PORT + "/" + DB_SVCNAME; 
	static Scanner input = new Scanner(System.in);
	
	static Connection conn = null;  // DB 접속
	static Statement  stmt = null;  // SQL Statement
	static ResultSet  rs = null;    // SQL 실행 결과
	
	// main 함수
	public static void main(String[] args) 
	{
		boolean done = false;
		
		JdbcTest test = new JdbcTest();
		test.connect();
		
		System.out.println("\n### 1. Insert, Delete, Update 질의 실습 ###");
		System.out.println("\n### Jobs테이블을 복사하여 Jobs_copy테이블을 생성하시오 ###");
		//test.executeCreate("jobs_copy", "jobs");
		
		System.out.println("\n### 직업 테이블 Jobs_copy 에 새로운 직업 Computer Engineer에 관한 정보를 INSERT하는 코드를 공부하고, 수행하여 결과를 확인하시오 ###");
		test.executeSelect(true, "job_id = 'CP_EN'");
		test.executeInsert();
		test.executeSelect(true, "job_id = 'CP_EN'");
		
		System.out.println("\n### 동일한 방법으로 직업 테이블 Jobs_copy안의 Computer Engineer의 직업을 DELETE하는 코드를 공부하고, 수행하여 결과를 확인하시오 ###");
		test.executeSelect(true, "job_id = 'CP_EN'");
		test.executeDelete();
		test.executeSelect(true, "job_id = 'CP_EN'");
		
		System.out.println("\n### Jobs_copy테이블에서 JOB_TITLE에 ‘Marketing’글자가 있는 직업의 MAX_SALARY를 500증가시키기 위한 질의를 작성하고, 그 결과를 확인하시오 ###");
		test.executeSelect(true, "job_title like 'Marketing%'");
		test.executeUpdate("jobs_copy2", "max_salary = max_salary + 500 where job_title like 'Marketing%'");
		test.executeSelect(true, "job_title like 'Marketing%'");
		
		
		System.out.println("\n### 2. 아래 2-4번에서 요구되는 테이블을 식별하고, 각 테이블을 복사한 테이블을 생성하시오 ###");
		//test.executeCreate("e_copy", "employees");
		//test.executeCreate("c_copy", "countries");
		//test.executeCreate("d_copy", "departments");
		//test.executeCreate("l_copy", "locations");
		
		
		System.out.println("\n### 3. 각 나라에 속한 사원들의 급여의 총 합을 기준으로 나라 이름을 정렬하여 출력하시오 ###");
		test.executeSelect_3(true, "country_name from c_copy c "
				+ "join l_copy l on c.country_id = l.country_id "
				+ "join d_copy d on l.location_id = d.location_id "
				+ "join e_copy e on d.department_id = e.department_id "
				+ "group by country_name "
				+ "order by sum(e.salary)");
		
		
		System.out.println("\n### 4. 급여(SALARY)가 10,000이상인 사원들의 종합 급여가 가장 높은 부서 2곳의 부서 이름 (DEPARTMENT_NAME)과 속한 나라의 이름(CONTRY_NAME)을 출력하시오 ###");
		test.executeSelect_4(true, "d.department_name, c.country_name from d_copy d "
				+ "join l_copy l on d.location_id = l.location_id "
				+ "join c_copy c on l.country_id = c.country_id "
				+ "join e_copy e on d.department_id = e.department_id "
				+ "where e.salary >= 10000 "
				+ "group by d.department_name, c.country_name "
				+ "order by sum(e.salary) desc "
				+ "fetch first 2 rows only" );
		
		
		System.out.println("\n### 5. ODBC를 이용한 사원 관리 프로그램을 작성하시오 ###");
		while (!done)
	    {
	        int num = Display();
	        switch (num)
	        {
	            case 0:
	                done = true;
	                break;
	            case 1:
	                print_First_Name();
	                break;
	            case 2:
	                print_Last_Name();
	                break;
	            case 3:
	                print_Specific_Name();
	                break;
	            case 4:
	                print_Department_Name();
	                break;
	            case 5:
	                print_MinMax_Sal_3();
	                break;
	            case 6:
	                change_Salary();
	                break;
	            case 7:
	                change_Department();
	                break;
	            case 8:
	                change_PhoneNumber();
	                break;
	            default:
	                System.out.println("Unknown command");
	                break;
	        }
	    }
		
		input.close();
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
	public static void disconnect()
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
	
	// 1번문제 SQL Select 문 수행
	public void executeSelect(boolean condFlag, String condition)
	{
		if (conn == null) return;
		try 
		{
			String strSQL = "select job_id, job_title, max_salary from jobs_copy where " +
					(condFlag ? condition:"");
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
					System.out.println("max_salary : " + rs.getInt(3));			
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
	
	// 3번문제 SQL Select 문 수행
	public void executeSelect_3(boolean condFlag, String condition)
	{
		if (conn == null) return;
		try 
		{
			String strSQL = "select " +
					(condFlag ? condition:"");
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
					System.out.println("country_name : " + rs.getString(1));		
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
	
	// 4번문제 SQL Select 문 수행
	public void executeSelect_4(boolean condFlag, String condition)
	{
		if (conn == null) return;
		try 
		{
			String strSQL = "select " +
					(condFlag ? condition:"");
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
					System.out.println("department_name : " + rs.getString(1));
					System.out.println("country_name : " + rs.getString(2));		
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

	// 4번문제 SQL Select 문 수행
	public static void executeSelect_5(boolean condFlag, String condition)
	{
		if (conn == null) return;
		try 
		{
			String strSQL = "select * from employees where " +
					(condFlag ? condition:"");
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
					System.out.println("employee_id : " + rs.getInt(1));
					System.out.println("first_name : " + rs.getString(2));
					System.out.println("last_name : " + rs.getString(3));
					System.out.println("email : " + rs.getString(4));
					System.out.println("phone_number : " + rs.getString(5));
					System.out.println("hire_date : " + rs.getString(6));
					System.out.println("job_id : " + rs.getString(7));
					System.out.println("salary : " + rs.getDouble(8));
					System.out.println("commission_pct : " + rs.getDouble(9));
					System.out.println("manager_id : " + rs.getInt(10));
					System.out.println("deparment_id : " + rs.getInt(11));
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
	
	// 1번 SQL Insert 문 수행
	public void executeInsert()
	{
		if (conn == null) return;
		try
		{
			String strSQL = "insert into jobs_copy values('CP_EN', 'Computer Engineer', 20000, 60000)";
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
	
	// 1번 SQL Delete 문 수행
	public void executeDelete()
	{
		if (conn == null) return;
		try
		{
			String strSQL = "delete from jobs_copy where job_title = 'Computer Engineer'";
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

	// SQL Create 문 수행
	public void executeCreate(String copy, String origin)
	{
		if (conn == null) return;
		try
		{
			String strSQL = "create table ";
			String strSQL2 = " as select * from ";
			strSQL = strSQL + copy + strSQL2 + origin;
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
		
	// SQL Update 문 수행
	public static void executeUpdate(String table, String condition)
	{
		if (conn == null) return;
		try
		{
			String strSQL = "update ";
			String strSQL2 = " set ";
			strSQL = strSQL + table + strSQL2 + condition;
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
	
	// 여기서부터 끝까지 5번 문제의 사원 관리 프로그램을 만드는 함수
	public static int Display()
	{
	    int num;
	    
	    System.out.println("### 사원관리 프로그램 ###");

	    System.out.println("### 사원 조회 기능 ###");
	    System.out.println("1. FIRST_NAME\n"
	    		+ "2. LAST_NAME\n"
	    		+ "3. 이름에 특정 문자가 포함된 사원\n"
	    		+ "4. 부서의 이름을 기준으로 부서 소속의 사원 조회\n"
	    		+ "5. 특정 부서에서 가장 적은 급여의 사원 3명과 가장 많은 급여의 3명 조회");
	    
	    System.out.println("### 사원 정보 수정 ###");
	    System.out.println("6. 사원의 SALARY 변경\n"
	    		+ "7. 사원의 부서 이동\n"
	    		+ "8. 사원의 핸드폰 번호 수정");

	    System.out.println("0. 종료");

	    num = input.nextInt();
	    return num;
	}

	static void print_First_Name()
	{
	    String cond = "FIRST_NAME = ";
	    String str;

	    System.out.println("FIRST_NAME을 입력하세요\n형식 : 'FIRST_NAME'");
	    str = input.next();
	    cond += str;
	    executeSelect_5(true, cond);
	}

	static void print_Last_Name()
	{
	    String cond = "LAST_NAME = ";
	    String str;
	    
	    System.out.println("LAST_NAME을 입력하세요\n형식 : 'LAST_NAME'");
	    str = input.next();
	    cond += str;
	    executeSelect_5(true, cond);    
	}

	static void print_Specific_Name()
	{
		int num;
	    String cond1 = "FIRST_NAME LIKE ", cond2 = "LAST_NAME LIKE ";
	    String str;

	    System.out.println("1.FIRST_NAME\n2.LAST_NAME");
	    num = input.nextInt();
	    if (num == 1)
	    {
	    	System.out.println("특정 문자가 들어가 있는 FIRST_NAME을 입력하세요\n형식 : '%FIRST_NAME%'");
	    	str = input.next();
		    cond1 += str;
		    executeSelect_5(true, cond1);
	    }
	    else if (num == 2)
	    {
	    	System.out.println("특정 문자가 들어가 있는 LAST_NAME을 입력하세요\n형식 : '%LAST_NAME%'");
	    	str = input.next();
		    cond2 += str;
		    executeSelect_5(true, cond2);
	    }
	    else
	    	System.out.println("1번 또는 2번을 입력해주세요");
	}

	static void print_Department_Name()
	{
	    String cond = "DEPARTMENT_ID = ";
	    String str;

	    System.out.println("DEPARTMENT_ID를 입력하세요\n형식 : DEPARTMENT_ID(int)");
	    str = input.next();
	    cond += str;
	    executeSelect_5(true, cond);
	}

	static void print_MinMax_Sal_3()
	{
	    String cond1 = "DEPARTMENT_ID = ", cond2;
	    String str;
	    String order_asc = " ORDER BY SALARY";
	    String order_desc = " ORDER BY SALARY DESC";
	    String three = " fetch first 3 rows only";

	    System.out.println("DEPARTMENT_ID를 입력하세요\n형식 : DEPARTMENT_ID(int)");
	    str = input.next();
	    cond1 += str;
	    cond2 = cond1;
	    cond1 += order_desc;
	    cond1 += three;
	    System.out.println("가장 적은 급여 3명");
	    executeSelect_5(true, cond1);

	    cond2 += order_asc;
	    cond2 += three;
	    System.out.println("가장 많은 급여 3명");
	    executeSelect_5(true, cond2);
	}

	static void change_Salary()
	{
	    String cond1 = "EMPLOYEE_ID = ", cond2 = "SALARY = ", cond3 = " where ";
	    String str1, str2;

	    System.out.println("EMPLOYEE_ID를 입력하세요\n형식 : EMPLOYEE_ID(int)");
	    str1 = input.next();
	    cond1 += str1;

	    System.out.println("변경할 SALARY를 입력하세요");
	    str2 = input.next();
	    cond2 += (str2 + cond3 + cond1);
	    
	    System.out.println("- 변경 전");
	    executeSelect_5(true, cond1);
	    
	    System.out.println("- 변경 후");
	    executeUpdate("employees", cond2);
	    executeSelect_5(true, cond1);
	    
	}

	static void change_Department()
	{
	    String cond1 = "EMPLOYEE_ID = ", cond2 = "department_id = ", cond3 = " where ";
	    String str1, str2;

	    System.out.println("EMPLOYEE_ID를 입력하세요\n형식 : EMPLOYEE_ID");
	    str1 = input.next();
	    cond1 += str1;

	    System.out.println("변경할 DEPARTMENT_ID를 입력하세요");
	    str2 = input.next();
	    cond2 += (str2 + cond3 + cond1);

	    System.out.println("- 변경 전");
	    executeSelect_5(true, cond1);

	    System.out.println("- 변경 후");
	    executeUpdate("employees", cond2);
	    executeSelect_5(true, cond1);
	}

	static void change_PhoneNumber()
	{
	    String cond1 = "EMPLOYEE_ID = ", cond2 = "phone_number = ", cond3 = " where ";
	    String str1, str2;

	    System.out.println("EMPLOYEE_ID를 입력하세요\n형식 : EMPLOYEE_ID");
	    str1 = input.next();
	    cond1 += str1;

	    System.out.println("변경할 PHONE_NUMBER를 입력하세요");
	    str2 = input.next();
	    cond2 += (str2 + cond3 + cond1);

	    System.out.println("- 변경 전");
	    executeSelect_5(true, cond1);

	    System.out.println("- 변경 후");
	    executeUpdate("employees", cond2);
	    executeSelect_5(true, cond1);
	}
}
