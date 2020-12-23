#pragma once
#ifndef PATTERNBATCH_H
#define PATTERNBATCH_H

class patternBatch
{
public:
	patternBatch(MODULEENTRY32 module);
	~patternBatch() noexcept = default;

	void add(std::string name, std::string pattern, std::function<void(uintptr_t)> callback);
	void run() const;

	struct patternTask
	{
		std::string m_name;
		std::string m_pattern;
		std::function<void(uintptr_t)> m_callback;

		patternTask(std::string name, std::string pattern, std::function<void(uintptr_t)> callback) :
			m_name(std::move(name)),
			m_pattern(std::move(pattern)),
			m_callback(std::move(callback))
		{}
	};
private:
	void findPattern(const patternTask& task) const;
	inline bool compareBytes(const unsigned char* bytes, const char* pattern) const;

	uintptr_t m_moduleBase;
	DWORD m_moduleSize;
	std::vector<unsigned char> m_moduleBytes;
	std::vector<patternTask> m_tasks;
};

#endif