#include <Windows.h>
#include <iostream>
#include <string>
#include <limits>
#include <sql.h>
#include <sqlext.h>
using namespace std;


//ODBC conn
SQLHENV hEnv;
SQLHDBC hDbc;
SQLHSTMT hStmt;

//접속 정보
SQLCHAR* ODBC_Name = (SQLCHAR*)"HR";  // ODBC 연결 이름
SQLCHAR* ODBC_ID = (SQLCHAR*)"HR";    // 사용자 이름
SQLCHAR* ODBC_PW = (SQLCHAR*)"1234"; // 사용자 암호

bool DBConnect();
void DBDisconnect();
bool DBExecuteSQL();
bool DBExecuteSQL(SQLCHAR* query);
bool DBPrepare(SQLCHAR* query_for_statement);


void PrintResult2() // 2번 문제를 구하는 SELECT 문 결과를 접근하는 방법
{
    SQLCHAR job_id[11];
    SQLCHAR job_title[36];
    int max_salary;

    SQLLEN i_job_id, i_job_title, i_max_salary;

    // select 질의 수행 결과를 저장할 변수를 지정한다. (Column number는 1부터 시작한다)
    SQLBindCol(hStmt, 1, SQL_CHAR, &job_id, sizeof(job_id), &i_job_id);
    SQLBindCol(hStmt, 2, SQL_CHAR, &job_title, sizeof(job_title), &i_job_title);
    SQLBindCol(hStmt, 3, SQL_INTEGER, &max_salary, sizeof(max_salary), &i_max_salary);

    // 질의 수행 결과를 hStmt로 가져온다. 이때, 바인딩된 변수에 값이 할당된다.
    if (SQLFetch(hStmt) == SQL_NO_DATA)
        cout << "No tuples in the result" << endl;
    else {
        do {
            cout << job_id << ' ' << job_title << ' ' << max_salary << endl;
        } while (SQLFetch(hStmt) != SQL_NO_DATA);
    }

    // hStmt 커서 해제
    if (hStmt)
        SQLCloseCursor(hStmt);
}

void PrintResult3() // 3번 문제를 구하는 SELECT 문 결과를 접근하는 방법
{
    SQLCHAR country_name[61];

    SQLLEN i_country_name;

    // select 질의 수행 결과를 저장할 변수를 지정한다. (Column number는 1부터 시작한다)
    SQLBindCol(hStmt, 1, SQL_CHAR, &country_name, sizeof(country_name), &i_country_name);
    
    // 질의 수행 결과를 hStmt로 가져온다. 이때, 바인딩된 변수에 값이 할당된다.
    if (SQLFetch(hStmt) == SQL_NO_DATA)
        cout << "No tuples in the result" << endl;
    else {
        do {
            cout << country_name << endl;
        } while (SQLFetch(hStmt) != SQL_NO_DATA);
    }

    // hStmt 커서 해제
    if (hStmt)
        SQLCloseCursor(hStmt);
}

void PrintResult4() // 4번 문제를 구하는 SELECT 문 결과를 접근하는 방법
{
    SQLCHAR department_name[31];
    SQLCHAR country_name[61];

    SQLLEN i_department_name, i_country_name;

    // select 질의 수행 결과를 저장할 변수를 지정한다. (Column number는 1부터 시작한다)
    SQLBindCol(hStmt, 1, SQL_CHAR, &department_name, sizeof(department_name), &i_department_name);
    SQLBindCol(hStmt, 2, SQL_CHAR, &country_name, sizeof(country_name), &i_country_name);

    // 질의 수행 결과를 hStmt로 가져온다. 이때, 바인딩된 변수에 값이 할당된다.
    if (SQLFetch(hStmt) == SQL_NO_DATA)
        cout << "No tuples in the result" << endl;
    else {
        do {
            cout << department_name << ' ' << country_name << endl;
        } while (SQLFetch(hStmt) != SQL_NO_DATA);
    }

    // hStmt 커서 해제
    if (hStmt)
        SQLCloseCursor(hStmt);
}

void PrintResult5() // EMPLOYEES 테이블의 모든 어트리뷰트를 구하는 SELECT 문 결과를 접근하는 방법
{
    int employee_id;
    int manager_id;
    int department_id;
    double salary;
    double commission_pct;
    SQLCHAR first_name[21];
    SQLCHAR last_name[26];
    SQLCHAR email[26];
    SQLCHAR phone_number[21];
    SQLCHAR hire_date[40];
    SQLCHAR job_id[11];

    SQLLEN i_employee_id, i_manager_id, i_department_id, i_salary, i_commission_pct,
        i_first_name, i_last_name, i_email, i_phone_number, i_hire_date, i_job_id;

    // select 질의 수행 결과를 저장할 변수를 지정한다. (Column number는 1부터 시작한다)
    SQLBindCol(hStmt, 1, SQL_INTEGER, &employee_id, sizeof(employee_id), &i_employee_id);
    SQLBindCol(hStmt, 2, SQL_CHAR, &first_name, sizeof(first_name), &i_first_name);
    SQLBindCol(hStmt, 3, SQL_CHAR, &last_name, sizeof(last_name), &i_last_name);
    SQLBindCol(hStmt, 4, SQL_CHAR, &email, sizeof(email), &i_email);
    SQLBindCol(hStmt, 5, SQL_CHAR, &phone_number, sizeof(phone_number), &i_phone_number);
    SQLBindCol(hStmt, 6, SQL_CHAR, &hire_date, sizeof(hire_date), &i_hire_date);
    SQLBindCol(hStmt, 7, SQL_CHAR, &job_id, sizeof(job_id), &i_job_id);
    SQLBindCol(hStmt, 8, SQL_DOUBLE, &salary, sizeof(salary), &i_salary);
    SQLBindCol(hStmt, 9, SQL_DOUBLE, &commission_pct, sizeof(commission_pct), &i_commission_pct);
    SQLBindCol(hStmt, 10, SQL_INTEGER, &manager_id, sizeof(manager_id), &i_manager_id);
    SQLBindCol(hStmt, 11, SQL_INTEGER, &department_id, sizeof(department_id), &i_department_id);

    // 질의 수행 결과를 hStmt로 가져온다. 이때, 바인딩된 변수에 값이 할당된다.
    if (SQLFetch(hStmt) == SQL_NO_DATA)
        cout << "No tuples in the result" << endl;
    else {
        do {
            cout << employee_id << ' ' << first_name << ' ' << last_name << ' ' <<
                email << ' ' << phone_number << ' ' << hire_date << ' ' << job_id << ' ' <<
                salary << ' ' << commission_pct << ' ' << manager_id << ' ' << department_id << endl;
        } while (SQLFetch(hStmt) != SQL_NO_DATA);
    }

    // hStmt 커서 해제
    if (hStmt)
        SQLCloseCursor(hStmt);
}

void QueryJobs_copy(const char* condStr = NULL) // JOBS_COPY 테이블에 대한 질의 수행 (cond: where 절의 조건식)
{
    char sqlStrBuf[100];

    strcpy_s(sqlStrBuf, sizeof(sqlStrBuf), "SELECT JOB_ID, JOB_TITLE, MAX_SALARY FROM JOBS_COPY");
    if (condStr) {
        strcat_s(sqlStrBuf, sizeof(sqlStrBuf), " WHERE ");
        strcat_s(sqlStrBuf, sizeof(sqlStrBuf), condStr);
    }

    if (DBExecuteSQL((SQLCHAR*)sqlStrBuf) == false) {
        cout << "질의 에러" << endl;
        return;
    }

    PrintResult2();
}

void QueryEMP(const char* condStr = NULL) // EMPLOYEES 테이블에 대한 질의 수행 (cond: where 절의 조건식)
{
    char sqlStrBuf[100];

    strcpy_s(sqlStrBuf, sizeof(sqlStrBuf), "SELECT * FROM EMPLOYEES");
    if (condStr) {
        strcat_s(sqlStrBuf, sizeof(sqlStrBuf), " WHERE ");
        strcat_s(sqlStrBuf, sizeof(sqlStrBuf), condStr);
    }

    if (DBExecuteSQL((SQLCHAR*)sqlStrBuf) == false) {
        cout << "질의 에러" << endl;
        return;
    }

    cout << endl;
    PrintResult5();
}

void QueryEMP2(const char* condStr = NULL, const char* condStr2 = NULL) // EMPLOYEES 테이블에 대한 질의 수행 (cond: where 절의 조건식)
{
    char sqlStrBuf[100];

    strcpy_s(sqlStrBuf, sizeof(sqlStrBuf), "UPDATE EMPLOYEES SET ");
    if (condStr) {
        strcat_s(sqlStrBuf, sizeof(sqlStrBuf), condStr2);
        strcat_s(sqlStrBuf, sizeof(sqlStrBuf), " WHERE ");
        strcat_s(sqlStrBuf, sizeof(sqlStrBuf), condStr);
    }

    if (DBExecuteSQL((SQLCHAR*)sqlStrBuf) == false) {
        cout << "질의 에러" << endl;
        return;
    }
}

int Display()
{
    int num;

    cout << endl << "### 사원관리 프로그램 ###" << endl;

    cout << endl << "### 사원 조회 기능 ###" << endl;
    cout << endl << "1. FIRST_NAME\n\
2. LAST_NAME\n\
3. 이름에 특정 문자가 포함된 사원\n\
4. 부서의 이름을 기준으로 부서 소속의 사원 조회\n\
5. 특정 부서에서 가장 적은 급여의 사원 3명과 가장 많은 급여의 3명 조회" << endl;

    cout << endl << "### 사원 정보 수정 ###" << endl;
    cout << endl << "6. 사원의 SALARY 변경\n\
7. 사원의 부서 이동\n\
8. 사원의 핸드폰 번호 수정" << endl;

    cout << endl << "0. 종료" << endl;

    cin >> num;
    cin.ignore();

    return num;
}

void print_First_Name()
{
    string cond = "FIRST_NAME = ";
    string str;

    cout << endl << "FIRST_NAME을 입력하세요\n형식 : 'FIRST_NAME'" << endl;
    getline(cin, str);
    cond = cond.append(str);
    QueryEMP(cond.c_str());
}

void print_Last_Name()
{
    string cond = "LAST_NAME = ";
    string str;

    cout << endl << "LAST_NAME을 입력하세요\n형식 : 'LAST_NAME'" << endl;
    getline(cin, str);
    cond = cond.append(str);
    QueryEMP(cond.c_str());
}

void print_Specific_Name()
{
    int num;
    string cond1 = "FIRST_NAME LIKE ", cond2 = "LAST_NAME LIKE ";
    string str;

    cout << endl << "1.FIRST_NAME\n2.LAST_NAME" << endl;
    cin >> num;
    cin.ignore();
    if (num == 1)
    {
        cout << endl << "특정 문자가 들어가 있는 FIRST_NAME을 입력하세요\n형식 : '%FIRST_NAME%'" << endl;
        getline(cin, str);
        cond1 = cond1.append(str);
        QueryEMP(cond1.c_str());
    }
    else if (num == 2)
    {
        cout << endl << "특정 문자가 들어가 있는 LAST_NAME을 입력하세요\n형식 : '%LAST_NAME%'" << endl;
        getline(cin, str);
        cond2 = cond2.append(str);
        QueryEMP(cond2.c_str());
    }
    else
        cout << endl << "1번 또는 2번을 입력해주세요" << endl;
}

void print_Department_Name()
{
    string cond = "DEPARTMENT_ID = ";
    string str;

    cout << endl << "DEPARTMENT_ID를 입력하세요\n형식 : DEPARTMENT_ID(int)" << endl;
    getline(cin, str);
    cond = cond.append(str);
    QueryEMP(cond.c_str());
}

void print_MinMax_Sal_3()
{
    string cond1 = "DEPARTMENT_ID = ", cond2;
    string str;
    string order_asc = " ORDER BY SALARY";
    string order_desc = " ORDER BY SALARY DESC";
    string three = " fetch first 3 rows only";

    cout << endl << "DEPARTMENT_ID를 입력하세요\n형식 : DEPARTMENT_ID(int)" << endl;
    getline(cin, str);
    cond1 = cond1.append(str);
    cond2 = cond1;
    cond1 = cond1.append(order_desc);
    cond1 = cond1.append(three);
    cout << endl << "가장 적은 급여 3명" << endl;
    QueryEMP(cond1.c_str());

    cond2 = cond2.append(order_asc);
    cond2 = cond2.append(three);
    cout << endl << "가장 많은 급여 3명" << endl;
    QueryEMP(cond2.c_str());
}

void change_Salary()
{
    string cond1 = "EMPLOYEE_ID = ", cond2 = "SALARY = ";
    string str1, str2;

    cout << endl << "EMPLOYEE_ID를 입력하세요\n형식 : EMPLOYEE_ID(int)" << endl;
    getline(cin, str1);
    cond1 = cond1.append(str1);

    cout << endl << "변경할 SALARY를 입력하세요" << endl;
    getline(cin, str2);
    cond2 = cond2.append(str2);

    cout << endl << "- 변경 전" << endl;
    QueryEMP(cond1.c_str());
    
    cout << endl << "- 변경 후" << endl;
    QueryEMP2(cond1.c_str(), cond2.c_str());
    QueryEMP(cond1.c_str());
    
}

void change_Department()
{
    string cond1 = "EMPLOYEE_ID = ", cond2 = "department_id = ";
    string str1, str2;

    cout << endl << "EMPLOYEE_ID를 입력하세요\n형식 : EMPLOYEE_ID" << endl;
    getline(cin, str1);
    cond1 = cond1.append(str1);

    cout << endl << "변경할 DEPARTMENT_ID를 입력하세요" << endl;
    getline(cin, str2);
    cond2 = cond2.append(str2);

    cout << endl << "- 변경 전" << endl;
    QueryEMP(cond1.c_str());

    cout << endl << "- 변경 후" << endl;
    QueryEMP2(cond1.c_str(), cond2.c_str());
    QueryEMP(cond1.c_str());
}

void change_PhoneNumber()
{
    string cond1 = "EMPLOYEE_ID = ", cond2 = "phone_number = ";
    string str1, str2;

    cout << endl << "EMPLOYEE_ID를 입력하세요\n형식 : EMPLOYEE_ID" << endl;
    getline(cin, str1);
    cond1 = cond1.append(str1);

    cout << endl << "변경할 PHONE_NUMBER를 입력하세요" << endl;
    getline(cin, str2);
    cond2 = cond2.append(str2);

    cout << endl << "- 변경 전" << endl;
    QueryEMP(cond1.c_str());

    cout << endl << "- 변경 후" << endl;
    QueryEMP2(cond1.c_str(), cond2.c_str());
    QueryEMP(cond1.c_str());
}


int main()
{
    bool done = false;

    if (DBConnect() == false)
    {
        cout << "접속 에러" << endl;
        return -1;
    }

    cout << endl << "### 1. Insert, Delete, Update 질의 실습 ###" << endl;
    cout << endl << "### Jobs테이블을 복사하여 Jobs_copy테이블을 생성하시오 ###" << endl;
    DBExecuteSQL((SQLCHAR*)"create table jobs_copy as select * from jobs");
    
    cout << endl << "### 직업 테이블 Jobs_copy 에 새로운 직업 Computer Engineer에 관한 정보를 INSERT하는 코드를 공부하고, 수행하여 결과를 확인하시오 ###" << endl;
    QueryJobs_copy("job_id = 'CP_EN'");
    DBExecuteSQL((SQLCHAR*)"insert into jobs_copy values('CP_EN', 'Computer Engineering', 20000, 60000)");
    QueryJobs_copy("job_id = 'CP_EN'");

    cout << endl << "### 동일한 방법으로 직업 테이블 Jobs_copy안의 Computer Engineer의 직업을 DELETE하는 코드를 공부하고, 수행하여 결과를 확인하시오 ### " << endl;
    DBExecuteSQL((SQLCHAR*)"delete from jobs_copy where job_id='CP_EN'");
    QueryJobs_copy("job_id = 'CP_EN'");
    
    cout << endl << "### Jobs_copy테이블에서 JOB_TITLE에 ‘Marketing’글자가 있는 직업의 MAX_SALARY를 500증가시키기 위한 질의를 작성하고, 그 결과를 확인하시오 ###" << endl;
    QueryJobs_copy("job_title like 'Marketing%'");
    DBExecuteSQL((SQLCHAR*)"update jobs_copy set max_salary = max_salary + 500 where job_title like 'Marketing%'");
    QueryJobs_copy("job_title like 'Marketing%'");

    
    cout << endl << "### 2. 아래 2-4번에서 요구되는 테이블을 식별하고, 각 테이블을 복사한 테이블을 생성하시오 ###" << endl;
    DBExecuteSQL((SQLCHAR*)"create table e_copy as select * from employees");
    DBExecuteSQL((SQLCHAR*)"create table c_copy as select * from countries");
    DBExecuteSQL((SQLCHAR*)"create table d_copy as select * from departments");
    DBExecuteSQL((SQLCHAR*)"create table l_copy as select * from locations");


    cout << endl << "### 3. 각 나라에 속한 사원들의 급여의 총 합을 기준으로 나라 이름을 정렬하여 출력하시오 ###" << endl;
    DBExecuteSQL((SQLCHAR*)"select country_name from c_copy c \
        join l_copy l on c.country_id = l.country_id \
        join d_copy d on l.location_id = d.location_id \
        join e_copy e on d.department_id = e.department_id \
        group by country_name \
        order by sum(e.salary)");
    PrintResult3();


    cout << endl << "### 4. 급여(SALARY)가 10,000이상인 사원들의 종합 급여가 가장 높은 부서 2곳의 부서 이름 (DEPARTMENT_NAME)과 속한 나라의 이름(CONTRY_NAME)을 출력하시오 ###" << endl;
    DBExecuteSQL((SQLCHAR*)"select d.department_name, c.country_name from d_copy d \
        join l_copy l on d.location_id = l.location_id \
        join c_copy c on l.country_id = c.country_id \
        join e_copy e on d.department_id = e.department_id \
        where e.salary >= 10000 \
        group by d.department_name, c.country_name \
        order by sum(e.salary) desc \
        fetch first 2 rows only");
    PrintResult4();
    
    cout << endl << "### 5. ODBC를 이용한 사원 관리 프로그램을 작성하시오 ###" << endl;
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
                cerr << "Unknown command" << endl;
                break;
        }
    }

    DBDisconnect();

    return 0;
}

bool DBConnect() //데이터베이스 연결
{
    if (SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &hEnv) != SQL_SUCCESS)
        return false;
    if (SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER) != SQL_SUCCESS)
        return false;
    if (SQLAllocHandle(SQL_HANDLE_DBC, hEnv, &hDbc) != SQL_SUCCESS)
        return false;
    if (SQLConnectA(hDbc, ODBC_Name, SQL_NTS, ODBC_ID, SQL_NTS, ODBC_PW, SQL_NTS) != SQL_SUCCESS)
        return false;
    if (SQLAllocHandle(SQL_HANDLE_STMT, hDbc, &hStmt) != SQL_SUCCESS)
        return false;
    return true;
}
void DBDisconnect() //데이터베이스 연결 해제
{
    if (hStmt)
        SQLFreeHandle(SQL_HANDLE_STMT, hStmt);
    if (hDbc)
        SQLDisconnect(hDbc);
    if (hDbc)
        SQLFreeHandle(SQL_HANDLE_DBC, hDbc);
    if (hEnv)
        SQLFreeHandle(SQL_HANDLE_ENV, hEnv);
}
bool DBExecuteSQL()
{
    if (SQLExecute(hStmt) != SQL_SUCCESS)
        return false;
    return true;
}
bool DBPrepare(SQLCHAR* query_for_statement)
{
    if (SQLPrepareA(hStmt, query_for_statement, SQL_NTS) != SQL_SUCCESS)
        return false;
    return true;
}

bool DBExecuteSQL(SQLCHAR* query) //질의 수행
{
    if (SQLExecDirectA(hStmt, query, SQL_NTS) != SQL_SUCCESS)
        return false;
    return true;
}

