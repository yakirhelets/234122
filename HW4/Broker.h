#ifndef MTM4_BROKER_H
#define MTM4_BROKER_H

#include <map>
#include <list>
#include "SortedSet.h"
#include "Publisher.h"
#include "Subscriber.h"
#include "BrokerIfc.h"

class Broker : public BrokerIfc {
    class CompareClients {
    public:
        bool operator()(const Client* c1, const Client* c2) const {
            return (c1->getId()<c2->getId());
        }
    };

    std::map<Topic, SortedSet<const Client*, CompareClients> > clientMap;

    virtual void subscribeToTopic(const Subscriber& sub, const Topic& t);

    virtual void unsubscribeToTopic(const Subscriber& sub, const Topic& t);

    virtual void publishTopic(const Publisher& pub, const Topic& t);

    virtual void unpublishTopic(const Publisher& pub, const Topic& t);

    virtual void publishMessage(const Topic& t, const std::string& message, const Client& sender) const;

public:
    Broker() = default;
//    Broker (const BrokerIfc& broker) {
//        return broker;
//    };
//    Broker& operator=(const BrokerIfc& broker) {
//        return broker;
//    };
    void sendMaintenanceMessageAll(std::list<Topic> t, std::string message);
    void sendMaintenanceMessageAny(std::list<Topic> t, std::string message);
    std::map<Topic, SortedSet<const Client*, CompareClients> >::iterator getMapBegin(){
        return clientMap.begin();
    };
    std::map<Topic, SortedSet<const Client*, CompareClients> >::iterator getMapEnd(){
        return clientMap.end();
    };

};


#endif //MTM4_BROKER_H
