#include "IOSystem.h"


IOSystem::IOSystem(MessageBus* mbus) : System(mbus) {

}


IOSystem::~IOSystem() {
}

// checks keypresses
void IOSystem::checkKeyPresses() {
	Msg* m = new Msg(UPDATE_TEST_OBJ, "");

	if (GetKeyState('W') & 0x8000) {
		m->data += "W";
	}
	
	if (GetKeyState('S') & 0x8000) {
		m->data += "S";
	}
	
	if (GetKeyState('A') & 0x8000) {
		m->data += "A";
	}
	
	if (GetKeyState('D') & 0x8000) {
		m->data += "D";
	}

	if (GetKeyState('Z') & 0x8000) {
		m->data += "Z";
	}

	msgBus->postMessage(m);
}


// Currently the IO system doesn't really do anything other than check IO, there should be no messages for it to handle
// theorietically the IO system jsut posts messages
void IOSystem::handleMessage(Msg *msg) {
	// call the parent first 
	System::handleMessage(msg);
	// personal call 
	switch (msg->type) {
	default:
		break;
	}
}