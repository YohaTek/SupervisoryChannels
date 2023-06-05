packet tokenType {
   
    int destinationNode[100]; // the destination of channel , getDestination[2] - returns the destination of the 2nd channel
    bool isFree[100];
    int numFreeChannels;
    int freeChannles[100];
}

packet dataType {
    int source;
    int destination;
    int packetLength;
    int fragment;
    
}

packet aggregatedType extends dataType{
int length;
int qos;
bool isFragmentable;
}