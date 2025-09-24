#include "ObjectHandleTable.h"

#include "Object.h"

ObjectHandleTable::ObjectHandleTable()
	: m_objects(1) // index 는 1부터 시작. 0은 유효하지 않은 값
{
}

ObjectHandleTable& ObjectHandleTable::Get()
{
	static ObjectHandleTable s_instance;

	return s_instance;
}

void ObjectHandleTable::RegisterObject(Object* object)
{
	// 이미 등록된 오브젝트를 또 등록 불가
	assert(object->m_handle.index == 0);

	unsigned index = 0;

	if (m_freeIndexes.empty())
	{
		index = static_cast<unsigned>(m_objects.size());
		m_generations.push_back(1); // generation 은 1부터 시작. 0 은 유효하지 않은 값
		m_objects.push_back(object);
	}
	else
	{
		index = m_freeIndexes.front();
		m_freeIndexes.pop();
		++m_generations[index];
		m_objects[index] = object;

		object->m_handle = ObjectHandle{ index, m_generations[index] };
	}
}

void ObjectHandleTable::UnregisterObject(Object* object)
{
	// 등록되지 않은 오브젝트는 해제 불가
	assert(object->m_handle.index != 0);

	if (IsValid(object->m_handle))
	{
		m_objects[object->m_handle.index] = nullptr;
		m_freeIndexes.push(object->m_handle.index);
	}
}

bool ObjectHandleTable::IsValid(const ObjectHandle& handle) const
{
	return handle.index < m_objects.size()
		&& m_objects[handle.index] != nullptr
		&& m_generations[handle.index] == handle.generation;
}
