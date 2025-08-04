#include "pch.h"
#include "CoreGobal.h"
#include "ThreadManager.h"

ThreadManager* GThreadManager = nullptr;
class CoreGobal
{
public:
	CoreGobal()
	{
		GThreadManager = new ThreadManager();
	}
	~CoreGobal()
	{
		delete GThreadManager;
	}
} GCoreGlobal;