#include "pch.h"
#include "CoreGobal.h"
#include "ThreadManager.h"
#include "DeadLockProfiler.h"

ThreadManager* GThreadManager = nullptr;
DeadLockProfiler* GDeadLockProfiler = nullptr;

class CoreGobal
{
public:
	CoreGobal()
	{
		GThreadManager = new ThreadManager();
		GDeadLockProfiler = new DeadLockProfiler();
	}
	~CoreGobal()
	{
		delete GThreadManager;
		delete GDeadLockProfiler;
	}
} GCoreGlobal;