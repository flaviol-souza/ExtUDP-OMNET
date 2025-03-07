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

#ifndef __EXTUDP_OMNET_QUEUEBASE_H_
#define __EXTUDP_OMNET_QUEUEBASE_H_

#include <omnetpp.h>

/**
 * Abstract base class for single-server queues.
 */
class QueueBase : public cSimpleModule
{
  private:
    cMessage *msgServiced;
    cMessage *endServiceMsg;
    cQueue queue;

  public:
    QueueBase();
    virtual ~QueueBase();

  protected:
    virtual void initialize();
    virtual void handleMessage(cMessage *msg);

    // hook functions to (re)define behaviour
    virtual void arrival(cMessage *msg) {}
    virtual simtime_t startService(cMessage *msg) = 0;
    virtual void endService(cMessage *msg) = 0;
};

#endif
