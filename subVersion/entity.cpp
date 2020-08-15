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
#include "entity.h"

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

void entity::getCos()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_dwpPosBase + OFFSET_ENTITY_POSBASE_COS, &m_fCos);
	return;
}

void entity::setCos(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_dwpPosBase + OFFSET_ENTITY_POSBASE_COS, &value);
	return;
}

void entity::getSin()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_dwpPosBase + OFFSET_ENTITY_POSBASE_SIN, &m_fSin);
	return;
}

void entity::setSin(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_dwpPosBase + OFFSET_ENTITY_POSBASE_SIN, &value);
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
	m_playerDataCur.m_isInit = true;
	this->getMaxHealth();
	this->getWaterProof();
	if (m_playerDataCur.m_isInit != m_playerDataRestore.m_isInit)
	{
		m_playerDataRestore = m_playerDataCur;
	}
	return 1;
}

void player::restorePlayerData()
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_dwpBase + OFFSET_ENTITY_HEALTH_MAX, &m_playerDataRestore.m_maxHealth);
	g_pMemMan->writeMem<DWORD>((DWORD_PTR)m_dwpBase + OFFSET_PLAYER_WATER_PROOF, &m_playerDataRestore.m_dwWaterProof);
}

void player::getMaxHealth()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_dwpBase + OFFSET_ENTITY_HEALTH_MAX, &m_playerDataCur.m_maxHealth);
	return;
}

void player::setMaxHealth(float hp)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_dwpBase + OFFSET_ENTITY_HEALTH_MAX, &hp);
	return;
}

void player::getVehicleDamageMult()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_dwpPlayerInfo + OFFSET_PLAYER_VEHICLE_DAMAGE_MP, &m_flVehicleDamageMult);
	return;
}

void player::setVehicleDamageMult(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_dwpPlayerInfo + OFFSET_PLAYER_VEHICLE_DAMAGE_MP, &value);
	return;
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

void player::getNpcIgnore()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR)m_dwpPlayerInfo + OFFSET_PLAYER_INFO_NPC_IGNORE, &m_dwNpcIgnore);
	return;
}

void player::setNpcIgnore(DWORD value)
{
	g_pMemMan->writeMem<DWORD>((DWORD_PTR)m_dwpPlayerInfo + OFFSET_PLAYER_INFO_NPC_IGNORE, &value);
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
	this->setBoost(1.f);
	this->setRocketRechargeSpeed(0.5f);
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
	g_pMemMan->writeMem<float>((DWORD_PTR)m_dwpBase + OFFSET_VEHICLE_HEALTH2, &hp);
	return;
}

bool vehicle::loadHandling()
{
	if(m_dwpBase == 0)
		return 0;
	this->getMass();
	this->getBuoyancy();
	this->getAcceleration();
	this->getBrakeForce();
	this->getHandbrakeForce();
	this->getTractionCurveMin();
	this->getDeformationDamageMult();
	this->getColisionDamageMult();
	this->getWeaponDamageMult();
	this->getEngineDamageMult();
	this->getUpShift();
	this->getDownShift();
	this->getSuspensionForce();
	this->getSuspensionHeigh();
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
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_MASS, &m_handlingRestore.m_fMass);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_BUOYANCY, &m_handlingRestore.m_fBuoyancy);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_ACCELERATION, &m_handlingRestore.m_fAcceleration);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_BRAKEFORCE, &m_handlingRestore.m_fBrakeForce);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_HANDBRAKEFORCE, &m_handlingRestore.m_fHandbrakeForce);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_TRACTION_CURVE_MIN, &m_handlingRestore.m_fTractionCurveMin);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_DEFORM_MULTIPLIER, &m_handlingRestore.m_fDeformationDamageMult);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_COLISION_DAMAGE_MP, &m_handlingRestore.m_fColisionDamageMult);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_WEAPON_DAMAGE_MP, &m_handlingRestore.m_fWeaponDamageMult);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_ENGINE_DAMAGE_MP, &m_handlingRestore.m_fEngineDamageMult);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_UPSHIFT, &m_handlingRestore.m_fUpShift);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_DOWNSHIFT, &m_handlingRestore.m_fDownShift);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_SUSPENSION_FORCE, &m_handlingRestore.m_fSuspensionForce);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_handlingRestore.m_dwpHandling + OFFSET_VEHICLE_HANDLING_SUSPENSION_HEIGH, &m_handlingRestore.m_fSuspensionHeigh);
	return;
}

void vehicle::getMass()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_MASS, &m_handlingCur.m_fMass);
	return;
}

void vehicle::setMass(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_MASS, &value);
	return;
}

void vehicle::getBuoyancy()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_BUOYANCY, &m_handlingCur.m_fBuoyancy);
	return;
}

void vehicle::setBuoyancy(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_BUOYANCY, &value);
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

void vehicle::getHandbrakeForce()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_HANDBRAKEFORCE, &m_handlingCur.m_fHandbrakeForce);
	return;
}

void vehicle::setHandbrakeForce(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_HANDBRAKEFORCE, &value);
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

void vehicle::getBoost()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_dwpBase + OFFSET_VEHICLE_BOOST, &m_fBoost);
	return;
}

void vehicle::setBoost(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_dwpBase + OFFSET_VEHICLE_BOOST, &value);
	return;
}

void vehicle::getRocketRechargeSpeed()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_dwpBase + OFFSET_VEHICLE_RECHARGE_SPEED, &m_fRocketRechargeSpeed);
	return;
}

void vehicle::setRocketRechargeSpeed(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_dwpBase + OFFSET_VEHICLE_RECHARGE_SPEED, &value);
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

void vehicle::getEngineManagementSystem()
{
}

void vehicle::setEngineManagementSystem(BYTE value)
{
}

void vehicle::getBrakes()
{
}

void vehicle::setBrakes(BYTE value)
{
}

void vehicle::getTransmission()
{
}

void vehicle::setTransmission(BYTE value)
{
}

void vehicle::getSuspension()
{
}

void vehicle::setSuspension(BYTE value)
{
}

void vehicle::getArmor()
{
}

void vehicle::setArmor(BYTE value)
{
}

void vehicle::getTurboTuning()
{
}

void vehicle::setTurboTuning(BYTE value)
{
}

void vehicle::getNeonLightRed()
{
}

void vehicle::setNeonLightRed(BYTE value)
{
}

void vehicle::getNeonLightGreen()
{
}

void vehicle::setNeonLightGreen(BYTE value)
{
}

void vehicle::getNeonLightBlue()
{
}

void vehicle::setNeonLightBlue(BYTE value)
{
}

void vehicle::getNeonLightLeft()
{
}

void vehicle::setNeonLightLeft(BYTE value)
{
}

void vehicle::getNeonLightRight()
{
}

void vehicle::setNeonLightRight(BYTE value)
{
}

void vehicle::getNeonLightFront()
{
}

void vehicle::setNeonLightFront(BYTE value)
{
}

void vehicle::getNeonLightBack()
{
}

void vehicle::setNeonLightBack(BYTE value)
{
}

void vehicle::getTyreSmoke()
{
}

void vehicle::setTyreSmoke(BYTE value)
{
}

void vehicle::getTyreSmokeRed()
{
}

void vehicle::setTyreSmokeRed(BYTE value)
{
}

void vehicle::getTyreSmokeGreen()
{
}

void vehicle::setTyreSmokeGreen(BYTE value)
{
}

void vehicle::getTyreSmokeBlue()
{
}

void vehicle::setTyreSmokeBlue(BYTE value)
{
}

void vehicle::getLimoWindows()
{
}

void vehicle::setLimoWindows(BYTE value)
{
}

void vehicle::getVehicleMissles()
{
}

void vehicle::setVehicleMissles(BYTE value)
{
}

void vehicle::getAircraftBombs()
{
}

void vehicle::setAircraftBombs(BYTE value)
{
}

void vehicle::getAircraftCountermeasures()
{
}

void vehicle::setAircraftCountermeasures(BYTE value)
{
}

void vehicle::getOppressorMK2Misseles()
{
}

void vehicle::setOppressorMK2Misseles(BYTE value)
{
}

void vehicle::getTampaMissles()
{
}

void vehicle::setTampaMissles(BYTE value)
{
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

void vehicle::getSuspensionHeigh()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_SUSPENSION_HEIGH, &m_handlingCur.m_fSuspensionHeigh);
	return;
}

void vehicle::setSuspensionHeigh(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_SUSPENSION_HEIGH, &value);
	return;
}

void vehicle::getColisionDamageMult()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_COLISION_DAMAGE_MP, &m_handlingCur.m_fColisionDamageMult);
	return;
}

void vehicle::setColisionDamageMult(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_COLISION_DAMAGE_MP, &value);
	return;
}

void vehicle::getWeaponDamageMult()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_WEAPON_DAMAGE_MP, &m_handlingCur.m_fWeaponDamageMult);
	return;
}

void vehicle::setWeaponDamageMult(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_WEAPON_DAMAGE_MP, &value);
	return;
}

void vehicle::getEngineDamageMult()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_ENGINE_DAMAGE_MP, &m_handlingCur.m_fEngineDamageMult);
	return;
}

void vehicle::setEngineDamageMult(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_handlingCur.m_dwpHandling + OFFSET_VEHICLE_HANDLING_ENGINE_DAMAGE_MP, &value);
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
	this->getForceOnPed();
	this->getForceOnVehicle();
	this->getForceOnHeli();
	this->getRecoil();
	this->getSpread();
	this->getRange();
	this->getSpinUp();
	this->getSpin();
	this->getBulletBatch();
	this->getImpactType();
	this->getImpactExplosion();
	this->getMuzzleVelocity();
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
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_FORCE_ON_PED, &m_weapDataRestore.m_fForceOnPed);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_FORCE_ON_VEHICLE, &m_weapDataRestore.m_fForceOnVehicle);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_FORCE_ON_HELI, &m_weapDataRestore.m_fForceOnHeli);
	g_pMemMan->writeMem<DWORD>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_BULLET_BATCH, &m_weapDataRestore.m_dwBulletBatch);
	g_pMemMan->writeMem<DWORD>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_IMPACT_TYPE, &m_weapDataRestore.m_dwImpactType);
	g_pMemMan->writeMem<DWORD>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_IMPACT_EXPLOSION, &m_weapDataRestore.m_dwImpactExplosion);
	g_pMemMan->writeMem<float>((DWORD_PTR) m_weapDataRestore.m_dwpWeapon + OFFSET_WEAPON_MUZZLE_VELOCITY, &m_weapDataRestore.m_fMuzzleVelocity);
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

void weapon::getForceOnPed()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_FORCE_ON_PED, &m_weapDataCur.m_fForceOnPed);
	return;
}

void weapon::setForceOnPed(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_FORCE_ON_PED, &value);
	return;
}

void weapon::getForceOnVehicle()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_FORCE_ON_VEHICLE, &m_weapDataCur.m_fForceOnVehicle);
	return;
}

void weapon::setForceOnVehicle(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_FORCE_ON_VEHICLE, &value);
	return;
}

void weapon::getForceOnHeli()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_FORCE_ON_HELI, &m_weapDataCur.m_fForceOnHeli);
	return;
}

void weapon::setForceOnHeli(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_FORCE_ON_HELI, &value);
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

void weapon::getImpactType()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR)m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_IMPACT_TYPE, &m_weapDataCur.m_dwImpactType);
	return;
}

void weapon::setImpactType(DWORD value)
{
	g_pMemMan->writeMem<DWORD>((DWORD_PTR)m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_IMPACT_TYPE, &value);
	return;
}

void weapon::getImpactExplosion()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR)m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_IMPACT_EXPLOSION, &m_weapDataCur.m_dwImpactExplosion);
	return;
}

void weapon::setImpactExplosion(DWORD value)
{
	g_pMemMan->writeMem<DWORD>((DWORD_PTR)m_weapDataCur.m_dwpWeapon + OFFSET_WEAPON_IMPACT_EXPLOSION, &value);
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

/*
	TUNABLE
*/

tunable::tunable()
{
}

tunable::~tunable() 
{
	this->restoreTunable();
}

void tunable::restoreTunable()
{
	setRpMult(1);
	setApMult(1);
	setMinMissionPayout(0);
}

void tunable::getRpMult()
{
	g_pMemMan->readMem<float>((DWORD_PTR) m_dwpTunableBase + OFFSET_TUNABLE_RP_MULTIPLIER, &m_fRpMult);
	return;
}

void tunable::setRpMult(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_RP_MULTIPLIER, &value);
	return;
}

void tunable::getApMult()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_AP_MULTIPLIER, &m_fApMult);
	return;
}

void tunable::setApMult(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_AP_MULTIPLIER, &value);
	return;
}

void tunable::getMinMissionPayout()
{
	g_pMemMan->readMem<float>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_MIN_MISSION_PAYOUT, &m_fMinMissionPayout);
	return;
}

void tunable::setMinMissionPayout(float value)
{
	g_pMemMan->writeMem<float>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_MIN_MISSION_PAYOUT, &value);
	return;
}

void tunable::getOrbitalCannonCooldown()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_ORBITAL_CANNON_COOLDOWN, &m_dwOrbitalCannonCooldown);
	return;
}

void tunable::setOrbitalCannonCooldown(DWORD value)
{
	g_pMemMan->writeMem<DWORD>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_ORBITAL_CANNON_COOLDOWN, &value);
	return;
}

void tunable::getBunkerResearch()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_BUNKER_RESEARCH, &m_dwBunkerResearch);
	return;
}

void tunable::setBunkerResearch(DWORD value)
{
	g_pMemMan->writeMem<DWORD>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_BUNKER_RESEARCH, &value);
	return;
}

void tunable::getAntiIdleKick1()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_ANTI_IDLE_KICK1, &m_dwAntiIdleKick1);
	return;
}

void tunable::setAntiIdleKick1(DWORD value)
{
	g_pMemMan->writeMem<DWORD>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_ANTI_IDLE_KICK1, &value);
	return;
}

void tunable::getAntiIdleKick2()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_ANTI_IDLE_KICK2, &m_dwAntiIdleKick2);
	return;
}

void tunable::setAntiIdleKick2(DWORD value)
{
	g_pMemMan->writeMem<DWORD>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_ANTI_IDLE_KICK2, &value);
	return;
}

void tunable::getAntiIdleKick3()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_ANTI_IDLE_KICK3, &m_dwAntiIdleKick3);
	return;
}

void tunable::setAntiIdleKick3(DWORD value)
{
	g_pMemMan->writeMem<DWORD>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_ANTI_IDLE_KICK3, &value);
	return;
}

void tunable::getAntiIdleKick4()
{
	g_pMemMan->readMem<DWORD>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_ANTI_IDLE_KICK4, &m_dwAntiIdleKick4);
	return;
}

void tunable::setAntiIdleKick4(DWORD value)
{
	g_pMemMan->writeMem<DWORD>((DWORD_PTR)m_dwpTunableBase + OFFSET_TUNABLE_ANTI_IDLE_KICK4, &value);
	return;
}



/*
	GLOBAL
*/

global::global()
{
}

global::~global()
{
}

bool global::initStatPtr(HMODULE base)
{
	m_dwpStatCall = getGlobal(1377170, base) + 0x2398;
	m_dwpStatHash = getGlobal(1387876, base) + 0x20;
	m_dwpStatValue = getGlobal(939452, base) + 0xACB0;


	return m_dwpStatCall != 0 || m_dwpStatHash != 0 || m_dwpStatValue != 0;
}

bool global::initMoneyPtr(HMODULE base)
{
	m_dwpMoneyObject = getGlobal(2513253, base);
	m_dwpMoneyVal = getGlobal(2513247 + 1, base);
	m_dwpMoneyPosX = getGlobal(2513247 + 3, base);
	m_dwpMoneyPosY = getGlobal(2513247 + 4, base);
	m_dwpMoneyPosZ = getGlobal(2513247 + 5, base);

	return  m_dwpMoneyObject != 0 || m_dwpMoneyVal != 0 || m_dwpMoneyPosX != 0
		|| m_dwpMoneyPosY != 0 || m_dwpMoneyPosZ != 0;
}

bool global::initSessionPtr(HMODULE base)
{
	m_dwpSessionID = getGlobal(1312836, base);
	m_dwpSessionTransition = getGlobal(1312425, base);

	return  m_dwpSessionID != 0 || m_dwpSessionTransition != 0;
}

bool global::initIntoPVPtr(HMODULE base)
{
	m_dwpIntoPersonalVehicle = getGlobal(2409287 + 8, base);

	return  m_dwpIntoPersonalVehicle != 0 ;
}

bool global::initVehiclePtr(HMODULE base)
{
	m_dwpVehicleSpawn1 = getGlobal(OFFSET_GLOBAL_VEHICLE_HASH + 2, base);
	m_dwpVehicleSpawn2 = getGlobal(OFFSET_GLOBAL_VEHICLE_HASH + 5, base);
	m_dwpVehicleHash = getGlobal(OFFSET_GLOBAL_VEHICLE_HASH + 27 + 66, base);
	m_dwpVehicleKickPrevent1 = getGlobal(OFFSET_GLOBAL_VEHICLE_HASH + 121, base);
	m_dwpVehicleKickPrevent2 = getGlobal(OFFSET_GLOBAL_VEHICLE_HASH + 122, base);
	m_dwpVehicleX = getGlobal(OFFSET_GLOBAL_VEHICLE_HASH + 7 + 0, base);
	m_dwpVehicleY = getGlobal(OFFSET_GLOBAL_VEHICLE_HASH + 7 + 1, base);
	m_dwpVehicleZ = getGlobal(OFFSET_GLOBAL_VEHICLE_HASH + 7 + 2, base);
	m_dwpPrimaryColor = getGlobal(OFFSET_GLOBAL_VEHICLE_HASH + 27 + 5, base);
	m_dwpSecondaryColor = getGlobal(OFFSET_GLOBAL_VEHICLE_HASH + 27 + 6, base);

	return m_dwpVehicleSpawn1 != 0 ||m_dwpVehicleSpawn2 != 0 ||m_dwpVehicleHash != 0 ||
		m_dwpVehicleKickPrevent1 != 0 ||m_dwpVehicleKickPrevent2 != 0 ||
		m_dwpVehicleX != 0 ||m_dwpVehicleY != 0 ||
		m_dwpVehicleZ != 0 ||m_dwpPrimaryColor != 0 ||m_dwpSecondaryColor;
}


bool global::findMoneyPtr(HMODULE base)
{
	float buffer;
	DWORD_PTR dwpTemp = getGlobal(2513247, base);
	g_pMemMan->readMem<float>(dwpTemp, &buffer);
	m_dwpMoneyCall = getGlobal(85 * (DWORD)buffer + 4263954 + 69, base);
	return true;
}

DWORD_PTR global::getGlobal(int atIndex, HMODULE base)
{
	DWORD_PTR dwpBuffer;
	g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)base + ADDRESS_GLOBAL + 8 * ((atIndex >> 0x12) & 0x3F), &dwpBuffer);
	return 8 * (atIndex & 0x3FFFF) + dwpBuffer;
}

void global::getStatHash()
{
	g_pMemMan->readMem<unsigned int>(m_dwpStatHash, &m_dwStatHash);
	return;
}

void global::setStatHash(unsigned int value)
{
	g_pMemMan->writeMem<unsigned int>(m_dwpStatHash, &value);
	return;
}

void global::getStatValue()
{
	g_pMemMan->readMem<int>(m_dwpStatValue, &m_dwStatValue);
	return;
}

void global::setStatValue(int value)
{
	g_pMemMan->writeMem<int>(m_dwpStatValue, &value);
	return;
}

void global::getStatCall()
{
	g_pMemMan->readMem<int>(m_dwpStatCall, &m_dwStatCall);
	return;
}

void global::setStatCall(int value)
{
	g_pMemMan->writeMem<int>(m_dwpStatCall, &value);
	return;
}

void global::getMoneyObject()
{
	g_pMemMan->readMem<BYTE>(m_dwpMoneyObject, &m_btMoneyObject);
	return;
}

void global::setMoneyObject(BYTE value)
{
	g_pMemMan->writeMem<BYTE>(m_dwpMoneyObject, &value);
	return;
}

void global::getMoneyVal()
{
	g_pMemMan->readMem<int>(m_dwpMoneyVal, &m_dwMoneyVal);
	return;
}

void global::setMoneyVal(int value)
{
	g_pMemMan->writeMem<int>(m_dwpMoneyVal, &value);
	return;
}

void global::getMoneyPosX()
{
	g_pMemMan->readMem<float>(m_dwpMoneyPosX, &m_fMoneyPosX);
	return;
}

void global::setMoneyPosX(float value)
{
	g_pMemMan->writeMem<float>(m_dwpMoneyPosX, &value);
	return;
}

void global::getMoneyPosY()
{
	g_pMemMan->readMem<float>(m_dwpMoneyPosY, &m_fMoneyPosY);
	return;
}

void global::setMoneyPosY(float value)
{
	g_pMemMan->writeMem<float>(m_dwpMoneyPosY, &value);
	return;
}

void global::getMoneyPosZ()
{
	g_pMemMan->readMem<float>(m_dwpMoneyPosZ, &m_fMoneyPosZ);
	return;
}

void global::setMoneyPosZ(float value)
{
	g_pMemMan->writeMem<float>(m_dwpMoneyPosZ, &value);
	return;
}

void global::getMoneyCall()
{
	g_pMemMan->readMem<BYTE>(m_dwpMoneyCall, &m_btMoneyCall);
	return;
}

void global::setMoneyCall(BYTE value)
{
	g_pMemMan->writeMem<BYTE>(m_dwpMoneyCall, &value);
	return;
}

void global::getSessionTransition()
{
	g_pMemMan->readMem<int>(m_dwpSessionTransition, &m_dwSessionTransition);
	return;
}

void global::setSessionTransition(int value)
{
	g_pMemMan->writeMem<int>(m_dwpSessionTransition, &value);
	return;
}

void global::getSessionID()
{
	g_pMemMan->readMem<int>(m_dwpSessionID, &m_dwSessionID);
	return;
}

void global::setSessionID(int value)
{
	g_pMemMan->writeMem<int>(m_dwpSessionID, &value);
	return;
}

void global::getIntoPersonalVehicle()
{
	g_pMemMan->readMem<int>(m_dwpIntoPersonalVehicle, &m_dwIntoPersonalVehicle);
	return;
}

void global::setIntoPersonalVehicle(int value)
{
	g_pMemMan->writeMem<int>(m_dwpIntoPersonalVehicle, &value);
	return;
}

void global::getVehicleSpawn1()
{
	g_pMemMan->readMem<int>(m_dwpVehicleSpawn1, &m_dwVehicleSpawn1);
	return;
}

void global::setVehicleSpawn1(int value)
{
	g_pMemMan->writeMem<int>(m_dwpVehicleSpawn1, &value);
	return;
}

void global::getVehicleSpawn2()
{
	g_pMemMan->readMem<int>(m_dwpVehicleSpawn2, &m_dwVehicleSpawn2);
	return;
}

void global::setVehicleSpawn2(int value)
{
	g_pMemMan->writeMem<int>(m_dwpVehicleSpawn2, &value);
	return;
}

void global::getVehicleHash()
{
	g_pMemMan->readMem<unsigned int>(m_dwpVehicleHash, &m_dwVehicleHash);
	return;
}

void global::setVehicleHash(unsigned int value)
{
	g_pMemMan->writeMem<unsigned int>(m_dwpVehicleHash, &value);
	return;

}

void global::getVehicleKickPrevent1()
{
	g_pMemMan->readMem<int>(m_dwpVehicleKickPrevent1, &m_dwVehicleKickPrevent1);
	return;
}

void global::setVehicleKickPrevent1(int value)
{
	g_pMemMan->writeMem<int>(m_dwpVehicleKickPrevent1, &value);
	return;
}

void global::getVehicleKickPrevent2()
{
	g_pMemMan->readMem<int>(m_dwpVehicleKickPrevent2, &m_dwVehicleKickPrevent2);
	return;
}

void global::setVehicleKickPrevent2(int value)
{
	g_pMemMan->writeMem<int>(m_dwpVehicleKickPrevent2, &value);
	return;
}

void global::getVehiclePosX()
{
	g_pMemMan->readMem<float>(m_dwpVehicleX, &m_fVehicleX);
	return;
}

void global::setVehiclePosX(float value)
{
	g_pMemMan->writeMem<float>(m_dwpVehicleX, &value);
	return;
}

void global::getVehiclePosY()
{
	g_pMemMan->readMem<float>(m_dwpVehicleY, &m_fVehicleY);
	return;
}

void global::setVehiclePosY(float value)
{
	g_pMemMan->writeMem<float>(m_dwpVehicleY, &value);
	return;
}

void global::getVehiclePosZ()
{
	g_pMemMan->readMem<float>(m_dwpVehicleZ, &m_fVehicleZ);
	return;
}

void global::setVehiclePosZ(float value)
{
	g_pMemMan->writeMem<float>(m_dwpVehicleZ, &value);
	return;
}

void global::getPrimaryColor()
{
	g_pMemMan->readMem<BYTE>(m_dwpPrimaryColor, &m_btPrimaryColor);
	return;
}

void global::setPrimaryColor(BYTE value)
{
	g_pMemMan->writeMem<BYTE>(m_dwpPrimaryColor, &value);
	return;
}

void global::getSecondaryColor()
{
	g_pMemMan->readMem<BYTE>(m_dwpSecondaryColor, &m_btSecondaryColor);
	return;
}

void global::setSecondaryColor(BYTE value)
{
	g_pMemMan->writeMem<BYTE>(m_dwpSecondaryColor, &value);
	return;
}

replayInterface::replayInterface()
{
}

replayInterface::~replayInterface()
{
}

void replayInterface::initPeds()
{
	for (size_t i = 0; i < dw_curPedNum; i++)
	{
		delete g_pPedList[i];
		g_pPedList[i] = nullptr;
	}
	dw_curPedNum = 256;
	//g_pMemMan->readMem<DWORD>((DWORD_PTR)m_dwpPedInterface + OFFSET_PED_INTERFACE_CUR_PEDS, &dw_curPedNum);

	for (size_t i = 0; i < dw_curPedNum; i++)
	{
		g_pPedList[i] = new entity();
		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)m_dwpPedList + i * 0x10, &g_pPedList[i]->m_dwpBase);
		g_pMemMan->readMem<DWORD_PTR>((DWORD_PTR)g_pPedList[i]->m_dwpBase + OFFSET_ENTITY_POSBASE, &g_pPedList[i]->m_dwpPosBase);
	}
}
