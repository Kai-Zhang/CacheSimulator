#include "DirectCache.h"
#include <cmath>

void DirectCache::read (const unsigned int address)
{
	++ accessTimes, ++ readTimes;

	int cacheAddress = address >> (int)(std::log(cacheBlockSize)/std::log(2));
	int blockIndex = cacheAddress % numOfBlocks;
	int memoryTag = cacheAddress / numOfBlocks;
	
	if ( cacheFlags[blockIndex].valid )
		if ( cacheFlags[blockIndex].tag == memoryTag )	// Hit
			// Send Data to Register
			++ hitTimes, ++ readHit; 
		else	// Miss And Replace
			// Load From Memory to Cache
			cacheFlags[blockIndex].tag = memoryTag;
	else { // Miss And Load
		// Load From Memory to Cache
		cacheFlags[blockIndex].valid=true;
		cacheFlags[blockIndex].tag=memoryTag;
	}
}
void DirectCache::write (const unsigned int address)
{
	++ accessTimes, ++ writeTimes;

	int cacheAddress = address >> (int)(std::log(cacheBlockSize)/std::log(2));
	int blockIndex = cacheAddress % numOfBlocks;
	int memoryTag = cacheAddress / numOfBlocks;

	if ( cacheFlags[blockIndex].valid )
		if ( cacheFlags[blockIndex].tag == memoryTag )	// Hit
			// Send Data to Register
			++ hitTimes, ++ writeHit; 
		else	// Miss And Replace
			// Load From Memory to Cache
			cacheFlags[blockIndex].tag = memoryTag;
	else { // Miss And Load
		// Load From Memory to Cache
		cacheFlags[blockIndex].valid=true;
		cacheFlags[blockIndex].tag=memoryTag;
	}
}