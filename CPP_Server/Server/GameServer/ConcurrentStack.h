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
		Node(const T& value) : data(value), next(nullptr)	// 생성자
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
		_popCount++;

		Node* oldNode = _head;

		while (oldNode && _head.compare_exchange_weak(oldNode, oldNode->next) == false)
		{

		}

		if (oldNode == nullptr) 
		{
			_popCount--;
			return false;
		}

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

		// delete oldNode;	// 곧이곧대로 삭제를 못한다.
		TryDelete(oldNode);
		return true;
	}

	void TryDelete(Node* _oldNode)
	{
		// 나 외에 누가 있는가?
		if (_popCount == 1)
		{
			// 나 혼자네 라고 생각을 할 수 있지만
			// 일단 혼자라면 그냥 delete _oldNode;
			// 여기서 근데 popCount가 1인 걸 확인하고 delet 하는 사이에 누군가 TryPop을 할 수 있지 않나?
			// 그건 맞다

			//하지만 함수 호출 순서를 유심히 보면
			// 1) 데이터 분리
			// 2) count 체크
			// 3) 나 혼자면 삭제
			// 데이터 분리에서 Compare and Swap에서 이미 조건문에 따라 스택에서 빠져나온 상태라
			// 그러니깐 이미 누군가 건드리고 있다면 문제지만
			// 그게 아니라 이미 스택에서 빠져나온 상태라 
			// 거의 동시에 Pop하는 스레드는 CAS에서 조건이 불일치하여 같은 head를 참조할 수 없다.
			// 그래서 여기서 count가 1라면 실질적으로 나 혼자라 삭제할 수 있다.

			// 그리고 이왕 혼자인거, 삭제할 예약된 다른 데이터들도 전부 삭제해보자
			// 여기서 문제는 다른 노드들은 누군가 참조를 하고 있을 수 있기 때문에 한 번더 체크를 해줘야 한다.
			
			Node* node = _pendingList.exchange(nullptr);	// 꺼내오고 null로 미는 그 사이에도 혹시 다른 node 추가를 생각해서 동시에 작업하자.

			if (--_popCount == 0) // 이제 여기서는 더 이상 필요없으니 줄여주는데
			{
				// 여기로 들어오면 pendingList를 가져오는 사이에도 누군가 개입을 안 했다는 의미다.
				// 그러면 가져온 pendingList들을 삭제하면 된다.
				// 여기 까지 들어와서 누군가 pop을 해서 popCount를 늘리면 pendingList에 노드를 추가해도
				// 이미 pendingList 노드를 꺼내와서 아무런 상관이 없다.
				DeleteNodes(node);
			}
			else if(node)
			{
				// 누가 끼어들었으니 pendinfList에서 뺴낸 것들을 다시 돌려놔야함.
				ChainPendingNodeList(node);
			}

			delete _oldNode;	// 위에서 말한 나 혼자이니 내가 삭제할 노드는 그냥 바로 삭제
		}
		else
		{
			/*TryDelete가 실행되는 순간에 popCount가 1을 넘어버렷다면, 
				Pop이 다른 쓰레드에서 호출된 상태이므로 지금 당장 삭제하지 말고 삭제 예약만 해놓도록 한다.*/
		}
	}

	void ChainPendingNodeList(Node* first, Node* last)
	{
		last->next = _pendingList;	// 빼낸 마지막 노드를 현재 pendingList 와 연결
		// 이후 pendingList에 빼낸 node 중 젤 앞에 있는 것을 가리키가 만들어야 한다. -> _pendingList = first;
		// 여기서도 마찬가지로 누군가 끼어들 수 있어서 CAS 연산을 사용
		while (_pendingList.compare_exchange_weak(last->next, first) == false)
		{	}


	}
	// 해당 함수는 첫 번째 노드만을 알고 있을 때, 함수에서 마지막 노드를 찾는 오버로딩 함수
	void ChainPendingNodeList(Node* node) // 매개변수는 첫 번째 노드
	{
		Node* last = node;
		while (last->next)
			last = last->next;

		ChainPendingNodeList(node, last);
	}

	// 이 함수는 빼낸 pendingList 노드가 하나 뿐인 경우를 위한 특수 함수
	void ChainPendingNode(Node* node)
	{
		ChainPendingNodeList(node, node);
	}

	static void DeleteNodes(Node* node)
	{
		// node에 이어진 삭제 예약된 노드들을 싸그리 다 삭제하자
		while (node)
		{
			Node* next = node->next;
			delete node;
			node = next;
		}
	}
private:
	atomic<Node*> _head;

	atomic<int> _popCount = 0;	// Pop을 실행 중인 스레드 개수
	atomic<Node*> _pendingList;	// 삭제 되어야 할 노드들 (여기서는 첫번째 노드만 알면 나머지 접근 가능)
};

template<typename T>
class LockFreeStackSmartPointer {
	struct Node
	{
		Node(const T& value) : data(make_shared<T>(value)), next(nullptr)
		{

		}

		shared_ptr<T> data;
		shared_ptr<Node> next;
	};

public:

	void Push(const T& value)
	{
		shared_ptr<Node> node = make_shared<Node>(value);
		node->next = std::atomic_load(&_head);

		while (std::atomic_compare_exchange_weak(&_head, &node->next, node) == false)
		{		}

		_head = node;
	}

	shared_ptr<T> TryPop()
	{
		shared_ptr<Node> oldHead = std::atomic_load(&_head);

		while (std::atomic_compare_exchange_weak(&_head, &oldHead, oldHead->next) == false)
		{		}

		if (oldHead == nullptr)
			return shared_ptr<T>();

		return oldHead->data;
	}

private:
	shared_ptr<Node> _head;
};