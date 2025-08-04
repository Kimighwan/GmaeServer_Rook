#pragma once
#include <stack>
#include <map>
#include <vector>

/*------------------
   DeadLockProfiler
-------------------*/

class DeadLockProfiler
{
public:
	void PushLock(const char* name);
	void PopLock(const char* name);
	void CheckCycle();
private:
	void Dfs(int32 here);

private:
	unordered_map<const char*, int32> _nameToId;
	unordered_map<int32, const char*> _IdToName;
	stack<int32> _lockStack;
	map<int32, set<int32>> _lockHistory;

	Mutex _lock;

private: // 사이클 체크 할 때마다 매번 초기화 해서 체크함.
	vector<int32> _discoveredOrder;	// 노드가 발견된 순서를 기록
	int32 _discoveredCount = 0;		// 노드가 발견된 순서
	vector<bool> _finished;			// Dfs(i)가 종료 여부
	vector<int32> _parent;			// 내가 발견된 이유, 즉 부모 노드
};

