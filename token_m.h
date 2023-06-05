//
// Generated file, do not edit! Created by nedtool 5.6 from token.msg.
//

#ifndef __TOKEN_M_H
#define __TOKEN_M_H

#if defined(__clang__)
#  pragma clang diagnostic ignored "-Wreserved-id-macro"
#endif
#include <omnetpp.h>

// nedtool version check
#define MSGC_VERSION 0x0506
#if (MSGC_VERSION!=OMNETPP_VERSION)
#    error Version mismatch! Probably this file was generated by an earlier version of nedtool: 'make clean' should help.
#endif



/**
 * Class generated from <tt>token.msg:1</tt> by nedtool.
 * <pre>
 * packet tokenType
 * {
 *     int destinationNode[100]; // the destination of channel , getDestination[2] - returns the destination of the 2nd channel
 *     bool isFree[100];
 *     int numFreeChannels;
 *     int freeChannles[100];
 * }
 * </pre>
 */
class tokenType : public ::omnetpp::cPacket
{
  protected:
    int destinationNode[100];
    bool isFree[100];
    int numFreeChannels;
    int freeChannles[100];

  private:
    void copy(const tokenType& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const tokenType&);

  public:
    tokenType(const char *name=nullptr, short kind=0);
    tokenType(const tokenType& other);
    virtual ~tokenType();
    tokenType& operator=(const tokenType& other);
    virtual tokenType *dup() const override {return new tokenType(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual unsigned int getDestinationNodeArraySize() const;
    virtual int getDestinationNode(unsigned int k) const;
    virtual void setDestinationNode(unsigned int k, int destinationNode);
    virtual unsigned int getIsFreeArraySize() const;
    virtual bool getIsFree(unsigned int k) const;
    virtual void setIsFree(unsigned int k, bool isFree);
    virtual int getNumFreeChannels() const;
    virtual void setNumFreeChannels(int numFreeChannels);
    virtual unsigned int getFreeChannlesArraySize() const;
    virtual int getFreeChannles(unsigned int k) const;
    virtual void setFreeChannles(unsigned int k, int freeChannles);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const tokenType& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, tokenType& obj) {obj.parsimUnpack(b);}

/**
 * Class generated from <tt>token.msg:9</tt> by nedtool.
 * <pre>
 * packet dataType
 * {
 *     int source;
 *     int destination;
 *     int packetLength;
 *     int fragment;
 * 
 * }
 * </pre>
 */
class dataType : public ::omnetpp::cPacket
{
  protected:
    int source;
    int destination;
    int packetLength;
    int fragment;

  private:
    void copy(const dataType& other);

  protected:
    // protected and unimplemented operator==(), to prevent accidental usage
    bool operator==(const dataType&);

  public:
    dataType(const char *name=nullptr, short kind=0);
    dataType(const dataType& other);
    virtual ~dataType();
    dataType& operator=(const dataType& other);
    virtual dataType *dup() const override {return new dataType(*this);}
    virtual void parsimPack(omnetpp::cCommBuffer *b) const override;
    virtual void parsimUnpack(omnetpp::cCommBuffer *b) override;

    // field getter/setter methods
    virtual int getSource() const;
    virtual void setSource(int source);
    virtual int getDestination() const;
    virtual void setDestination(int destination);
    virtual int getPacketLength() const;
    virtual void setPacketLength(int packetLength);
    virtual int getFragment() const;
    virtual void setFragment(int fragment);
};

inline void doParsimPacking(omnetpp::cCommBuffer *b, const dataType& obj) {obj.parsimPack(b);}
inline void doParsimUnpacking(omnetpp::cCommBuffer *b, dataType& obj) {obj.parsimUnpack(b);}


#endif // ifndef __TOKEN_M_H
