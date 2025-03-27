#ifndef __EXTUDP_OMNET_UDPCLIENT_H_
#define __EXTUDP_OMNET_UDPCLIENT_H_

#include <omnetpp.h>
#include "UDPBasicApp.h"

class UDPClient : public UDPBasicApp
{
    protected:
        virtual void initialize(int stage);
        virtual void processStart() override;
        virtual void processSend() override;
        virtual void processPacket(cPacket *packet) override;

    public:
        virtual ~UDPClient() {}
};

#endif
