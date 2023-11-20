#include <pcap.h>

class packet {

public:

    packet();
    ~packet();

    bool capturePacket(pcap_t **handle, char *dev, const char *filter_exp);

    bool getTCPsegment(char **tcp_segment, pcap_t *handle, timeval *ts);

    bool filterPacket(int *packet_type, char *tcp_segment, const uint FILTER_NAME);

private:

};
