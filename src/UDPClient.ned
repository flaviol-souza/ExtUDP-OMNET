package extudp_omnet;

import inet.applications.udpapp.UDPBasicApp;

simple UDPClient extends UDPBasicApp
{
    parameters:
        @display("i=block/app");
}