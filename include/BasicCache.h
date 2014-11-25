#ifndef BASICCACHE_H_
#define BASICCACHE_H_

class BasicCache
{
protected:
// Parameter of cache
	const int capacity;
	const int cacheBlockSize;
	const int numOfBlocks;

// Counters
	int accessTimes;
	int readTimes, writeTimes;
	int hitTimes;
	int readHit, writeHit;
public:
	BasicCache (const int cap, const int block): capacity(cap), cacheBlockSize(block), numOfBlocks(cap/block),\
							accessTimes(0), readTimes(0), writeTimes(0), hitTimes(0), readHit(0), writeHit(0) {  }
	virtual ~BasicCache () {  }

// Cache I/O
	virtual void read (const unsigned int address) = 0;
	virtual void write (const unsigned int address) = 0;

// Counters-Get Methods
	virtual int getAccessTimes () const { return accessTimes; }
	virtual int getReadTimes () const { return readTimes; }
	virtual int getWriteTimes () const { return writeTimes; }
	virtual int getHitTimes () const { return hitTimes; }
	virtual int getReadHitTimes () const { return readHit; }
	virtual int getWriteHitTimes () const { return writeHit; }

// Reset
	virtual void reset () { accessTimes = readTimes = writeTimes = hitTimes = readHit = writeHit = 0; }
};

#endif