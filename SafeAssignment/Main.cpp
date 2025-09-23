#include <iostream>
#include <vector>
#include <functional>

class Object;

void SafeAssignment(Object*& at, Object* ptr);

class Object
{
private:
	std::vector<std::function<void()>> m_onDestroyCallbacks;

public:
	~Object()
	{
		callOnDestroy();
	}

public:
	void DoSomething()
	{
		std::cout << "DoSomething" << std::endl;
	}

private:
	void callOnDestroy()
	{
		for (const auto& func : m_onDestroyCallbacks)
		{
			func();
		}
	}

	friend void SafeAssignment(Object*& at, Object* ptr);
};

void SafeAssignment(Object*& at, Object* ptr)
{
	if (ptr == nullptr)
	{
		return;
	}

	at = ptr;

	ptr->m_onDestroyCallbacks.push_back([&at, ptr]()
		{
			if (at == ptr)
			{
				at = nullptr;
			}
		}
	);
}

int main()
{
	Object* a = new Object;

	Object* b = nullptr;

	SafeAssignment(b, a);

	b->DoSomething();

	delete a;

	if (b == nullptr)
	{
		std::cout << "b is not valid" << std::endl;
	}
	else
	{
		std::cout << "b is valid" << std::endl;
	}

	Object* c = new Object;

	Object* d = nullptr;

	Object* e = new Object;

	SafeAssignment(d, c);

	d->DoSomething();

	SafeAssignment(d, e);

	delete c;

	if (d == nullptr)
	{
		std::cout << "d is not valid" << std::endl;
	}
	else
	{
		std::cout << "d is valid" << std::endl;
	}

	delete e;
}