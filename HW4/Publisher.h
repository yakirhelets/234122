#ifndef MTM4_PUBLISHER_H
#define MTM4_PUBLISHER_H

#include "Client.h"

class Publisher : public Client {

public:
    Publisher(int priority, BrokerIfc& broker, std::ostream& messagesSink = std::cout);

    void publishTopic(const Topic& t);
    void unpublishTopic(const Topic& t);
    void unpublishAll();
    virtual void sendMessage(const std::string message, const Topic& t) const;
    virtual void receiveMessage(const std::string message, const Topic& t, const Client& c) const {}

};


#endif //MTM4_PUBLISHER_H
