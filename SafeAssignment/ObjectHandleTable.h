#pragma once

#include <vector>
#include <queue>
#include <cassert>

class Object;

struct ObjectHandle
{
	// 0은 유효하지 않은 값
	unsigned index = 0;
	unsigned generation = 0;

	bool operator==(const ObjectHandle& other)
	{
		return index == other.index && generation == other.generation;
	}
};

class ObjectHandleTable
{
private:
	std::vector<Object*> m_objects;
	std::vector<unsigned> m_generations;
	std::queue<unsigned> m_freeIndexes;

private:
	ObjectHandleTable();
	ObjectHandleTable(const ObjectHandleTable&) = delete;
	ObjectHandleTable& operator=(const ObjectHandleTable&) = delete;
	ObjectHandleTable(ObjectHandleTable&&) = delete;
	ObjectHandleTable& operator=(ObjectHandleTable&&) = delete;
	~ObjectHandleTable() = default;

public:
	static ObjectHandleTable& Get();
	void RegisterObject(Object* object);
	void UnregisterObject(Object* object);
	bool IsValid(const ObjectHandle& handle) const;
};