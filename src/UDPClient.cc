#include "UDPClient.h"
#include "UDPControlInfo_m.h"
#include "UDPSocket.h"
#include "IPvXAddressResolver.h"

Define_Module(UDPClient);

/*void UDPClient::initialize()
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
}*/

void UDPClient::initialize(int stage)
{
    UDPBasicApp::initialize(stage);
    if (stage == 0) EV << "UDPClient initialized\n";
}

/*void UDPClient::initialize(int stage)
{
    EV << "UDPBasicApp initialized stage: " << stage << endl;
    UDPBasicApp::initialize(stage);
    if (stage == 0) {
        EV << "UDPClient initialized on port: " << localPort << endl;
    }
}*/

void UDPClient::processStart()
{
    socket.setOutputGate(gate("udpOut"));
    socket.bind(localPort);
    setSocketOptions();

    const char *destAddr = par("destAddress");
    IPvXAddress result;
    IPvXAddressResolver().tryResolve(destAddr, result);
    if (result.isUnspecified())
        EV << "Cannot resolve destination address: " << destAddr << endl;
    else
        destAddresses.push_back(result);

    if (destAddresses.empty()) {
        EV << "ERROR: No destination address found! Cannot send packets." << endl;
        endSimulation();
    } else {
        EV << "UDPClient initialized. DestAddresses size: " << destAddresses.size() << endl;
        selfMsg->setKind(SEND);
        processSend();
    }
}

void UDPClient::processSend()
{
    EV << "UDPClient is sending packet\n";
    UDPBasicApp::processSend();
}

/*void UDPClient::processSend()
{
    if (destAddresses.empty()) {
        EV << "UDPClient: No destination address found, not sending packets!" << endl;
        return;
    }

    char msgName[32];
    sprintf(msgName, "UDPClientData-%d", numSent);
    cPacket *payload = new cPacket(msgName);
    payload->setByteLength(par("messageLength").longValue());

    IPvXAddress destAddr = destAddresses[0];

    emit(sentPkSignal, payload);
    socket.sendTo(payload, destAddr, destPort);
    numSent++;

    EV << "UDPClient: Sent message: " << msgName << " to " << destAddr << ":" << destPort << endl;

    simtime_t nextSend = simTime() + par("sendInterval").doubleValue();
    if (stopTime < SIMTIME_ZERO || nextSend < stopTime)
    {
        selfMsg->setKind(SEND);
        scheduleAt(nextSend, selfMsg);
    }
    else
    {
        selfMsg->setKind(STOP);
        scheduleAt(stopTime, selfMsg);
    }
}*/

void UDPClient::processPacket(cPacket *pk)
{
    emit(rcvdPkSignal, pk);
    EV << "Received packet: " << pk->getName() << endl;

    delete pk;
    numReceived++;
}
