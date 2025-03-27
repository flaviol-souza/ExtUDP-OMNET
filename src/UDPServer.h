#ifndef __EXTUDP_OMNET_UDPSERVER_H_
#define __EXTUDP_OMNET_UDPSERVER_H_

#include <omnetpp.h>
#include "UDPBasicApp.h"

class UDPServer : public UDPBasicApp
{
    protected:
        virtual void initialize(int stage);
        virtual void processPacket(cPacket *packet) override;
        virtual void finish();

    public:
        virtual ~UDPServer() {}
};

#endif
