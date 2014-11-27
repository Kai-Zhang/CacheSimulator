#include "SetAssociativeCache.h"
#include <cmath>
#include <cstdlib>
#include <ctime>

void SetAssociativeCache::read (const unsigned int address)
{
	++ accessTimes, ++ readTimes;

	int cacheAddress = address >> (int)(std::log(cacheBlockSize*WAY_NUMBER)/std::log(2));
	int groupIndex = cacheAddress % numOfGroups;
	int memoryTag = cacheAddress / numOfGroups;

	bool isAllocated = false;
	for ( int i=0; i < WAY_NUMBER; ++i )
	{
		if ( cacheFlags[groupIndex].blocks[i].valid )	// Hit
			if ( cacheFlags[groupIndex].blocks[i].tag == memoryTag )
			{
				// Send Data to Register
				++ hitTimes, ++ readHit, isAllocated = true;
				if ( LRUselected )
					LRUProcess (groupIndex,i);
				break;
			}
		if ( !cacheFlags[groupIndex].blocks[i].valid )	// Miss And Load
		{
			cacheFlags[groupIndex].blocks[i].valid = true;
			cacheFlags[groupIndex].blocks[i].tag = memoryTag;
			isAllocated = true;
			if ( LRUselected )
				LRUProcess (groupIndex,i);
		}
	}

	if ( !isAllocated )	// Miss And Replacement
	{
		if ( LRUselected )
			LRUReplace (groupIndex,memoryTag);
		else
			RandomReplace (groupIndex,memoryTag);
	}
}
void SetAssociativeCache::write (const unsigned int address)
{
	++ accessTimes, ++ writeTimes;

	int cacheAddress = address >> (int)(std::log(cacheBlockSize*WAY_NUMBER)/std::log(2));
	int groupIndex = cacheAddress % numOfGroups;
	int memoryTag = cacheAddress / numOfGroups;

	bool isAllocated = false;
	for ( int i=0; i < WAY_NUMBER; ++i )
	{
		if ( cacheFlags[groupIndex].blocks[i].valid )	// Hit
			if ( cacheFlags[groupIndex].blocks[i].tag == memoryTag )
			{
				// Get Data From Register
				++ hitTimes, ++ writeHit, isAllocated = true;	// Dirty Bit Is Not Used Since Refresh Time Is Not Told
				if ( LRUselected )
					LRUProcess (groupIndex,i);
				break;
			}
		if ( !cacheFlags[groupIndex].blocks[i].valid )	// Miss And Write Allocate
		{
			cacheFlags[groupIndex].blocks[i].valid = true;
			cacheFlags[groupIndex].blocks[i].tag = memoryTag;
			isAllocated = true;
			if ( LRUselected )
				LRUProcess (groupIndex,i);
		}
	}

	if ( !isAllocated )	// Miss And Replacement
	{
		if ( LRUselected )
			LRUReplace (groupIndex,memoryTag);
		else
			RandomReplace (groupIndex,memoryTag);
	}
}
void SetAssociativeCache::LRUProcess (const int groupIndex,const int blockIndex)
{
	cacheFlags[groupIndex].blocks[blockIndex].LRUtag = 0;	// Recently Used Block
	for ( int i=0; i < WAY_NUMBER; ++i )	// Others' LRUtag Increment
		if ( cacheFlags[groupIndex].blocks[blockIndex].valid )
		{
			if ( i != blockIndex )
				++ cacheFlags[groupIndex].blocks[i].LRUtag;
		} else break;	// Break When Firstly Meet Invalid Block
}
void SetAssociativeCache::LRUReplace (const int groupIndex, const int memoryTag)
{
	int leastUsedIndex = 0;
	short leastUsedTimes = cacheFlags[groupIndex].blocks[0].LRUtag ++;

	for ( int i=1; i < WAY_NUMBER; ++i )	// Find the Least Used Block
		if ( cacheFlags[groupIndex].blocks[i].LRUtag > leastUsedTimes )
		{
			leastUsedTimes = cacheFlags[groupIndex].blocks[i].LRUtag ++;
			leastUsedIndex = i;
		} else {
			++ cacheFlags[groupIndex].blocks[i].LRUtag;
		}
	
	cacheFlags[groupIndex].blocks[leastUsedIndex].tag = memoryTag;	// Replace It
	cacheFlags[groupIndex].blocks[leastUsedIndex].LRUtag = 0;
}
void SetAssociativeCache::RandomReplace (const int groupIndex, const int memoryTag)
{
	srand (time(0));
	int randIndex = rand() % WAY_NUMBER;

	cacheFlags[groupIndex].blocks[randIndex].tag = memoryTag;
	LRUProcess (groupIndex,randIndex);
}
