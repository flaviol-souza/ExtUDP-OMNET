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

package extudp_omnet.simulations;

import extudp_omnet.UDPServer;
import extudp_omnet.Cloud;
import extudp_omnet.ExtUDPClient;
import extudp_omnet.UDPClient;

network UDPNet
{
    parameters:
        int numClients @prompt("Number of clients") = default(1);
    submodules:
        server: UDPServer {
            parameters:
                @display("p=426,178");
        }
        cloud: Cloud {
            parameters:
                @display("p=274,182");
            gates:
                g[numClients+2];
        }
        extClient: ExtUDPClient {
            parameters:
                addr = 1;
                srvAddr = 0;
                @display("p=102,92;t=Run the simulation\\, then start: udp localhost 4560 Then if on Unix\\, type: Ctrl+] ``mode character''");
        }
        client[numClients]: UDPClient {
            parameters:
                addr = 2+index;
                srvAddr = 0;
                @display("p=102,162,col");
        }
    connections:
        server.g <--> cloud.g[0];
        extClient.g <--> cloud.g[1];
        client[i].g <--> cloud.g[i+2] for i=0..numClients-1;
}
