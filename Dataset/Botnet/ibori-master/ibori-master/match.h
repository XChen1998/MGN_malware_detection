#include <string>
#include <time.h>
#include <pcap.h>

class match {

public:

    struct DATA {
        std::string url;
        std::string id;
        std::string password;
        std::string cookie;
        time_t cdate;
    } data;

    match();
    ~match();

    bool searchKeyword(int *packet_type, char* tcp_segment, DATA **data, timeval ts);

private:

};
