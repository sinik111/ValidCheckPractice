#include <unordered_map>
#include <memory>
#include <iostream>

int handleGenerator = 0;

// 실제 데이터를 담는 클래스
class RawObject
{
private:
    // RawObject의 유효성 검사를 위한 세대 번호
    int generation = 0;
    int someData = 0;

public:
    int GetGeneration() const { return generation; }
    void IncrementGeneration() { generation++; }

    // ...
    int GetSomeData() { return someData; }
    void SetSomeData(int data) { someData = data; }

    // 풀에서 재사용될 때 초기화하는 함수
    void Reset() { someData = 0; }
};

// 모든 오브젝트를 관리하는 중앙 집중식 매니저
class ObjectManager
{
private:
    std::unordered_map<int, std::unique_ptr<RawObject>> pool;
    std::vector<int> freeHandles; // 사용 후 반납된 핸들을 관리
    int currentHandleId = 0;

public:
    // Object 핸들을 생성하고 반환
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

    // 핸들을 사용하여 RawObject를 반환
    RawObject* GetRawObject(int handle)
    {
        auto iter = pool.find(handle);
        if (iter == pool.end())
        {
            return nullptr; // 유효하지 않은 핸들
        }
        return iter->second.get();
    }

    // 오브젝트를 파괴(풀에 반환)
    void DestroyObject(int handle)
    {
        auto iter = pool.find(handle);
        if (iter == pool.end())
        {
            return;
        }
        iter->second->IncrementGeneration(); // 세대 번호 증가
        iter->second->Reset(); // 데이터 초기화
        freeHandles.push_back(handle);
        pool.erase(iter);
    }
};

// 전역 매니저
ObjectManager manager;

// 사용자가 사용하는 Object 래퍼 클래스
class Object
{
private:
    int handle = -1;
    int generation = 0;

public:
    Object() = default;
    Object(int h, int g) : handle(h), generation(g) {}

    // 유효성 검사
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

    // 파괴 함수
    void Destroy()
    {
        if (IsValid())
        {
            manager.DestroyObject(handle);
            // 핸들 무효화
            handle = -1;
            generation = -1;
        }
    }
};

// Object를 생성하는 팩토리 함수
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
    std::cout << "obj1 data: " << obj1.GetSomeData() << std::endl; // 출력: 100

    Object obj2 = obj1; // 복사 생성자, obj1과 같은 핸들을 가짐

    // obj1을 파괴하면
    obj1.Destroy();

    // obj2는 이제 유효하지 않음
    if (!obj2.IsValid())
    {
        std::cout << "obj2 is no longer valid." << std::endl;
    }

    return 0;
}