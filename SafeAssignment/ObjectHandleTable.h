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

	void Reset()
	{
		index = 0;
		generation = 0;
	}
};

class ObjectHandleTable // TODO: ObjectHandleTable 말고 다른 이름이 맞나?
{
	using flag = char;
	enum ValidFlag
	{
		INVALID = 0,
		VALID	= 1
	};

private:
	std::vector<flag> m_validFlags;
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