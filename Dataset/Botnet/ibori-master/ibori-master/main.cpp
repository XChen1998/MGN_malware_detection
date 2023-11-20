#include "packet.h"
#include "dbconn.h"

#include <QCoreApplication>
#include <stdio.h>
#include <pcap.h>

#include <boost/regex.hpp>
#include <iostream>
#include <string>
#include <string.h>
#include <time.h>

#define Debug 0

using namespace std;

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc != 2 ) {
        printf("ibory <interface>\n");  return(0);
    }

    char *dev = argv[1];
    printf("Device: %s\n", dev);


    // ============ 변수 선언 ============
    pcap_t* handle;
    timeval ts;
    const char *filter_exp = "tcp port 80";        /* Filter expression */
    char* tcp_segment;
    int packet_type = -1;
    const uint HTTP_REQUEST = 0;                /* Packet types */
    const uint FTP = 1;
    const uint TELNET = 2;

    match *mt = new match();
    match::DATA *data = &mt->data;                                                  // Data
    const uint DATA_SIZE = 1000;
    match::DATA *dataset = (match::DATA *)malloc(sizeof(match::DATA)*DATA_SIZE);    // Dataset
    memset(dataset, 0, sizeof(match::DATA)*DATA_SIZE);

    dbconn *dc = new dbconn();
    uint data_cnt = 0;
    time_t now_t, last_t = time(0);

    // ============ 1. Capture TCP packets ============
    packet *pk = new packet();
    if (!pk->capturePacket(&handle, dev, filter_exp)) return(2);

    int i_cnt = 0;
    while(true) {

        // ============ 2. Extract TCP segments and Select packets ============
        if (!pk->getTCPsegment(&tcp_segment, handle, &ts)) continue;

        if (!pk->filterPacket(&packet_type, tcp_segment, HTTP_REQUEST) &&
            !pk->filterPacket(&packet_type, tcp_segment, FTP) &&
            !pk->filterPacket(&packet_type, tcp_segment, TELNET)) {
            continue;
        }

        // ============ 3. Find matches of keywords from the segments ============
        memset(data, 0, sizeof(match::DATA));
        if (!mt->searchKeyword(&packet_type, tcp_segment, &data, ts)) continue;

        // ============ 4. Filter data out does not meet requirements ===============
        if ( data->url.empty() ||
             ( data->id.empty() && data->password.empty() && data->cookie.empty() )
           ) {
            continue;
        }

#if _Debug
        if ( data_cnt == 10 ) {
            for (int i=0; i<10; i++) {
                printf("\n[%d] url : %s\n", i, dataset[i].url.c_str());
                printf("[%d] id : %s\n", i, dataset[i].id.c_str());
                printf("[%d] passwd : %s\n", i, dataset[i].password.c_str());
                printf("[%d] cookie : %s\n", i, dataset[i].cookie.c_str());
                printf("[%d] date : %d\n", i, dataset[i].cdate);
            }
        }
#endif

        // ============ 5. Insert dataset to DB (every 10sec or 100 dataset is ready) ============
//        now_t = time(0);
//        if ( data_cnt < 5 && now_t - last_t < 10 ) {
            memcpy(&dataset[data_cnt++], data, sizeof(match::DATA));
//            continue;
//        }

        dc->insertData2DB(dataset, data_cnt);

        data_cnt = 0;
//        last_t = now_t;
        memset(dataset, 0, sizeof(match::DATA)*DATA_SIZE);  // Empty dataset.
    }

    /* And close the session */
    pcap_close(handle);

    delete mt, dc, pk;

    return a.exec();
    return(0);
}
