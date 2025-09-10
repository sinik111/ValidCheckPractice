#include <unordered_map>
#include <memory>
#include <iostream>

int handleGenerator = 0;

// ���� �����͸� ��� Ŭ����
class RawObject
{
private:
    // RawObject�� ��ȿ�� �˻縦 ���� ���� ��ȣ
    int generation = 0;
    int someData = 0;

public:
    int GetGeneration() const { return generation; }
    void IncrementGeneration() { generation++; }

    // ...
    int GetSomeData() { return someData; }
    void SetSomeData(int data) { someData = data; }

    // Ǯ���� ����� �� �ʱ�ȭ�ϴ� �Լ�
    void Reset() { someData = 0; }
};

// ��� ������Ʈ�� �����ϴ� �߾� ���߽� �Ŵ���
class ObjectManager
{
private:
    std::unordered_map<int, std::unique_ptr<RawObject>> pool;
    std::vector<int> freeHandles; // ��� �� �ݳ��� �ڵ��� ����
    int currentHandleId = 0;

public:
    // Object �ڵ��� �����ϰ� ��ȯ
    int CreateObjectHandle()
    {
        int newHandle;
        if (!freeHandles.empty())
        {
            newHandle = freeHandles.back();
            freeHandles.pop_back();
        }
        else
        {
            newHandle = currentHandleId++;
        }

        pool.emplace(newHandle, std::make_unique<RawObject>());
        return newHandle;
    }

    // �ڵ��� ����Ͽ� RawObject�� ��ȯ
    RawObject* GetRawObject(int handle)
    {
        auto iter = pool.find(handle);
        if (iter == pool.end())
        {
            return nullptr; // ��ȿ���� ���� �ڵ�
        }
        return iter->second.get();
    }

    // ������Ʈ�� �ı�(Ǯ�� ��ȯ)
    void DestroyObject(int handle)
    {
        auto iter = pool.find(handle);
        if (iter == pool.end())
        {
            return;
        }
        iter->second->IncrementGeneration(); // ���� ��ȣ ����
        iter->second->Reset(); // ������ �ʱ�ȭ
        freeHandles.push_back(handle);
        pool.erase(iter);
    }
};

// ���� �Ŵ���
ObjectManager manager;

// ����ڰ� ����ϴ� Object ���� Ŭ����
class Object
{
private:
    int handle = -1;
    int generation = 0;

public:
    Object() = default;
    Object(int h, int g) : handle(h), generation(g) {}

    // ��ȿ�� �˻�
    bool IsValid() const
    {
        RawObject* raw = manager.GetRawObject(handle);
        if (!raw) return false;
        return raw->GetGeneration() == generation;
    }

    void SetSomeData(int data)
    {
        if (IsValid())
        {
            manager.GetRawObject(handle)->SetSomeData(data);
        }
    }

    int GetSomeData()
    {
        if (IsValid())
        {
            return manager.GetRawObject(handle)->GetSomeData();
        }
        return 0;
    }

    // �ı� �Լ�
    void Destroy()
    {
        if (IsValid())
        {
            manager.DestroyObject(handle);
            // �ڵ� ��ȿȭ
            handle = -1;
            generation = -1;
        }
    }
};

// Object�� �����ϴ� ���丮 �Լ�
Object CreateObject()
{
    int newHandle = manager.CreateObjectHandle();
    RawObject* raw = manager.GetRawObject(newHandle);
    return Object(newHandle, raw->GetGeneration());
}

int main()
{
    Object obj1 = CreateObject();
    obj1.SetSomeData(100);
    std::cout << "obj1 data: " << obj1.GetSomeData() << std::endl; // ���: 100

    Object obj2 = obj1; // ���� ������, obj1�� ���� �ڵ��� ����

    // obj1�� �ı��ϸ�
    obj1.Destroy();

    // obj2�� ���� ��ȿ���� ����
    if (!obj2.IsValid())
    {
        std::cout << "obj2 is no longer valid." << std::endl;
    }

    return 0;
}