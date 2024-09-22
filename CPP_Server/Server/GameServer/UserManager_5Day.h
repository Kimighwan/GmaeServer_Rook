#pragma once
#include <mutex>

class User
{
	// 
};

class UserManager_5Day
{
public:
	static UserManager_5Day* Instance()
	{
		static UserManager_5Day instance;
		return &instance;
	}

	User* GetUser(int32 id)
	{
		lock_guard<mutex> guard(_mutex);
		return nullptr;
	}

	void ProcessSave();

	void Test(){}

private:
	mutex _mutex;
};

