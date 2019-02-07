//
// Created by izikgo on 5/23/2016.
//

#ifndef MTM4_ENCRYPTIONPUBLISHER_H
#define MTM4_ENCRYPTIONPUBLISHER_H


#include "Publisher.h"

class EncryptionPublisher : public Publisher {
    char key;
public:
    EncryptionPublisher(int priority, BrokerIfc& broker, char key, std::ostream& messagesSink);
    void sendMessage(const std::string message, const Topic& t) const override;
    virtual void receiveMessage(const std::string message, const Topic& t, const Client& c) const override{};
};



#endif //MTM4_ENCRYPTIONPUBLISHER_H
