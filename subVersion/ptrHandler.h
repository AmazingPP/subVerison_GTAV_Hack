#pragma once
#include "stdafx.h"

template<class T>
class prtHandler
{
public:
	prtHandler<T>(DWORD_PTR ptr);

	T value();

	T operator=(const T value) {
		g_pMemMan->writeMem(m_ptr, value);
		return value;
	}

	constexpr bool operator==(const T other) noexcept {
		return other == this->value();
	}

	constexpr bool operator!=(const T other) noexcept {
		return other != this->value();
	}

	constexpr bool operator>(const T other) noexcept {
		return other > this->value();
	}

	constexpr bool operator<(const T other) noexcept {
		return other < this->value();
	}

private:
	DWORD_PTR m_ptr;
};

template<class T>
inline prtHandler<T>::prtHandler(DWORD_PTR ptr) :
	m_ptr(ptr)
{
}

template<class T>
inline T prtHandler<T>::value()
{
	T out;
	g_pMemMan->readMem<T>(m_ptr, &out);
	return out;
}