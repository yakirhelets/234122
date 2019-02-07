#include "Subscriber.h"

Subscriber::Subscriber(int priority, BrokerIfc& broker, std::ostream&
   messagesSink) : Client::Client(priority, broker, messagesSink) {}

void Subscriber::subscribeToTopic(const Topic& t) {
    broker.subscribeToTopic(*this,t);
    topics.insert(t);
}
    
void Subscriber::unsubscribeToTopic(const Topic& t) {
    if (topics.find(t) == topics.end()) {
        throw NonSubscribedTopic();
    } else {
        broker.unsubscribeToTopic(*this,t);
        topics.erase(t);
    }
}

void Subscriber::unsubscribeAll() {
    for(std::set<Topic>::iterator it = topics.begin() ; it != topics.end() ; ++it ) {
        Subscriber::unsubscribeToTopic(*it);
//        topics.erase(*it);
    }
}
    
void Subscriber::receiveMessage(const std::string message, const Topic& t, const Client& c) const {
    if (topics.find(t) == topics.end()) {
        throw NonSubscribedTopic();
    }
	messagesSink << "Topic: " << t << ". Sender: #" <<
	c.getId() << ". Receiver: #" << this->getId() << ". Message: "
	<< message << std::endl;
}

