#ifndef SIMULATIONSHELL_H_
#define SIMULATIONSHELL_H_
#include <iostream>
#include "BasicCache.h"
#include "DirectCache.h"
#include "FullyAssociativeCache.h"
#include "SetAssociativeCache.h"

class SimulationShell
{
protected:
	BasicCache* cache;
public:
	SimulationShell (): cache(NULL) { }
	~SimulationShell () { if ( cache != NULL ) delete cache; std::cout << ""; std::cout << "+------------------------------- Goodbye! --------------------------------+\n"; }

	void start ();
	void showGreetings () const;
	void initializeCache ();
	void dataProcess ();
	void showCacheData () const;
};

#endif
