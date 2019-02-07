#ifndef MTM4_CLIENT_H
#define MTM4_CLIENT_H

#include <iostream>
#include <exception>
#include <string>
#include <set>
#include "BrokerIfc.h"

class Client {

protected:
    BrokerIfc& broker;
    int priority;
    std::ostream& messagesSink;
    std::set<Topic> topics;
    static int id;
public:
    class ClientException : public std::exception {};
    class IllegalPriority : public ClientException {};
    class NonSubscribedTopic : public ClientException {};
    class NonPublishedTopic : public ClientException {};

    Client(int priority, BrokerIfc& broker, std::ostream& messagesSink =
    std::cout);
    Client(const Client& c) = delete;
    Client& operator=(const Client&) = delete;
    virtual ~Client();
    int getPriority() const;
    int getId() const;
    std::ostream& getMessagesSink() const;
    void receiveMaintenanceMessage(const std::string& s) const;
    virtual void receiveMessage(const std::string message, const Topic& t, const Client& c) const =0;

};


#endif //MTM4_CLIENT_H
