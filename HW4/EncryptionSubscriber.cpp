#include "EncryptionSubscriber.h"


EncryptionSubscriber::EncryptionSubscriber(int priority, BrokerIfc& broker, char key, std::ostream& messagesSink)
: Subscriber::Subscriber(priority, broker, messagesSink), key(key) {}

void EncryptionSubscriber::receiveMessage(const std::string message, const Topic& t, const Client& c) const {
    std::string encrypted = message;
    for (unsigned int i=0 ; i<message.size() ; ++i) {
        encrypted[i]=(encrypted[i])^key;
    }
//    this->getMessagesSink() << "Topic:" << t << "Sender:" << c.getId() <<
//    "Receiver:#" << this->getId() << "Message:" << encrypted << std::endl;
    Subscriber::receiveMessage(encrypted, t, c);
}

