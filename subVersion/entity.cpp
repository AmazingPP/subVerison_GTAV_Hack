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

/*
	ENTITY
*/
entity::entity(){}
entity::~entity()
{
	this->setGod(0);
}

void entity::getPos()
{
	g_pMemMan->readMem<v3>((DWORD_PTR) m_dwpBase + OFFSET_ENTITY_POS, &m_v3Pos);
	return;
}

void entity::setPos(v3 dest)
{
	g_pMemMan->writeMem<v3>((DWORD_PTR) m_dwpPosBase + OFFSET_ENTITY_POSBASE_POS, &dest);
	g_pMemMan->writeMem<v3>((DWORD_PTR) m_dwpBase + OFFSET_ENTITY_POS, &dest);
	return;
}

void entity::getGod()
{
	g_pMemMan->readMem<BYTE>((DWORD_PTR) m_dwpBase + OFFSET_ENTITY_GOD, &m_btGod);
	return;
}

void entity::setGod(BYTE value)
{
	g_pMemMan->writeMem<BYTE>((DWORD_PTR) m_dwpBase + OFFSET_ENTITY_GOD, &value);
	return;
}

void entity::getHealth()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_dwpBase + OFFSET_ENTITY_HEALTH, &m_cmHp.cur);
	g_pMemMan->readMem<float>((DWORD_PTR) m_dwpBase + OFFSET_ENTITY_HEALTH_MAX, &m_cmHp.max);
	return;
}
void entity::setHealth(float hp)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_dwpBase + OFFSET_ENTITY_HEALTH, &hp);
	return;
}

/*
	PLAYER
*/
player::player(){}
player::~player() 
{
	this->setRunSpeed(1);
	this->setSwimSpeed(1);
	this->setRagdoll(m_btRagdoll | 0x20);
	this->setSeatbelt((m_btSeatbelt & 0x01) ? m_btSeatbelt ^ 0x01 : m_btSeatbelt);
	restorePlayerData();
	//this->setWantedCanChange(1.f);
}

void player::getHealth()
{
	entity::getHealth();
	g_pMemMan->readMem<float>((DWORD_PTR) m_dwpBase + OFFSET_PLAYER_ARMOR, &m_flArmor);
	return;
}

void player::setHealth(float hp, float armor)
{
	entity::setHealth(hp);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_dwpBase + OFFSET_PLAYER_ARMOR, &armor);
	return;
}

bool player::loadPlayerData()
{
	if (m_dwpBase == 0)
		return 0;
	this->getWaterProof();

	m_playerDataRestore = m_playerDataCur;
	return 1;
}

void player::restorePlayerData()
{
	g_pMemMan->writeMem<DWORD>((DWORD_PTR)m_dwpBase + OFFSET_PLAYER_WATER_PROOF, &m_playerDataRestore.m_dwWaterProof);
}

void player::getWanted()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR) m_dwpPlayerInfo + OFFSET_PLAYER_INFO_WANTED, &m_dwWanted);
	return;
}

void player::setWanted(DWORD stars)
{
	g_pMemMan->writeMem<DWORD>((DWORD_PTR) m_dwpPlayerInfo + OFFSET_PLAYER_INFO_WANTED, &stars);
	return;
}

void player::getWantedCanChange()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_dwpPlayerInfo + OFFSET_PLAYER_INFO_WANTED_CAN_CHANGE, &m_flWantedCanChange);
	return;
}

void player::setWantedCanChange(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_dwpPlayerInfo + OFFSET_PLAYER_INFO_WANTED_CAN_CHANGE, &value);
	return;
}

void player::getInVehicle()
{
	BYTE	btRead;
	g_pMemMan->readMem<BYTE>((DWORD_PTR) m_dwpBase + OFFSET_PLAYER_INVEHICLE, &btRead);
	m_bIsInVehicle		= !((btRead >> 4) & 1);
	return;
}

void player::getRunSpeed()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_dwpPlayerInfo + OFFSET_PLAYER_INFO_RUN_SPD, &m_flRunSpd);
	return;
}

void player::setRunSpeed(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_dwpPlayerInfo + OFFSET_PLAYER_INFO_RUN_SPD, &value);
	return;
}

void player::getSwimSpeed()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_dwpPlayerInfo + OFFSET_PLAYER_INFO_SWIM_SPD, &m_flSwimSpd);
	return;
}

void player::setSwimSpeed(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_dwpPlayerInfo + OFFSET_PLAYER_INFO_SWIM_SPD, &value);
	return;
}

void player::getFrameFlags()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR) m_dwpPlayerInfo + OFFSET_PLAYER_INFO_FRAMEFLAGS, &m_dwFrameFlags);
	return;
}

void player::setFrameFlags(DWORD value)
{
	g_pMemMan->writeMem<DWORD>((DWORD_PTR) m_dwpPlayerInfo + OFFSET_PLAYER_INFO_FRAMEFLAGS, &value);
	return;
}

void player::getWaterProof()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR)m_dwpBase + OFFSET_PLAYER_WATER_PROOF, &m_playerDataCur.m_dwWaterProof);
	return;
}

void player::setWaterProof(DWORD value)
{
	g_pMemMan->writeMem<DWORD>((DWORD_PTR)m_dwpBase + OFFSET_PLAYER_WATER_PROOF, &value);
	return;
}

void player::getRagdoll()
{
	g_pMemMan->readMem<BYTE>((DWORD_PTR) m_dwpBase + OFFSET_PLAYER_RAGDOLL, &m_btRagdoll);
	return;
}

void player::setRagdoll(BYTE value)
{
	g_pMemMan->writeMem<BYTE>((DWORD_PTR) m_dwpBase + OFFSET_PLAYER_RAGDOLL, &value);
	return;
}

void player::getSeatbelt()
{
	g_pMemMan->readMem<BYTE>((DWORD_PTR) m_dwpBase + OFFSET_PLAYER_SEATBELT, &m_btSeatbelt);
	return;
}

void player::setSeatbelt(BYTE value)
{
	g_pMemMan->writeMem<BYTE>((DWORD_PTR) m_dwpBase + OFFSET_PLAYER_SEATBELT, &value);
	return;
}

void player::getStamina()
{
	g_pMemMan->readMem<curmax>((DWORD_PTR) m_dwpPlayerInfo + OFFSET_PLAYER_INFO_STAMINA, &m_cmStamina);
	return;
}

void player::setStamina(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_dwpPlayerInfo + OFFSET_PLAYER_INFO_STAMINA, &value);
	return;
}

/*
	VEHICLE
*/

vehicle::vehicle()
{
	m_cmHp.max = 1000.f;
	m_cmHpVehicle.max = 1000.f;
}
vehicle::~vehicle()
{
	this->restoreHandling();
	this->setGravity(9.8f);
	//if(m_btOpenableDoors[0] != m_btOpenableDoors[1])
	//	this->setOpenableDoors(m_btOpenableDoors[1]);
}

void vehicle::getHealth()
{
	entity::getHealth();
	g_pMemMan->readMem<float>((DWORD_PTR) m_dwpBase + OFFSET_VEHICLE_HEALTH, &m_cmHpVehicle.cur);
	return;
}

void vehicle::setHealth(float hp)
{
	entity::setHealth(hp);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_dwpBase + OFFSET_VEHICLE_HEALTH, &hp);
	return;
}

bool vehicle::loadHandling()
{
	if(m_dwpBase == 0)
		return 0;
	this->getAcceleration();
	this->getBrakeForce();
	this->getTractionCurveMin();
	this->getDeformationDamageMult();
	this->getUpShift();
	this->getDownShift();
	this->getSuspensionForce();
	if(m_handlingCur.m_dwpHandling != m_handlingRestore.m_dwpHandling)
	{
		if(m_handlingRestore.m_dwpHandling != 0)
			this->restoreHandling();
		m_handlingRestore	= m_handlingCur;
	}
	return 1;
}

void vehicle::restoreHandling()
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_ACCELERATION, &m_handlingRestore.m_fAcceleration);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_BRAKEFORCE, &m_handlingRestore.m_fBrakeForce);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_TRACTION_CURVE_MIN, &m_handlingRestore.m_fTractionCurveMin);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_DEFORM_MULTIPLIER, &m_handlingRestore.m_fDeformationDamageMult);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_UPSHIFT, &m_handlingRestore.m_fUpShift);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_DOWNSHIFT, &m_handlingRestore.m_fDownShift);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_SUSPENSION_FORCE, &m_handlingRestore.m_fSuspensionForce);
	return;
}

void vehicle::getAcceleration()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_ACCELERATION, &m_handlingCur.m_fAcceleration);
	return;
}

void vehicle::setAcceleration(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_ACCELERATION, &value);
	return;
}

void vehicle::getBrakeForce()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_BRAKEFORCE, &m_handlingCur.m_fBrakeForce);
	return;
}

void vehicle::setBrakeForce(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_BRAKEFORCE, &value);
	return;
}

void vehicle::getTractionCurveMin()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_TRACTION_CURVE_MIN, &m_handlingCur.m_fTractionCurveMin);
	return;
}

void vehicle::setTractionCurveMin(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_TRACTION_CURVE_MIN, &value);
	return;
}

void vehicle::getGravity()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_dwpBase + OFFSET_VEHICLE_GRAVITY, &m_fGravity);
	return;
}

void vehicle::setGravity(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_dwpBase + OFFSET_VEHICLE_GRAVITY, &value);
	return;
}

void vehicle::getBulletproofTires()
{
	g_pMemMan->readMem<BYTE>((DWORD_PTR) m_dwpBase + OFFSET_VEHICLE_BULLETPROOF_TIRES, &m_btBulletproofTires);
	return;
}

void vehicle::setBulletproofTires(BYTE value)
{
	g_pMemMan->writeMem<BYTE>((DWORD_PTR) m_dwpBase + OFFSET_VEHICLE_BULLETPROOF_TIRES, &value);
	return;
}

void vehicle::getDeformationDamageMult()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_DEFORM_MULTIPLIER, &m_handlingCur.m_fDeformationDamageMult);
	return;
}

void vehicle::setDeformationDamageMult(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_DEFORM_MULTIPLIER, &value);
	return;
}

void vehicle::getUpShift()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_UPSHIFT, &m_handlingCur.m_fUpShift);
	return;
}

void vehicle::setUpShift(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_UPSHIFT, &value);
	return;
}

void vehicle::getSuspensionForce()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_SUSPENSION_FORCE, &m_handlingCur.m_fSuspensionForce);
	return;
}

void vehicle::setSuspensionForce(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_SUSPENSION_FORCE, &value);
	return;
}

/*void vehicle::getOpenableDoors()
{
	g_pMemMan->readMem<BYTE>((DWORD_PTR) m_dwpBase + OFFSET_VEHICLE_OPENABLE_DOORS, m_btOpenableDoors);
	return;
}

void vehicle::setOpenableDoors(BYTE value)
{
	g_pMemMan->writeMem<BYTE>((DWORD_PTR) m_dwpBase + OFFSET_VEHICLE_OPENABLE_DOORS, &value);
	return;
}*/

void vehicle::getDownShift()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_DOWNSHIFT, &m_handlingCur.m_fDownShift);
	return;
}

void vehicle::setDownShift(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_DOWNSHIFT, &value);
	return;
}

/*
	WEAPON
*/

weapon::weapon() : m_dwHash(0) {};
weapon::~weapon()
{
	this->restoreWeapon();
}

bool weapon::findAmmoBase()
{
	if(m_weapDataCur.m_dwpWeapon == 0)
		return 0;

	DWORD_PTR	dwpBase	= m_dwpAmmoInfo,
				dwpBase2;
	DWORD		dwTmp;
	do
	{
		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) dwpBase	+ OFFSET_WEAPON_AMMOINFO_CUR_1, &dwpBase);
		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR) dwpBase	+ OFFSET_WEAPON_AMMOINFO_CUR_2, &dwpBase2);
		g_pMemMan->readMem<DWORD>	((DWORD_PTR) dwpBase2	+ OFFSET_WEAPON_AMMOINFO_TYPE, &dwTmp);
		if(dwpBase == 0 || dwpBase2 == 0)
			return 0;
	}
	while(dwTmp == 0);
	m_dwpCurAmmoBase	= dwpBase2;
	return 1;
}

void weapon::getCurAmmo()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR) m_dwpCurAmmoBase + OFFSET_WEAPON_AMMOINFO_CURAMMO, &m_dwCurAmmo);
	return;
}

void weapon::setCurAmmo(DWORD ammo)
{
	g_pMemMan->writeMem<DWORD>((DWORD_PTR) m_dwpCurAmmoBase + OFFSET_WEAPON_AMMOINFO_CURAMMO, &ammo);
	return;
}

void weapon::getMaxAmmo()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR) m_dwpAmmoInfo + OFFSET_WEAPON_AMMOINFO_MAX, &m_dwMaxAmmo);
	return;
}

bool weapon::loadWeapon()
{
	if(m_weapDataCur.m_dwpWeapon == 0)
		return 0;
	this->getHash();
	this->getBulletDamage();
	this->getReloadSpeed();
	this->getReloadVehicle();
	this->getRecoil();
	this->getSpread();
	this->getRange();
	this->getSpinUp();
	this->getSpin();
	this->getBulletBatch();
	this->getMuzzleVelocity();
	this->getBatchSpread();
	if(m_weapDataCur.m_dwHash != m_weapDataRestore.m_dwHash)
	{
		if(m_weapDataRestore.m_dwpWeapon != 0)
			this->restoreWeapon();
		m_weapDataRestore	= m_weapDataCur;
	}
	return 1;
}

void weapon::restoreWeapon()
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_SPREAD, &m_weapDataRestore.m_fSpread);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_RECOIL, &m_weapDataRestore.m_fRecoil);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_BULLET_DMG, &m_weapDataRestore.m_fDamage);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_RELOAD_MULTIPLIER, &m_weapDataRestore.m_fReload);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_RELOAD_VEHICLE, &m_weapDataRestore.m_fReloadVeh);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_RANGE, &m_weapDataRestore.m_fRange);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_SPINUP, &m_weapDataRestore.m_fSpinUp);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_SPIN, &m_weapDataRestore.m_fSpin);
	g_pMemMan->writeMem<DWORD>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_BULLET_BATCH, &m_weapDataRestore.m_dwBulletBatch);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_MUZZLE_VELOCITY, &m_weapDataRestore.m_fMuzzleVelocity);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_BATCH_SPREAD, &m_weapDataRestore.m_fBatchSpread);
	return;
}

void weapon::getReloadSpeed()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon	+ OFFSET_WEAPON_RELOAD_MULTIPLIER, &m_weapDataCur.m_fReload);
	return;
}

void weapon::setReloadSpeed(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_RELOAD_MULTIPLIER, &value);
	return;
}

void weapon::getReloadVehicle()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon	+ OFFSET_WEAPON_RELOAD_VEHICLE, &m_weapDataCur.m_fReloadVeh);
	return;
}

void weapon::setReloadVehicle(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_RELOAD_VEHICLE, &value);
	return;
}

void weapon::getBulletDamage()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon	+ OFFSET_WEAPON_BULLET_DMG, &m_weapDataCur.m_fDamage);
	return;
}

void weapon::setBulletDamage(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_BULLET_DMG, &value);
	return;
}

void weapon::getRecoil()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon	+ OFFSET_WEAPON_RECOIL, &m_weapDataCur.m_fRecoil);
	return;
}

void weapon::setRecoil(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_RECOIL, &value);
	return;
}

void weapon::getSpread()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon	+ OFFSET_WEAPON_SPREAD, &m_weapDataCur.m_fSpread);
	return;
}

void weapon::setSpread(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_SPREAD, &value);
	return;
}

void weapon::getRange()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon	+ OFFSET_WEAPON_RANGE, &m_weapDataCur.m_fRange);
	return;
}

void weapon::setRange(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_RANGE, &value);
	return;
}

void weapon::getSpinUp()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon	+ OFFSET_WEAPON_SPINUP, &m_weapDataCur.m_fSpinUp);
	return;
}

void weapon::setSpinUp(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_SPINUP, &value);
	return;
}

void weapon::getSpin()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon	+ OFFSET_WEAPON_SPIN, &m_weapDataCur.m_fSpin);
	return;
}

void weapon::setSpin(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_SPIN, &value);
	return;
}

void weapon::getHash()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR) m_weapDataCur.m_dwpWeapon	+ OFFSET_WEAPON_NAME_HASH, &m_weapDataCur.m_dwHash);
	return;
}

void weapon::getBulletBatch()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR) m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_BULLET_BATCH, &m_weapDataCur.m_dwBulletBatch);
	return;
}

void weapon::setBulletBatch(DWORD value)
{
	g_pMemMan->writeMem<DWORD>((DWORD_PTR) m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_BULLET_BATCH, &value);
	return;
}

void weapon::getMuzzleVelocity()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_MUZZLE_VELOCITY, &m_weapDataCur.m_fMuzzleVelocity);
	return;
}

void weapon::setMuzzleVelocity(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_MUZZLE_VELOCITY, &value);
	return;
}

void weapon::getBatchSpread()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_BATCH_SPREAD, &m_weapDataCur.m_fBatchSpread);
	return;
}

void weapon::setBatchSpread(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_BATCH_SPREAD, &value);
	return;
}