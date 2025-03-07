//=========================================================================
//  CSOCKETRTSCHEDULER.CC - part of
//
//                  OMNeT++/OMNEST
//           Discrete System Simulation in C++
//
// Author: Andras Varga, 2005
//
//=========================================================================

/*--------------------------------------------------------------*
  Copyright (C) 2005-2008 Andras Varga

  This file is distributed WITHOUT ANY WARRANTY. See the file
  `license' for details on this and other legal matters.
*--------------------------------------------------------------*/


#include "SocketRTScheduler.h"

Register_Class(cSocketRTScheduler);

Register_GlobalConfigOption(CFGID_SOCKETRTSCHEDULER_PORT, "socketrtscheduler-port", CFG_INT, "4560", "When cSocketRTScheduler is selected as scheduler class: the port number cSocketRTScheduler listens on.");

inline std::ostream& operator<<(std::ostream& out, const timeval& tv)
{
    return out << (unsigned long)tv.tv_sec << "s" << tv.tv_usec << "us";
}

cSocketRTScheduler::cSocketRTScheduler() : cScheduler()
{
    listenerSocket = INVALID_SOCKET;
    addrLen = sizeof(clientAddr);
}

cSocketRTScheduler::~cSocketRTScheduler()
{
    if (listenerSocket != INVALID_SOCKET)
        closesocket(listenerSocket);
}

void cSocketRTScheduler::startRun()
{
    if (initsocketlibonce()!=0)
        throw cRuntimeError("cSocketRTScheduler: Cannot initialize socket library");

    gettimeofday(&baseTime, NULL);

    module = NULL;
    notificationMsg = NULL;
    recvBuffer = NULL;
    recvBufferSize = 0;
    numBytesPtr = NULL;

    port = ev.getConfig()->getAsInt(CFGID_SOCKETRTSCHEDULER_PORT);
    setupListener();
}

void cSocketRTScheduler::setupListener()
{
    listenerSocket = socket(AF_INET, SOCK_DGRAM, 0);
    if (listenerSocket == INVALID_SOCKET)
        throw cRuntimeError("cSocketRTScheduler: cannot create socket");

    sockaddr_in sinInterface;
    sinInterface.sin_family = AF_INET;
    sinInterface.sin_addr.s_addr = INADDR_ANY;
    sinInterface.sin_port = htons(port);
    if (bind(listenerSocket, (sockaddr*)&sinInterface, sizeof(sockaddr_in)) == SOCKET_ERROR)
        throw cRuntimeError("cSocketRTScheduler: socket bind() failed");
}

void cSocketRTScheduler::endRun()
{
    if (listenerSocket != INVALID_SOCKET)
        closesocket(listenerSocket);
}

void cSocketRTScheduler::executionResumed()
{
    gettimeofday(&baseTime, NULL);
    baseTime = timeval_substract(baseTime, SIMTIME_DBL(simTime()));
}

void cSocketRTScheduler::setInterfaceModule(cModule *mod, cMessage *notifMsg, char *buf, int bufSize, int *nBytesPtr)
{
    if (module)
        throw cRuntimeError("cSocketRTScheduler: setInterfaceModule() already called");
    if (!mod || !notifMsg || !buf || !bufSize || !nBytesPtr)
        throw cRuntimeError("cSocketRTScheduler: setInterfaceModule(): arguments must be non-NULL");

    module = mod;
    notificationMsg = notifMsg;
    recvBuffer = buf;
    recvBufferSize = bufSize;
    numBytesPtr = nBytesPtr;
    *numBytesPtr = 0;
}

bool cSocketRTScheduler::receiveWithTimeout(long usec)
{
    fd_set readFDs;
    FD_ZERO(&readFDs);
    FD_SET(listenerSocket, &readFDs);

    timeval timeout;
    timeout.tv_sec = 0;
    timeout.tv_usec = usec;

    if (select(FD_SETSIZE, &readFDs, NULL, NULL, &timeout) > 0)
    {
        if (FD_ISSET(listenerSocket, &readFDs))
        {
            int nBytes = recvfrom(listenerSocket, recvBuffer, recvBufferSize, 0, (sockaddr*)&clientAddr, &addrLen);
            if (nBytes > 0)
            {
                EV << "UDP Packet Received: " << recvBuffer << endl;
                *numBytesPtr = nBytes;
                notificationMsg->setArrival(module, -1, simTime());
                simulation.msgQueue.insert(notificationMsg);
                return true;
            }
        }
    }
    return false;
}

int cSocketRTScheduler::receiveUntil(const timeval& targetTime)
{
    timeval curTime;
    gettimeofday(&curTime, NULL);
    while (timeval_greater(targetTime, curTime))
    {
        if (receiveWithTimeout(100000))
            return 1;
        gettimeofday(&curTime, NULL);
    }
    return 0;
}

cMessage *cSocketRTScheduler::getNextEvent()
{
    if (!module)
        throw cRuntimeError("cSocketRTScheduler: setInterfaceModule() not called: it must be called from a module's initialize() function");

    timeval targetTime;
    cMessage *msg = sim->msgQueue.peekFirst();
    if (!msg)
    {
        targetTime.tv_sec = LONG_MAX;
        targetTime.tv_usec = 0;
    }
    else
    {
        simtime_t eventSimtime = msg->getArrivalTime();
        targetTime = timeval_add(baseTime, SIMTIME_DBL(eventSimtime));
    }

    timeval curTime;
    gettimeofday(&curTime, NULL);
    if (timeval_greater(targetTime, curTime))
    {
        int status = receiveUntil(targetTime);
        if (status == 1)
            msg = sim->msgQueue.peekFirst();
    }
    return msg;
}

void cSocketRTScheduler::sendBytes(const char *buf, size_t numBytes)
{
    if (clientAddr.sin_port == 0)
    {
        EV << "No valid client address to send response." << endl;
        return;
    }
    int sentBytes = sendto(listenerSocket, buf, numBytes, 0, (sockaddr*)&clientAddr, addrLen);
    if (sentBytes < 0)
    {
        EV << "Error sending UDP response." << endl;
    }
    else
    {
        EV << "Sent UDP response: " << buf << endl;
    }
}

