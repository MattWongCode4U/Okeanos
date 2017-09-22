#pragma once
#include "Msg.h"
#include "System.h"
#include <list>

class System;

class MessageBus {
public:
	MessageBus();
	~MessageBus();

	void postMessage(Msg* m);
	void addSystem(System s);
private:
	std::list<System> systemList;
};

