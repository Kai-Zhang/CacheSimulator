#ifndef DIRECTCACHE_H_
#define DIRECTCACHE_H_
#include "BasicCache.h"

class DirectCache: public BasicCache
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
// I/O units
	struct Block
	{
		bool valid;
		int tag;
		Block () { valid = false; tag = 0; }
	};
	Block* cacheFlags;
public:
	DirectCache (const int cap, const int block): BasicCache(cap,block) { cacheFlags = new Block [numOfBlocks]; }
	virtual ~DirectCache () { delete [] cacheFlags; }

	virtual void read (const unsigned int address);
	virtual void write (const unsigned int address);
};

#endif