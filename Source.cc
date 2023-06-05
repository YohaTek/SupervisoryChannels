#include <omnetpp.h>
#include "token_m.h"
#include "parameters.h"


using namespace omnetpp;

class Source : public cSimpleModule
{
  private:
    int sentMsg;
    double sendingInterval;
    dataType* messageData;
    cMessage* msg;
  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage* msg) override;
    virtual void finish() override;
};

Define_Module(Source);

void Source::initialize()
{
    sentMsg=0;
    WATCH(sentMsg);
    msg = new cMessage("alarm message");
    scheduleAt(0.01,msg);
}

void Source::handleMessage(cMessage* msg)
{
  sendingInterval = uniform(0.1,0.2);
  messageData = new dataType("message Data");
  int x = intuniform(0,3);
  int y = intuniform(0,3);
  messageData->setSource(x);
  messageData->setDestination(y);
  EV<<"source set to "<<x<<endl;
  send(messageData,"out");
  scheduleAt(sendingInterval,msg);
  sentMsg++;
}

void Source::finish()
{
    recordScalar("sent Messages", sentMsg);
}
