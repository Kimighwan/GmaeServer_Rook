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
		// �հ� �۾�,,, �հ� ���� ��
		return nullptr;
	}

	void ProcessLogin();
private:
	mutex _mutex;
	// �پ��� ������.. ���� ������, �̸� ���
};

