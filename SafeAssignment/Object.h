#pragma once

#include <iostream>
#include <vector>
#include <functional>

#include "ObjectHandleTable.h"
#include "Delegate.h"

// ���̽� Ŭ����
class Object
{
private:
	// �Ҹ��� �� ȣ���� �Լ����� ��� ����.
	Delegate<> m_onDestroyCallbacks;
	ObjectHandle m_handle;
	bool m_isDestroyed = false;

public:
	Object();
	virtual ~Object();

protected:
	template<typename T>
	void SafeAssign(T*& out, T* ptr)
	{
		// Object �� ��ӹ��� Ŭ������ ��� ����
		static_assert(std::is_base_of<Object, T>());

		// ptr �� nullptr �̸� �ǹ̾���.
		if (ptr == nullptr)
		{
			return;
		}

		// out ���ٰ� ptr�� �ּҸ� ������.
		// �����̱� ������ ���� ������ ���� ��.

		out = ptr;

		// ���� ĸ�ķ� out �� ptr �� �Ѱ���.
		// out �� ������ �޾Ƽ� �� �ݹ��� ȣ��Ǵ� ������ at ���� ��.
		// ptr�� ������ �޾Ƽ� �� ���ٰ� �����Ǵ� ������ ptr ���� ��.

		ptr->m_onDestroyCallbacks.Add(this, [&out, ptr]()
			{
				// ȣ���� ��, �����ߴ� ������ ptr �� ���� out �� �ּҰ� ���� ��� at �� nullptr�� �������.

				if (out == ptr)
				{
					out = nullptr;
				}

				// �ٸ��� �̹� �ٸ� �ּҰ� �Ҵ�� ���̱� ������ �Ȱǵ帲.
			}
		);
	}

	void CallOnDestroy();

	// �Լ����� ���� ���Ϳ� ������ �� �ְ� friend ����.
	template<typename T>
	friend class ObjectManager;
	friend class ObjectHandleTable;
	friend class Delegate<>;
};