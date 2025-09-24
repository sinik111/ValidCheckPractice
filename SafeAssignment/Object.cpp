#include "Object.h"

Object::Object()
{
	ObjectHandleTable::Get().RegisterObject(this);
}

Object::~Object()
{
	// 소멸할 때 호출.
	CallOnDestroy();

	ObjectHandleTable::Get().UnregisterObject(this);
}

void Object::DoSomething()
{
	std::cout << "DoSomething" << std::endl;
}

void Object::CallOnDestroy()
{
	m_onDestroyCallbacks.Invoke();
}
