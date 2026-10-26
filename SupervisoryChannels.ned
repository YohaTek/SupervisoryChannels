//import ned.DatarateChannel;

simple RingNode
{
    parameters:
        @display("i=block/queue,,50;bgb=450,450");

    gates:
        input dataIn;
        input signalingIn;
        output dataOut;
        output signalingOut;
        output outtosink;
        input sourceIn[10];
}

simple Source
{
    gates:
        output out;
}
simple Sink
{
    gates:
        input in;
}

// Define the network topology module
network RingTopology
{
    @display("bgb=744.55334,422.32666");
    types:        // Supervisory Channel
        channel dataChannel0 extends ned.DatarateChannel
        {
            parameters:
                delay = 10us;
                datarate = supervisoryChannelRate;
        }

    submodules:
        node[4]: RingNode {
            @display("p=326.04,302.20667,ri,300,300;b=10,10,oval;i=abstract/opticalswitch");
        }

        source[10]: Source {
            @display("p=49.573334,91.520004");
        }
        sink1: Sink {
            @display("p=364.17334,27.646667");
        }
        sink2: Sink {
            @display("p=511.94,36.226665");
        }
        sink3: Sink {
            @display("p=233.56667,47.666668");
        }
        sink0: Sink {
            @display("p=74.36,185.90001");
        }

    connections allowunconnected:

        node[3].dataOut --> node[0].dataIn;
        node[0].dataOut --> node[1].dataIn;
        node[1].dataOut --> node[2].dataIn;
        node[2].dataOut --> node[3].dataIn;

        node[0].outtosink --> sink0.in;
        node[1].outtosink --> sink1.in;
        node[2].outtosink --> sink2.in;
        node[3].outtosink --> sink3.in;

        node[0].signalingOut --> node[1].signalingIn;
        node[1].signalingOut --> node[2].signalingIn;
        node[2].signalingOut --> node[3].signalingIn;
        node[3].signalingOut --> node[0].signalingIn;

        for i=0..9 {
            source[i].out --> node[0].sourceIn[i];
        }
}
