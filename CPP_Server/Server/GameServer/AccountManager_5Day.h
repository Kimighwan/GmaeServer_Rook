#pragma once
#include <mutex>

class Account 
{

};

class AccountManager_5Day
{
public:
	static AccountManager_5Day* Instance()
	{
		static AccountManager_5Day instance;
		return &instance;
	}
	Account* GetAccount(int32 id)
	{
		lock_guard<mutex> guard(_mutex);
		// 먼가 작업,,, 먼가 갖고 옴
		return nullptr;
	}

	void ProcessLogin();
private:
	mutex _mutex;
	// 다양한 정보들.. 계정 생성일, 이름 등등
};

