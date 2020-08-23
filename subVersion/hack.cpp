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
#include "scriptGlobal.h"
#include <string>

/*
	TRAINER
*/

trainer::trainer()
{
	m_keyTmr = clock();
}

trainer::~trainer() {}
void	trainer::checkKeys() {}

bool	trainer::checkKeyState(int key)
{
	if
		(
			clock() - m_keyTmr > 100 &&
			(GetAsyncKeyState(key) & 0x8001) == 0x8001
			)
	{
		m_keyTmr = clock();
		return true;
	}
	return false;
}


/*
	HACK
*/


hack::hack() {
	m_explosion = ImpactExplosionEnum::DefaultBullets;
}

hack::~hack() {}

void hack::checkKeys()
{
	if (checkKeyState(g_pSettings->m_iKeys[keyExit]))
	{
		g_bKillHack = true;
		killProgram();
		return;
	}
	if (checkKeyState(g_pSettings->m_iKeys[keyMenu]))
	{
		g_pSettings->toggleMenu();
		return;
	}

	//hotkeys
	if (checkKeyState(g_pSettings->m_iKeys[keyHotTeleport]))
	{
		g_pHack->teleportWaypoint();
		return;
	}
	if (checkKeyState(g_pSettings->m_iKeys[keyHotWanted]))
	{
		g_pHack->notWanted();
		return;
	}
	if (checkKeyState(g_pSettings->m_iKeys[keyHotHealth]))
	{
		g_pHack->restoreHealth();
		return;
	}
	if (checkKeyState(g_pSettings->m_iKeys[keyHotAmmo]))
	{
		g_pHack->fillAmmo();
		return;
	}

	//if menu is not active, no need to check other keys
	if (!g_pSettings->isMenuActive())
		return;

	//menu navigation
	if (checkKeyState(g_pSettings->m_iKeys[keyMenuDown]))
	{
		g_pSettings->menuDown();
		return;
	}
	if (checkKeyState(g_pSettings->m_iKeys[keyMenuUp]))
	{
		g_pSettings->menuUp();
		return;
	}
	if (checkKeyState(g_pSettings->m_iKeys[keyMenuRight]))
	{
		g_pSettings->menuRight();
		return;
	}
	if (checkKeyState(g_pSettings->m_iKeys[keyMenuLeft]))
	{
		g_pSettings->menuLeft();
		return;
	}
	if (checkKeyState(g_pSettings->m_iKeys[keyMenuTabNext]))
	{
		g_pSettings->menuTabRight();
		return;
	}
	if (checkKeyState(g_pSettings->m_iKeys[keyMenuTabPrev]))
	{
		g_pSettings->menuTabLeft();
		return;
	}
	if (checkKeyState(g_pSettings->m_iKeys[keyMenuSave]))
	{
		featTeleport* tp = dynamic_cast<featTeleport*>(g_pSettings->getFeatureCur(g_pSettings->getActiveFeature()));
		if (tp == nullptr || tp->m_tpType != tp_saved)
			return;
		m_player.getPos();
		tp->m_v3Pos.x = m_player.m_v3Pos.x;
		tp->m_v3Pos.y = m_player.m_v3Pos.y;
		tp->m_v3Pos.z = m_player.m_v3Pos.z;
		g_pSettings->m_iniParser.setValue<float>(tp->m_szIniKey + "_x", m_player.m_v3Pos.x, "Teleport");
		g_pSettings->m_iniParser.setValue<float>(tp->m_szIniKey + "_y", m_player.m_v3Pos.y, "Teleport");
		g_pSettings->m_iniParser.setValue<float>(tp->m_szIniKey + "_z", m_player.m_v3Pos.y, "Teleport");
		return;
	}
	if (checkKeyState(g_pSettings->m_iKeys[keyMenuSelect]))
	{
		g_pSettings->menuSelect();
		return;
	}
	if (checkKeyState(g_pSettings->m_iKeys[keyMenuBack]))
	{
		g_pSettings->menuBack();
		return;
	}
}

BYTE hack::initPointers()
{

	BYTE r = 0;

	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_hModule + ADDRESS_WORLD, &m_dwpWorldBase);
	if (m_dwpWorldBase == 0)
		return INITPTR_INVALID_WORLD;

	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_hModule + ADDRESS_TUNABLE, &m_dwpTunableBase);
	if (m_dwpTunableBase == 0)
		return INITPTR_INVALID_TUNABLE;
	m_tunable.m_dwpTunableBase = m_dwpTunableBase;

	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_hModule + ADDRESS_GLOBAL, &m_dwpGlobalBase);
	if (m_dwpGlobalBase == 0)
		return INITPTR_INVALID_GLOBAL;
	m_global.m_dwpGlobalBase = m_dwpGlobalBase;
	m_mpId = "MP0_";

	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_hModule + ADDRESS_REPLAY_INTERFACE, &m_dwpReplayInterfaceBase);
	if (m_dwpReplayInterfaceBase == 0)
		return INITPTR_INVALID_REPLAY_INTERFACE;
	m_replayInterface.m_dwpPedInterface = m_dwpReplayInterfaceBase;
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_dwpReplayInterfaceBase + OFFSET_REPLAY_PED_INTERFACE, &m_replayInterface.m_dwpPedInterface);
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_replayInterface.m_dwpPedInterface + OFFSET_PED_INTERFACE_PED_LIST, &m_replayInterface.m_dwpPedList);

	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_dwpWorldBase + OFFSET_PLAYER, &m_dwpPlayerBase);
	if (m_dwpPlayerBase == 0)
		return INITPTR_INVALID_PLAYER;
	m_player.m_dwpBase = m_dwpPlayerBase;
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_dwpPlayerBase + OFFSET_ENTITY_POSBASE, &m_player.m_dwpPosBase);
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_dwpPlayerBase + OFFSET_PLAYER_INFO, &m_player.m_dwpPlayerInfo);
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_dwpPlayerBase + OFFSET_ENTITY_ATTACKER, &m_dwpAttackerBase);

	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_dwpPlayerBase + OFFSET_PLAYER_VEHICLE, &m_dwpVehicleBase);
	if (m_dwpVehicleBase == 0)
		r |= INITPTR_INVALID_VEHICLE;
	else
	{
		m_vehicle.m_dwpBase = m_dwpVehicleBase;
		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_dwpVehicleBase + OFFSET_ENTITY_POSBASE, &m_vehicle.m_dwpPosBase);
		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_dwpVehicleBase + OFFSET_VEHICLE_HANDLING, &m_vehicle.m_handlingCur.m_dwpHandling);
	}

	//g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_hModule + ADDRESS_WEAPON, &m_dwpWeaponBase);
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_dwpPlayerBase + OFFSET_WEAPON_MANAGER, &m_dwpWeaponManager);
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_dwpWeaponManager + OFFSET_WEAPON_CURRENT, &m_dwpWeaponCur);
	if (m_dwpWeaponManager == 0 || m_dwpWeaponCur == 0 )// || m_dwpWeaponBase == 0
		r |= INITPTR_INVALID_WEAPON;
	else
	{
		m_weapon.m_weapDataCur.m_dwpWeapon = m_dwpWeaponCur;
		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_dwpWeaponCur + OFFSET_WEAPON_AMMOINFO, &m_dwpAmmoInfo);
		m_weapon.m_dwpAmmoInfo = m_dwpAmmoInfo;
	}

	return r;
}

void hack::getWaypoint()
{
	DWORD_PTR a = (DWORD_PTR)m_hModule + ADDRESS_BLIP;
	for (size_t i = 2000; i > 1; i--)
	{
		DWORD64 n;
		DWORD buf[2];
		g_pMemMan->readMem<DWORD64>((DWORD_PTR)a + (i * 8), &n);
		g_pMemMan->readMem<DWORD>((DWORD_PTR)n + 0x40, &buf[0]);
		g_pMemMan->readMem<DWORD>((DWORD_PTR)n + 0x48, &buf[1]);
		if (n > 0 && buf[0] == 8 && buf[1] == 84)
		{
			g_pMemMan->readMem<v2>((DWORD_PTR)n + 0x10, &m_v2Waypoint);
		}
	}

	//g_pMemMan->readMem<v2>((DWORD_PTR) m_hModule + ADDRESS_WAYPOINT, &m_v2Waypoint);
	return;
}

void hack::getObjective()
{
	DWORD_PTR a = (DWORD_PTR)m_hModule + ADDRESS_BLIP;
	for (size_t i = 2000; i > 1; i--)
	{
		DWORD64 n;
		DWORD buf[2];
		g_pMemMan->readMem<DWORD64>((DWORD_PTR)a + (i * 8), &n);
		g_pMemMan->readMem<DWORD>((DWORD_PTR)n + 0x40, &buf[0]);
		g_pMemMan->readMem<DWORD>((DWORD_PTR)n + 0x48, &buf[1]);
		if (n > 0 && buf[0] == 1 && (buf[1] == 5 || buf[1] == 60 || buf[1] == 66))
		{
			g_pMemMan->readMem<v3>((DWORD_PTR)n + 0x10, &m_v3Objective);
		}
	}

	//g_pMemMan->readMem<v3>((DWORD_PTR) m_hModule + ADDRESS_OBJECTIVE, &m_v3Objective);
	return;
}

void hack::teleport(v3 v)
{
	m_player.getInVehicle();
	if (m_player.m_bIsInVehicle)
	{
		m_vehicle.setPos(v);
		return;
	}
	m_player.setPos(v);
	return;
}

bool hack::teleportWaypoint()
{
	getWaypoint();
	if (m_v2Waypoint.x == 64000.f && m_v2Waypoint.y == 64000.f)
		return false;

	v3 dest(m_v2Waypoint.x, m_v2Waypoint.y, -225.f);
	teleport(dest);
	return true;
}


bool hack::teleportObjective()
{
	getObjective();
	if (m_v3Objective.x == 64000.f && m_v3Objective.y == 64000.f && m_v3Objective.z == 64000.f)
		return false;

	v3 dest(m_v3Objective.x, m_v3Objective.y, m_v3Objective.z + 1.f);
	teleport(dest);
	return true;
}

void hack::restoreHealth()
{
	m_player.getHealth();
	if (m_player.m_flArmor < 100.f || m_player.m_cmHp.cur < m_player.m_cmHp.max)
		m_player.setHealth(m_player.m_cmHp.max, 100.f);
	return;
}

void hack::restoreVehicleHealth()
{
	m_vehicle.getHealth();
	if ((m_vehicle.m_cmHp.cur < m_vehicle.m_cmHp.max && m_vehicle.m_cmHp.cur > 0.f) ||
		(m_vehicle.m_cmHpVehicle.cur < m_vehicle.m_cmHpVehicle.max && m_vehicle.m_cmHpVehicle.cur > 0.f))
		m_vehicle.setHealth(m_vehicle.m_cmHp.max);
	return;
}

void hack::restoreStamina()
{
	m_player.getStamina();
	if (m_player.m_cmStamina.cur < m_player.m_cmStamina.max)
		m_player.setStamina(m_player.m_cmStamina.max);
	return;
}

void hack::notWanted()
{
	m_player.getWanted();
	if (m_player.m_dwWanted == 0)
		return;
	m_player.setWanted(0);
	feat* fpWanted = g_pSettings->getFeature(g_iFeature[FEATURE_P_WANTED]);
	if (fpWanted->m_bOn)
		fpWanted->toggle();
	return;
}

void hack::killNpc()
{
	if (m_dwpAttackerBase == 0)
		return;
	for (int i = 0; i < 3; i++)
	{
		DWORD_PTR	npc;
		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_dwpAttackerBase + (i * OFFSET_ATTACKER_DISTANCE), &npc);
		if (npc == m_dwpPlayerBase || npc == 0)
			continue;
		float		health;
		g_pMemMan->readMem<float>((DWORD_PTR)npc + OFFSET_ENTITY_HEALTH, &health);
		float v = 0.f;
		if (health > v)
			g_pMemMan->writeMem<float>((DWORD_PTR)npc + OFFSET_ENTITY_HEALTH, &v);
	}
	return;
}

void hack::fillAmmo()
{
	if (!m_weapon.findAmmoBase())
		return;
	m_weapon.getCurAmmo();
	m_weapon.getMaxAmmo();
	if (m_weapon.m_dwCurAmmo != m_weapon.m_dwMaxAmmo)
		m_weapon.setCurAmmo(m_weapon.m_dwMaxAmmo);
	return;
}

void hack::setImpactExplosion(float* arg)
{
	this->m_explosion = (ImpactExplosionEnum)((DWORD)*arg);
}

//void hack::fillAllAmmo(float* arg)
//{
//	for (size_t i = 0; i < 0x400; i++)
//	{
//		DWORD_PTR dwpWeapon, dwpAmmoInfo, dwpAmmoPtr1, dwpAmmoPtr2;
//		DWORD dwCurAmmo, dwMaxAmmo;
//		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_dwpWeaponBase + i, &dwpWeapon);
//		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)dwpWeapon + OFFSET_WEAPON_AMMOINFO, &dwpAmmoInfo);
//
//		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)dwpAmmoInfo + OFFSET_WEAPON_AMMOINFO_CUR_1, &dwpAmmoPtr1);
//		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)dwpAmmoPtr1 + OFFSET_WEAPON_AMMOINFO_CUR_2, &dwpAmmoPtr2);
//		g_pMemMan->readMem<DWORD>((DWORD_PTR)dwpAmmoPtr2 + OFFSET_WEAPON_AMMOINFO_CURAMMO, &dwCurAmmo);
//		if (dwCurAmmo >= 0 && dwCurAmmo <= 9999)
//		{
//			g_pMemMan->readMem<DWORD>((DWORD_PTR)dwpAmmoInfo + OFFSET_WEAPON_AMMOINFO_MAX, &dwMaxAmmo);
//			if (dwMaxAmmo >= 20 && dwMaxAmmo <= 9999 && dwCurAmmo < dwMaxAmmo)
//			{
//				g_pMemMan->writeMem<DWORD>((DWORD_PTR)dwpAmmoPtr2 + OFFSET_WEAPON_AMMOINFO_CURAMMO, &dwMaxAmmo);
//			}
//		}
//	}
//}

void hack::healVehicle(float* arg)
{
	restoreVehicleHealth();
}

void hack::healPlayer(float* arg)
{
	restoreHealth();
}

void hack::suicide(float* arg)
{
	m_player.setHealth(0, 0);
}

void hack::fillAmmo(float* arg)
{
	this->fillAmmo();
}

void hack::fillSkillLevels(float* arg)
{
	dStatPushBack(string_to_hash("SCRIPT_INCREASE_STAM"), 100);
	dStatPushBack(string_to_hash("SCRIPT_INCREASE_SHO"), 100);
	dStatPushBack(string_to_hash("SCRIPT_INCREASE_STRN"), 100);
	dStatPushBack(string_to_hash("SCRIPT_INCREASE_STL"), 100);
	dStatPushBack(string_to_hash("SCRIPT_INCREASE_FLY"), 100);
	dStatPushBack(string_to_hash("SCRIPT_INCREASE_DRIV"), 100);
	dStatPushBack(string_to_hash("SCRIPT_INCREASE_LUNG"), 100);
}

void hack::fillAllSnacks(float* arg)
{
	dStatPushBack(string_to_hash("NO_BOUGHT_YUM_SNACKS"), 30);
	dStatPushBack(string_to_hash("NO_BOUGHT_HEALTH_SNACKS"), 15);
	dStatPushBack(string_to_hash("NO_BOUGHT_EPIC_SNACKS"), 5);
	dStatPushBack(string_to_hash("NUMBER_OF_ORANGE_BOUGHT"), 10);
	dStatPushBack(string_to_hash("NUMBER_OF_BOURGE_BOUGHT"), 10);
	dStatPushBack(string_to_hash("CIGARETTES_BOUGHT"), 20);
	dStatPushBack(string_to_hash("MP_CHAR_ARMOUR_5_COUNT"), 10);
}

void hack::casinoStat(float* arg)
{
	dStatPushBack(string_to_hash("H3_COMPLETEDPOSIX"), -1);
}

void hack::casinoStatBitSet1(float* arg)
{
	dStatPushBack(string_to_hash("H3OPT_BITSET1"), 0);
	switch ((int)*arg)
	{
	case 0:
		dStatPushBack(string_to_hash("H3OPT_APPROACH"), 1);
		break;
	case 1:
		dStatPushBack(string_to_hash("H3OPT_APPROACH"), 2);
		break;
	case 2:
		dStatPushBack(string_to_hash("H3OPT_APPROACH"), 3);
		break;
	case 3:
		dStatPushBack(string_to_hash("H3OPT_TARGET"), 0);
		break;
	case 4:
		dStatPushBack(string_to_hash("H3OPT_TARGET"), 1);
		break;
	case 5:
		dStatPushBack(string_to_hash("H3OPT_TARGET"), 2);
		break;
	case 6:
		dStatPushBack(string_to_hash("H3OPT_TARGET"), 3);
		break;
	case 7:
		dStatPushBack(string_to_hash("H3OPT_ACCESSPOINTS"), -1);
		break;
	case 8:
		dStatPushBack(string_to_hash("H3OPT_POI"), -1);
		break;
	default:
		break;
	}
	dStatPushBack(string_to_hash("H3OPT_BITSET1"), -1);
}

void hack::casinoStatBitSet2(float* arg)
{
	dStatPushBack(string_to_hash("H3OPT_BITSET0"), 0);
	switch ((int)*arg)
	{
	case 0:
		dStatPushBack(string_to_hash("H3OPT_DISRUPTSHIP"), 3);
		break;
	case 1:
		dStatPushBack(string_to_hash("H3OPT_KEYLEVELS"), 2);
		break;
	case 2:
		dStatPushBack(string_to_hash("H3OPT_CREWWEAP"), 5);
		break;
	case 3:
		dStatPushBack(string_to_hash("H3OPT_CREWDRIVER"), 5);
		break;
	case 4:
		dStatPushBack(string_to_hash("H3OPT_CREWHACKER"), 4);
		break;
	case 5:
		dStatPushBack(string_to_hash("H3OPT_VEHS"), 0);
		break;
	case 6:
		dStatPushBack(string_to_hash("H3OPT_VEHS"), 1);
		break;
	case 7:
		dStatPushBack(string_to_hash("H3OPT_VEHS"), 2);
		break;
	case 8:
		dStatPushBack(string_to_hash("H3OPT_VEHS"), 3);
		break;
	case 9:
		dStatPushBack(string_to_hash("H3OPT_WEAPS"), 0);
		break;
	case 10:
		dStatPushBack(string_to_hash("H3OPT_WEAPS"), 1);
		break;
	default:
		dStatPushBack(string_to_hash("H3OPT_DISRUPTSHIP"), 3);
		dStatPushBack(string_to_hash("H3OPT_KEYLEVELS"), 2);
		dStatPushBack(string_to_hash("H3OPT_CREWWEAP"), 5);
		dStatPushBack(string_to_hash("H3OPT_CREWDRIVER"), 5);
		dStatPushBack(string_to_hash("H3OPT_CREWHACKER"), 4);
		dStatPushBack(string_to_hash("H3OPT_VEHS"), 3);
		dStatPushBack(string_to_hash("H3OPT_WEAPS"), 0);
		break;
	}
	dStatPushBack(string_to_hash("H3OPT_BITSET0"), -1);
}

void hack::unlockHeistCars(float* arg)
{
	dStatPushBack(string_to_hash("CHAR_FM_VEHICLE_1_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_VEHICLE_2_UNLCK"), -1);
	for (size_t i = 1; i <= 7; i++)
	{
		dStatPushBack(string_to_hash("CHAR_FM_CARMOD_"+ std::to_string(i) +"_UNLCK"), -1);
	}
}

void hack::unlockLSC(float* arg)
{
	dStatPushBack(string_to_hash("RACES_WON"), 50);
	dStatPushBack(string_to_hash("CHAR_FM_CARMOD_1_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CARMOD_2_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CARMOD_3_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CARMOD_4_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CARMOD_5_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CARMOD_6_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CARMOD_7_UNLCK"), -1);
	dStatPushBack(string_to_hash("NUMBER_SLIPSTREAMS_IN_RACE"), 110);
	dStatPushBack(string_to_hash("NUMBER_TURBO_STARTS_IN_RACE"), 90);
	dStatPushBack(string_to_hash("USJS_FOUND"), 50);
	dStatPushBack(string_to_hash("USJS_COMPLETED"), 50);
	dStatPushBack(string_to_hash("TIMES_RACE_BEST_LAP", "MPPLY_"), 101);
	dStatPushBack(string_to_hash("AWD_FMRALLYWONDRIVE"), 2);
	dStatPushBack(string_to_hash("AWD_FMWINSEARACE"), 2);
	dStatPushBack(string_to_hash("AWD_FMWINAIRRACE"), 2);
	dStatPushBack(string_to_hash("AWD_FM_RACES_FASTEST_LAP"), 101);
}

void hack::unlockWeaponCamos(float* arg)
{
	dStatPushBack(string_to_hash("PISTOL_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("CMBTPISTOL_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("PISTOL50_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("APPISTOL_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("MICROSMG_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("SMG_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("ASLTSMG_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("ASLTRIFLE_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("CRBNRIFLE_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("ADVRIFLE_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("MG_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("CMBTMG_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("ASLTMG_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("PUMP_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("SAWNOFF_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("BULLPUP_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("ASLTSHTGN_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("SNIPERRFL_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("HVYSNIPER_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("GRNLAUNCH_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("RPG_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("MINIGUNS_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("GRENADE_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("SMKGRENADE_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("STKYBMB_ENEMY_KILLS"), 600);
	dStatPushBack(string_to_hash("MOLOTOV_ENEMY_KILLS"), 600);
}

//void hack::unlockWeapon(float* arg)
//{
//	globalStatSetInt(string_to_hash("CHAR_WEAP_UNLOCKED"), -1);
//	globalStatSetInt(string_to_hash("CHAR_WEAP_UNLOCKED2"), -1);
//	globalStatSetInt(string_to_hash("CHAR_WEAP_ADDON_1_UNLCK"), -1);
//	globalStatSetInt(string_to_hash("CHAR_WEAP_ADDON_2_UNLCK"), -1);
//	globalStatSetInt(string_to_hash("CHAR_WEAP_ADDON_3_UNLCK"), -1);
//	globalStatSetInt(string_to_hash("CHAR_WEAP_ADDON_4_UNLCK"), -1);
//	globalStatSetInt(string_to_hash("CHAR_FM_WEAP_UNLOCKED"), -1);
//	globalStatSetInt(string_to_hash("CHAR_FM_WEAP_UNLOCKED2"), -1);
//	globalStatSetInt(string_to_hash("CHAR_FM_WEAP_ADDON_1_UNLCK"), -1);
//	globalStatSetInt(string_to_hash("CHAR_FM_WEAP_ADDON_2_UNLCK"), -1);
//	globalStatSetInt(string_to_hash("CHAR_FM_WEAP_ADDON_3_UNLCK"), -1);
//	globalStatSetInt(string_to_hash("CHAR_FM_WEAP_ADDON_4_UNLCK"), -1);
//	globalStatSetInt(string_to_hash("CHAR_FM_WEAP_ADDON_5_UNLCK"), -1);
//}

void hack::unlockAllAwards(float* arg)
{
	dStatPushBack(string_to_hash("AWD_100_KILLS_PISTOL"), 500);
	dStatPushBack(string_to_hash("AWD_100_KILLS_SNIPER"), 500);
	dStatPushBack(string_to_hash("AWD_50_KILLS_GRENADES"), 500);
	dStatPushBack(string_to_hash("AWD_100_KILLS_SHOTGUN"), 500);
	dStatPushBack(string_to_hash("AWD_100_KILLS_SMG"), 500);
	dStatPushBack(string_to_hash("AWD_50_KILLS_ROCKETLAUNCH"), 500);
	dStatPushBack(string_to_hash("AWD_25_KILLS_STICKYBOMBS"), 500);
	dStatPushBack(string_to_hash("AWD_20_KILLS_MELEE"), 500);
	dStatPushBack(string_to_hash("AWD_100_HEADSHOTS"), 500);
	dStatPushBack(string_to_hash("AWD_50_VEHICLES_BLOWNUP"), 500);
	dStatPushBack(string_to_hash("AWD_ENEMYDRIVEBYKILLS"), 500);
	dStatPushBack(string_to_hash("AWD_COPS_KILLED"), 500);
	dStatPushBack(string_to_hash("AWD_BUY_EVERY_GUN"), 1);
	dStatPushBack(string_to_hash("AWD_DRIVE_ALL_COP_CARS"), 1);
	dStatPushBack(string_to_hash("AWD_VEHICLE_JUMP_OVER_40M"), 50);
	dStatPushBack(string_to_hash("AWD_VEHICLE_JUMP0_OVER_40M"), 50);
	dStatPushBack(string_to_hash("AWD_VEHICLE_JUMP1_OVER_40M"), 50);
	dStatPushBack(string_to_hash("AWD_NO_ARMWRESTLING_WINS"), 50);
	dStatPushBack(string_to_hash("AWD_KILLS_MACHINEGUN"), 50);
	dStatPushBack(string_to_hash("AWD_ODD_JOBS"), 52);
	dStatPushBack(string_to_hash("AWD_PREPARATION"), 50);
	dStatPushBack(string_to_hash("AWD_ASLEEPONJOB"), 20);
	dStatPushBack(string_to_hash("AWD_DAICASHCRAB"), 100000);
	dStatPushBack(string_to_hash("AWD_BIGBRO"), 40);
	dStatPushBack(string_to_hash("AWD_SHARPSHOOTER"), 40);
	dStatPushBack(string_to_hash("AWD_RACECHAMP"), 40);
	dStatPushBack(string_to_hash("AWD_BATSWORD"), 1000000);
	dStatPushBack(string_to_hash("AWD_COINPURSE"), 950000);
	dStatPushBack(string_to_hash("AWD_ASTROCHIMP"), 3000000);
	dStatPushBack(string_to_hash("AWD_MASTERFUL"), 40000);
	dStatPushBack(string_to_hash("AWD_5STAR_WANTED_AVOIDANCE"), 50);
	dStatPushBack(string_to_hash("AWD_CAR_BOMBS_ENEMY_KILLS"), 50);
	dStatPushBack(string_to_hash("AWD_CARS_EXPORTED"), 50);
	dStatPushBack(string_to_hash("AWD_CONTROL_CROWDS"), 25);
	dStatPushBack(string_to_hash("AWD_DAILYOBJCOMPLETED"), 100);
	dStatPushBack(string_to_hash("AWD_DO_HEIST_AS_MEMBER"), 25);
	dStatPushBack(string_to_hash("AWD_DO_HEIST_AS_THE_LEADER"), 25);
	dStatPushBack(string_to_hash("AWD_DROPOFF_CAP_PACKAGES"), 100);
	dStatPushBack(string_to_hash("AWD_FINISH_HEIST_SETUP_JOB"), 50);
	dStatPushBack(string_to_hash("AWD_FINISH_HEISTS"), 50);
	dStatPushBack(string_to_hash("AWD_FM_DM_3KILLSAMEGUY"), 50);
	dStatPushBack(string_to_hash("AWD_FM_DM_KILLSTREAK"), 100);
	dStatPushBack(string_to_hash("AWD_FM_DM_STOLENKILL"), 50);
	dStatPushBack(string_to_hash("AWD_FM_DM_TOTALKILLS"), 500);
	dStatPushBack(string_to_hash("AWD_FM_DM_WINS"), 50);
	dStatPushBack(string_to_hash("AWD_FM_GOLF_HOLE_IN_1"), 300);
	dStatPushBack(string_to_hash("AWD_FM_GOLF_BIRDIES"), 25);
	dStatPushBack(string_to_hash("AWD_FM_GOLF_WON"), 25);
	dStatPushBack(string_to_hash("AWD_FM_GTA_RACES_WON"), 50);
	dStatPushBack(string_to_hash("AWD_FM_RACE_LAST_FIRST"), 25);
	dStatPushBack(string_to_hash("AWD_FM_RACES_FASTEST_LAP"), 50);
	dStatPushBack(string_to_hash("AWD_FM_SHOOTRANG_CT_WON"), 25);
	dStatPushBack(string_to_hash("AWD_FM_SHOOTRANG_RT_WON"), 25);
	dStatPushBack(string_to_hash("AWD_FM_SHOOTRANG_TG_WON"), 25);
	dStatPushBack(string_to_hash("AWD_FM_TDM_MVP"), 50);
	dStatPushBack(string_to_hash("AWD_FM_TDM_WINS"), 50);
	dStatPushBack(string_to_hash("AWD_FM_TENNIS_ACE"), 25);
	dStatPushBack(string_to_hash("AWD_FM_TENNIS_WON"), 25);
	dStatPushBack(string_to_hash("AWD_FMBASEJMP"), 25);
	dStatPushBack(string_to_hash("AWD_FMBBETWIN"), 50000);
	dStatPushBack(string_to_hash("AWD_FMCRATEDROPS"), 25);
	dStatPushBack(string_to_hash("AWD_FMDRIVEWITHOUTCRASH"), 30);
	dStatPushBack(string_to_hash("AWD_FMHORDWAVESSURVIVE"), 10);
	dStatPushBack(string_to_hash("AWD_FMKILLBOUNTY"), 25);
	dStatPushBack(string_to_hash("AWD_FMRALLYWONDRIVE"), 25);
	dStatPushBack(string_to_hash("AWD_FMRALLYWONNAV"), 25);
	dStatPushBack(string_to_hash("AWD_FMREVENGEKILLSDM"), 50);
	dStatPushBack(string_to_hash("AWD_FMSHOOTDOWNCOPHELI"), 25);
	dStatPushBack(string_to_hash("AWD_FMWINAIRRACE"), 25);
	dStatPushBack(string_to_hash("AWD_FMWINRACETOPOINTS"), 25);
	dStatPushBack(string_to_hash("AWD_FMWINSEARACE"), 25);
	dStatPushBack(string_to_hash("AWD_HOLD_UP_SHOPS"), 20);
	dStatPushBack(string_to_hash("AWD_KILL_CARRIER_CAPTURE"), 100);
	dStatPushBack(string_to_hash("AWD_KILL_PSYCHOPATHS"), 100);
	dStatPushBack(string_to_hash("AWD_KILL_TEAM_YOURSELF_LTS"), 25);
	dStatPushBack(string_to_hash("AWD_LAPDANCES"), 25);
	dStatPushBack(string_to_hash("AWD_LESTERDELIVERVEHICLES"), 25);
	dStatPushBack(string_to_hash("AWD_MENTALSTATE_TO_NORMAL"), 50);
	dStatPushBack(string_to_hash("AWD_NIGHTVISION_KILLS"), 100);
	dStatPushBack(string_to_hash("AWD_NO_HAIRCUTS"), 25);
	dStatPushBack(string_to_hash("AWD_ODISTRACTCOPSNOEATH"), 25);
	dStatPushBack(string_to_hash("AWD_ONLY_PLAYER_ALIVE_LTS"), 50);
	dStatPushBack(string_to_hash("AWD_PARACHUTE_JUMPS_20M"), 50);
	dStatPushBack(string_to_hash("AWD_PARACHUTE_JUMPS_50M"), 50);
	dStatPushBack(string_to_hash("AWD_PASSENGERTIME"), 4);
	dStatPushBack(string_to_hash("AWD_PICKUP_CAP_PACKAGES"), 100);
	dStatPushBack(string_to_hash("AWD_RACES_WON"), 50);
	dStatPushBack(string_to_hash("AWD_SECURITY_CARS_ROBBED"), 25);
	dStatPushBack(string_to_hash("AWD_TAKEDOWNSMUGPLANE"), 50);
	dStatPushBack(string_to_hash("AWD_TIME_IN_HELICOPTER"), 4);
	dStatPushBack(string_to_hash("AWD_TRADE_IN_YOUR_PROPERTY"), 25);
	dStatPushBack(string_to_hash("AWD_VEHICLES_JACKEDR"), 500);
	dStatPushBack(string_to_hash("AWD_WIN_AT_DARTS"), 25);
	dStatPushBack(string_to_hash("AWD_WIN_CAPTURE_DONT_DYING"), 25);
	dStatPushBack(string_to_hash("AWD_WIN_CAPTURES"), 50);
	dStatPushBack(string_to_hash("AWD_WIN_GOLD_MEDAL_HEISTS"), 25);
	dStatPushBack(string_to_hash("AWD_WIN_LAST_TEAM_STANDINGS"), 50);
	dStatPushBack(string_to_hash("AWD_DANCE_TO_SOLOMUN"), 100);
	dStatPushBack(string_to_hash("AWD_DANCE_TO_TALEOFUS"), 100);
	dStatPushBack(string_to_hash("AWD_DANCE_TO_DIXON"), 100);
	dStatPushBack(string_to_hash("AWD_DANCE_TO_BLKMAD"), 100);
	dStatPushBack(string_to_hash("AWD_CLUB_DRUNK"), 200);
	dStatPushBack(string_to_hash("AWD_WATCH_YOUR_STEP"), 50);
	dStatPushBack(string_to_hash("AWD_TOWER_OFFENSE"), 50);
	dStatPushBack(string_to_hash("AWD_READY_FOR_WAR"), 50);
	dStatPushBack(string_to_hash("AWD_THROUGH_A_LENS"), 50);
	dStatPushBack(string_to_hash("AWD_SPINNER"), 50);
	dStatPushBack(string_to_hash("AWD_YOUMEANBOOBYTRAPS"), 50);
	dStatPushBack(string_to_hash("AWD_MASTER_BANDITO"), 50);
	dStatPushBack(string_to_hash("AWD_SITTING_DUCK"), 50);
	dStatPushBack(string_to_hash("AWD_CROWDPARTICIPATION"), 50);
	dStatPushBack(string_to_hash("AWD_KILL_OR_BE_KILLED"), 50);
	dStatPushBack(string_to_hash("AWD_MASSIVE_SHUNT"), 25);
	dStatPushBack(string_to_hash("AWD_YOURE_OUTTA_HERE"), 200);
	dStatPushBack(string_to_hash("AWD_WEVE_GOT_ONE"), 50);
	dStatPushBack(string_to_hash("AWD_ARENA_WAGEWORKER"), 20000000);
	dStatPushBack(string_to_hash("AWD_TIME_SERVED"), 1000);
	dStatPushBack(string_to_hash("AWD_TOP_SCORE"), 500000);
	dStatPushBack(string_to_hash("AWD_CAREER_WINNER"), 1000);
}

void hack::unlockClothes(float* arg)
{
	dStatPushBack(string_to_hash("CHAR_FM_CLOTHES_1_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CLOTHES_2_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CLOTHES_3_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CLOTHES_4_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CLOTHES_5_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CLOTHES_6_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CLOTHES_7_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CLOTHES_8_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CLOTHES_9_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CLOTHES_10_UNLCK"), -1);
	dStatPushBack(string_to_hash("CHAR_FM_CLOTHES_11_UNLCK"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_OUTFIT"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_OUTFIT"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_JBIB"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_JBIB_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_JBIB_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_JBIB_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_JBIB_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_JBIB_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_JBIB_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_FEET"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_FEET_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_FEET_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_FEET_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_FEET_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_FEET_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_FEET_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_HAIR_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_OUTFIT"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_OUTFIT"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_JBIB_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_JBIB"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_JBIB_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_JBIB_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_JBIB_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_JBIB_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_JBIB_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_JBIB_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_LEGS_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_LEGS_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_FEET_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_FEET"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_FEET_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_FEET_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_FEET_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_FEET_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_FEET_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_FEET_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_FEET_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_PROPS"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_PROPS_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_PROPS_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_PROPS_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_PROPS_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_PROPS_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_PROPS_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_PROPS_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_PROPS_8"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_PROPS_9"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_PROPS_10"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_TEETH"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_TEETH_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_TEETH_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_TEETH"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_TEETH_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_TEETH_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_BERD"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_BERD_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_BERD_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_BERD_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_BERD_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_BERD_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_BERD_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_BERD_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_BERD"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_BERD_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_BERD_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_BERD_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_BERD_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_BERD_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_BERD_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_BERD_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_TORSO"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_TORSO"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_SPECIAL"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_SPECIAL_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_SPECIAL_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_SPECIAL_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_SPECIAL_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_SPECIAL_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_SPECIAL_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_SPECIAL_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_SPECIAL2"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_SPECIAL2_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_SPECIAL"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_SPECIAL_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_SPECIAL_2"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_SPECIAL_3"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_SPECIAL_4"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_SPECIAL_5"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_SPECIAL_6"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_SPECIAL_7"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_SPECIAL2"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_SPECIAL2_1"), -1);
	dStatPushBack(string_to_hash("CLTHS_AVAILABLE_DECL"), -1);
	dStatPushBack(string_to_hash("CLTHS_ACQUIRED_DECL"), -1);
	for (size_t i = 0; i <= 205; i++)
	{
		dStatPushBack(string_to_hash("DLC_APPAREL_ACQUIRED_" + std::to_string(i)), -1);
	}
	for (size_t i = 0; i <= 31; i++)
	{
		dStatPushBack(string_to_hash("ADMIN_CLOTHES_GV_BS_" + std::to_string(i)), -1);
	}
}

void hack::intoPV(float* arg)
{
	scriptGlobal(2409287).at(8).as<int>() = 1;
	Sleep(500);
	if (scriptGlobal(2409287).at(8).as<int>().value() == 1)
	{
		scriptGlobal(2409287).at(8).as<int>() = 0;
	}
}

void hack::loadSession(float* arg)
{
	if (m_global.initSessionPtr(m_hModule))
	{
		m_global.setSessionID((int)*arg);
		m_global.setSessionTransition(1);
		Sleep(400);
		m_global.setSessionTransition(0);
	}
}

void hack::forwardTeleport(float* arg)
{
	m_player.getPos();
	v3 v3Pos = m_player.m_v3Pos;
	m_player.getCos();
	m_player.getSin();
	float fAngle = m_player.m_fCos;
	float fAngle2 = m_player.m_fSin;
	v3Pos.x +=*arg * fAngle2;
	v3Pos.y +=*arg * fAngle;
	teleport(v3Pos);
}

void hack::spawnVehicle(float* arg)
{
	if (m_global.initVehiclePtr(m_hModule))
	{
		int vehTypeIndex = (int)*arg / 1000;
		int vehIndex = (int)*arg % 1000;
		m_player.getPos();
		v3 v3Pos = m_player.m_v3Pos;
		m_player.getCos();
		m_player.getSin();
		float fAngle = m_player.m_fCos;
		float fAngle2 = m_player.m_fSin;
		v3Pos.x += 6 * fAngle2;
		v3Pos.y += 6 * fAngle;

		m_global.setVehicleHash(joaat(vehiclePreview[vehTypeIndex].second[vehIndex].VCode));
		m_global.setVehicleKickPrevent1(2);
		m_global.setVehicleKickPrevent2(14);
		m_global.setSecondaryColor(-1);
		m_global.setPrimaryColor(-1);
		m_global.setVehiclePosX(v3Pos.x);
		m_global.setVehiclePosY(v3Pos.y);
		m_global.setVehiclePosZ(-255);
		if (true)
		{
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 10, m_hModule), 1);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 11, m_hModule), 1);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 12, m_hModule), 1);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 13, m_hModule), 1);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 14, m_hModule), 1);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 15, m_hModule), 1);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 16, m_hModule), 1);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 17, m_hModule), 1);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 18, m_hModule), 1);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 19, m_hModule), 1);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 20, m_hModule), 1);

			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 21, m_hModule), 4);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 22, m_hModule), 3);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 23, m_hModule), 3);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 24, m_hModule), 57);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 25, m_hModule), 4);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 26, m_hModule), 5);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 28, m_hModule), 1);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 30, m_hModule), 1);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 32, m_hModule), 1);
			g_pMemMan->writeMem<BYTE>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 65, m_hModule), 1);

			g_pMemMan->writeMem<int>(m_global.getGlobal(GLOBAL_VEHICLE_HASH + 27 + 77, m_hModule), 0xF0400200);
		}

		m_global.setVehicleSpawn2(1);
		m_global.setVehicleSpawn1(1);
	}
}

void hack::selfDropMoney(feat* feature)
{
	if (!m_bSelfDropInit)
	{
		m_bSelfDropInit = true;
		std::thread t([=] {
			while (true)
			{
				if (feature->m_bOn)
				{
					if (m_global.initMoneyPtr(m_hModule))
					{
						m_player.getPos();
						m_global.setMoneyObject(1);
						m_global.setMoneyVal(2000);
						m_global.setMoneyPosX(m_player.m_v3Pos.x);
						m_global.setMoneyPosY(m_player.m_v3Pos.y);
						m_global.setMoneyPosZ(m_player.m_v3Pos.z + 5);
						m_global.findMoneyPtr(m_hModule);
						m_global.setMoneyCall(2);
					}
				}
				Sleep(100);
			}
		});
		t.detach();
	}
}


void hack::dStatPushBack(unsigned int hash, int value)
{
	this->m_dStat.push_back(std::pair<unsigned int, int>(hash, value));
}

void hack::callMerryweather(std::ptrdiff_t index)
{
	scriptGlobal(GLOBAL_MERRYWEATHER).at(index).as<int>() = 1;
}

void hack::consumeStatQueue()
{
	if (!m_bInit)
	{
		m_bInit = true;
		std::thread tConsumeStatQueue([=] {
			while (true)
			{
				if (!m_dStat.empty())
				{
					g_pD3D9Render->m_bMBShowing = true;
					g_pD3D9Render->m_sTitle = L"正在处理队列";
					g_pD3D9Render->m_sDetail = L"剩余" + std::to_wstring(m_dStat.size()) + L"个待处理";
					if (m_global.initStatPtr(m_hModule))
					{
						m_global.getStatHash();
						unsigned int resotreHash = m_global.m_dwStatHash;
						m_global.getStatValue();
						DWORD resotreValue = m_global.m_dwStatValue;
						m_global.getStatCall();
						DWORD call = m_global.m_dwStatCall;

						m_global.setStatHash(m_dStat.front().first);
						m_global.setStatValue(m_dStat.front().second);
						m_global.setStatCall(-1);
						Sleep(1000);
						m_global.setStatHash(resotreHash);
						m_global.setStatValue(resotreValue);
					}
					m_dStat.pop_front();
				}
				else
				{
					g_pD3D9Render->m_bMBShowing = false;
				}
				Sleep(1);
			}
		});
		tConsumeStatQueue.detach();
	}
}

void hack::killAllNpc(float* arg)
{
	m_replayInterface.initPeds();
	for (size_t i = 0; i < m_replayInterface.dw_curPedNum; i++)
	{
		if (i == 10)
			continue;
		m_replayInterface.g_pPedList[i]->getHealth();
		if (m_replayInterface.g_pPedList[i]->m_cmHp.cur > 0)
		{
			m_replayInterface.g_pPedList[i]->setHealth(0);
		}
	}
}

void hack::renderPlayerList(int parent,int playerList[32])
{
	for (size_t i = 0; i < 32; i++)
	{
		g_pSettings->updataFeature(playerList[i], -1, parent, L"玩家" + std::to_wstring(i), feat_teleport,tp_static, -1336.f, -3044.f, -225.f);
	}
}

void hack::waterProof(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_player.m_playerDataCur.m_dwWaterProof != m_player.m_playerDataRestore.m_dwWaterProof)
				m_player.setWaterProof(m_player.m_playerDataRestore.m_dwWaterProof);
			feature->m_bRestored = true;
		}
		return;
	}
	if (m_player.m_playerDataCur.m_dwWaterProof != m_player.m_playerDataRestore.m_dwWaterProof + 0x1000000)
		m_player.setWaterProof(m_player.m_playerDataRestore.m_dwWaterProof + 0x1000000);
	return;
}

void hack::undeadOffradar(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_player.m_playerDataCur.m_maxHealth != m_player.m_playerDataRestore.m_maxHealth)
				m_player.setMaxHealth(m_player.m_playerDataRestore.m_maxHealth);
			feature->m_bRestored = true;
		}
		return;
	}
	if (m_player.m_playerDataCur.m_maxHealth > 0)
		m_player.setMaxHealth(0);
	return;
}

void hack::superPunch(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_player.getVehicleDamageMult();
			if (m_player.m_flVehicleDamageMult != 1)
				m_player.setVehicleDamageMult(1);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_player.m_flVehicleDamageMult != fValue)
		m_player.setVehicleDamageMult(fValue);
	return;
}

void hack::noSpread(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_weapon.m_weapDataCur.m_fSpread != m_weapon.m_weapDataRestore.m_fSpread)
				m_weapon.setSpread(m_weapon.m_weapDataRestore.m_fSpread);
			feature->m_bRestored = true;
		}
		return;
	}
	if (m_weapon.m_weapDataCur.m_fSpread != 0)
		m_weapon.setSpread(0);
	return;
}

void hack::noRecoil(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_weapon.m_weapDataCur.m_fRecoil != m_weapon.m_weapDataRestore.m_fRecoil)
				m_weapon.setRecoil(m_weapon.m_weapDataRestore.m_fRecoil);
			feature->m_bRestored = true;
		}
		return;
	}
	if (m_weapon.m_weapDataCur.m_fRecoil != 0)
		m_weapon.setRecoil(0);
	return;
}

void hack::quickReload(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_weapon.m_weapDataCur.m_fReload != m_weapon.m_weapDataRestore.m_fReload || m_weapon.m_weapDataCur.m_fReloadVeh != m_weapon.m_weapDataRestore.m_fReloadVeh)
			{
				m_weapon.setReloadSpeed(m_weapon.m_weapDataRestore.m_fReload);
				m_weapon.setReloadVehicle(m_weapon.m_weapDataRestore.m_fReloadVeh);
			}
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = m_weapon.m_weapDataRestore.m_fReload * static_cast<featSlider*>(feature)->m_fValue;
	if (m_weapon.m_weapDataCur.m_fReload != fValue)
		m_weapon.setReloadSpeed(fValue);
	if (m_weapon.m_weapDataCur.m_fReloadVeh != 0)
		m_weapon.setReloadVehicle(0);
	return;
}

void hack::bulletDamage(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_weapon.m_weapDataCur.m_fDamage != m_weapon.m_weapDataRestore.m_fDamage)
				m_weapon.setBulletDamage(m_weapon.m_weapDataRestore.m_fDamage);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = m_weapon.m_weapDataRestore.m_fDamage * static_cast<featSlider*>(feature)->m_fValue;
	if (m_weapon.m_weapDataCur.m_fDamage != fValue)
		m_weapon.setBulletDamage(fValue);
	return;
}

void hack::weaponRange(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_weapon.m_weapDataCur.m_fRange != m_weapon.m_weapDataRestore.m_fRange)
				m_weapon.setRange(m_weapon.m_weapDataRestore.m_fRange);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = m_weapon.m_weapDataRestore.m_fRange * static_cast<featSlider*>(feature)->m_fValue;
	if (m_weapon.m_weapDataCur.m_fRange != fValue)
		m_weapon.setRange(fValue);
	return;
}

void hack::weaponSpin(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_weapon.m_weapDataCur.m_fSpinUp != m_weapon.m_weapDataRestore.m_fSpinUp || m_weapon.m_weapDataCur.m_fSpin != m_weapon.m_weapDataRestore.m_fSpin)
			{
				m_weapon.setSpinUp(m_weapon.m_weapDataRestore.m_fSpinUp);
				m_weapon.setSpin(m_weapon.m_weapDataRestore.m_fSpin);
			}
			feature->m_bRestored = true;
		}
		return;
	}
	if (m_weapon.m_weapDataCur.m_fSpinUp != 0 || m_weapon.m_weapDataCur.m_fSpin != 0)
	{
		m_weapon.setSpin(0);
		m_weapon.setSpinUp(0);
	}
	return;
}

void hack::weaponForceOnPed(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_weapon.m_weapDataCur.m_fForceOnPed != m_weapon.m_weapDataRestore.m_fForceOnPed)
				m_weapon.setForceOnPed(m_weapon.m_weapDataRestore.m_fForceOnPed);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = m_weapon.m_weapDataRestore.m_fForceOnPed * static_cast<featSlider*>(feature)->m_fValue;
	if (m_weapon.m_weapDataCur.m_fForceOnPed != fValue)
		m_weapon.setForceOnPed(fValue);
	return;
}

void hack::weaponForceOnVehicle(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_weapon.m_weapDataCur.m_fForceOnVehicle != m_weapon.m_weapDataRestore.m_fForceOnVehicle)
				m_weapon.setForceOnVehicle(m_weapon.m_weapDataRestore.m_fForceOnVehicle);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = m_weapon.m_weapDataRestore.m_fForceOnVehicle * static_cast<featSlider*>(feature)->m_fValue;
	if (m_weapon.m_weapDataCur.m_fForceOnVehicle != fValue)
		m_weapon.setForceOnVehicle(fValue);
	return;
}

void hack::weaponForceOnHeli(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_weapon.m_weapDataCur.m_fForceOnHeli != m_weapon.m_weapDataRestore.m_fForceOnHeli)
				m_weapon.setForceOnHeli(m_weapon.m_weapDataRestore.m_fForceOnHeli);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = m_weapon.m_weapDataRestore.m_fForceOnHeli * static_cast<featSlider*>(feature)->m_fValue;
	if (m_weapon.m_weapDataCur.m_fForceOnHeli != fValue)
		m_weapon.setForceOnHeli(fValue);
	return;
}

void hack::weaponBulletEdit(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_weapon.m_weapDataCur.m_dwImpactType != m_weapon.m_weapDataRestore.m_dwImpactType)
				m_weapon.setImpactType(m_weapon.m_weapDataRestore.m_dwImpactType);
			if (m_weapon.m_weapDataCur.m_dwImpactExplosion != ImpactExplosionEnum::DefaultBullets)
				m_weapon.setImpactExplosion(ImpactExplosionEnum::DefaultBullets);
			feature->m_bRestored = true;
		}
		return;
	}
	if (m_weapon.m_weapDataCur.m_dwImpactType != ImpactTypeEnum::Explosives)
		m_weapon.setImpactType(ImpactTypeEnum::Explosives);
	if (m_weapon.m_weapDataCur.m_dwImpactExplosion !=  m_explosion)
		m_weapon.setImpactExplosion(m_explosion);
	return;
}

void hack::runSpeed(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_player.getRunSpeed();
			if (m_player.m_flRunSpd > 1.f)
				m_player.setRunSpeed(1.f);
			feature->m_bRestored = true;
		}
		return;
	}
	m_player.getRunSpeed();
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_player.m_flRunSpd != fValue)
		m_player.setRunSpeed(fValue);
	return;
}

void hack::swimSpeed(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_player.getSwimSpeed();
			if (m_player.m_flSwimSpd > 1.f)
				m_player.setSwimSpeed(1.f);
			feature->m_bRestored = true;
		}
		return;
	}
	m_player.getSwimSpeed();
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_player.m_flSwimSpd != fValue)
		m_player.setSwimSpeed(fValue);
	return;
}

void hack::npcIgnore(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_player.getNpcIgnore();
			if (m_player.m_dwNpcIgnore > 0)
				m_player.setNpcIgnore(0);
			feature->m_bRestored = true;
		}
		return;
	}
	m_player.getNpcIgnore();
	if (m_player.m_dwNpcIgnore != 0x450000)
		m_player.setNpcIgnore(0x450000);
	return;
}

void hack::godMode(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_player.getGod();
			if (m_player.m_btGod > 0)
				m_player.setGod(0);
			feature->m_bRestored = true;
		}
		return;
	}
	m_player.getGod();
	if (m_player.m_btGod < 1)
		m_player.setGod(1);
	return;
}

void hack::frameFlags(feat* featSuperJump, feat* featExplosiveMelee, feat* featFireAmmo, feat* featExplosiveAmmo)
{
	if (!featSuperJump->m_bOn && !featExplosiveMelee->m_bOn && !featFireAmmo->m_bOn && !featExplosiveAmmo->m_bOn)
		return;
	DWORD dwValue = 0;
	if (featSuperJump->m_bOn)
		dwValue += 64;
	if (featExplosiveMelee->m_bOn)
		dwValue += 32;
	if (featFireAmmo->m_bOn)
		dwValue += 16;
	if (featExplosiveAmmo->m_bOn)
		dwValue += 8;
	m_player.setFrameFlags(dwValue);
	return;
}

void hack::vehicleGod(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_vehicle.getGod();
			if (m_vehicle.m_btGod > 0)
				m_vehicle.setGod(0);
			feature->m_bRestored = true;
		}
		return;
	}
	m_vehicle.getGod();
	if (m_vehicle.m_btGod < 1)
		m_vehicle.setGod(1);
	return;
}

void hack::infAmmo(feat* feature)
{
	BYTE	cur[3] = {};
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			g_pMemMan->readMem<BYTE>((DWORD_PTR)m_hModule + ADDRESS_AMMO, cur, sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
			BYTE	value[3] = { 0x41, 0x2B, 0xD1 };
			if (cur[0] != value[0])
				g_pMemMan->writeMem<BYTE>((DWORD_PTR)m_hModule + ADDRESS_AMMO, value , sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
			feature->m_bRestored = true;
		}
		return;
	}
	g_pMemMan->readMem<BYTE>((DWORD_PTR)m_hModule + ADDRESS_AMMO, cur, sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
	BYTE	value[3] = { 0x90, 0x90, 0x90 };
	if (cur[0] != value[0])
		g_pMemMan->writeMem<BYTE>((DWORD_PTR)m_hModule + ADDRESS_AMMO, value, sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
	return;
}

void hack::noReload(feat* feature)
{
	BYTE	cur[3] = {};
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			g_pMemMan->readMem<BYTE>((DWORD_PTR)m_hModule + ADDRESS_MAGAZINE, cur, sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
			BYTE	value[3] = { 0x41, 0x2B, 0xC9 };
			if (cur[0] != value[0])
				g_pMemMan->writeMem<BYTE>((DWORD_PTR)m_hModule + ADDRESS_MAGAZINE, value, sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
			feature->m_bRestored = true;
		}
		return;
	}
	g_pMemMan->readMem<BYTE>((DWORD_PTR)m_hModule + ADDRESS_MAGAZINE, cur, sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
	BYTE	value[3] = { 0x90, 0x90, 0x90 };
	if (cur[0] != value[0])
		g_pMemMan->writeMem<BYTE>((DWORD_PTR)m_hModule + ADDRESS_MAGAZINE, value, sizeof(BYTE) * 3, PAGE_EXECUTE_READWRITE);
	return;
}

void hack::seatbelt(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_player.getSeatbelt();
			if (m_player.m_btSeatbelt & 0x01)
				m_player.setSeatbelt(m_player.m_btSeatbelt ^ 0x01);
			feature->m_bRestored = true;
		}
		return;
	}
	m_player.getSeatbelt();
	if (!(m_player.m_btSeatbelt & 0x01))
		m_player.setSeatbelt(m_player.m_btSeatbelt | 0x01);
	return;
}

void hack::noRagdoll(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_player.getRagdoll();
			if (!(m_player.m_btRagdoll & 0x20))
				m_player.setRagdoll(m_player.m_btRagdoll | 0x20);
			feature->m_bRestored = true;
		}
		return;
	}
	m_player.getRagdoll();
	if (m_player.m_btRagdoll & 0x20)
		m_player.setRagdoll(m_player.m_btRagdoll ^ 0x20);
	return;
}

void hack::vehicleAccel(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_vehicle.m_handlingCur.m_fAcceleration != m_vehicle.m_handlingRestore.m_fAcceleration)
				m_vehicle.setAcceleration(m_vehicle.m_handlingRestore.m_fAcceleration);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = m_vehicle.m_handlingRestore.m_fAcceleration * static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_handlingCur.m_fAcceleration != fValue)
		m_vehicle.setAcceleration(fValue);
	return;
}

void hack::vehicleBrake(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_vehicle.m_handlingCur.m_fBrakeForce != m_vehicle.m_handlingRestore.m_fBrakeForce)
				m_vehicle.setBrakeForce(m_vehicle.m_handlingRestore.m_fBrakeForce);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = m_vehicle.m_handlingRestore.m_fBrakeForce * static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_handlingCur.m_fBrakeForce != fValue)
		m_vehicle.setBrakeForce(fValue);
	return;
}

void hack::neverWanted(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_player.getWantedCanChange();
			if (m_player.m_flWantedCanChange != 1.f)
				m_player.setWantedCanChange(1.f);
			feature->m_bRestored = true;
		}
		return;
	}
	m_player.getWantedCanChange();
	if (m_player.m_flWantedCanChange != 0.f)
		m_player.setWantedCanChange(0.f);
	return;
}

void hack::vehicleTraction(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_vehicle.m_handlingCur.m_fTractionCurveMin != m_vehicle.m_handlingRestore.m_fTractionCurveMin)
				m_vehicle.setTractionCurveMin(m_vehicle.m_handlingRestore.m_fTractionCurveMin);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = m_vehicle.m_handlingRestore.m_fTractionCurveMin * static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_handlingCur.m_fTractionCurveMin != fValue)
		m_vehicle.setTractionCurveMin(fValue);
	return;
}

void hack::vehicleGravity(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_vehicle.getGravity();
			if (m_vehicle.m_fGravity != 9.8f)
				m_vehicle.setGravity(9.8f);
			feature->m_bRestored = true;
		}
		return;
	}
	m_vehicle.getGravity();
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_fGravity != fValue)
		m_vehicle.setGravity(fValue);
	return;
}

void hack::vehicleBulletproofTires(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_vehicle.getBulletproofTires();
			if (m_vehicle.m_btBulletproofTires & 0x20)
				m_vehicle.setBulletproofTires(m_vehicle.m_btBulletproofTires ^ 0x20);
			feature->m_bRestored = true;
		}
		return;
	}
	m_vehicle.getBulletproofTires();
	if (!(m_vehicle.m_btBulletproofTires & 0x20))
		m_vehicle.setBulletproofTires(m_vehicle.m_btBulletproofTires | 0x20);
	return;
}

void hack::wanted(feat* feature)
{
	if (!feature->m_bOn)
		return;
	m_player.getWanted();
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_player.m_dwWanted != (DWORD)fValue)
	{
		m_player.setWanted((DWORD)fValue);
		feat* fpNeverWanted = g_pSettings->getFeature(g_iFeature[FEATURE_P_NEVERWANTED]);
		if (fpNeverWanted->m_bOn)
			fpNeverWanted->toggle();
	}
	return;
}

void hack::bulletBatch(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_weapon.m_weapDataCur.m_dwBulletBatch != m_weapon.m_weapDataRestore.m_dwBulletBatch)
				m_weapon.setBulletBatch(m_weapon.m_weapDataRestore.m_dwBulletBatch);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = m_weapon.m_weapDataRestore.m_dwBulletBatch * static_cast<featSlider*>(feature)->m_fValue;
	fValue = (fValue > 25.f) ? 25.f : fValue;
	if (m_weapon.m_weapDataCur.m_dwBulletBatch != fValue)
		m_weapon.setBulletBatch((DWORD)fValue);
	return;
}

void hack::muzzleVelocity(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_weapon.m_weapDataCur.m_fMuzzleVelocity != m_weapon.m_weapDataRestore.m_fMuzzleVelocity)
				m_weapon.setMuzzleVelocity(m_weapon.m_weapDataRestore.m_fMuzzleVelocity);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = m_weapon.m_weapDataRestore.m_fMuzzleVelocity * static_cast<featSlider*>(feature)->m_fValue;
	if (m_weapon.m_weapDataCur.m_fMuzzleVelocity != fValue)
		m_weapon.setMuzzleVelocity(fValue);
	return;
}

void hack::vehicleDeformation(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_vehicle.m_handlingCur.m_fDeformationDamageMult != m_vehicle.m_handlingRestore.m_fDeformationDamageMult)
				m_vehicle.setDeformationDamageMult(m_vehicle.m_handlingRestore.m_fDeformationDamageMult);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_handlingCur.m_fDeformationDamageMult != fValue)
		m_vehicle.setDeformationDamageMult(fValue);
	return;
}

void hack::vehicleUpShift(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_vehicle.m_handlingCur.m_fUpShift != m_vehicle.m_handlingRestore.m_fUpShift)
				m_vehicle.setUpShift(m_vehicle.m_handlingRestore.m_fUpShift);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_handlingCur.m_fUpShift != fValue)
		m_vehicle.setUpShift(fValue);
	return;
}

void hack::vehicleSuspensionForce(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_vehicle.m_handlingCur.m_fSuspensionForce != m_vehicle.m_handlingRestore.m_fSuspensionForce)
				m_vehicle.setSuspensionForce(m_vehicle.m_handlingRestore.m_fSuspensionForce);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = m_vehicle.m_handlingRestore.m_fSuspensionForce * static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_handlingCur.m_fSuspensionForce != fValue)
		m_vehicle.setSuspensionForce(fValue);
	return;
}

void	hack::vehicleDownShift(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_vehicle.m_handlingCur.m_fDownShift != m_vehicle.m_handlingRestore.m_fDownShift)
				m_vehicle.setDownShift(m_vehicle.m_handlingRestore.m_fDownShift);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_handlingCur.m_fDownShift != fValue)
		m_vehicle.setDownShift(fValue);
	return;
}

void hack::vehicleMass(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_vehicle.m_handlingCur.m_fMass != m_vehicle.m_handlingRestore.m_fMass)
				m_vehicle.setMass(m_vehicle.m_handlingRestore.m_fMass);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_handlingCur.m_fMass != fValue)
		m_vehicle.setMass(fValue);
	return;
}

void hack::vehicleBuoyancy(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_vehicle.m_handlingCur.m_fBuoyancy != m_vehicle.m_handlingRestore.m_fBuoyancy)
				m_vehicle.setBuoyancy(m_vehicle.m_handlingRestore.m_fBuoyancy);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_handlingCur.m_fBuoyancy != fValue)
		m_vehicle.setBuoyancy(fValue);
	return;
}

void hack::vehicleHandbrakeForce(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_vehicle.m_handlingCur.m_fHandbrakeForce != m_vehicle.m_handlingRestore.m_fHandbrakeForce)
				m_vehicle.setHandbrakeForce(m_vehicle.m_handlingRestore.m_fHandbrakeForce);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_handlingCur.m_fHandbrakeForce != fValue)
		m_vehicle.setHandbrakeForce(fValue);
	return;
}

void hack::boost(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_vehicle.getBoost();
			if (m_vehicle.m_fBoost != 1.25)
				m_vehicle.setBoost(1.25);
			feature->m_bRestored = true;
		}
		return;
	}
	m_vehicle.getBoost();
	if (m_vehicle.m_fBoost != 1.25)
		m_vehicle.setBoost(1.25);
	return;
}

void hack::vehicleRocketRechargeSpeed(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_vehicle.getRocketRechargeSpeed();
			if (m_vehicle.m_fRocketRechargeSpeed != 0.5f)
				m_vehicle.setRocketRechargeSpeed(0.5f);
			feature->m_bRestored = true;
		}
		return;
	}
	m_vehicle.getRocketRechargeSpeed();
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_fRocketRechargeSpeed != fValue)
		m_vehicle.setRocketRechargeSpeed(fValue);
	return;
}

void hack::vehicleSuspensionHeigh(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_vehicle.m_handlingCur.m_fSuspensionHeigh != m_vehicle.m_handlingRestore.m_fSuspensionHeigh)
				m_vehicle.setSuspensionHeigh(m_vehicle.m_handlingRestore.m_fSuspensionHeigh);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_handlingCur.m_fSuspensionHeigh != fValue)
		m_vehicle.setSuspensionHeigh(fValue);
	return;
}

void hack::vehicleColisionDamageMult(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_vehicle.m_handlingCur.m_fColisionDamageMult != m_vehicle.m_handlingRestore.m_fColisionDamageMult)
				m_vehicle.setColisionDamageMult(m_vehicle.m_handlingRestore.m_fColisionDamageMult);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_handlingCur.m_fColisionDamageMult != fValue)
		m_vehicle.setColisionDamageMult(fValue);
	return;
}

void hack::vehicleWeaponDamageMult(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_vehicle.m_handlingCur.m_fWeaponDamageMult != m_vehicle.m_handlingRestore.m_fWeaponDamageMult)
				m_vehicle.setWeaponDamageMult(m_vehicle.m_handlingRestore.m_fWeaponDamageMult);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_handlingCur.m_fWeaponDamageMult != fValue)
		m_vehicle.setWeaponDamageMult(fValue);
	return;
}

void hack::vehicleEngineDamageMult(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			if (m_vehicle.m_handlingCur.m_fEngineDamageMult != m_vehicle.m_handlingRestore.m_fEngineDamageMult)
				m_vehicle.setEngineDamageMult(m_vehicle.m_handlingRestore.m_fEngineDamageMult);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	if (m_vehicle.m_handlingCur.m_fEngineDamageMult != fValue)
		m_vehicle.setEngineDamageMult(fValue);
	return;
}

void hack::tunableRpMult(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_tunable.getRpMult();
			if (m_tunable.m_fRpMult != 1)
				m_tunable.setRpMult(1);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	m_tunable.getRpMult();
	if (m_tunable.m_fRpMult != fValue)
		m_tunable.setRpMult(fValue);
	return;
}

void hack::tunableApMult(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_tunable.getApMult();
			if (m_tunable.m_fApMult != 1)
				m_tunable.setApMult(1);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	m_tunable.getApMult();
	if (m_tunable.m_fApMult != fValue)
		m_tunable.setApMult(fValue);
	return;
}

void hack::tunableMissionPayout(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_tunable.getMinMissionPayout();
			if (m_tunable.m_fMinMissionPayout != 0)
				m_tunable.setMinMissionPayout(0);
			feature->m_bRestored = true;
		}
		return;
	}
	float fValue = static_cast<featSlider*>(feature)->m_fValue;
	m_tunable.getMinMissionPayout();
	if (m_tunable.m_fMinMissionPayout != fValue)
		m_tunable.setMinMissionPayout(fValue);
	return;
}

void hack::tunableOrbitalCannonCooldown(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_tunable.getOrbitalCannonCooldown();
			if (m_tunable.m_dwOrbitalCannonCooldown != 2880000)
				m_tunable.setOrbitalCannonCooldown(2880000);
			feature->m_bRestored = true;
		}
		return;
	}
	m_tunable.getOrbitalCannonCooldown();
	if (m_tunable.m_dwOrbitalCannonCooldown != 0)
		m_tunable.setOrbitalCannonCooldown(0);
	return;
}

void hack::tunableBunkerResearch(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_tunable.getBunkerResearch();
			if (m_tunable.m_dwBunkerResearch != 0)
				m_tunable.setBunkerResearch(0);
			feature->m_bRestored = true;
		}
		return;
	}
	m_tunable.getBunkerResearch();
	if (m_tunable.m_dwBunkerResearch != 1)
		m_tunable.setBunkerResearch(1);
	return;
}

void hack::tunableAntiIdleKick(feat* feature)
{
	if (!feature->m_bOn)
	{
		if (!feature->m_bRestored)
		{
			m_tunable.getAntiIdleKick1();
			m_tunable.getAntiIdleKick2();
			m_tunable.getAntiIdleKick3();
			m_tunable.getAntiIdleKick4();
			if (m_tunable.m_dwAntiIdleKick1 != 120000)
				m_tunable.setAntiIdleKick1(120000);
			if (m_tunable.m_dwAntiIdleKick2 != 300000)
				m_tunable.setAntiIdleKick2(300000);
			if (m_tunable.m_dwAntiIdleKick3 != 600000)
				m_tunable.setAntiIdleKick3(600000);
			if (m_tunable.m_dwAntiIdleKick4 != 900000)
				m_tunable.setAntiIdleKick4(900000);
			feature->m_bRestored = true;
		}
		return;
	}
	m_tunable.getAntiIdleKick1();
	m_tunable.getAntiIdleKick2();
	m_tunable.getAntiIdleKick3();
	m_tunable.getAntiIdleKick4();
	if (m_tunable.m_dwAntiIdleKick1 != 2000000000)
		m_tunable.setAntiIdleKick1(2000000000);
	if (m_tunable.m_dwAntiIdleKick2 != 2000000000)
		m_tunable.setAntiIdleKick2(2000000000);
	if (m_tunable.m_dwAntiIdleKick3 != 2000000000)
		m_tunable.setAntiIdleKick3(2000000000);
	if (m_tunable.m_dwAntiIdleKick4 != 2000000000)
		m_tunable.setAntiIdleKick4(2000000000);
	return;
}

void hack::ammoDrop(float* arg)
{
	callMerryweather(833);
}

void hack::miniGunDrop(float* arg)
{
	callMerryweather(843);
}

void hack::boatTaxi(float* arg)
{
	callMerryweather(834);
}

void hack::heliTaxi(float* arg)
{
	callMerryweather(835);
}

void hack::backupHeli(float* arg)
{
	callMerryweather(4389);
}

void hack::airstrike(float* arg)
{
	callMerryweather(4390);
}

void hack::about(float* arg)
{
	switch ((int)*arg)
	{
	case 0:
		WinExec("explorer.exe https://github.com/AmazingPP/subVerison_GTAV_Hack", SW_NORMAL);
		break;
	case 1:
		WinExec("explorer.exe https://github.com/AmazingPP/subVerison_GTAV_Hack/releases", SW_NORMAL);
		break;
	default:
		break;
	}
	
}
