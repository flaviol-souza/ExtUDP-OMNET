package extudp_omnet;

import inet.applications.udpapp.UDPBasicApp;

simple UDPServer extends UDPBasicApp
{
    parameters:
        @display("i=block/server");
}