#include "pch.h"
#include "CoreGobal.h"
#include "ThreadManager.h"

ThreadManager* GThreadManager = nullptr;

CoreGobal::CoreGobal()
{
	GThreadManager = new ThreadManager();
}

CoreGobal::~CoreGobal()
{
	delete GThreadManager;
}
