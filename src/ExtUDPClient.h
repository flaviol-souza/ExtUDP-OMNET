//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

#ifndef __EXTUDP_OMNET_EXTUDPCLIENT_H_
#define __EXTUDP_OMNET_EXTUDPCLIENT_H_

#include <platdep/sockets.h>
#include <omnetpp.h>
#include "UDPPkt_m.h"
#include "SocketRTScheduler.h"

/**
 * TODO - Generated class
 */
class ExtUDPClient : public cSimpleModule
{
    private:
        cMessage *rtEvent;
        cSocketRTScheduler *rtScheduler;

        char recvBuffer[1024];
        int numRecvBytes;

        int addr;
        int srvAddr;

    public:
        ExtUDPClient();
        virtual ~ExtUDPClient();

    protected:
        virtual void initialize();
        virtual void handleMessage(cMessage *msg);
        void handleSocketEvent();
        void handleReply(UDPPkt *udpReply);
};

#endif
