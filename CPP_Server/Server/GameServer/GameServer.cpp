#include "pch.h"
#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>
#include <Windows.h>

#include "UserManager_5Day.h"
#include "AccountManager_5Day.h"

#pragma region Chap1 ~ Chap9(SpinLock)

//mutex m;
//int sum = 0;

//class SpinLock {
//public:
//    void lock()
//    {
//        // CAS(Compare and Swap) 
//        bool expected = false;
//        bool desired = true;
//
//        //// CAS 의사 코드
//        //if (_locked == expected)   // 이렇게 locked랑 expected가 같을 수 있고
//        //{
//        //    expected = _locked; // ???? 밑에 이전에 이렇게 넣어주는데 이건 이해가 안됨
//        //    _locked = desired;
//        //    return true;
//        //}
//        //else                       // 다를 수 있지?
//        //{
//        //    expected = _locked; // ???? 밑에 이전에 이렇게 넣어주는데 이건 이해가 안됨
//        //    return false;
//        //}
//
//        while (_locked.compare_exchange_strong(expected, desired) == false)
//        {
//            expected = false;
//
//            //this_thread::sleep_for(std::chrono::milliseconds(100));
//            this_thread::sleep_for(100ms);
//            //this_thread::yield();
//        }
//
//
//
//        /*while (_locked)  // 잘 못된 코드 // 위의 CAS를 이용한 것이 올바른 코드
//        {
//
//        }
//
//        _locked = true;*/
//    }
//
//    void unlock()
//    {
//        _locked = false;
//    }
//
//private:
//    atomic<bool> _locked = false;
//};
//
//
//void Add()
//{
//    for (int32 i = 0; i < 10000; i++) {
//        lock_guard<mutex> guard(m);
//        sum++;
//    }
//}
//
//
//void Sub()
//{
//    for (int32 i = 0; i < 10000; i++)
//    {
//        lock_guard<mutex> guard(m);
//        sum--;
//    }
//
//}
//
//int main()
//{
//    thread t1(Add);
//    thread t2(Sub);
//
//    t1.join();
//    t2.join();
//
//    cout << sum << "\n";
//
//
//    /* t.hardware_concurrency();
//     t.get_id();
//     t.detach();
//     t.joinable();
//     t.join();*/
//
//
//}

#pragma endregion

#pragma region Chap10(Event)

//mutex m;
//queue<int32> q;	// 공용데이터를 관리할 큐
//HANDLE handle;
//
//void Producer()	// 계속 데이터를 만드는 스레드 역할
//{
//	while (true) 
//	{
//		::WaitForSingleObject(handle, INFINITE);
//		//::ResetEvent(handle);
//
//		{
//			unique_lock<mutex> lock(m);
//			q.push(100);
//		}
//
//		// 예시 1번) 한 쪽에서 100ms마다 데이터를 큐에 밀어 넣기
//		// this_thread::sleep_for(100ms); 
//		
//		::SetEvent(handle);
//
//		// 예시 2번) 이번에는 진짜 어쩌다가 한 번씩 데이터를 추가하는 상황
//		this_thread::sleep_for(1000000000ms);	
//	}
//}
//
//void Consumer()// 만들어진 데이터를 계속 소모하는 스레드 역할 
//{
//	while (true)
//	{
//		unique_lock<mutex> lock(m);
//		if (!q.empty())
//		{
//			int32 data = q.front();
//			q.pop();
//			cout << data << endl;
//		}
//	}
//}
//
//int main()
//{
//	handle = ::CreateEvent(NULL, FALSE, FALSE, NULL);
//
//	thread t1(Producer);
//	thread t2(Consumer);
//
//	t1.join();
//	t2.join();
//}

#pragma endregion



