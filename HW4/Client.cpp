#include "Client.h"


int Client::id = 0;

Client::Client(int priority, BrokerIfc& broker, std::ostream& messagesSink)
		: broker(broker), priority(priority), messagesSink(messagesSink), topics(){
	if (priority < 0) {
		throw Client::IllegalPriority();
	} else {
		(this->id)++;
	}
}

Client::~Client() =default;

int Client::getPriority() const {
	return priority;
}

int Client::getId() const {
	return id;
}

std::ostream& Client::getMessagesSink() const {
    return messagesSink;
}

void Client::receiveMaintenanceMessage(const std::string& s) const {
    this->messagesSink << "Client #" << this->getId()
    << " received maintenance message: " << s << std::endl;
}
