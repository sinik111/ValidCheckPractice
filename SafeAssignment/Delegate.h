#pragma once

#include <vector>
#include <functional>
#include <algorithm>

#include "ObjectHandleTable.h"

template<typename... Args>
class Delegate
{
private:
	struct CallbackInfo
	{
		ObjectHandle handle;
		std::function<void(Args...)> func;
	};

	std::vector<CallbackInfo> m_callbackInfos;

public:
	template<typename T>
	void Add(T* instance, void (T::*memberFunc)(Args...))
	{
		static_assert(std::is_base_of_v<Object, T>);

		m_callbackInfos.emplace_back(
			instance->m_handle,
			[instance, memberFunc](Args... args)
			{
				(instance->*memberFunc)(args...);
			}
		);
	}

	template<typename T>
	void Add(T* instance, std::function<void(Args...)> func)
	{
		static_assert(std::is_base_of_v<Object, T>);

		m_callbackInfos.emplace_back(CallbackInfo{ instance->m_handle, func });
	}

	template<typename T>
	void Remove(T* instance)
	{
		m_callbackInfos.erase(
			std::remove_if(
				m_callbackInfos.begin(),
				m_callbackInfos.end(),
				[handle = instance->m_handle](const auto& info)
				{
					return info.handle == handle;
				}
			),
			m_callbackInfos.end()
		);
	}

	void Invoke(Args... args)
	{
		for (int i = 0; i < m_callbackInfos.size();)
		{
			if (!ObjectHandleTable::Get().IsValid(m_callbackInfos[i].handle))
			{
				std::swap(m_callbackInfos[i], m_callbackInfos.back());

				m_callbackInfos.pop_back();

				continue;
			}

			m_callbackInfos[i].func(args...);

			++i;
		}
	}
};