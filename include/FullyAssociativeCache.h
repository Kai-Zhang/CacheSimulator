#ifndef FULLYASSOCIATIVECACHE_H_
#define FULLYASSOCIATIVECACHE_H_
#include "BasicCache.h"

class FullyAssociativeCache: public BasicCache
{
protected:
	// Data imported from basic class
	/*
	struct Block
	{
		bool valid;
		bool LRUjudge;
		int tag;
	};
	const int capacity;
	const int cacheBlockSize;
	const int numOfBlocks;

	int accessTimes;
	int readTimes, writeTimes;
	int hitTimes;
	int readHit, writeHit;
	*/
// Replacement Method
	const bool LRUselected;
// I/O units
	struct Block
	{
		bool valid;
		short LRUtag;
		int tag;
		Block () { valid = false; LRUtag = 0; tag = 0; }
	};
	Block* cacheFlags;
// Replacement Algorithm
	virtual void LRUProcess (const int blockIndex);
	virtual void LRUReplace (const int memoryTag);
	virtual void RandomReplace (const int memoryTag);
public:
	FullyAssociativeCache (const int cap, const int block, const bool isLRU=false): BasicCache(cap,block), LRUselected(isLRU) { cacheFlags = new Block [numOfBlocks]; }
	virtual ~FullyAssociativeCache () { delete [] cacheFlags; }

	virtual void read (const unsigned int address);
	virtual void write (const unsigned int address);
};

#endif
