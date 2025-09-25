#include "ObjectHandleTable.h"

#include "Object.h"

ObjectHandleTable::ObjectHandleTable()
	: m_validFlags{ INVALID }, m_generations(1) // index 는 1부터 시작. 0은 유효하지 않은 값
{
}

ObjectHandleTable& ObjectHandleTable::Get()
{
	static ObjectHandleTable s_instance;

	return s_instance;
}

void ObjectHandleTable::RegisterObject(Object* object)
{
	unsigned index = 0;

	if (m_freeIndexes.empty())
	{
		index = static_cast<unsigned>(m_validFlags.size());
		m_generations.push_back(1); // generation 은 1부터 시작. 0 은 유효하지 않은 값
		m_validFlags.push_back(VALID);
	}
	else
	{
		index = m_freeIndexes.front();
		m_freeIndexes.pop();
		++m_generations[index];
		m_validFlags[index] = VALID;
	}
	
	object->m_handle = ObjectHandle{ index, m_generations[index] };
}

void ObjectHandleTable::UnregisterObject(Object* object)
{
	if (IsValid(object->m_handle))
	{
		m_validFlags[object->m_handle.index] = INVALID;
		m_freeIndexes.push(object->m_handle.index);
		object->m_handle.Reset();
	}
}

bool ObjectHandleTable::IsValid(const ObjectHandle& handle) const
{
	return m_validFlags[handle.index] && m_generations[handle.index] == handle.generation;
}
