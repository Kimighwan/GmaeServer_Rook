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

template<typename T>
class LockFreeStack {
	struct Node
	{
		Node(const T& value) : data(value)	// 생성자
		{

		}

		T data;
		Node* next;
	};

public:

	void Push(const T& value)
	{
		Node* newNode = new Node(value);	// new Node
		newNode->next = _head;

		while (_head.compare_exchange_weak(newNode->next, newNode) == false)
		{

		}

		/* 위 코드의 의미 유사 코드
		if (_head == newNode->next)
		{
			_head = newNode;
			return true;
		}
		else {
			newNode->next = _head;
			return false;
		}
		*/


		_head = newNode;
	}

	bool TryPop(T& value) 
	{
		Node* oldNode = _head;

		while (oldNode && _head.compare_exchange_weak(oldNode, oldNode->next) == false)
		{

		}

		if (oldNode == nullptr) return false;

		/* 위 코드의 의미 유사 코드
		if (oldNode == _head)
		{
			_head = oldNode->next;
			return true;
		}
		else 
		{
			oldNode = _head;
			return false;
		}
		*/

		// 여기 사이에 누군가 개입하면 유효하지 않은 값을 복사하게 되는 것이 아닌가?

		value = oldNode->data;	// return Data

		// delete oldNode;
		return true;
	}
private:
	atomic<Node*> _head;
};