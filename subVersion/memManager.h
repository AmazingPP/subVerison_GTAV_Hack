/*
	Copyright 2016-2017 sub1to

	This file is part of subVersion GTA:O SC External Hack.

    subVersion GTA:O SC External Hack is free software: you can redistribute
	it and/or modify it under the terms of the GNU General Public License
	as published by the Free Software Foundation, either version 3 of the
	License, or (at your option) any later version.

    subVersion GTA:O SC External Hack is distributed in the hope that it
	will be useful, but WITHOUT ANY WARRANTY; without even the implied
	warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See
	the GNU General Public License for more details.

    You should have received a copy of the GNU General Public License along
    with subVersion GTA:O SC External Hack.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once
#ifndef MEMMANAGER_H
#define MEMMANAGER_H

class memManager
{
	public:
				memManager();
				memManager(LPCSTR name);
				~memManager();
		bool	attach();
MODULEENTRY32	getModule(const std::string& moduleName = "GTA5.exe");
		bool	findWindow();
		HWND	getWindow();
		void	setWindowName(LPCSTR str);
		void	initPtr();

		template <typename rT>
		inline rT readMem(DWORD_PTR address)
		{
			rT out;
			ReadProcessMemory(m_hProc, (LPVOID)address, &out, sizeof(rT), nullptr);
			return out;
		}

		template <typename rT>
		inline void	readMem(DWORD_PTR address, rT* output, DWORD size = NULL, DWORD prot = NULL)
		{
			size	= (size == NULL) ? sizeof(rT) : size;
			ReadProcessMemory(m_hProc, (LPVOID) address, output, size, NULL);
			return;
		}

		template <typename wT>
		inline void	writeMem(DWORD_PTR address, wT* value , DWORD size = NULL, DWORD prot = NULL)
		{
			size	= (size == NULL) ? sizeof(wT) : size;
			WriteProcessMemory(m_hProc, (LPVOID) address, value, size, NULL);
			return;
		}

		template <typename wT>
		inline void	writeMem(DWORD_PTR address, wT value, DWORD size = NULL, DWORD prot = NULL)
		{
			size = (size == NULL) ? sizeof(wT) : size;
			WriteProcessMemory(m_hProc, (LPVOID)address, &value, size, NULL);
			return;
		}
	protected:
		LPCSTR	m_szWindowName;
		HWND	m_hWndTarget;
		HANDLE	m_hProc		= nullptr;
};

extern memManager*	g_pMemMan;

#endif