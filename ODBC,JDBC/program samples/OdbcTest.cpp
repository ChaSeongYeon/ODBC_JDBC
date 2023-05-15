#include <Windows.h>
#include <iostream>
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
SQLCHAR* ODBC_PW = (SQLCHAR*)"tiger"; // 사용자 암호

bool DBConnect();
void DBDisconnect();
bool DBExecuteSQL();
bool DBExecuteSQL(SQLCHAR* query);
bool DBPrepare(SQLCHAR* query_for_statement);


void PrintResult() // JOBS 테이블의 JOB_ID, JOB_TITLE, MAX_SALARY 를 구하는 SELECT 문 결과를 접근하는 방법
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

void QueryJobs(const char* condStr = NULL) // JOBS 테이블에 대한 질의 수행 (cond: where 절의 조건식)
{
    char sqlStrBuf[100];

    strcpy_s(sqlStrBuf, sizeof(sqlStrBuf), "SELECT JOB_ID, JOB_TITLE, MAX_SALARY FROM JOBS");
    if (condStr) {
        strcat_s(sqlStrBuf, sizeof(sqlStrBuf), " WHERE ");
        strcat_s(sqlStrBuf, sizeof(sqlStrBuf), condStr);
    }

    if (DBExecuteSQL((SQLCHAR*)sqlStrBuf) == false) {
        cout << "질의 에러" << endl;
        return;
    }

    PrintResult();
}

bool InsertJob(SQLCHAR* job_id, SQLCHAR* job_name, SQLINTEGER min_salary, SQLINTEGER max_salary)
{
    SQLBindParameter(hStmt, 1, SQL_PARAM_INPUT, SQL_CHAR, SQL_CHAR, 0, 0, job_id, 0, NULL);
    SQLBindParameter(hStmt, 2, SQL_PARAM_INPUT, SQL_CHAR, SQL_CHAR, 0, 0, job_name, 0, NULL);
    SQLBindParameter(hStmt, 3, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &min_salary, 0, NULL);
    SQLBindParameter(hStmt, 4, SQL_PARAM_INPUT, SQL_INTEGER, SQL_INTEGER, 0, 0, &max_salary, 0, NULL);

    if (DBExecuteSQL())
        return true;
    return false;
}

void InsertTest() //Insert질의 테스트
{
    DBPrepare((SQLCHAR*)"insert into jobs values(?,?,?,?)");

    InsertJob((SQLCHAR*)"CP_EN", (SQLCHAR*)"Computer Engineer", 20000, 60000);
    InsertJob((SQLCHAR*)"AT_PT", (SQLCHAR*)"Artist Painter", 10000, 80000);
}


int main()
{
    if (DBConnect() == false)
    {
        cout << "접속 에러" << endl;
        return -1;
    }

    cout << "### JOBS 테이블 전체 출력 ###" << endl;
    QueryJobs();
    
    cout << endl << "### CP_EN 삽입 전 JOBS 테이블 CP_EN 검색 ###" << endl;
    QueryJobs("job_id = 'CP_EN'");

    DBExecuteSQL((SQLCHAR*)"insert into jobs values('CP_EN', 'Computer Engineering', 20000, 60000)");

    cout << endl << "### CP_EN 삽입 후 JOBS 테이블 CP_EN 검색 ###" << endl;
    QueryJobs("job_id = 'CP_EN'");

    DBExecuteSQL((SQLCHAR*)"delete from jobs where job_id='CP_EN'");

    cout << endl << "### CP_EN 삭제 후 JOBS 테이블 CP_EN 검색 ###" << endl;
    QueryJobs("job_id = 'CP_EN'");

    InsertTest(); // Prepare를 이용한 CP_EN, AT_PT 삽입
    cout << endl << "### Prepare를 이용한 CP_EN, AT_PT 삽입 후 JOBS 테이블 CP_EN, AT_PT 검색 ###" << endl;
    QueryJobs("job_id in ('CP_EN', 'AT_PT')");

    DBExecuteSQL((SQLCHAR*)"delete from jobs where job_id in ('CP_EN', 'AT_PT')");

    cout << endl << "### CP_EN, AT_PT 삭제 후 JOBS 테이블 CP_EN, AT_PT 검색 ###" << endl;
    QueryJobs("job_id in('CP_EN', 'AT_PT')");
    
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

