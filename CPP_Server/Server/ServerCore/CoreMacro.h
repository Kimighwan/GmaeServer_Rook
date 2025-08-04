#pragma once

#define OUT

/*--------------
	  Lock
--------------*/

#define USE_MANY_LOCKS(count) Lock _locks[count];	// lock을 여러개 사용
#define USE_LOCK			  USE_MANY_LOCKS(1)	// lock을 1개 사용

// 여러개의 lock을 사용시 몇 번째 인덱스의 락을 잡아줄 것이냐
// #을 2개 쓰면 컴파일러가 치환을 해줌
#define READ_LOCK_IDX(idx)	  ReadLockGuard readLockGuard_##idx(_locks[idx]);
#define READ_LOCK			  READ_LOCK_IDX(0)
#define WRITE_LOCK_IDX(idx)	  WriteLockGuard writeLockGuard_##idx(_locks[idx]);
#define WRITE_LOCK			  WRITE_LOCK_IDX(0)

/*--------------
      Crash
--------------*/

#define CRASH(cause)							\
{												\
	int* crash = nullptr;						\
	__analysis_assume(crash != nullptr);		\
	*crash = 0xDEADEEEEF;						\
}

#define ASSERT_CRASH(expr)			\
{									\
	if(!expr)						\
	{								\
		CRASH("ASSERT_CRASH");		\
		__analysis_assume(expr);	\
	}								\
}