#ifndef MTM4_SUBSCRIBER_H
#define MTM4_SUBSCRIBER_H

#include"Client.h"
#include<map>
#include"Broker.h"

class Subscriber : public Client {

public:
	Subscriber(int priority, BrokerIfc& broker, std::ostream& messagesSink = std::cout);

	void subscribeToTopic(const Topic& t);
	void unsubscribeToTopic(const Topic& t);
	void unsubscribeAll();
	virtual void receiveMessage(const std::string message, const Topic& t, const Client& c) const;
};


#endif //MTM4_SUBSCRIBER_H
