#include "Broker.h"

void Broker::subscribeToTopic(const Subscriber& sub, const Topic& t) {
	const Client* ptr = &sub;
	clientMap[t].insert(ptr);
}

void Broker::unsubscribeToTopic(const Subscriber& sub, const Topic& t) {
	std::map<Topic,SortedSet<const Client*,CompareClients>>::iterator it = clientMap.find(t);
	if (it != clientMap.end() ) {
		const Client* ptr = &sub;
		clientMap[t].remove(ptr);
	}
}

void Broker::publishTopic(const Publisher& pub, const Topic& t) {
	const Client* ptr = &pub;
	clientMap[t].insert(ptr);
}

void Broker::unpublishTopic(const Publisher& pub, const Topic& t) {
	const Client* ptr = &pub;
	clientMap[t].remove(ptr);
}

void Broker::publishMessage(const Topic& t, const std::string& message,
												const Client& sender) const {
	if (clientMap.find(t) != clientMap.end()) {
		SortedSet<const Client*,CompareClients> set = clientMap.at(t);
		for(SortedSet<const Client*,CompareClients>::iterator it = set.begin()
		; it != set.end() ; ++it ) {
			(*it)->receiveMessage(message, t, sender);
		}
	}
	else {
		throw "somthing";
	}
}

void Broker::sendMaintenanceMessageAny(std::list<Topic> t, std::string message){
	SortedSet<const Client*,CompareClients> united = SortedSet<const Client*,CompareClients>();
	for(std::list<Topic>::iterator it = t.begin() ; it != t.end() ; ++it ) {
		united |= clientMap[(*it)];
	}
	for(SortedSet<const Client*,CompareClients>::iterator it = united.begin()
	; it != united.end() ; ++it ) {
		(*it)->receiveMaintenanceMessage(message);
	}
}

void Broker::sendMaintenanceMessageAll(std::list<Topic> t, std::string message){
	SortedSet<const Client*,CompareClients> intersected = clientMap[*(t.begin())];
	for(std::list<Topic>::iterator it = t.begin() ; it != t.end() ; ++it ) {
		intersected &= clientMap[(*it)];
	}
	for(SortedSet<const Client*,CompareClients>::iterator it = intersected.begin()
	; it != intersected.end() ; ++it ) {
		(*it)->receiveMaintenanceMessage(message);
	}
}
