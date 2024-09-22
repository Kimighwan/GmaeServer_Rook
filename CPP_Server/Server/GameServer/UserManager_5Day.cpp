#include "pch.h"
#include "UserManager_5Day.h"
#include "AccountManager_5Day.h"

void UserManager_5Day::ProcessSave()
{
	// userLock
	lock_guard<mutex> guard(_mutex);

	// accountLock
	Account* account = AccountManager_5Day::Instance()->GetAccount(100);
	// Test
	
}
