#include "scriptGlobal.h"

scriptGlobal::scriptGlobal(std::size_t index) :
	m_index(index)
{
}

scriptGlobal scriptGlobal::at(std::ptrdiff_t index)
{
	return scriptGlobal(m_index + index);
}

scriptGlobal scriptGlobal::at(std::ptrdiff_t index, std::size_t size)
{
	return scriptGlobal(m_index + 1 + (index * size));
}

DWORD_PTR scriptGlobal::get()
{
	DWORD_PTR dwpBuffer;
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)g_pHack->m_hModule + ADDRESS_GLOBAL + 8 * ((m_index >> 0x12) & 0x3F), &dwpBuffer);
	return 8 * (m_index & 0x3FFFF) + dwpBuffer;
}