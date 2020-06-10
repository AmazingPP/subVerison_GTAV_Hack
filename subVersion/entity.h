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
#ifndef ENTITY_H
#define ENTITY_H

typedef struct
{
	float cur, max;
} curmax;

class entity
{
	public:
		curmax	m_cmHp;
		v3		m_v3Pos;
		BYTE	m_btGod;

						entity();
						~entity();
		virtual void	getPos();
		virtual void	setPos(v3 dest);
		virtual void	getHealth();
		virtual void	setHealth(float hp);
		virtual void	getGod();
		virtual void	setGod(BYTE value);

		DWORD_PTR	m_dwpBase,
					m_dwpPosBase;
};

class player : public entity
{
	public:
		float	m_flArmor,
				m_flRunSpd,
				m_flSwimSpd,
				m_flWantedCanChange,
				m_flVehicleDamageMult;
		DWORD	m_dwWanted,
				m_dwNpcIgnore,
				m_dwFrameFlags;
		BYTE	m_btRagdoll,
				m_btSeatbelt;
		bool	m_bIsInVehicle;
		curmax	m_cmStamina;

		struct playerData
		{
			bool	m_isInit;
			DWORD	m_dwWaterProof;
			float	m_maxHealth;
		};
		playerData	m_playerDataRestore,//the original values will be stored here.
					m_playerDataCur;

						player();
						~player();
		virtual	void	getHealth();
		virtual	void	setHealth(float hp, float armor);
				bool	loadPlayerData();
				void	restorePlayerData();
				void	getMaxHealth();
				void	setMaxHealth(float hp);
				void	getVehicleDamageMult();
				void	setVehicleDamageMult(float value);
				void	getWanted();
				void	setWanted(DWORD stars);
				void	getWantedCanChange();
				void	setWantedCanChange(float value);
				void	getInVehicle();
				void	getRunSpeed();
				void	setRunSpeed(float value);
				void	getSwimSpeed();
				void	setSwimSpeed(float value);
				void	getNpcIgnore();
				void	setNpcIgnore(DWORD value);
				void	getFrameFlags();
				void	setFrameFlags(DWORD value);
				void	getWaterProof();
				void	setWaterProof(DWORD value);
				void	getRagdoll();
				void	setRagdoll(BYTE value);
				void	getSeatbelt();
				void	setSeatbelt(BYTE value);
				void	getStamina();
				void	setStamina(float value);

		DWORD_PTR	m_dwpPlayerInfo;
};

class vehicle : public entity
{
	public:
		curmax	m_cmHpVehicle;
		float	m_fBoost,
				m_fRocketRechargeSpeed,
				m_fGravity;
		BYTE	m_btBulletproofTires;

		struct vehicleHandling
		{
			DWORD_PTR	m_dwpHandling = 0;
			float		m_fMass,
						m_fBuoyancy,
						m_fAcceleration,
						m_fUpShift,
						m_fDownShift,
						m_fBrakeForce,
						m_fHandbrakeForce,
						m_fTractionCurveMin,
						m_fSuspensionForce,
						m_fSuspensionHeigh,
						m_fColisionDamageMult,
						m_fWeaponDamageMult,
						m_fDeformationDamageMult,
						m_fEngineDamageMult;
		};

		vehicleHandling	m_handlingRestore,
						m_handlingCur;

				vehicle();
				~vehicle();
		void	getHealth();
		void	setHealth(float hp);
		bool	loadHandling();
		void	restoreHandling();
		void	getMass();
		void	setMass(float value);
		void	getBuoyancy();
		void	setBuoyancy(float value);
		void	getAcceleration();
		void	setAcceleration(float value);
		void	getBrakeForce();
		void	setBrakeForce(float value);
		void	getHandbrakeForce();
		void	setHandbrakeForce(float value);
		void	getTractionCurveMin();
		void	setTractionCurveMin(float value);
		void	getGravity();
		void	setGravity(float value);
		void	getBoost();
		void	setBoost(float value);
		void	getRocketRechargeSpeed();
		void	setRocketRechargeSpeed(float value);
		void	getBulletproofTires();
		void	setBulletproofTires(BYTE value);
		void	getDeformationDamageMult();
		void	setDeformationDamageMult(float value);
		void	getUpShift();
		void	setUpShift(float value);
		void	getSuspensionForce();
		void	setSuspensionForce(float value);
		void	getSuspensionHeigh();
		void	setSuspensionHeigh(float value);
		void	getColisionDamageMult();
		void	setColisionDamageMult(float value);
		void	getWeaponDamageMult();
		void	setWeaponDamageMult(float value);
		void	getEngineDamageMult();
		void	setEngineDamageMult(float value);
		void	getDownShift();
		void	setDownShift(float value);
};

enum ImpactTypeEnum
{
	Fists = 2,
	Bullets = 3,
	Explosives = 5,
};

enum ImpactExplosionEnum
{
	DefaultBullets = 4294967295,
	GrenadeExplosion = 1,
	StickyBombExplosion = 2,
	MoltovCoctailExplosion = 3,
	SuperLauncher = 59,
	LightningExplosion = 58,
	BigExplosion = 4,
	MediumExplosion = 40,
	TinyExplosion = 25,
	BigFireyExplosion = 5,
	SmallWaterSpray = 11,
	SmallFireSpray = 12,
	BigWaterSpray = 13,
	BigFireSpray = 14,
	MK2ExplosiveBullets = 18,
	SmokeGrenade = 19,
	TearGas = 20,
	TearGas2 = 21,
	RedFlareSmoke = 22,
	CoolGroundExplosion = 23,
	CRAZYSHOCKWAVEEXPLOSION = 26,
	HUGEFireyExplosion = 28,
	MassiveBlimpExplosion = 29,
	MassiveBlimpExplosion2 = 37,
	LargeExplosionFallingDebris = 31,
	FireBallExplosion = 32,
	FireworkExplosion = 38,
	SnowballHit = 39,
	TinyExplosions2 = 33,
	JustScreenShake = 41,
	SPOOFEXPLOSION = 99
};

class weapon
{
	public:
		DWORD		m_dwCurAmmo,
					m_dwMaxAmmo,
					m_dwHash;		//name hash
		struct weaponData
		{
			DWORD		m_dwHash = 0,
						m_dwBulletBatch,
						m_dwImpactType,
						m_dwImpactExplosion;
			DWORD_PTR	m_dwpWeapon	= 0;
			float		m_fSpread,		
						m_fRecoil,
						m_fDamage,
						m_fReload,
						m_fReloadVeh,
						m_fRange,
						m_fSpinUp,
						m_fSpin,
						m_fMuzzleVelocity,
						m_fPenetration,
						m_fForceOnPed,
						m_fForceOnVehicle,
						m_fForceOnHeli,
						m_fBatchSpread;
		};

		weaponData	m_weapDataRestore,//the original values will be stored here.
					m_weapDataCur;
				
				weapon();
				~weapon();
		bool	loadWeapon();
		void	restoreWeapon();
		void	getReloadSpeed();
		void	setReloadSpeed(float value);
		void	getReloadVehicle();
		void	setReloadVehicle(float value);
		void	getBulletDamage();
		void	setBulletDamage(float value);
		void	getRecoil();
		void	setRecoil(float value);
		void	getSpread();
		void	setSpread(float value);
		void	getRange();
		void	setRange(float value);
		void	getSpinUp();
		void	setSpinUp(float value);
		void	getSpin();
		void	setSpin(float value);
		void	getForceOnPed();
		void	setForceOnPed(float value);
		void	getForceOnVehicle();
		void	setForceOnVehicle(float value);
		void	getForceOnHeli();
		void	setForceOnHeli(float value);
		void	getHash();
		bool	findAmmoBase();
		void	getCurAmmo();
		void	setCurAmmo(DWORD ammo);
		void	getMaxAmmo();
		void	getBulletBatch();
		void	setBulletBatch(DWORD value);
		void	getImpactType();
		void	setImpactType(DWORD  value);
		void	getImpactExplosion();
		void	setImpactExplosion(DWORD  value);
		void	getMuzzleVelocity();
		void	setMuzzleVelocity(float value);
		void	getBatchSpread();
		void	setBatchSpread(float value);

		DWORD_PTR	m_dwpAmmoInfo,
					m_dwpCurAmmoBase;
};

class tunable
{
	public:
		float		m_fRpMult,
					m_fApMult,
					m_fMinMissionPayout;
				tunable();
				~tunable();
		void	restoreTunable();
		void	getRpMult();
		void	setRpMult(float value);
		void	getApMult();
		void	setApMult(float value);
		void	getMinMissionPayout();
		void	setMinMissionPayout(float value);

		DWORD_PTR	m_dwpTunableBase;
};
#endif