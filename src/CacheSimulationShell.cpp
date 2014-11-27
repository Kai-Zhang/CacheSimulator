#include "CacheSimulationShell.h"
#include <cstdlib>
#include <cmath>
#include <fstream>
#include <ctime>

void SimulationShell::start ()
{
	showGreetings ();
	initializeCache ();
	char loopContinue = 'Y';
	while ( loopContinue == 'Y' || loopContinue == 'y' )
	{
		dataProcess ();
		showCacheData ();
		std::cout << " Clear the cache and process another file? ( Y for yes, others for no )\n"
				<< " > ";
		std::cin >> loopContinue;
		cache->reset ();
	}
}
void SimulationShell::showGreetings () const
{
	std::cout << "+--------------------------- Cache Simulation ----------------------------+" << std::endl;
	std::cout << "  Welcome to Cache Simulation Program" << std::endl;
	std::cout << "  Follow the orders to build a cache" << std::endl << std::endl;
}
void SimulationShell::initializeCache ()
{
	// Get Cache Size
	std::cout << " Please input the size of cache:\n"
			<< "   Unit: KB\n"
			<< "   Notice: The input will be automatically transfromed into the power of 2\n"
			<< " > ";
	int cacheSize = 0;
	std::cin >> cacheSize;
	cacheSize = (int)(std::log(cacheSize)/std::log(2));
	cacheSize = std::pow(2,cacheSize) * 1024;

	// Get Cacheline ( Block ) Size
	std::cout << " Please input the size of cacheline:\n"
			<< "   Unit: Byte\n"
			<< "   Notice: The input will be automatically transfromed into the power of 2\n"
			<< " > ";
	int blockSize = 0;
	std::cin >> blockSize;
	blockSize = (int)(std::log(blockSize)/std::log(2));
	blockSize = std::pow(2,blockSize);

	// Get Cache Type
	std::cout << " Please select the type of cacheline:\n"
			<< "   1. Direct-Mapped.\n"
			<< "   2. 4-Way Set-Associative.\n"
			<< "   3. Fully-Associative.\n"
			<< " > ";
	char cacheType = 0;
	std::cin >> cacheType;
	
	// Build A Cache
	switch ( cacheType )
	{
	case '1': cache = new DirectCache (cacheSize,blockSize); break;
	case '2': std::cout << " Please select the method of replacement:\n"
				<< "   1. Least Recently Used\n"
				<< "   2. Random\n"
				<< " > ";
			std::cin >> cacheType;
			cache = new SetAssociativeCache (cacheSize,blockSize,cacheType == '1'); break;
	case '3': std::cout << " Please select the method of replacement:\n"
				<< "   1. Least Recently Used\n"
				<< "   2. Random\n"
				<< " > ";
			std::cin >> cacheType;
			cache = new FullyAssociativeCache (cacheSize,blockSize,cacheType == '1'); break;
	default: std::cerr << " Sorry your choice is invalid.\n"; exit(-1); break;
	}
}
void SimulationShell::dataProcess ()
{
	// Open the instruction file
	std::cout << " Please input the name of instruction file.\n"
			<< "   Default path: testcase/\n"
			<< "   Notice: Absolute path is acceptable.\n     But '\\' must be replaced by '\\\\' mannually.\n"
			<< " > ";
	char path [50] = "testcase/";
	std::cin >> (path + 9);
	std::cout << " Opening the file of instructions...\n";
	std::ifstream readInstr (path);
	if ( !readInstr )
	{
		std::cerr << " File don't exist!\n";
		return;
	}
	std::cout << " Done.\n";

	// Read in the instruction
	std::cout << " Start instruction processing...\n";
	long startTime = time (0);
	char type = 'l'; unsigned int address = 0;
	while ( readInstr >> type && ! readInstr.eof () )
	{
		readInstr >> std::hex >> address;
		if ( type == 'l' )
			cache->read (address);
		else if ( type == 's' )
			cache->write (address);
	}
	long endTime = time (0);
	std::cout << " Instructions proceeded. Time Used: " << endTime - startTime << "s\n";
}
void SimulationShell::showCacheData () const
{
	std::cout << " ----------------------------- Data Output ------------------------------\n";
	std::cout << " Number of cache access: " << cache->getAccessTimes () << std::endl
			<< " Number of cache load: " << cache->getReadTimes () << std::endl
			<< " Number of cache store: " << cache->getWriteTimes () << std::endl;
	if ( cache->getAccessTimes () == 0 )
		return;
	std::cout << " Average cache hit rate: " << 100.0*cache->getHitTimes () / cache->getAccessTimes () << "%" << std::endl;
	if ( cache->getReadTimes () == 0 )
		return;
	std::cout << " cache hit rate for loads: " << 100.0*cache->getReadHitTimes () / cache->getReadTimes () << "%" << std::endl;
	if ( cache->getWriteTimes () == 0 )
		return;
	std::cout << " cache hit rate for stores: " << 100.0*cache->getWriteHitTimes () / cache->getWriteTimes () << "%"  << std::endl;
	std::cout << " ------------------------------------------------------------------------\n";
}
