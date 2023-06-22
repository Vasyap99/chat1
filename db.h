#pragma once

#include <iostream>
#include <windows.h>
#include <sqlext.h>
#include <sqltypes.h>
#include <sql.h>

using namespace std;

class db_conn{
public:
    SQLLEN SQL_RESULT_LEN { 240};
    SQLLEN SQL_RETURN_CODE_LEN {1024};

    SQLHANDLE sqlConnHandle { nullptr }; // дескриптор для соединения с базой данных
    SQLHANDLE sqlEnvHandle  { nullptr }; // дескриптор окружения базы данных
    SQLHANDLE sqlStmtHandle{ nullptr };  // дескриптор для выполнения запросов к базе данных
    SQLCHAR retconstring[1024] {}; // строка для кода возврата из функций API ODBC


bool db_connect() {
	
	/*    constexpr auto SQL_RESULT_LEN = 240;
    constexpr auto SQL_RETURN_CODE_LEN = 1024;

    SQLHANDLE sqlConnHandle { nullptr }; // дескриптор для соединения с базой данных
    SQLHANDLE sqlEnvHandle  { nullptr }; // дескриптор окружения базы данных
    SQLHANDLE sqlStmtHandle{ nullptr };  // дескриптор для выполнения запросов к базе данных
    SQLCHAR retconstring[SQL_RETURN_CODE_LEN] {}; // строка для кода возврата из функций API ODBC*/
	

    // Выделяем дескриптор для базы данных
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
        return false;

   if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
        return false;

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
        return false;

    cout << "Attempting connection to SQL Server...\n";

    // Устанавливаем соединение с сервером  
    switch (  
	          SQLDriverConnect(
			    sqlConnHandle, 
        		GetDesktopWindow(), 
        		/*(SQLWCHAR*)*/(unsigned char*)"SERVER=localhost;PORT=5432;DATABASE=kk;UID=postgres;PWD=kostia24;",  //PostgreSQL ANSI ODBC Driver    DRIVER={PostgreSQL ANSI};
        		SQL_NTS,
        		retconstring,
        		1024,
        		NULL,
        		SQL_DRIVER_COMPLETE)
			) {

  	
    	case SQL_SUCCESS:
    	case SQL_SUCCESS_WITH_INFO:
        	cout << "Successfully connected to SQL Server\n";
        	break;

    	case SQL_INVALID_HANDLE:
    	case SQL_ERROR:
        	cout << "Could not connect to SQL Server\n";
        	return false;

    	default:
        	break;
    }

    // Если соединение не установлено, то выходим из программы
    return SQL_SUCCESS == SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle);
}

bool db_ex_query(string s){
	return SQL_SUCCESS == SQLExecDirect(sqlStmtHandle,(unsigned char*)s.c_str(), SQL_NTS);
}

    

/*
        //Объявление структуры данных для результата запроса версии SQL
        SQLCHAR sqlVersion[SQL_RESULT_LEN];
        SQLLEN ptrSqlVersion;

        while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlVersion, SQL_RESULT_LEN, &ptrSqlVersion);

            //Выведем на экран версию SQL
            cout << "\nQuery Result:\n\n";
            cout << sqlVersion << endl;
        }
    }
*/

    // Закрываем соединение и выходим из программы
void db_close(){
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);/**/  ////!!!!!!!!
}

};









int db_main() {

    constexpr auto SQL_RESULT_LEN = 240;
    constexpr auto SQL_RETURN_CODE_LEN = 1024;

    SQLHANDLE sqlConnHandle { nullptr }; // дескриптор для соединения с базой данных
    SQLHANDLE sqlEnvHandle  { nullptr }; // дескриптор окружения базы данных
    SQLHANDLE sqlStmtHandle{ nullptr };  // дескриптор для выполнения запросов к базе данных
   SQLCHAR retconstring[SQL_RETURN_CODE_LEN] {}; // строка для кода возврата из функций API ODBC


    // Выделяем дескриптор для базы данных
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlEnvHandle))
        goto COMPLETED;

   if (SQL_SUCCESS != SQLSetEnvAttr(sqlEnvHandle, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, 0))
       goto COMPLETED;

    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlEnvHandle, &sqlConnHandle))
        goto COMPLETED;

    cout << "Attempting connection to SQL Server...\n";

    // Устанавливаем соединение с сервером  
    switch (  
	          SQLDriverConnect(
			    sqlConnHandle, 
        		GetDesktopWindow(), 
        		/*(SQLWCHAR*)*/(unsigned char*)"SERVER=localhost;PORT=5432;DATABASE=kk;UID=postgres;PWD=kostia24;",  //PostgreSQL ANSI ODBC Driver    DRIVER={PostgreSQL ANSI};
        		SQL_NTS,
        		retconstring,
        		1024,
        		NULL,
        		SQL_DRIVER_COMPLETE)
			) {

  	
    case SQL_SUCCESS:
    case SQL_SUCCESS_WITH_INFO:
        cout << "Successfully connected to SQL Server\n";
        break;

    case SQL_INVALID_HANDLE:
    case SQL_ERROR:
        cout << "Could not connect to SQL Server\n";
        goto COMPLETED;

    default:
        break;
    }

    // Если соединение не установлено, то выходим из программы
    if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnHandle, &sqlStmtHandle))
        goto COMPLETED;

    cout << "\nExecuting T-SQL query...\n";

    // Если выполнение запроса с ошибками, то выходим из программы
    if (SQL_SUCCESS != SQLExecDirect(sqlStmtHandle, /*(SQLWCHAR*)L*/(unsigned char*)"SELECT now()", SQL_NTS)) {
        cout << "Error querying SQL Server \n";
        goto COMPLETED;
    }
    else {

        //Объявление структуры данных для результата запроса версии SQL
        SQLCHAR sqlVersion[SQL_RESULT_LEN];//
        SQLLEN ptrSqlVersion;

        while (SQLFetch(sqlStmtHandle) == SQL_SUCCESS) {

            SQLGetData(sqlStmtHandle, 1, SQL_CHAR, sqlVersion, SQL_RESULT_LEN, &ptrSqlVersion);

            //Выведем на экран версию SQL
            cout << "\nQuery Result:\n\n";
            cout << sqlVersion << endl;
        }
    }

    // Закрываем соединение и выходим из программы
COMPLETED:
    SQLFreeHandle(SQL_HANDLE_STMT, sqlStmtHandle);
    SQLDisconnect(sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_DBC, sqlConnHandle);
    SQLFreeHandle(SQL_HANDLE_ENV, sqlEnvHandle);/**/  ////!!!!!!!!

    // Пауза перед закрытием консольного приложения
    cout << "\nPress any key to exit...";
    getchar();
}



