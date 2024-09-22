#include "pch.h"
#include "AccountManager_5Day.h"
#include "UserManager_5Day.h"

void AccountManager_5Day::ProcessLogin()
{
	// accountLock
	lock_guard<mutex> guard(_mutex);

	// userLock
	User* user = UserManager_5Day::Instance()->GetUser(100);
}