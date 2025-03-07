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

#include <ctype.h>
#include "UDPServer.h"
#include "UDPPkt_m.h"


Define_Module(UDPServer);

simtime_t UDPServer::startService(cMessage *msg)
{
    EV << "Starting service of " << msg->getName() << endl;
    return par("serviceTime").doubleValue();
}

void UDPServer::endService(cMessage *msg)
{
    EV << "Completed service of " << msg->getName() << endl;

    UDPPkt *udpPkt = check_and_cast<UDPPkt *>(msg);

    std::string reply = processChars(udpPkt->getPayload());
    udpPkt->setPayload(reply.c_str());
    udpPkt->setName(reply.c_str());

    int clientAddr = udpPkt->getSrcAddress();
    int srvAddr = udpPkt->getDestAddress();
    udpPkt->setDestAddress(clientAddr);
    udpPkt->setSrcAddress(srvAddr);

    send(msg, "g$o");
}

std::string UDPServer::processChars(const char *chars)
{
    std::string s = chars;
    for (unsigned int i = 0; i < s.length(); i++)
        s.at(i) = toupper(s.at(i));
    return s;
}
