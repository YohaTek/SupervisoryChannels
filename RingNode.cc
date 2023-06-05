#include <omnetpp.h>
#include "token_m.h"
#include "parameters.h"  // for taking values that are constant
#include <iostream>  // printing the standard input/output
#include <vector>    // for the vector instance result
#include <algorithm> // for fillin function while tring to reset the vector instance called result

using namespace omnetpp;

class RingNode : public cSimpleModule
{
  private:
    dataType* dataMessage;
    dataType *dataMsg;
    tokenType* signalingMessage;
    int destinationNode;
    int receivedMsg;
    int bufferLength[NUM_LOWBUFFERS]; //
    cQueue buffer[10]; //
    int bufferCapacity;
    bool isFree[NUM_CHANNELS]; //
    int destTable[NUM_CHANNELS]; //
    int tableofFreeChannels[NUM_CHANNELS];//
    int numofFreeChannels;
    simtime_t siganl_to_message_delay;
    bool hasToken;
    bool hasUplinkData; // if the node has data that needs to be added when the token arrives
    struct channelType{
        int destination;
        int wavelength;
        int channelnumber;
    };
    channelType channels[NUM_CHANNELS]; //
    std::vector<int> result;
    std::map<int, cPacketQueue> aggregatedPackets;  // Map to store aggregated packets for each destination
    cPacketQueue aggregatedPkt;
    cQueue high_buffer_rx[NUM_HIGHBUFFERS];
    cQueue high_buffer_tx[NUM_HIGHBUFFERS];
    cQueue classifing_buffer[NUM_HIGHBUFFERS];
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage* msg) override;
    virtual void processBuffer();
    virtual void processChannels(tokenType* token);
    virtual void forwardToken(tokenType* token);
    virtual void uplinkData(dataType *dataMsg);
    virtual void encapsulatePackets(int destination);
    virtual void classifing_Aggregation();
    virtual void deAggregation(aggregatedType *aggregatedPkt);
};

Define_Module(RingNode);

void RingNode::initialize()
{
    signalingMessage = new tokenType("SignalingMessage");
    dataMessage = new dataType("DataMessage");
    destinationNode = 3;
    receivedMsg=0;
    numofFreeChannels=0;
    WATCH(receivedMsg);
    bufferCapacity=1000;

    hasUplinkData=false;

    for(int i=0;i<10;i++){
        destTable[i] = -1;
        isFree[i] = true;
        tableofFreeChannels[i]=i;
    }
    for(int i=0;i<NUM_LOWBUFFERS;i++){
       bufferLength[i] = 0;
    }

    siganl_to_message_delay = 0.01;

    if (getIndex() == 0) {
        hasToken=true;
        scheduleAt(siganl_to_message_delay,signalingMessage);
    }
    else{
        hasToken=false;
    }
    // Initialize aggregated packet queues for each destination
       for (int i = 0; i < NUM_NODES; i++) {
           aggregatedPackets[i] = cPacketQueue();
       }
}

void RingNode::handleMessage(cMessage* msg)
{
    tokenType *token = dynamic_cast<tokenType *>(msg);
    if (token != nullptr)
    {
        hasToken = true;
        processChannels(token);
        classifing_Aggregation();
        processBuffer();
        forwardToken(token);
    }
    else
    {
        dataType *dataMsg = dynamic_cast<dataType *>(msg);

 /*
        hasToken==True

     int x = dataMsg->getSource();
          int y = dataMsg->getDestination();

       if(hasToken==true)
          {
            processBuffer();
          }
        else{
            uplinkData(dataMsg);
          }
 */

        uplinkData(dataMsg);

    }
}

void RingNode::processChannels(tokenType *token){
EV<<"processChannels() called at node "<<getIndex()<<endl;

int j=0;
for(int i=0;i<NUM_CHANNELS;i++)
 {
    if(token->getDestinationNode(i) == getIndex()) // if the channel is destined for the present node
      {
        receivedMsg++;
        destTable[i]=0;
        isFree[i]=true; // set the channel as free channel
        numofFreeChannels++;
        tableofFreeChannels[i]=i;
        j++;
        token->setDestinationNode(i,-1); // indicating that it is free,
        token->setNumFreeChannels(j);
        result.push_back(i);
      }
    else{
         if(token->getDestinationNode(i) == -1) // if the channel free
           {
             result.push_back(i);
             numofFreeChannels++;
           }
    }
 }

  if(hasUplinkData==true)
    {
      int x = token->getNumFreeChannels();
      for(int i=0;i<x;i++) //x or  numofFreeChannels can be modified by the controller, depending on if there is a need for reserving a channel
         {
            // int destination = uniform(0,4);
             int y = result[i];
             token->setDestinationNode(y,2);
         }
     }
}

void RingNode::processBuffer(){
EV<<"processBuffer() called at Node" <<getIndex()<<endl;

 for(int i=0;i<NUM_CHANNELS;i++){
        if(destTable[i]==getIndex())
         {
           while(!high_buffer_rx[i].isEmpty()) {
                aggregatedPkt = (aggregatedType *)high_buffer_rx[i].pop();
                deAggregation();
              }
         }
    else{
          while(!high_buffer_rx[i].isEmpty()) {
               aggregatedPkt = (aggregatedType *)high_buffer_rx[i].pop();
               //sendDelayed(dataMsg,siganl_to_message_delay,"dataOut");
               send(aggregatedPkt,"dataOut");
             }
          //hasUplinkData=false;
       }
 }
}

void RingNode::uplinkData(dataType *dataMsg){
    EV<<"uplinkData() called at Node "<<getIndex()<<endl;
    int x = dataMsg->getSource();
    buffer[x].insert(dataMsg);
    //int dest = dataMsg->getDestination();
    //aggregatedPackets[dest].insert(dataMsg);
    EV<<"inserting packets to buffer "<<x<<" of node "<<getIndex()<<endl;
    //hasUplinkData=true;
}

void RingNode::classifing_Aggregation(){
    for(int i=0;i<NUM_lOWBUFFERS;i++){
          for(int j=0;j<NUM_NODES;j++){
                while(!buffer[i].isEmpty()) {
                     dataMsg = (dataType *)buffer[i].pop();
                     int destination = dataMsg->getDestinationNode();

                     // Encapsulate packet and add it to the aggregated packet queue for the corresponding destination
                     classifing_buffer[destination].insert(dataMsg); // this is like the high capacity buffer

                     // Check if the aggregated packet queue reached a certain size or if a timeout occurred
                     if(classifing_buffer[destination].getLength()>= aggregatedPacketSize || simTime()>= timeout) {
                            // Create an aggregated packet
                            aggregatedType *aggregatedPacket = new aggregatedType();

                            // Add all packets from the queue to the aggregated packet
                            while (!classifing_buffer[destination].isEmpty()) {
                                 aggregatedType *packet = check_and_cast<aggregatedType *>(classifing_buffer[destination].pop());
                                 aggregatedPacket->encapsulate(packet);
                              }
                         high_buffer_tx[destination].insert(aggregatedPacket);
                     }
                }
          }
    }
}

void RingNode::forwardToken(tokenType* token)
 {
    EV<<"forwardToken() called "<<endl;
    send(token,"signalingOut");

    std::fill(result.begin(), result.end(), -1);
    numofFreeChannels=0;


    hasToken=false;
 }

void RingNode::encapsulatePackets(int destination){
    EV<<"encapsulatePackets called at Node" <<getIndex()<<endl;
}

void RingNode::deAggregation(aggregatedType *aggregatedPkt)
{
    EV<<" deAggregation called at Node "<<getIndex()<<endl;
    int leng = aggregatedPkt->getLength();

    for (int i=0;i<leng;i++)
       {
        dataMsg = new dataType();
        dataMsg = decapsulate(aggregatedPkt);
        send(aggregatedPkt,"outtosink");
       }

}
