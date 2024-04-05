#include "pch.h"
#include "MutexLock.h"

namespace GS
{
	MutexLock* MutexLock::s_Instance = nullptr;
	static std::mutex s_mutex;

	MutexLock* MutexLock::GetInstance()
	{
		if (s_Instance)
			return s_Instance;
		s_Instance = new MutexLock();
		return s_Instance;
	}

	MutexLock::MutexLock()
	{}
	MutexLock::~MutexLock()
	{
		for (auto i : m_MutexLock)
		{
			delete i;
		}
	}
	void MutexLock::Lock(const std::string & str)
	{
		//std::lock_guard<std::mutex> lkg(s_mutex);

		MutexLock* instance = GetInstance();
		

		if (instance->m_LockMap.find(str) != instance->m_LockMap.end())
		{
			instance->m_MutexLock[instance->m_LockMap[str]]->lock();
			return;
		}

		std::mutex* t_mutex = new std::mutex();
		instance->m_MutexLock.push_back(t_mutex);
		instance->m_LockMap[str] = instance->m_MutexLock.size() - 1;
		(instance->m_MutexLock[instance->m_MutexLock.size() - 1])->lock();
	}
	void MutexLock::Unlock(const std::string& str)
	{
		//std::lock_guard<std::mutex> lkg(s_mutex);

		MutexLock* instance = GetInstance();
		if (instance->m_LockMap.find(str) != instance->m_LockMap.end())
		{
			instance->m_MutexLock[instance->m_LockMap[str]]->unlock();
		}
		else
		{
			std::cout << "[error] No Mutex Lock named \"" << str << "\".\n";
		}
	}
	bool MutexLock::TryLock(const std::string& str)
	{
		//std::lock_guard<std::mutex> lkg(s_mutex);

		MutexLock* instance = GetInstance();


		if (instance->m_LockMap.find(str) != instance->m_LockMap.end())
		{
			return instance->m_MutexLock[instance->m_LockMap[str]]->try_lock();
		}
		std::mutex* t_mutex = new std::mutex();
		instance->m_LockMap[str] = instance->m_MutexLock.size() - 1;
		return (instance->m_MutexLock[instance->m_MutexLock.size() - 1])->try_lock();
	}
	void MutexLock::Free()
	{
		if (s_Instance)
			delete s_Instance;
	}
}