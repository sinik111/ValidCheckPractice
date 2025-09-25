#pragma once

#include <vector>

#include "ObjectHandleTable.h"
#include "Delegate.h"

// 베이스 클래스
class Object
{
private:
	// 소멸할 때 호출할 함수들을 담는 Delegate.
	Delegate<> m_onDestroyCallbacks;
	ObjectHandle m_handle;

public:
	Object();
	virtual ~Object();

protected:
	template<typename T>
	void SafeAssign(T*& out, T* ptr)
	{
		// Object 를 상속받은 클래스만 사용 가능
		static_assert(std::is_base_of<Object, T>());

		// ptr 이 nullptr 이면 의미없음.
		if (ptr == nullptr)
		{
			return;
		}

		// out 에다가 ptr의 주소를 대입함.
		// 참조이기 때문에 실제 변수에 대입 됨.

		out = ptr;

		// 람다 캡쳐로 out 과 ptr 을 넘겨줌.
		// out 은 참조로 받아서 이 콜백이 호출되는 순간의 out 값을 씀.
		// ptr은 값으로 받아서 이 람다가 생성되는 순간의 ptr 값을 씀.

		ptr->m_onDestroyCallbacks.Add(this, [&out, ptr]()
			{
				// 호출할 때, 생성했던 순간의 ptr 과 현재 out 의 주소가 같은 경우 at 을 nullptr로 만들어줌.

				if (out == ptr)
				{
					out = nullptr;
				}

				// 다르면 이미 다른 주소가 할당된 것이기 때문에 안건드림.
			}
		);
	}

private:
	void CallOnDestroy();

	friend class ObjectManager;
	friend class ObjectHandleTable;
	friend class Delegate<>;
};