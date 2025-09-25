#pragma once

#include <vector>

#include "ObjectHandleTable.h"
#include "Delegate.h"

// ���̽� Ŭ����
class Object
{
private:
	// �Ҹ��� �� ȣ���� �Լ����� ��� Delegate.
	Delegate<> m_onDestroyCallbacks;
	ObjectHandle m_handle;

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
		// out �� ������ �޾Ƽ� �� �ݹ��� ȣ��Ǵ� ������ out ���� ��.
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

private:
	void CallOnDestroy();

	friend class ObjectManager;
	friend class ObjectHandleTable;
	friend class Delegate<>;
};