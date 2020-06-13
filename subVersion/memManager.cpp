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

#include "stdafx.h"

memManager::memManager() {}

memManager::memManager(LPCSTR name)
{
	m_szWindowName = name;
}

memManager::~memManager()
{
	CloseHandle(m_hProc);
}

bool	memManager::attach()
{
	if(m_hProc != nullptr)
		return 1;

	if(!this->findWindow())
		return 0;

	DWORD dwProcId;
	GetWindowThreadProcessId(m_hWndTarget, &dwProcId);
	if(!dwProcId)
		return 0;

	m_hProc = OpenProcess(PROCESS_VM_OPERATION | PROCESS_VM_READ | PROCESS_VM_WRITE | PROCESS_QUERY_INFORMATION, 0, dwProcId);
	if(m_hProc == nullptr)
		return 0;
	initPtr();

	return 1;
}

HMODULE memManager::getModuleAddress(char* moduleName)
{
	HMODULE	hModBuf[0xff];
	DWORD	bN;
	UINT	i;
	
	if(EnumProcessModulesEx(m_hProc, hModBuf, sizeof(hModBuf), &bN, LIST_MODULES_64BIT))
	{
		for(i = 0; i < (bN / sizeof(HMODULE)); i++)
		{
			TCHAR szPath[MAX_PATH];
			if(GetModuleFileNameEx(m_hProc, hModBuf[i], szPath, sizeof(szPath) / sizeof(TCHAR)))
			{
				std::string szName = szPath;
				if(szName.find(moduleName) != std::string::npos)
					return hModBuf[i];
			}
		}
	}
	return 0;
}

bool	memManager::findWindow()
{
	m_hWndTarget = FindWindow(nullptr, m_szWindowName);
	if(!m_hWndTarget)
		return 0;
	return 1;
}

HWND	memManager::getWindow()
{
	return m_hWndTarget;
}

void	memManager::setWindowName(LPCSTR str)
{
	m_szWindowName	= str;
}

void memManager::initPtr()
{
	HMODULE hModule = memManager::getModuleAddress("steam_api64.dll");
	if (hModule != 0)
	{
		//Steam
		ADDRESS_WORLD = 0x24B0C50;
		ADDRESS_BLIP = 0x1F373D0;
		ADDRESS_AMMO = 0x0EE1E19;
		ADDRESS_MAGAZINE = 0x0EE1DD4;
		ADDRESS_TUNABLE = 0x2D46198;
		ADDRESS_WEAPON = 0x02810670;
		ADDRESS_GLOBAL = 0x2D46190;
		ADDRESS_PLAYER_LIST = 0x1D7D4C0;
	}
	else
	{
		//Epic
		ADDRESS_WORLD = 0x24AECE0;
		ADDRESS_BLIP = 0x1F35770;
		ADDRESS_AMMO = 0x0EE073D;
		ADDRESS_MAGAZINE = 0x0EE06F8;
		ADDRESS_TUNABLE = 0x2D431F8;
		ADDRESS_WEAPON = 0x0280E650;
		ADDRESS_GLOBAL = 0x2D431F0;
		ADDRESS_PLAYER_LIST = 0x1D7B318;
	}
}
