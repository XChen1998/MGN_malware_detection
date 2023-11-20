#include "match.h"
#include <stdio.h>
#include <boost/regex.hpp>

// On/Off Switch For Dubugging
#define _Debug 1

// ======================= 전역 변수 =========================
boost::smatch m;                        /* Matches */
boost::regex URL( ".*Host\: ([^\\r]*).*" );                                                 // m[1]
boost::regex Cookie( ".*Cookie\: ([^\\r]*).*" );                                            // m[1]
boost::regex User_ID( ".*(?:\\_|\\&|^)(?:M_ID|mb_id|id|Id|userId|user_id|login|pUserLoginId)\=([^\\&]*).*" );          // m[1]
boost::regex User_ID2( ".*\<user_id\>\<\!\\[\\w+\\[(\.+)\\]\\]\>\<\/user_id\>.*" );        // m[1]
boost::regex User_ID3( ".*(?:user|username)\=([^\\&]*).*" );                              // m[1]
boost::regex Password( ".*(?:\\&pw|pwd|pPw|M_PWD|mb_password|pass_wd|password|Password)\=([^\\r\\&\\;\\s]*).*" );    // m[1]
boost::regex Password2( ".*\<password\>\<\!\\[\\w+\\[(\.+)\\]\\]\>\<\/password\>.*" );     // m[1]

const int HTTP_REQUEST = 0;            /* Packet types */
const int FTP = 1;
const int TELNET = 2;


match::match(){
    memset(&data, 0, sizeof(match::DATA));
}

match::~match(){}


bool match::searchKeyword(int *packet_type, char* tcp_segment, DATA **data, timeval ts) {

    std::string text = tcp_segment;
//    printf("---------tcp_segment : %s\n", text.c_str());

    bool result = false;

    switch ( *packet_type ) {
    case HTTP_REQUEST :
        if (boost::regex_match(text, m, URL)) {
            (*data)->url = m[1];
#if _Debug
            printf("# URL : %s\n", (*data)->url.c_str());
#endif
            result = true;
        }

        if (boost::regex_match(text, m, Cookie)) {
            (*data)->cookie = m[1];
#if _Debug
            printf("# Cookie : %s\n", (*data)->cookie.c_str());
#endif
            result = true;
        }

        if (boost::regex_match(text, m, User_ID)) {
            (*data)->id = m[1];
#if _Debug
            printf("# User_ID : %s\n", (*data)->id.c_str());
#endif
            result = true;
        }


        if (boost::regex_match(text, m, User_ID2)) {
            (*data)->id = m[1];
#if _Debug
            printf("# User_ID2 : %s\n", (*data)->id.c_str());
#endif
            result = true;
        }

        if (boost::regex_match(text, m, User_ID3)) {
            (*data)->id = m[1];
#if _Debug
            printf("# User_ID3 : %s\n", (*data)->id.c_str());
#endif
            result = true;
        }


        if (boost::regex_match(text, m, Password)) {
            (*data)->password = m[1];
#if _Debug
            printf("# Password : %s\n", (*data)->password .c_str());
#endif
            result = true;
        }


        if (boost::regex_match(text, m, Password2)) {
            (*data)->password = m[1];
#if _Debug
            printf("# Password2 : %s\n", (*data)->password .c_str());
#endif
            result = true;
        }

        if ( ts.tv_sec != 0 ) {
            (*data)->cdate = ts.tv_sec;
        }

        break;

    case FTP :

        break;

    case TELNET :

        break;

    default :
        return true;
    }
}
