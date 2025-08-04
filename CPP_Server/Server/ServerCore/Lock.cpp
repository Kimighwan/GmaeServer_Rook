#include "pch.h"
#include "Lock.h"
#include "CoreTLS.h"
#include "DeadLockProfiler.h"

void Lock::WriteLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif


	// 2
	// 만약 동일한 스레드가 소유하고 있다면 무조건 성곡하는 정책
	// => 재귀적으로 락 잡기 허용 -> 같은 스레드가 같은 락을 잡는 상황
	const uint32 currentLcokThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == currentLcokThreadId)
	{
		_writeCount++;
		return;
	}

	// 1
	// 먼저 생각할 것은 아무도 소유 및 공유하고 있지 않을 때, 경합해서 소유권을 얻는 다.
	// 여기서 소유권은 상호배타적 소유권으로 누군가 Write를 하는 것이고
	// 공유는 누군가 ReadLock을 통해 사용하고 있는 상황이다.
	// 그래서 아무도 공유(소유)하지 않다면 모든 플래그가 0이라는 뜻

	const int64 beginTIck = ::GetTickCount64();
	// 텅비었다면 스레드 id 상위 16비트에 채워주기
	const uint32 desired = ((LThreadId << 16) & WRITE_THREAD_MASK);
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			uint32 expected = EMPTY_FLAG;	// 예상한 값
			if (_lockFlag.compare_exchange_weak(OUT expected, desired))
			{
				// 락 플래그가 빈 상태라고 하면 락 플래그에 나의 스레드 id를 넣을 것인데
				// 빈 상태를 확인하고 넣는 과정 사이에 누군가 개입을 생각하여 CAS 연산을 수행한다.
				// 여기 까지 왔으면 경합에서 이겼기 때문에 lock을 흭득한 상태이다.
				_writeCount++;
				return;
			}
		}

		if (::GetTickCount64() - beginTIck >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		this_thread::yield();
	}
}

void Lock::WriteUnLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif

	// ReadLock 다 풀기 전에는 WriteUnLock 불가능
	// 지금 구현하는 정책은 R -> W를 불가능하기 때문에...
	// 그러면 WirteLock에서 구현해야 하는 게 아님??
	if ((_lockFlag.load() & READ_THREAD_MASK) != 0)
		CRASH("INVALID_UNLOCKL_ORDER");

	const int32 lockCount = --_writeCount;
	if (lockCount == 0)
		_lockFlag.store(EMPTY_FLAG);
}

void Lock::ReadLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PushLock(name);
#endif

	// 동일한 스레드가 소유하고 있다면 가능
	const uint32 currentLcokThreadId = (_lockFlag.load() & WRITE_THREAD_MASK) >> 16;
	if (LThreadId == currentLcokThreadId)
	{
		_lockFlag.fetch_add(1);
		return;
	}

	const int64 beginTIck = ::GetTickCount64();
	// 아무도 소유(WriteLock)하고 있지 않을 때 경합해서 공유 카운트를 올린다.
	// 하위 16비트에 해당 카운트를 저장함
	while (true)
	{
		for (uint32 spinCount = 0; spinCount < MAX_SPIN_COUNT; spinCount++)
		{
			// read 마스크니 상위 16비트는 0 -> 아무도 write 하지 않는 상황을 예상
			uint32 expected = (_lockFlag.load() & READ_THREAD_MASK);	// 예상한 값
			// 예상한 값이 맞다면 expected+1 한 값으로 바꿔치기 해줌
			if (_lockFlag.compare_exchange_weak(OUT expected, expected + 1))
				return;
		}

		if (::GetTickCount64() - beginTIck >= ACQUIRE_TIMEOUT_TICK)
			CRASH("LOCK_TIMEOUT");

		// 여기 왔으면 실패했으니 5천번 돌아서 실패를 하면 그냥 일단 스레드를 잠재운다.
		this_thread::yield();
	}
}

void Lock::ReadUnLock(const char* name)
{
#if _DEBUG
	GDeadLockProfiler->PopLock(name);
#endif

	if ((_lockFlag.fetch_sub(1) & READ_THREAD_MASK) == 0)
		CRASH("MULTIPLE_UNLOCK");
}
