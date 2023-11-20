#include "match.h"

class dbconn {

public:

    dbconn();
    ~dbconn();

    bool insertData2DB(match::DATA *dataset, uint data_cnt);

    void getStringDate(time_t cdate, char* text);

private:

    int genSerialNumber(int *num_cnt, int *data_cnt, time_t cdate);
};
