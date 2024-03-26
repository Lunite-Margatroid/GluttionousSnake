#pragma once

namespace GS
{
	class MutexLock
	{
	private:
		static MutexLock* s_Instance;
		static MutexLock* GetInstance();

		std::vector<std::mutex*> m_MutexLock;
		std::unordered_map<std::string, int> m_LockMap;
		MutexLock();
		~MutexLock();
	public:
		static void Lock(const std::string& str);
		static void Unlock(const std::string& str);
		static bool TryLock(const std::string& str);
	};
}