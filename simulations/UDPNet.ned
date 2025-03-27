package extudp_omnet.simulations;

import inet.networklayer.autorouting.ipv4.IPv4NetworkConfigurator;
import inet.nodes.inet.Router;
import inet.nodes.inet.StandardHost;
import inet.util.ThruputMeteringChannel;

network UDPNet
{        
    parameters:
        int n;
        @display("bgb=593,356");
    types:
        channel ethernetline extends ThruputMeteringChannel
        {
            delay = 0.1us;
            datarate = 10Mbps;
            thruputDisplayFormat = "u";
        }
    submodules:
        router: Router {
            @display("p=282,182");
        }
        client[n]: StandardHost {
            @display("i=device/laptop;p=144,299,r");
        }
        server: StandardHost {
            @display("p=282,47;i=device/server_l");
        }
        configurator: IPv4NetworkConfigurator {
            @display("p=75,47");
        }
    connections:
        for i=0..n-1 {
            client[i].ethg++ <--> ethernetline <--> router.ethg++;
        }
        router.pppg++ <--> ethernetline <--> server.pppg++;
}