#pragma once
#include "Types.h"

/*------------------
	RW SpinLock
-------------------*/

/*----------------------------------------------
[WWWWWWWW][WWWWWWWW][RRRRRRRR][RRRRRRRR]
W : WriteFlag (Exclusive Lock Owner Thraead ID)
R : ReadFlag  (Shared Lock Count)
----------------------------------------------*/

class Lock
{
	enum : uint32
	{
		ACQUIRE_TIMEOUT_TICK = 10000,		// 최대로 기다려줄 틱
		MAX_SPIN_COUNT = 5000,				// 스핀을 최대 몇 번 돌 것인가
		WRITE_THREAD_MASK = 0xFFFF'0000,	// 상위 16비트를 뽑아내기 위한 마스크
		READ_THREAD_MASK = 0x0000'FFFF,		// 하위 16비트를 뽑아내기 위한 마스크
		EMPTY_FLAG = 0x0000'0000
	};

public:
	void WriteLock();
	void WriteUnLock();
	void ReadLock();
	void ReadUnLock();

private:
	Atomic<uint32> _lockFlag = EMPTY_FLAG;
	uint16 _writeCount = 0;	// 타입은 16비트로 충분할 거 같아서 정했음
};

/*------------------
	LockGuard
-------------------*/

//RAII 패턴
class ReadLockGuard
{
public:
	ReadLockGuard(Lock& lock) : _lock(lock) { _lock.ReadLock(); }
	~ReadLockGuard() { _lock.ReadUnLock(); }
private:
	Lock& _lock;
};

class WriteLockGuard
{
public:
	WriteLockGuard(Lock& lock) : _lock(lock) { _lock.WriteLock(); }
	~WriteLockGuard() { _lock.WriteUnLock(); }
private:
	Lock& _lock;
};