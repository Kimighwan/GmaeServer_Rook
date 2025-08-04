#include "pch.h"
#include <iostream>
#include <CorePch.h>
#include <atomic>
#include <mutex>
#include <windows.h>
#include <future>
//#include "UserManager_5Day.h" 
//#include "AccountManager_5Day.h"
//#include "ConcurrentStack.h"
//#include "ConcurrentQueue.h"
#include "CoreMacro.h"
#include "ThreadManager.h"

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

#pragma region Chap11(Condition Variable)
//
//condition_variable cv;
//
//mutex m;
//queue<int32> q;                                            
//HANDLE handle;
//
//void Producer()
//{
//	while (true) 
//	{
//		/*1) Lock 잡기
//		2) 공유 변수 값 작업 - 데이터 작업
//		3) Lock 풀기
//		4) 조건 변수를 통해 다른 쓰레드에게 통지*/
//
//		{
//			unique_lock<mutex> lock(m);
//			q.push(100);
//		}
//
//		 
//	}
//}
//
//void Consumer()
//{
//	while (true)
//	{
//		unique_lock<mutex> lock(m);
//
//		cv.wait(lock, []() {return q.empty() == false; });
//
//		//if (!q.empty())
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

#pragma region Chap12 - Future

//#include <future>
//
//int result;
//
//int Calculate()
//{
//	int sum = 0;
//
//	for (int i = 0; i < 100'000; i++)\
//		sum += i;
//
//	result = sum; // 방법 2
//
//	return sum;
//}
//
//void PromiseWorker(promise<string>&& promise)
//{
//	promise.set_value("Secret Message");
//}
//
//void TaskWorker(packaged_task<int(void)> && task)
//{
//	task();
//}
//
//int main() {
//
//	//// 동기(synchronous)실행
//	//int sum = Calculate();
//	//cout << sum << endl;
//
//	////방법 1 - start
//	//thread t(Calculate());
//
//	//// TODO
//
//	//t.join();
//	//// 방법 1 - end
//
//	
//	// std::future
//
//	future<int> future = async(launch::async, Calculate);
//
//	// TODO
//	future_status status = future.wait_for(1ms);
//	if (status == future_status::ready)
//	{
//
//	}
//
//	int sum = future.get();
//
//
//	class Knight
//	{
//	public:
//		int GetHp() { return 100; }
//	};
//
//	Knight knight;
//	std::future<int> future2 = async(launch::async, &Knight::GetHp, knight);	// Kngiht::GetHp() 와 같다.
//
//	// std::promise
//	{ 
//		promise<string> promise;
//		std::future<string> future3 = promise.get_future();
//
//		thread t3(PromiseWorker, move(promise));
//
//		string m = future3.get();
//		cout << m;
//
//		t3.join();
//	}
//
//
//	// std::packaged_task
//	{
//		packaged_task<int(void)> task(Calculate);
//		std::future<int> future4 = task.get_future();
//
//		thread t4(TaskWorker, move(task));
//
//		int sum4 = future4.get();
//		cout << sum4;
//	}
//}

#pragma endregion

#pragma region  Chap13 - Chach

//int buffer[10000][10000];
//
//int main() {
//	memset(buffer, 0, sizeof(buffer));
//
//
//	{
//		auto start = GetTickCount64();
//
//		int sum = 0;
//		for (int i = 0; i < 10000; i++)
//			for (int j = 0; j < 10000; j++)
//				sum += buffer[i][j];
//
//		auto end = GetTickCount64();
//		cout << "TIme : " << (end - start) << endl;
//	}
//	// 더 빠름
//
//	{
//		auto start = GetTickCount64();
//
//		int sum = 0;
//		for (int i = 0; i < 10000; i++)
//			for (int j = 0; j < 10000; j++)
//				sum += buffer[j][i];
//
//		auto end = GetTickCount64();
//		cout << "TIme : " << (end - start) << endl;
//	}
//	// 더 느림
//}

#pragma endregion

#pragma region Chap14 - CPU Pipeㅣine

//int x = 0;
//int y = 0;
//int r1 = 0;
//int r2 = 0;
//
//volatile bool ready;
//
//void Thread_1() 
//{
//	if (!ready) return;
//
//	y = 1;	// Store y
//	r1 = x;	// Load x
//}
//
//void Thread_2()
//{
//	if (!ready) return;
//
//	x = 1;	// Store x
//	r2 = y;	// Load y
//}
//
//int main() {
//	int cnt = 0;
//
//	while (1)
//	{
//		ready = false;
//
//		cnt++;
//
//		x = y = r1 = r2 = 0;
//
//		thread t1(Thread_1);
//		thread t2(Thread_2);
//
//		ready = true;
//
//		t1.join();
//		t2.join();
//
//		if (r1 == 0 && r2 == 0)
//			break;
//	}
//
//	cout << cnt << "번 만에 빠져나옴";
//}

#pragma endregion

#pragma region Chap15 - Memory Model

//atomic<bool> ready;
//int value;
//
//void Productor() {
//	value = 10;
//	ready.store(true, memory_order_seq_cst);
//
//	// ready.store(true, memory_order_release); 2버전
//}
//
//void Consumer() {
//	// 준비가 될 때 까지 기다린다.
//	while (ready.load(memory_order_seq_cst) == false)
//		;
//
//	// ready.store(true, memory_order_acquire); 2버전
//
//	cout << value << "\n";
//}
//
//atomic<bool> flag;
//
//int main() 
//{
//	flag = false;
//
//	//flag = true
//	flag.store(true, memory_order_seq_cst);
//
//	// bool val = flag;
//	bool val = flag.load(memory_order_seq_cst);
//
//	// 이전 flag 값을 prev에 넣고, flag 값을 수정
//	{
//		// 하지만 여기에서 다른 쓰레드가 접근해 수정하는 상황이 있다면,
//		// prev의 값은 유효하지 않겠지?
//		/*bool prev = flag;
//		flag = true;*/
//
//		// 쓰기를 하면서 동시에 값을 뺀다.
//		bool prev = flag.exchange(true);
//	}
//
//	// CAS(Compare-And-Swap) 조건부 수정
//	{
//		bool expected = false;
//		bool desired = true;
//		flag.compare_exchange_strong(expected, desired);
//
//		// 위는 아래 상황이다
//		if (flag == expected)
//		{
//			flag = desired;
//			return true;
//		}
//		else
//		{
//			expected = true;
//			return false;
//		}
//	}
//
//#pragma region 정책 설명
//
//	ready = false;
//	value = 0;
//	thread t1(Productor);
//	thread t2(Consumer);
//	t1.join();
//	t2.join();
//
//
//#pragma endregion
//
//}

#pragma endregion

#pragma region Chap16 - Thread Local Storage
//
//thread_local int LThreadid = 0;
//
//void ThreadMain(int threadId)
//{
//	LThreadid = threadId;
//
//	while (true)
//	{
//		cout << "Hi! I am Thread" << LThreadid << "\n";
//		this_thread::sleep_for(1s);
//	}
//}
//
//int main() {
//	vector<thread> threads;
//
//	for (int i = 0; i < 10; i++)
//	{
//		int id = i + 1;
//		threads.push_back(thread(ThreadMain, id));
//	}
//
//
//	for (thread& t : threads)
//		t.join();
//}

#pragma endregion

#pragma region Chap17 - Lock_Based Stack/Queue
//
//LockStack<int> s;
//LockQueue<int> q;
//
//void Push()
//{
//	while (true)
//	{
//		int value = rand() % 100;
//		q.Push(value);
//
//		this_thread::sleep_for(10ms);
//	}
//}
//
//void Pop() {
//	while (true)
//	{
//		int data = 0;
//		if (q.TryPop(OUT data)/*q.WaitPop(OUT data)*/)
//			cout << data << "\n";
//	}
//}
//
//int main() {
//	thread t1(Push);
//	thread t2(Pop);
//	thread t3(Pop);
//
//	t1.join();
//	t2.join();
//	t3.join();
//}

#pragma endregion

#pragma region  Chap18 - Lock_Free Stack
//
//LockFreeStack<int> s;
//
//void Push()
//{
//	while (true)
//	{
//		int value = rand() % 100;
//		s.Push(value);
//
//		//this_thread::sleep_for(10ms); 극한의 상황을 보기 위해 주석 처리
//	}
//}
//
//void Pop() {
//	while (true)
//	{
//		int data = 0;
//		if (s.TryPop(OUT data)/*q.WaitPop(OUT data)*/)
//			cout << data << "\n";
//	}
//}
//
//int main() {
//	thread t1(Push);
//	thread t2(Pop);
//	thread t3(Pop);
//
//	t1.join();
//	t2.join();
//	t3.join();
//}

#pragma endregion

#pragma region Chap18 - Lock_Free_Smart_Pointer
//
////LockFreeStack<int> s;
//LockFreeStackSmartPointer<int> s;
//
//void Push()
//{
//	while (true)
//	{
//		int value = rand() % 100;
//		s.Push(value);
//		//this_thread::sleep_for(10ms); 극한의 상황을 보기 위해 주석 처리
//	}
//}
//
//void Pop() {
//	while (true)
//	{
//		auto dat = s.TryPop();
//	}
//}
//
//int main() {
//	thread t1(Push);
//	thread t2(Pop);
//	thread t3(Pop);
//
//	t1.join();
//	t2.join();
//	t3.join();
//}

#pragma endregion

#pragma region Chap24 - ThreadManager

//CoreGobal Core;
//
//void ThreadMain() {
//	while (true) {
//		cout << "Hi i am thread.." << LThreadId << endl;
//		this_thread::sleep_for(1s);
//	}
//}
//
//int main() {
//	//CRASH("test");
//	//int a = 3;
//	//ASSERT_CRASH(a != 3);
//
//	for (int i = 0; i < 5; i++)
//		GThreadManager->Launch(ThreadMain);
//	GThreadManager->Join();
//}

#pragma endregion

#pragma region Chap25 - Reader_Write Lock

class TestLock 
{
	USE_LOCK;

public:
	int32 TestRead()
	{
		READ_LOCK;

		if (_queue.empty())
			return -1;

		return _queue.front();
	}

	void TestPush()
	{
		WRITE_LOCK;

		_queue.push(rand() % 100);
	}

	void TestPop()
	{
		WRITE_LOCK;

		if (_queue.empty() == false)
			_queue.pop();
	}
private:
	queue<int32> _queue;
};

TestLock testLock;

void ThreadWrite()
{
	while (true)
	{
		testLock.TestPush();
		this_thread::sleep_for(1ms);
		testLock.TestPop();
	}
}

void ThreadRead()
{
	while (true)
	{
		int value = testLock.TestRead();
		cout << value << endl;
		this_thread::sleep_for(1ms);
	}
}

int main() {
	for (int i = 0; i < 2; i++)
		GThreadManager->Launch(ThreadWrite);

	for (int i = 0; i < 5; i++)
		GThreadManager->Launch(ThreadRead);

	GThreadManager->Join();
}

#pragma endregion
