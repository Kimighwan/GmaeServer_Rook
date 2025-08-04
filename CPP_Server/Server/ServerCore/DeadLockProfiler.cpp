#include "pch.h"
#include "DeadLockProfiler.h"

/*------------------
   DeadLockProfiler
-------------------*/

void DeadLockProfiler::PushLock(const char* name)
{
	LockGuard guard(_lock);

	// 아이디를 찾거나 발급한다.
	int32 lockId = 0;

	auto findIt = _nameToId.find(name);
	if (findIt == _nameToId.end())	// 한 번도 방문 한 적 없는 노드
	{
		lockId = static_cast<int32>(_nameToId.size());
		_nameToId[name] = lockId;
		_IdToName[lockId] = name;
	}
	else
	{
		lockId = findIt->second;
	}

	// 아직 잡고 있는 락이 없다면 처리할 일이 없다.
	// 잡고 있는 락이 있었다면 먼가 처리를 해준다.
	if (_lockStack.empty() == false)
	{
		// 기존에 발견되지 않은 케이스라면 데드락 여부를 확인한다
		const int32 prevId = _lockStack.top();
		// 현재 락을 재귀적으로 잡을 수 있으니 락Id가 같은지 체크한다.
		// 그래서 같다면 재귀적 상황은 데드락이 아니니 그냥 넘어가고
		// 같지 않다면 방문한적이 있는지 체크를 한다.
		if (lockId != prevId)
		{
			set<int32>& history = _lockHistory[prevId];
			if (history.find(lockId) == history.end())
			{
				history.insert(lockId);
				CheckCycle();
			}
		}
	}

	_lockStack.push(lockId);
}

void DeadLockProfiler::PopLock(const char* name)
{
	LockGuard guard(_lock);

	// pop을 할건데 비어있으면 무언가 이상한 상황이므로 크러쉬를 내준다.
	if (_lockStack.empty())
		CRASH("MUTIPLE_UNLOCK");

	// 현재 함수 PopLock 자체가 꼬이는 것도 체크를 해보자
	int32 lockId = _nameToId[name];

	if (_lockStack.top() != lockId)
	{
		// 현재 pop하려는 id와 스택의 top의 id가 다르면 먼가
		// push, pop 순서가 꼬인거라고 생각할 수 있다.
		CRASH("INVALID_UNLOCK");
		// 사실 이부분은 없어도 되는데 버그 예방 차원임, 안정화되면 없앨거다.
	}

	_lockStack.pop();
}

void DeadLockProfiler::CheckCycle()
{
	// 우선은 _discoveredOrder를 초기화 해줄 것이다.
	// -1이라고 하면 아직 방문한 적이 없는 상태라 본다.
	const int32 lockCount = static_cast<int32>(_nameToId.size());
	_discoveredOrder = vector<int32>(lockCount, -1);
	_discoveredCount = 0;
	_finished = vector<bool>(lockCount, false);
	_parent = vector<int32>(lockCount, -1);

	for (int32 lockId = 0; lockId < lockCount; lockId++)
		Dfs(lockId);

	// 연산이 끝났으면 정리한다.
	_discoveredOrder.clear();
	_finished.clear();
	_parent.clear();
}

void DeadLockProfiler::Dfs(int32 here)
{
	// 이미 방문을 했다면 리턴
	if (_discoveredOrder[here] != -1)
		return;

	_discoveredOrder[here] = _discoveredCount++;

	// 모든 인접한 정점을 순회한다.
	auto findIt = _lockHistory.find(here);
	if (findIt == _lockHistory.end())
	{
		// 아직 연결된 다른 정점이 없다는 이야기
		_finished[here] = true;
		return;
	}

	// 사이클 확인하기 위해 다음 노드를 방문해본다.
	// 현재 보고 있는 노드의 순방향 간선으로 연결된 목록들이 있을 것이다.
	// 그것을 순회한다.
	set<int32>& nextSet = findIt->second;
	for (int32 there : nextSet)
	{
		// 아직 방문하지 않았다면 방문한다.
		if (_discoveredOrder[there] == -1)
		{
			_parent[there] = here;
			Dfs(there);
			continue;
		}

		// 이미 방문한 적이 있다면 순방향 역방향을 체크한다.

		// here가 there보다 먼저 발견됐다면, there는 here의 후손이다. (순방향 간선)
		if (_discoveredOrder[here] < _discoveredOrder[there])
			continue;

		// 위 조건을 벗어나 순방향이 아니고, Dfs(there)가 아직 종료하지 않았다면
		// there는 here의 선조이다. (역방향 간선) == 사이클 상황
		if (_finished[there] == false)
		{
			// 그냥 크러쉬만 내면 어떤 경로를 통해 사이클이 나왔는지 확인하기 어려워서
			// 로그를 찍자

			printf("%s -> %s\n", _IdToName[here], _IdToName[there]);

			int32 now = here;
			while (true)
			{
				printf("%s -> %s\n", _IdToName[_parent[now]], _IdToName[now]);
				now = _parent[now];
				if (now == there)
					break;
			}
			CRASH("DELOCK_DETECED");
		}
	}

	_finished[here] = true;
}
