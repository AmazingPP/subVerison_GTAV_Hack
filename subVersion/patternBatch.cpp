#include "stdafx.h"
#include "patternBatch.h"


#define INRANGE(x,a,b) (x >= a && x <= b) 
#define getBits( x ) (INRANGE(x,'0','9') ? (x - '0') : ((x&(~0x20)) - 'A' + 0xa))
#define getByte( x ) (getBits(x[0]) << 4 | getBits(x[1]))

patternBatch::patternBatch(MODULEENTRY32 module) :
    m_moduleBase((uintptr_t)module.hModule),
    m_moduleSize(module.modBaseSize),
    m_moduleBytes(m_moduleSize)
{
    g_pMemMan->readMem(m_moduleBase, &m_moduleBytes[0], m_moduleSize);
}

void patternBatch::add(std::string name, std::string pattern, std::function<void(uintptr_t)> callback)
{
    m_tasks.emplace_back(std::move(name), std::move(pattern), std::move(callback));
}

void patternBatch::run() const
{
    std::vector<std::future<void>> results;

    for (const auto& task : m_tasks)
    {
        std::packaged_task<void()> packagedTask(std::bind(&patternBatch::findPattern, this, task));
        results.emplace_back(std::move(packagedTask.get_future()));
        std::thread(std::move(packagedTask)).detach();
    }

    for (const auto& result : results)
    {
        result.wait();
    }
}

void patternBatch::findPattern(const patternTask& task) const
{
    auto byteBase = const_cast<unsigned char*>(&m_moduleBytes.at(0));
    auto maxOffset = m_moduleSize - 0x1000;
    auto pattern = task.m_pattern.c_str();

    for (auto offset = 0UL; offset < maxOffset; ++offset)
    {
        if (compareBytes(byteBase + offset, pattern))
        {
            if (task.m_callback)
                std::invoke(std::move(task.m_callback), offset);
            break;
        }
    }
}

inline bool patternBatch::compareBytes(const unsigned char* bytes, const char* pattern) const
{
    for (; *pattern; *pattern != ' ' ? ++bytes : bytes, ++pattern)
    {
        if (*pattern == ' ' || *pattern == '?')
            continue;

        auto a = *bytes;
        auto b = getByte(pattern);
        if (*bytes != getByte(pattern))
            return false;

        ++pattern;
    }

    return true;
}