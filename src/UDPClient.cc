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

#include "UDPClient.h"

Define_Module(UDPClient);

void UDPClient::initialize()
{
    addr = par("addr");
    srvAddr = par("srvAddr");

    cMessage *timer = new cMessage("timer");
    scheduleAt(simTime() + par("sendIaTime").doubleValue(), timer);
}

void UDPClient::handleMessage(cMessage *msg)
{
    if (msg->isSelfMessage())
    {
        simulateUserTyping();
        scheduleAt(simTime() + par("sendIaTime").doubleValue(), msg);
    }
    else
    {
        processEcho(check_and_cast<UDPPkt *>(msg));
    }
}

void UDPClient::simulateUserTyping()
{
    UDPPkt *udpPkt = new UDPPkt("x");
    udpPkt->setPayload("x");
    udpPkt->setDestAddress(srvAddr);
    udpPkt->setSrcAddress(addr);

    send(udpPkt, "g$o");
}

void UDPClient::processEcho(UDPPkt *udpPkt)
{
    delete udpPkt;
}
