#pragma once
#include "stdafx.h"
#include "ptrHandler.h"

class scriptGlobal
{
public:
	explicit scriptGlobal(std::size_t index);

	scriptGlobal at(std::ptrdiff_t index);
	scriptGlobal at(std::ptrdiff_t index, std::size_t size);

	template <typename T>
	prtHandler<T> as()
	{
		return prtHandler<T>(get());
	}

private:
	DWORD_PTR get();
	std::size_t m_index;
};