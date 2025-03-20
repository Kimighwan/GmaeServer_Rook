#include "pch.h"
#include <iostream>

#include <thread>
#include <atomic>
#include <mutex>

#include "UserManager_5Day.h"
#include "AccountManager_5Day.h"

mutex m;
int sum = 0;

class SpinLock{
public:
    void lock() 
    {
        // CAS(Compare and Swap) 
        bool expected = false;
        bool desired = true;

        //// CAS 의사 코드
        //if (_locked == expected)   // 이렇게 locked랑 expected가 같을 수 있고
        //{
        //    expected = _locked; // ???? 밑에 이전에 이렇게 넣어주는데 이건 이해가 안됨
        //    _locked = desired;
        //    return true;
        //}
        //else                       // 다를 수 있지?
        //{
        //    expected = _locked; // ???? 밑에 이전에 이렇게 넣어주는데 이건 이해가 안됨
        //    return false;
        //}

        while (_locked.compare_exchange_strong(expected, desired) == false) 
        {
            expected = false;

            //this_thread::sleep_for(std::chrono::milliseconds(100));
            this_thread::sleep_for(100ms);
            //this_thread::yield();
        }


        
        /*while (_locked)  // 잘 못된 코드 // 위의 CAS를 이용한 것이 올바른 코드
        {

        }

        _locked = true;*/
    }

    void unlock()
    {
        _locked = false;
    }

private:
    atomic<bool> _locked = false;
};


void Add()
{
    for (int32 i = 0; i < 10000; i++) {
        lock_guard<mutex> guard(m);
        sum++;
    }
}


void Sub()
{
    for (int32 i = 0; i < 10000; i++)
    {
        lock_guard<mutex> guard(m);
        sum--;
    }

}

int main()
{
    thread t1(Add);
    thread t2(Sub);

    t1.join();
    t2.join();

    cout << sum << "\n";


   /* t.hardware_concurrency();
    t.get_id();
    t.detach();
    t.joinable();
    t.join();*/


}
