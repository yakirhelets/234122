#include "EncryptionPublisher.h"

EncryptionPublisher::EncryptionPublisher(int priority, BrokerIfc& broker, char key, std::ostream& messagesSink)
: Publisher::Publisher(priority, broker, messagesSink), key(key) {}

void EncryptionPublisher::sendMessage(const std::string message, const Topic& t) const {
    std::string decrypted = message;
    for (unsigned int i=0 ; i<message.size() ; ++i) {
        decrypted[i]=(decrypted[i])^key;
    }
    if (topics.find(t) == topics.end()) {
        throw "somthing";
    } else {
    	Publisher::sendMessage(decrypted, t);
    }
}


