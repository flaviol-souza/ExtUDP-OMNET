#include "UDPServer.h"
#include "UDPSocket.h"
#include "UDPControlInfo_m.h"

Define_Module(UDPServer);

void UDPServer::initialize(int stage)
{
    EV << "UDPServer initialized stage: " << stage << endl;
    UDPBasicApp::initialize(stage);
    if (stage == 0) {
        EV << "UDPServer initialized on port: " << localPort << endl;
    }
}


void UDPServer::processPacket(cPacket *pk)
{
    EV << "Received packet: " << UDPSocket::getReceivedPacketInfo(pk) << endl;

    if (pk->getKind() == UDP_I_ERROR)
    {
        // ICMP error report -- discard it
        delete pk;
    }
    else if (pk->getKind() == UDP_I_DATA)
    {
        // statistics
        numReceived++;
        emit(sentPkSignal, pk);

        // determine its source address/port
        UDPDataIndication *ctrl = check_and_cast<UDPDataIndication *>(pk->removeControlInfo());
        IPvXAddress srcAddr = ctrl->getSrcAddr();
        int srcPort = ctrl->getSrcPort();
        delete ctrl;

        if (srcAddr.isUnspecified()) {
            EV << "ERROR: Received packet from unspecified address!" << endl;
            delete pk;
            return;
        }

        // send back
        socket.sendTo(pk, srcAddr, srcPort);
        EV << "UDPServer received a packet. Sending reply.\n";
    }
    else
    {
        throw cRuntimeError("Message received with unexpected message kind = %d", pk->getKind());
    }
}

void UDPServer::finish()
{
    ApplicationBase::finish();
}

