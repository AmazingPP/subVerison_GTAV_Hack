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

//#define __SC_VERSION__
//#define __STEAM_VERSION__

#ifndef STDAFX_H
#define STDAFX_H

#include <windows.h>
#include <windowsx.h>
#include <string>
#include <sstream>
#include <fstream>
#include <ctime>
#include <psapi.h>
#include <vector>
#include <regex>
#include <string>
#include <map>

#include <Dwmapi.h>
#pragma comment(lib,"Dwmapi.lib")

#include "d3d9/d3d9.h"
#include "d3d9/d3dx9.h"
#pragma comment(lib,"d3d9/d3dx9.lib")
#pragma comment(lib,"d3d9/d3d9.lib")

#include "resource.h"

#include "vector.h"
#include "memManager.h"
#include "D3D9Render.h"
#include "settings.h"
#include "entity.h"
#include "hack.h"
#include "CallbackProxy.h"


//offset from WORLD
#define OFFSET_PLAYER					0x08			//playerbase

//entity offsets
#define OFFSET_ENTITY_POSBASE			0x30			//base of a structure that contains entity coords
#define OFFSET_ENTITY_POSBASE_POS		0x50			//vector3
#define OFFSET_ENTITY_POS				0x90			//vector3
#define OFFSET_ENTITY_GOD				0x189			//godmode; on = 1, off = 0; byte
#define OFFSET_ENTITY_HEALTH			0x280			//entity health (except for vehicles); float cur, float max
#define OFFSET_ENTITY_HEALTH_MAX		0x2A0			//they moved this away from curHealth in 1.36 :(
#define OFFSET_ENTITY_ATTACKER			0x2A8			//base to a list of the last 3 entities that attacked the current entity

//player (entity) offsets
#define OFFSET_PLAYER_VEHICLE					0xD28			//ptr to last used vehicle
#define OFFSET_PLAYER_INFO						0x10B8			//playerInfo struct
#define OFFSET_PLAYER_INFO_NAME					0x7C
#define OFFSET_PLAYER_INFO_SWIM_SPD				0x148			//swim speed; def 1; float
#define	OFFSET_PLAYER_INFO_RUN_SPD				0x14C			//run speed; def 1; float
#define OFFSET_PLAYER_INFO_FRAMEFLAGS			0x1F9			//frame flags; DWORD
#define OFFSET_PLAYER_INFO_WANTED_CAN_CHANGE	0x71C			//fWantedCanChange
#define OFFSET_PLAYER_INFO_WANTED				0x848			//wanted level; DWORD
#define OFFSET_PLAYER_INFO_STAMINA				0xCB0			//fStamina, fStaminaMax
#define OFFSET_PLAYER_RAGDOLL					0x10A8			//byte; CPed.noRagdoll: 0x20 = off; 0x00/0x01 = on
#define OFFSET_PLAYER_SEATBELT					0xC00			//byte; CPed.seatBelt: 0xC8 = off; 0xC9 = on
#define OFFSET_PLAYER_INVEHICLE					0x146B
#define OFFSET_PLAYER_ARMOR						0x14B8			//armour
#define OFFSET_PLAYER_WATER_PROOF				0x188			//water proof; DWORD; +0x1000000 = on
#define OFFSET_PLAYER_VEHICLE_DAMAGE_MP			0xCD8			//super punck/kick;float;

//vehicle offsets
#define OFFSET_VEHICLE_HEALTH						0x8E8			//vehicle health; 0.f-1000.f
#define	OFFSET_VEHICLE_HEALTH2						0x824			//vehicle health2; 0.f-1000.f
#define OFFSET_VEHICLE_HANDLING						0x918
#define OFFSET_VEHICLE_HANDLING_MASS				0xC				//fMass
#define OFFSET_VEHICLE_HANDLING_BUOYANCY			0x40			//fBuoyancy
#define OFFSET_VEHICLE_HANDLING_ACCELERATION		0x4C
#define OFFSET_VEHICLE_HANDLING_UPSHIFT				0x58
#define OFFSET_VEHICLE_HANDLING_DOWNSHIFT			0x5C
#define OFFSET_VEHICLE_HANDLING_BRAKEFORCE			0x6C
#define OFFSET_VEHICLE_HANDLING_HANDBRAKEFORCE		0x7C			//fHandbrakeForce
#define OFFSET_VEHICLE_HANDLING_TRACTION_CURVE_MIN	0x90			//fTractionCurveMin
#define OFFSET_VEHICLE_HANDLING_SUSPENSION_FORCE	0xBC			//fSuspensionForce
#define OFFSET_VEHICLE_HANDLING_SUSPENSION_HEIGH	0xD0			//fSuspensionHeight
#define OFFSET_VEHICLE_HANDLING_COLISION_DAMAGE_MP	0xF0			//fColisionDamageMult
#define OFFSET_VEHICLE_HANDLING_WEAPON_DAMAGE_MP	0xF4			//fWeaponDamageMult
#define OFFSET_VEHICLE_HANDLING_DEFORM_MULTIPLIER	0xF8			//fDeformationDamageMult
#define OFFSET_VEHICLE_HANDLING_ENGINE_DAMAGE_MP	0xFC			//fEngineDamageMult
#define OFFSET_VEHICLE_BULLETPROOF_TIRES			0x923			//btBulletproofTires;  (btBulletproofTires & 0x20) ? true : false
//#define OFFSET_VEHICLE_OPENABLE_DOORS				0xB30			//btOpenableDoors
#define OFFSET_VEHICLE_GRAVITY						0xC1C			//fGravity
#define OFFSET_VEHICLE_BOOST						0x320			//fBoost
#define OFFSET_VEHICLE_RECHARGE_SPEED				0x324			//fRocketRechargeSpeed


//weapon offsets
#define OFFSET_WEAPON_MANAGER			0x10C8			//from playerbase
#define OFFSET_WEAPON_CURRENT			0x20			//from weapon manager
#define OFFSET_WEAPON_AMMOINFO			0x60			//from weaponbase
#define OFFSET_WEAPON_AMMOINFO_MAX		0x28			//ammoinfo
#define OFFSET_WEAPON_AMMOINFO_CUR_1	0x08			//ptr lvl 1, ptr 1
#define OFFSET_WEAPON_AMMOINFO_CUR_2	0x00			//ptr tr lvl 2, ptr 1
#define OFFSET_WEAPON_AMMOINFO_CURAMMO	0x18			//offset to cur ammo
#define OFFSET_WEAPON_AMMOINFO_TYPE		0x0C			//offset to projectile type?
#define OFFSET_WEAPON_SPREAD			0x74			//float set to 0
#define OFFSET_WEAPON_BULLET_DMG		0xB0			//float times 10 (so when 0, it will stay 0)
#define OFFSET_WEAPON_RELOAD_MULTIPLIER	0x12C			//float times 10
#define OFFSET_WEAPON_RECOIL			0x2E8			//float set to 0
#define OFFSET_WEAPON_MODEL_HASH		0x14
#define OFFSET_WEAPON_NAME_HASH			0x10
#define OFFSET_WEAPON_RELOAD_VEHICLE	0x110
#define OFFSET_WEAPON_RANGE				0x28C
#define OFFSET_WEAPON_SPINUP			0x138
#define OFFSET_WEAPON_SPIN				0x13C
#define OFFSET_WEAPON_BULLET_BATCH		0x118			//dwBulletInBatch
#define OFFSET_WEAPON_MUZZLE_VELOCITY	0x114			//fMuzzleVelocity
#define OFFSET_WEAPON_BATCH_SPREAD		0x74			//fBatchSpread
#define OFFSET_WEAPON_IMPACT_TYPE		0x20			//dwImpactType; 1: Fists,3; Bullets,5: Explosives
#define OFFSET_WEAPON_IMPACT_EXPLOSION	0x24			//dwImpactExplosion
#define OFFSET_WEAPON_BULLET_DAMAGE		0xB0			//fBulletDamage
#define OFFSET_WEAPON_MUZZLE_FLASH_SIZE 0x1D4			//fMuzzleFlashSize
#define OFFSET_WEAPON_PENETRATION		0x108			//fPenetration
#define OFFSET_WEAPON_FORCE_ON_PED		0xD4			//fForceOnPed
#define OFFSET_WEAPON_FORCE_ON_VEHICLE	0xD8			//fForceOnVehicle(Bullet Mass)
#define OFFSET_WEAPON_FORCE_ON_HELI		0xDC			//fForceOnHeli
#define OFFSET_WEAPON_ROCKET_SPEED		0x58			//Rocket Speed (25000 instant);float
#define OFFSET_WEAPON_GRAVITY			0x68			//fThrowablesGravity


//tunable offsets
#define OFFSET_TUNABLE_RP_MULTIPLIER			0x10
#define OFFSET_TUNABLE_AP_MULTIPLIER			0x30F80
#define OFFSET_TUNABLE_MIN_MISSION_PAYOUT		0x4BC8			//fMinMissionPayout

#define OFFSET_ATTACKER_DISTANCE		0x18			//changed to 0x18, from 0x10


//feature indexing
#define FEATURE_P_GOD				0x00
#define FEATURE_P_NEVERWANTED		0x01
#define FEATURE_P_ANTINPC			0x02
#define FEATURE_V_GOD				0x03
#define FEATURE_W_SPREAD			0x04
#define FEATURE_W_RECOIL			0x05
#define FEATURE_W_RELOAD			0x06
#define FEATURE_W_DAMAGE			0x07
#define FEATURE_W_AMMO				0x08
#define FEATURE_W_RANGE				0x09
#define FEATURE_W_SPINUP			0x0A
#define FEATURE_P_RUNSPD			0x0B
#define FEATURE_P_SWIMSPD			0x0C
#define FEATURE_P_TRUEGOD			0x0D
#define FEATURE_P_SUPERJUMP			0x0E
#define FEATURE_P_EXPLOSIVEMELEE	0x0F
#define FEATURE_W_EXPLOSIVEAMMO		0x10
#define FEATURE_W_FIREAMMO			0x11
#define FEATURE_V_TRUEGOD			0x12
#define FEATURE_P_NORAGDOLL			0x13
#define FEATURE_V_SEATBELT			0x14
#define FEATURE_W_NORELOAD			0x15
#define FEATURE_V_ACCELERATION		0x16
#define FEATURE_V_BRAKEFORCE		0x17
#define FEATURE_V_TRACTION			0x18
#define FEATURE_V_GRAVITY			0x19
#define FEATURE_V_BULLETPROOFTIRES	0x1A
#define FEATURE_P_WANTED			0x1B
#define FEATURE_W_BULLET_BATCH		0x1C
#define FEATURE_W_MUZZLE_VELOCITY	0x1D
#define FEATURE_V_DEFORMATION		0x1E
#define FEATURE_V_UPSHIFT			0x1F
#define FEATURE_W_BATCH_SPREAD		0x20
#define FEATURE_V_SUSPENSION_FORCE	0x21
#define FEATURE_P_STAMINA			0x22
//#define FEATURE_V_DISABLE_DOORS		0x23
#define FEATURE_V_DOWNSHIFT			0x24
#define FEATURE_P_SUICIDE			0x25
#define FEATURE_W_FILL_AMMO			0x26
#define FEATURE_P_WATER_PROOF		0x27
#define FEATURE_P_HEAL				0x28
#define FEATURE_P_UNDEAD_OFFRADAR	0x29
#define FEATURE_P_SUPER_PUNCH		0x2A
#define FEATURE_V_MASS				0x2B
#define FEATURE_V_BUOYANCY			0x2C
#define FEATURE_V_HANDBRAKEFORCE	0x2D
#define FEATURE_V_SUSPENSION_HEIGH	0x2E
#define FEATURE_V_COLISION_DAMAGE_MP 0x2F
#define FEATURE_V_WEAPON_DAMAGE_MP	0x30
#define FEATURE_V_ENGINE_DAMAGE_MP	0x31
#define FEATURE_V_BOOST				0x32
#define FEATURE_V_RECHARGE_SPEED	0x33
#define FEATURE_V_HEAL				0x34
#define FEATURE_T_RP_MP				0x35
#define FEATURE_T_AP_MP				0x36
#define FEATURE_T_MISSION_PAYOUT	0x37

void	killProgram	();
DWORD	strToVk(std::string str);

extern HWND		g_hWnd;
extern int		g_iFeature[MAX_MENU_FEATURES];

extern bool		g_bKillSwitch;
extern bool		g_bKillRender;
extern bool		g_bKillAttach;
extern bool		g_bKillHack;
//Addresses from GTAV.exe module
extern long     ADDRESS_WORLD;				//48 8B 05 ? ? ? ? 45 ? ? ? ? 48 8B 48 08 48 85 C9 74 07
extern long		ADDRESS_WAYPOINT;			//4C 8D 05 ? ? ? ? 0F B7 C1
extern long		ADDRESS_AMMO;				//Ammo dec code; 41 2B D1 E8; 90 90 90 E8
extern long		ADDRESS_MAGAZINE;			//Magazine dec code; 41 2B C9 3B C8 0F; 90 90 90 3B C8 0F
extern long		ADDRESS_TUNABLE;
#endif
