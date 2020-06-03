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
		HMODULE	getModuleAddress(char* moduleName);
		bool	findWindow();
		HWND	getWindow();
		void	setWindowName(LPCSTR str);

		template <typename rT>
		void	readMem(DWORD_PTR address, rT* output, DWORD size = NULL, DWORD prot = NULL)
		{
			DWORD dwProtRet;
			size	= (size == NULL) ? sizeof(rT) : size;
			prot	= (prot == NULL) ? PAGE_READWRITE : prot;
			VirtualProtectEx(m_hProc, (LPVOID) address, size, prot, &dwProtRet);
			ReadProcessMemory(m_hProc, (LPVOID) address, output, size, NULL);
			VirtualProtectEx(m_hProc, (LPVOID) address, size, dwProtRet, &prot);
			return;
		}

		template <typename wT>
		void	writeMem(DWORD_PTR address, wT* value, DWORD size = NULL, DWORD prot = NULL)
		{
			DWORD dwProtRet;
			size	= (size == NULL) ? sizeof(wT) : size;
			prot	= (prot == NULL) ? PAGE_READWRITE : prot;
			VirtualProtectEx(m_hProc, (LPVOID) address, size, prot, &dwProtRet);
			WriteProcessMemory(m_hProc, (LPVOID) address, value, size, NULL);
			VirtualProtectEx(m_hProc, (LPVOID) address, size, dwProtRet, &prot);
			return;
		}
	protected:
		LPCSTR	m_szWindowName;
		HWND	m_hWndTarget;
		HANDLE	m_hProc		= nullptr;
};

extern memManager*	g_pMemMan;

#endif