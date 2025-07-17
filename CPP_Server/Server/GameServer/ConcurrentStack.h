#pragma once

#include <mutex>	// Stack using mutex

template<typename T>
class LockStack 
{
public:
	LockStack() {}
	LockStack(const LockStack&) = delete;
	LockStack& operator=(const LockStack&) = delete;

	void Push(T value)
	{
		lock_guard<mutex> lock(_mutex);	// Mutex locking using lock_guard
		_stack.push(std::move(value));	// Think about a case where can calculate faster using move
		_condVar.notify_one();
	}

	bool TryPop(T& value)
	{
		lock_guard<mutex> lock(_mutex);
		if (_stack.empty())
			return false;

		value = std::move(_stack.top());
		_stack.pop();
		return true;
	}

	void WaitPop(T& value)
	{
		unique_lock<mutex> lock(_mutex);
		_condVar.wait(lock, [this] {return _stack.empty() == false; });	// if empty, wait
		value = std::move(_stack.top());
		_stack.pop();
	}



	//// 일반적으로 Stack은 비어있는 지 확인을 해서 pop을 하는 것이 일반적이다.
	//bool Empty()
	//{
	//	lock_guard<mutex> lock(_mutex);
	//	return _stack.empty();
	//}
	//// 하지만 이것은 의미가 없는 짓이다.
	//// 비어 있는 것을 확인을 하고 POP을 한다면 그 사이에 또 무슨 일이 일어날 것인지 모르기 때문이다.


private:
	stack<T> _stack;
	mutex _mutex;
	condition_variable _condVar;
};

