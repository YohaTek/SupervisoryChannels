#include <omnetpp.h>
#include "token_m.h"
#include "parameters.h"

using namespace omnetpp;

class Sink : public cSimpleModule
{
  private:
    int receivedMsg;

  protected:
    virtual void initialize() override;
    virtual void handleMessage(cMessage* msg) override;
    virtual void finish() override;
};

Define_Module(Sink);

void Sink::initialize()
{
    receivedMsg=0;
    WATCH(receivedMsg);
}

void Sink::handleMessage(cMessage* msg)
{
  receivedMsg++;
  delete(msg);
}

void Sink::finish()
{
    recordScalar("received Messages", receivedMsg);

}
