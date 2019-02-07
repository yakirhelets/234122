//
// Created by izikgo on 5/23/2016.
//

#ifndef MTM4_ENCRYPTIONSUBSCRIBER_H
#define MTM4_ENCRYPTIONSUBSCRIBER_H


#include "Subscriber.h"

class EncryptionSubscriber : public Subscriber {
    char key;
public:
    EncryptionSubscriber(int priority, BrokerIfc& broker, char key, std::ostream& messagesSink);
    void receiveMessage(const std::string message, const Topic& t, const Client& c) const override;
};


#endif //MTM4_ENCRYPTIONSUBSCRIBER_H
