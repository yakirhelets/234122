#include "Publisher.h"

Publisher::Publisher(int priority, BrokerIfc& broker, std::ostream&
   messagesSink) : Client::Client(priority, broker, messagesSink) {}

void Publisher::publishTopic(const Topic& t) {
    broker.publishTopic(*this,t);
    topics.insert(t);
}

void Publisher::unpublishTopic(const Topic& t) {
    if (topics.find(t) == topics.end()) {
        throw NonPublishedTopic();
    } else {
        broker.unpublishTopic(*this,t);
        topics.erase(t);
    }
}
    
void Publisher::unpublishAll() {
    for(std::set<Topic>::iterator it = topics.begin() ; it != topics.end() ; ++it ) {
        broker.unpublishTopic((*this), (*it));
        topics.erase(*it);
    }
}

void Publisher::sendMessage(const std::string message, const Topic& t) const{
    if (topics.find(t) == topics.end()) {
        throw NonPublishedTopic();
    } else {
        broker.publishMessage(t, message, *this);
    }
}
