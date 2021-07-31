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
#ifndef HACK_H
#define HACK_H

#define INITPTR_INVALID_WORLD	1 << 0
#define INITPTR_INVALID_PLAYER	1 << 1
#define INITPTR_INVALID_VEHICLE	1 << 2
#define INITPTR_INVALID_WEAPON	1 << 3
#define INITPTR_INVALID_GLOBAL  1 << 5
#define INITPTR_INVALID_PLAYER_LIST 1 << 6
#define INITPTR_INVALID_REPLAY_INTERFACE 1 << 7
#define INITPTR_INVALID_UNK_MODEL 1 << 7
class trainer
{
	public:
		clock_t		m_keyTmr;

						trainer();
						~trainer();
		bool			checkKeyState(int key);
		virtual void	checkKeys();
};

class hack : public trainer
{
	public:
		v2		m_v2Waypoint;
		v3		m_v3Objective;
		player	m_player;
		vehicle m_vehicle;
		weapon	m_weapon;
		replayInterface	m_replayInterface;
		unkModel m_unkModel;
		std::string m_mpId;
		std::deque<std::pair<unsigned int,int>> m_dStat;
		ImpactExplosionEnum m_explosion;
		std::mutex m_mutex;
		BYTE original[5];

		HMODULE	m_hModule;

				hack();
				~hack();
		void	checkKeys();
		BYTE	initPointers();

		void	teleport(v3 v);
		bool	teleportWaypoint();
		bool	teleportObjective();
		void	restoreHealth();
		void	restoreVehicleHealth();
		void	restoreStamina();
		void	notWanted();
		void	killNpc();
		void	fillAmmo();
		void    consumeStatQueue();
		void	killAllNpc();
		void	tpAllNpc();
		void	tpHostilityNpc();
		void	killHostilityNpc();
		void	killHostilityNpcVeh();
		void	renderPlayerList();
		void	setImpactExplosion(ImpactExplosionEnum arg);
		//void	fillAllAmmo(float* arg);
		void	healVehicle();
		void	healPlayer();
		void	suicide();
		void	fillSkillLevels();
		void	fillAllSnacks();
		void	casinoStat();
		void	casinoStatBitSet1(int type);
		void	casinoStatBitSet2(int type);
		void	casinoHeistCut(feat* feature, int playerIndex);
		void	pericoStat();
		void	pericoStatBitSet1(int type);
		void	pericoStatBitSet2(int type);
		void	unlockHeistCars();
		void	unlockLSC();
		void	unlockWeaponCamos();
		//void	unlockWeapon();
		void	unlockAllAwards();
		void	unlockClothes();
		void	intoPV();
		void	loadSession(int id);
		void	setRank(int rank);
		void	forwardTeleport(float dist);
		void	spawnVehicle(int vehTypeIndex, int vehIndex);
		void	selfDropWeapon(int weaponTypeIndex, int weaponIndex);
		void	selfDropMoney(feat* feature);
		void	waterProof(feat* feature);
		void	undeadOffradar(feat* feature);
		void	superPunch(feat* feature);
		void	noSpread(feat* feature);
		void	noRecoil(feat* feature);
		void	quickReload(feat* feature);
		void	bulletDamage(feat* feature);
		void	weaponRange(feat* feature);
		void	weaponSpin(feat* feature);
		void	weaponForceOnPed(feat* feature);
		void	weaponForceOnVehicle(feat* feature);
		void	weaponForceOnHeli(feat* feature);
		void	weaponBulletEdit(feat* feature);
		void	runSpeed(feat* feature);
		void	swimSpeed(feat* feature);
		void	npcIgnore(feat* feature);
		void	godMode(feat* feature);
		void	frameFlags(feat* featSuperJump, feat* featExplosiveMelee, feat* featFireAmmo, feat* featExplosiveAmmo);
		void	vehicleGod(feat* feature);
		void	infAmmo(feat* feature);
		void	noReload(feat* feature);
		void	seatbelt(feat* feature);
		void	noRagdoll(feat* feature);
		void	vehicleAccel(feat* feature);
		void	vehicleBrake(feat* feature);
		void	neverWanted(feat* feature);
		void	vehicleTraction(feat* feature);
		void	vehicleGravity(feat* feature);
		void	vehicleBulletproofTires(feat* feature);
		void	wanted(feat* feature);
		void	bulletBatch(feat* feature);
		void	muzzleVelocity(feat* feature);
		void	vehicleDeformation(feat* feature);
		void	vehicleUpShift(feat* feature);
		void	vehicleSuspensionForce(feat* feature);
		void	vehicleDownShift(feat* feature);
		void	vehicleMass(feat* feature);
		void	vehicleBuoyancy(feat* feature);
		void	vehicleHandbrakeForce(feat* feature);
		void	boost(feat* feature);
		void	vehicleRocketRechargeSpeed(feat* feature);
		void	vehicleSuspensionHeigh(feat* feature);
		void	vehicleColisionDamageMult(feat* feature);
		void	vehicleWeaponDamageMult(feat* feature);
		void	vehicleEngineDamageMult(feat* feature);
		void	tunableRpMult(feat* feature);
		void	tunableMissionPayout(feat* feature);
		void	tunableOrbitalCannonCooldown(feat* feature);
		void	tunableBunkerResearch(feat* feature);
		void	tunableAntiIdleKick(feat* feature);
		void	removeSuicideCooldown(feat* feature);
		void	removePassiveModeCooldown(feat* feature);
		void	allowSellOnNonPublic(feat* feature);
		void	instantBullShark(feat* feature);
		void	bullSharkDrop();
		void	ammoDrop();
		void	miniGunDrop();
		void	boatTaxi();
		void	heliTaxi();
		void	backupHeli();
		void	airstrike();
		void	offRadar(feat* feature);
		void	ghostOragnization(feat* feature);
		void	blindCops(feat* feature);
		void	revealPlayers(feat* feature);
		void	disableThePhone(feat* feature);
		void	antiCEOKick(feat* feature);
		void	antiKickToSP(feat* feature);
		void	antiApartmentTp(feat* feature);
		void	antiRemoteBounty(feat* feature);
		void	antiWeatherControl(feat* feature);
		void	antiRemoteVehicleKick(feat* feature);
		void	antiRemoteForceMission(feat* feature);
		void	triggerBot(feat* feature);
		void	mpIndex(feat* feature);
		void	about(int arg);

	private:
		DWORD_PTR	m_dwpWorldBase,
					m_dwpPlayerBase,
					m_dwpVehicleBase,
					m_dwpAttackerBase,
					m_dwpWeaponManager,
					m_dwpWeaponCur,
					m_dwpAmmoInfo,
					m_dwpWeaponBase,
					m_dwpGlobalBase,
					m_dwpPlayerListBase,
					m_dwpReplayInterfaceBase,
					m_dwpUnkModelBase;
		bool		m_bInit,m_bSelfDropInit,m_bMouseDown;

		void	getWaypoint();
		void	getObjective();
		void	dStatPushBack(unsigned int hash, int value);
		void	callMerryweather(std::ptrdiff_t index);
		int		getPlayerId();
		int		getNetworkTime();
		void	setCasinoHeistCut(int playerIndex, int cut);
		int		getCasinoHeistCut(int playerIndex);
		void	createAmbientPickup(unsigned int pickupHash, float posX, float posY, float posZ, int value, unsigned int modelHash);
		void	blockScriptEvents(feat* feature, std::ptrdiff_t index);

		unsigned int joaat_with_mp_index(std::string input, std::string pre = "");
};

extern hack*		g_pHack;

#endif