#include "Main.h"
#include "ctpl_stl.h"

// insert game object list here
// List<GameObject> currObjects;

int main(int argc, char *argv[]) { 
	// Create worker thread pool
	ctpl::thread_pool p(16); // number of threads in pool
	// for p.push usage, see the ctpl_stl.h header file


	// create ONE message bus that goes to ALL the systems
	MessageBus* mbus = new MessageBus();

	// create IO system

	// Load Scene

	// while 1 & game interval is up
	/*
	
		for (GameObject obj : currObjects) {
			for (Component c : obj.components) {
				p.push(c.earlyUpdate());
			}
		}

		// wait for all threads to complete
	
		for (GameObject obj : currObjects) {
			for (Component c : obj.components) {
				p.push(c.update());		
			}
		}

		// wait for all threads to complete
	
		for (GameObject obj : currObjects) {
			for (Component c : obj.components) {
				p.push(c.lateUpdate());		
			}
		}

	*/
	
	

	/*
	
	SomeSystem* sysOne = new SomeSystem(mbus); // note that SomeSystem must extend from System. Pass in the message bus we created so that they're all using the same bus
	mbus->someSystem = sysOne;	// add the system to the bus so that the bus has a reference to it
	
	*/


}