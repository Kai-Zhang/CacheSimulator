#ifndef SETASSOCIATIVECACHE_H_
#define SETASSOCIATIVECACHE_H_
#include "BasicCache.h"

class SetAssociativeCache: public BasicCache
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
// Parameter of the set associative cache
	const int numOfGroups;
// Capacity of a cache way
	static const int WAY_NUMBER = 4;
// Replacememt method
	const bool LRUselected;
// I/O units
	struct Block
	{
		bool valid;
		short LRUtag;
		int tag;
		Block () { valid = false; LRUtag = 0; tag = 0; }
	};
// Way units
	struct Group
	{
		Block blocks[WAY_NUMBER];
		Group () { for ( int i=0; i < WAY_NUMBER; ++i ) blocks[i].valid = false, blocks[i].LRUtag = 0, blocks[i].tag = 0; }
	};
	Group* cacheFlags;
// Replacement Algorithm
	virtual void LRUProcess (const int groupIndex,const int blockIndex);
	virtual void LRUReplace (const int groupIndex, const int memoryTag);
	virtual void RandomReplace (const int groupIndex, const int memoryTag);
public:
	SetAssociativeCache (const int cap, const int block, const bool isLRU=false): BasicCache(cap,block), numOfGroups(cap/(block*WAY_NUMBER)), LRUselected(isLRU) { cacheFlags = new Group [numOfGroups]; }
	virtual ~SetAssociativeCache () { delete [] cacheFlags; }

	virtual void read (const unsigned int address);
	virtual void write (const unsigned int address);
};

#endif