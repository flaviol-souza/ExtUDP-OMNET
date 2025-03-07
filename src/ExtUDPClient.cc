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

#include "ExtUDPClient.h"

Define_Module(ExtUDPClient);

ExtUDPClient::ExtUDPClient()
{
    rtEvent = NULL;
}

ExtUDPClient::~ExtUDPClient()
{
    cancelAndDelete(rtEvent);
}

void ExtUDPClient::initialize()
{
    EV << "OMNeT++ UDP Client is running and listening..." << endl;

    rtEvent = new cMessage("rtEvent");
    rtScheduler = check_and_cast<cSocketRTScheduler *>(simulation.getScheduler());
    rtScheduler->setInterfaceModule(this, rtEvent, recvBuffer, 1024, &numRecvBytes);

    addr = par("addr");
    srvAddr = par("srvAddr");

    EV << "OMNeT++ UDP Client is running and listening..." << endl;
    EV << "addr:" << addr << endl;
    EV << "srvAddr:" << srvAddr << endl;


}

void ExtUDPClient::handleMessage(cMessage *msg)
{
    if (msg==rtEvent)
        handleSocketEvent();
    else
        handleReply(check_and_cast<UDPPkt *>(msg));
}

void ExtUDPClient::handleSocketEvent()
{
    // get data from buffer
    std::string text = std::string(recvBuffer, numRecvBytes);
    numRecvBytes = 0;

    // assemble and send Telnet packet
    UDPPkt *udpPkt = new UDPPkt();
    udpPkt->setPayload(text.c_str());
    udpPkt->setName(text.c_str());
    udpPkt->setDestAddress(srvAddr);
    udpPkt->setSrcAddress(addr);

    send(udpPkt,"g$o");
}

void ExtUDPClient::handleReply(UDPPkt *udpReply)
{
    const char *reply = udpReply->getPayload();
    rtScheduler->sendBytes(reply, strlen(reply));
    delete udpReply;
}
