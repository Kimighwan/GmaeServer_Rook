#pragma once

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