#include "FullyAssociativeCache.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

void FullyAssociativeCache::read (const unsigned int address)
{
	++ accessTimes, ++ readTimes;

	int memoryTag = address >> (int)(std::log(cacheBlockSize)/std::log(2));

	bool isAllocated = false;
	for ( int i=0; i < numOfBlocks; ++i )
	{
		if ( cacheFlags[i].valid )	// Hit
			if ( cacheFlags[i].tag == memoryTag )
			{
				// Send Data to Register
				++ hitTimes, ++ readHit, isAllocated = true;
				if ( LRUselected )
					LRUProcess (i);
				break;
			}
		if ( !cacheFlags[i].valid )	// Miss And Load
		{
			cacheFlags[i].valid = true;
			cacheFlags[i].tag = memoryTag;
			isAllocated = true;
			if ( LRUselected )
				LRUProcess (i);
		}
	}

	if ( !isAllocated )	// Miss And Replacement
	{
		if ( LRUselected )
			LRUReplace (memoryTag);
		else
			RandomReplace (memoryTag);
	}
}
void FullyAssociativeCache::write (const unsigned int address)
{
	++ accessTimes, ++ writeTimes;

	int memoryTag = address >> (int)(std::log(cacheBlockSize)/std::log(2));

	bool isAllocated = false;
	for ( int i=0; i < numOfBlocks; ++i )
	{
		if ( cacheFlags[i].valid )	// Hit
			if ( cacheFlags[i].tag == memoryTag )
			{
				// Get Data From Register
				++ hitTimes, ++ writeHit, isAllocated = true;	// Dirty Bit Is Not Used Since Refresh Time Is Not Told
				if ( LRUselected )
					LRUProcess (i);
				break;
			}
		if ( !cacheFlags[i].valid )	// Miss And Write Allocate
		{
			cacheFlags[i].valid = true;
			cacheFlags[i].tag = memoryTag;
			isAllocated = true;
			if ( LRUselected )
				LRUProcess (i);
		}
	}

	if ( !isAllocated )	// Miss And Replacement
	{
		if ( LRUselected )
			LRUReplace (memoryTag);
		else
			RandomReplace (memoryTag);
	}
}
void FullyAssociativeCache::LRUProcess (const int blockIndex)
{
	cacheFlags[blockIndex].LRUtag = 0;	// Recently Used Block
	for ( int i=0; i < numOfBlocks; ++i )	// Others' LRUtag Increment
		if ( cacheFlags[blockIndex].valid )
		{
			if ( i != blockIndex )
				++ cacheFlags[i].LRUtag;
		} else break;	// Break When Firstly Meet Invalid Block
}
void FullyAssociativeCache::LRUReplace (const int memoryTag)
{
	int leastUsedIndex = 0;
	short leastUsedTimes = cacheFlags[0].LRUtag ++;

	for ( int i=1; i < numOfBlocks; ++i )	// Find the Least Used Block
		if ( cacheFlags[i].LRUtag > leastUsedTimes )
		{
			leastUsedTimes = cacheFlags[i].LRUtag ++;
			leastUsedIndex = i;
		}
	
	cacheFlags[leastUsedIndex].tag = memoryTag;	// Replace It
	cacheFlags[leastUsedIndex].LRUtag = 0;
}
void FullyAssociativeCache::RandomReplace (const int memoryTag)
{
	srand (time(0));
	int randIndex = rand() % numOfBlocks;

	cacheFlags[randIndex].tag = memoryTag;
	LRUProcess (randIndex);
}
