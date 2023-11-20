/* Standard C++ includes */
#include <iostream>
#include <stdlib.h>
#include <string.h>

#include "mysql_connection.h"
#include "dbconn.h"

#include <cppconn/driver.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

#define Debug 0

using namespace std;

dbconn::dbconn(){}
dbconn::~dbconn(){}


bool dbconn::insertData2DB(match::DATA *dataset, uint data_cnt) {

    try {
        sql::Driver *driver;
        sql::Connection *con;
        sql::PreparedStatement *pstmt1, *pstmt2;

        /* Create a connection */
        driver = get_driver_instance();
        con = driver->connect("localhost:3306", "root", "toor");

        /* Connect to the MySQL ibory database */
        con->setSchema("ibory");

        pstmt1 = con->prepareStatement("INSERT INTO ibory.getpasswd (url, id, passwd, cdate) VALUES (?, ?, ?, ?)");

        for (int i=0; i<data_cnt; i++) {

#if Debug
            printf("\n dataset[%d].url : %s, empty() : %d", i, dataset[i].url.c_str(), dataset[i].url.empty());
            printf("\n dataset[%d].id : %s, empty() : %d", i, dataset[i].id.c_str(), dataset[i].id.empty());
            printf("\n dataset[%d].password : %s, empty() : %d\n", i, dataset[i].password.c_str(), dataset[i].password.empty());
#endif

            if ( dataset[i].url.empty() || dataset[i].id.empty() || dataset[i].password.empty() ) continue;

                pstmt1->setString(1, dataset[i].url);       // url
                pstmt1->setString(2, dataset[i].id);        // id
                pstmt1->setString(3, dataset[i].password);  // passwd

            if ( dataset[i].cdate != 0 ) {
                char *text = (char*)malloc(sizeof(char)*20);
                getStringDate(dataset[i].cdate, text);
                pstmt1->setDateTime(4, text);           // date
            } else {
                char *text = (char*)malloc(sizeof(char)*20);
                timeval tv;
                getStringDate(tv.tv_sec, text);
                pstmt1->setDateTime(4, text);
            }
            pstmt1->executeUpdate();
        }


        pstmt2 = con->prepareStatement("INSERT INTO ibory.getcookie (url, cookie, cdate) VALUES (?, ?, ?)");

        for (int i=0; i<data_cnt; i++) {

#if Debug
            printf("\n dataset[%d].url : %s, empty() : %d", i, dataset[i].url.c_str(), dataset[i].url.empty());
            printf("\n dataset[%d].cookie : %s, empty() : %d", i, dataset[i].cookie.c_str(), dataset[i].cookie.empty());
#endif

            if ( dataset[i].url.empty() || dataset[i].cookie.empty() ) continue;

            pstmt2->setString(1, dataset[i].url);     // url
            pstmt2->setString(2, dataset[i].cookie);  // cookie

            if ( dataset[i].cdate != 0 ) {
                char *text = (char*)malloc(sizeof(char)*20);
                getStringDate(dataset[i].cdate, text);
                pstmt2->setDateTime(3, text);        // date
            } else {
                char *text = (char*)malloc(sizeof(char)*20);
                timeval tv;
                getStringDate(tv.tv_sec, text);
                pstmt2->setDateTime(3, text);
            }
            pstmt2->executeUpdate();
        }
        delete pstmt1, pstmt2;
        delete con;

    } catch (sql::SQLException &e) {
        printf("# ERR: SQLException in %s ( %s ) on line %d\n", __FILE__, __FUNCTION__, __LINE__);
        printf("# ERR: $s %s\n", e.what());
        printf("(MySQL error code: $s, SQLState: %s )\n", e.getErrorCode(), e.getSQLState());
    }
}

void dbconn::getStringDate(time_t cdate, char* text) {
    strftime(text, 20, "%Y-%m-%d %H:%M:%S", localtime(&cdate));
}

int dbconn::genSerialNumber(int *num_cnt, int *data_cnt, time_t cdate) {
    tm *t = localtime(&cdate);
    char* num, buffer;

    sprintf(&buffer, "%d", t->tm_year-100);
    num += buffer;

    sprintf(&buffer, "%d", t->tm_mon+1);
    num += buffer;

    sprintf(&buffer, "%d", t->tm_mday);
    num += buffer;

    sprintf(&buffer, "%d", t->tm_hour);
    num += buffer;

    sprintf(&buffer, "%d", t->tm_min);
    num += buffer;

    sprintf(&buffer, "%d", t->tm_sec);
    num += buffer;

    return atoi(num);
}
