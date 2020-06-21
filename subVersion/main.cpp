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

//global vars
HWND		g_hWnd;
hack*		g_pHack;
settings*	g_pSettings;
memManager*	g_pMemMan;
D3D9Render*	g_pD3D9Render;
std::map<int, CallbackProxy<hack, float>*>* g_pCBMap;
int			g_iFeature[MAX_MENU_FEATURES]	= {};
int			g_iIndex;
int			g_iFeaturePlayerList[32];
std::pair<int, std::string> tbl_SessionMItems[] = {
	{-1,"离开线上"},{0,"公共战局"},{1,"创建公共战局"},{12,"加入帮会伙伴"},
	{2,"私人帮会战局"},{3,"帮会战局"},{9,"加入好友"},{6,"私人好友战局"},
	{10,"单人战局"},{11,"仅限邀请战局"}
};

bool		g_bKillSwitch	= false;
bool		g_bKillRender	= false;
bool		g_bKillAttach	= false;
bool		g_bKillHack		= false;

long		ADDRESS_WORLD		= 0;
long		ADDRESS_BLIP		= 0;
long		ADDRESS_AMMO		= 0;
long		ADDRESS_MAGAZINE	= 0;
long		ADDRESS_TUNABLE		= 0;
long		ADDRESS_WEAPON		= 0;
long		ADDRESS_GLOBAL		= 0;
long		ADDRESS_PLAYER_LIST = 0;
long		ADDRESS_REPLAY_INTERFACE = 0;
//fuction prototypes
LRESULT	__stdcall	WindowProc(	HWND	hWnd,
								UINT	message,
								WPARAM	wParam,
								LPARAM	lParam);
int					addFeature( int cat, 
								int parent,
								std::string name,
								featType type,
								void (hack::* fun)(float*),
								float arg);
DWORD __stdcall		threadAttach(LPVOID lpParam);
DWORD __stdcall		threadRender(LPVOID lpParam);
DWORD __stdcall		threadHack	(LPVOID lpParam);

int __stdcall WinMain(	HINSTANCE	hInstance,
					HINSTANCE	hPrevInstance,
					LPSTR		lpCmdLine,
					int			nCmdShow)
{
	if(FindWindow("sub1toOverlay", nullptr))	//make sure the hack is not already running
		exit(0);

	g_iIndex		= 0;
	g_pMemMan		= new memManager;
	g_pSettings		= new settings;
	g_pD3D9Render	= new D3D9Render;
	g_pHack			= new hack;
	g_pCBMap		= new std::map<int, CallbackProxy<hack, float>*>;

	//LPCSTR	szWindowTitleTarget	= "Untitled - Notepad";
	LPCSTR	szWindowTitleTarget	= "Grand Theft Auto V";
	LPCSTR	szWindowTitle		= "subVersion mAsk°重制版 v1.2.0";
	g_pMemMan->setWindowName(szWindowTitleTarget);
	g_pD3D9Render->m_szWindowTitle = szWindowTitle;

	g_pSettings->addFeatureCategory("玩家");		//0
	g_pSettings->addFeatureCategory("武器");		//1
	g_pSettings->addFeatureCategory("载具");		//2
	g_pSettings->addFeatureCategory("传送");		//3
	g_pSettings->addFeatureCategory("杂项");		//4
	

	g_iFeature[FEATURE_P_TRUEGOD]			= g_pSettings->addFeature(0, -1, "无敌", feat_toggle, "trueGodMode");
	g_iFeature[FEATURE_P_GOD]				= g_pSettings->addFeature(0, -1, "半无敌", feat_toggle, "godMode");
	g_iFeature[FEATURE_P_HEAL]				= addFeature(0, -1, "治疗", feat_btn, &hack::healPlayer, -1.f);
	g_iFeature[FEATURE_P_SUICIDE]			= addFeature(0, -1, "自杀", feat_btn, &hack::suicide, -1.f);
	g_iFeature[FEATURE_P_WANTED]			= g_pSettings->addFeature(0, -1, "通缉等级", feat_slider, "wanted", 0.f, 5.f, .2f);
	g_iFeature[FEATURE_P_NEVERWANTED]		= g_pSettings->addFeature(0, -1, "永不通缉", feat_toggle, "neverWanted");
	addFeature(0, -1, "杀死所有NPC", feat_btn, &hack::killAllNpc, -1.f);
	g_iFeature[FEATURE_P_ANTINPC]			= g_pSettings->addFeature(0, -1, "反NPC", feat_toggle, "antiNpc");
	g_iFeature[FEATURE_P_NPC_IGNORE]		= g_pSettings->addFeature(0, -1, "NPC无视玩家", feat_toggle, "npcIgnore");
	g_iFeature[FEATURE_P_RUNSPD]			= g_pSettings->addFeature(0, -1, "奔跑速度", feat_slider, "runSpd", 1.f, 5.f);
	g_iFeature[FEATURE_P_SWIMSPD]			= g_pSettings->addFeature(0, -1, "游泳速度", feat_slider, "swimSpd", 1.f, 5.f);
	g_iFeature[FEATURE_P_SUPER_PUNCH]		= g_pSettings->addFeature(0, -1, "近战击退倍数", feat_slider, "superPunch", 0.f, 1000.f, (float)1.f / 10.f);
	g_iFeature[FEATURE_P_SUPERJUMP]			= g_pSettings->addFeature(0, -1, "超级跳跃", feat_toggle, "superJump");
	g_iFeature[FEATURE_P_EXPLOSIVEMELEE]	= g_pSettings->addFeature(0, -1, "爆炸近战", feat_toggle, "explMelee");
	g_iFeature[FEATURE_P_UNDEAD_OFFRADAR]	= g_pSettings->addFeature(0, -1, "假死雷达隐匿", feat_toggle, "undeadOffradar");
	g_iFeature[FEATURE_P_NORAGDOLL]			= g_pSettings->addFeature(0, -1, "无布娃娃", feat_toggle, "noRagdoll");
	g_iFeature[FEATURE_P_WATER_PROOF]		= g_pSettings->addFeature(0, -1, "水下行走", feat_toggle, "waterProof");
	g_iFeature[FEATURE_P_STAMINA]			= g_pSettings->addFeature(0, -1, "无限耐力", feat_toggle, "infStam");

	g_iFeature[FEATURE_W_FILL_ALL_AMMO]		= addFeature(1, -1, "补满所有武器弹药", feat_btn, &hack::fillAllAmmo, -1.f);
	g_iFeature[FEATURE_W_FILL_AMMO]			= addFeature(1, -1, "补满当前武器弹药", feat_btn, &hack::fillAmmo, -1.f);
	g_iFeature[FEATURE_W_SPREAD]			= g_pSettings->addFeature(1, -1, "无扩散", feat_toggle, "noSpread");	
	g_iFeature[FEATURE_W_RECOIL]			= g_pSettings->addFeature(1, -1, "无后座", feat_toggle, "noRecoil");	
	g_iFeature[FEATURE_W_NORELOAD]			= g_pSettings->addFeature(1, -1, "无需换弹", feat_toggle, "noReload");
	g_iFeature[FEATURE_W_RELOAD]			= g_pSettings->addFeature(1, -1, "快速换弹", feat_slider, "quickReload", 1.f, 10.f);
	int bulletEdit = g_pSettings->addFeature(1, -1, "子弹编辑 >>", feat_parent);
	g_iFeature[FEATURE_W_BULLET_EDIT] = g_pSettings->addFeature(-1, bulletEdit, "开/关", feat_toggle, "bulletEdit");
	addFeature(-1, bulletEdit, "手榴弹", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::GrenadeExplosion);
	addFeature(-1, bulletEdit, "粘弹", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::StickyBombExplosion);
	addFeature(-1, bulletEdit, "燃烧瓶", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::MoltovCoctailExplosion);
	addFeature(-1, bulletEdit, "隐形爆炸", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::SuperLauncher);
	addFeature(-1, bulletEdit, "闪电爆炸", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::LightningExplosion);
	addFeature(-1, bulletEdit, "大爆炸", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::BigExplosion);
	addFeature(-1, bulletEdit, "大爆炸2", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::BigFireyExplosion);
	addFeature(-1, bulletEdit, "中爆炸", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::MediumExplosion);
	addFeature(-1, bulletEdit, "小爆炸", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::TinyExplosion);
	addFeature(-1, bulletEdit, "小爆炸2", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::TinyExplosions2);
	addFeature(-1, bulletEdit, "小水柱", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::SmallWaterSpray);
	addFeature(-1, bulletEdit, "小火柱", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::SmallFireSpray);
	addFeature(-1, bulletEdit, "大水柱", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::BigWaterSpray);
	addFeature(-1, bulletEdit, "大火柱", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::BigFireSpray);
	addFeature(-1, bulletEdit, "MK2爆炸子弹", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::MK2ExplosiveBullets);
	addFeature(-1, bulletEdit, "烟雾弹", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::SmokeGrenade);
	addFeature(-1, bulletEdit, "催泪瓦斯", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::TearGas);
	addFeature(-1, bulletEdit, "催泪瓦斯2", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::TearGas2);
	addFeature(-1, bulletEdit, "信号弹", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::RedFlareSmoke);
	addFeature(-1, bulletEdit, "带特效的爆炸", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::CoolGroundExplosion);
	addFeature(-1, bulletEdit, "大冲击波爆炸", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::CRAZYSHOCKWAVEEXPLOSION);
	addFeature(-1, bulletEdit, "大火灾爆炸", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::HUGEFireyExplosion);
	addFeature(-1, bulletEdit, "超大型飞艇爆炸", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::MassiveBlimpExplosion);
	addFeature(-1, bulletEdit, "超大型飞艇爆炸2", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::MassiveBlimpExplosion2);
	addFeature(-1, bulletEdit, "大爆炸+坠落碎片", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::LargeExplosionFallingDebris);
	addFeature(-1, bulletEdit, "火球爆炸", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::FireBallExplosion);
	addFeature(-1, bulletEdit, "烟花", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::FireworkExplosion);
	addFeature(-1, bulletEdit, "雪球", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::SnowballHit);
	addFeature(-1, bulletEdit, "屏幕抖动爆炸", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::JustScreenShake);
	addFeature(-1, bulletEdit, "假爆炸（无伤害）", feat_btn, &hack::setImpactExplosion, ImpactExplosionEnum::SPOOFEXPLOSION);
	g_iFeature[FEATURE_W_FORCE_ON_PED]		= g_pSettings->addFeature(1, -1, "人冲击力", feat_slider, "forceOnPed", 1.f, 10000.f);
	g_iFeature[FEATURE_W_FORCE_ON_VEHICLE]	= g_pSettings->addFeature(1, -1, "车辆冲击力", feat_slider, "forceOnVeh", 1.f, 10000.f);
	g_iFeature[FEATURE_W_FORCE_ON_HELI]		= g_pSettings->addFeature(1, -1, "直升机冲击力", feat_slider, "forceOnHeli", 1.f, 10000.f);
	g_iFeature[FEATURE_W_DAMAGE]			= g_pSettings->addFeature(1, -1, "武器伤害倍数", feat_slider, "bulletDamage", 1.f, 10.f);
	g_iFeature[FEATURE_W_AMMO]				= g_pSettings->addFeature(1, -1, "无限弹药", feat_toggle, "infAmmo");
	g_iFeature[FEATURE_W_RANGE]				= g_pSettings->addFeature(1, -1, "射程", feat_slider, "weapRange", 1.f, 10.f);
	g_iFeature[FEATURE_W_SPINUP]			= g_pSettings->addFeature(1, -1, "加特林无需预热", feat_toggle, "weapSpin");
	g_iFeature[FEATURE_W_EXPLOSIVEAMMO]		= g_pSettings->addFeature(1, -1, "爆炸子弹", feat_toggle, "explAmmo");
	g_iFeature[FEATURE_W_FIREAMMO]			= g_pSettings->addFeature(1, -1, "燃烧子弹", feat_toggle, "fireAmmo");
	g_iFeature[FEATURE_W_BULLET_BATCH]		= g_pSettings->addFeature(1, -1, "批量子弹", feat_slider, "bulletBatch", 1.f, 10.f, (float) 1.f / 9.f);
	g_iFeature[FEATURE_W_BATCH_SPREAD]		= g_pSettings->addFeature(1, -1, "批量传播", feat_slider, "batchSpread", 0.f, 0.12f);
	g_iFeature[FEATURE_W_MUZZLE_VELOCITY]	= g_pSettings->addFeature(1, -1, "初速", feat_slider, "muzzleVelo", 1.f, 10.f);

	g_iFeature[FEATURE_V_TRUEGOD]			= g_pSettings->addFeature(2, -1, "无敌", feat_toggle, "vehTrueGodMode");
	g_iFeature[FEATURE_V_GOD]				= g_pSettings->addFeature(2, -1, "半无敌", feat_toggle, "vehGodMode");
	g_iFeature[FEATURE_V_HEAL]				= addFeature(2, -1, "修复", feat_btn, &hack::healVehicle, -1.f);
	g_iFeature[FEATURE_V_BULLETPROOFTIRES]	= g_pSettings->addFeature(2, -1, "防爆轮胎", feat_toggle, "vehBulletproofTires");
	g_iFeature[FEATURE_V_SEATBELT]			= g_pSettings->addFeature(2, -1, "安全带", feat_toggle, "seatbelt");
	g_iFeature[FEATURE_V_GRAVITY]			= g_pSettings->addFeature(2, -1, "重力", feat_slider, "vehGravity", 0.f, 25.f);
	g_iFeature[FEATURE_V_BOOST]				= g_pSettings->addFeature(2, -1, "无限喷射", feat_toggle, "vehBoost");
	g_iFeature[FEATURE_V_RECHARGE_SPEED]	= g_pSettings->addFeature(2, -1, "喷射恢复速度", feat_slider, "vehRrchargeSpeed", .5f, 5.f);
	int handing = g_pSettings->addFeature(2, -1, "属性 >>", feat_parent);
	g_iFeature[FEATURE_V_MASS]				= g_pSettings->addFeature(-1, handing, "质量", feat_slider, "vehMass", 0.f, 1000000.f);
	g_iFeature[FEATURE_V_BUOYANCY]			= g_pSettings->addFeature(-1, handing, "浮力", feat_slider, "vehBuoyancy", 0.f, 1000.f);
	g_iFeature[FEATURE_V_ACCELERATION]		= g_pSettings->addFeature(-1, handing, "加速度", feat_slider, "vehAccel", 1.f, 1000.f);
	g_iFeature[FEATURE_V_UPSHIFT]			= g_pSettings->addFeature(-1, handing, "加挡速度", feat_slider, "vehUpShift", 1.f, 25.f);
	g_iFeature[FEATURE_V_DOWNSHIFT]			= g_pSettings->addFeature(-1, handing, "减档速度", feat_slider, "vehDownShift", 1.f, 25.f);
	g_iFeature[FEATURE_V_BRAKEFORCE]		= g_pSettings->addFeature(-1, handing, "刹车制动力", feat_slider, "vehBrakeForce", 1.f, 25.f);
	g_iFeature[FEATURE_V_HANDBRAKEFORCE]	= g_pSettings->addFeature(-1, handing, "手刹制动力", feat_slider, "vehBuoyancy", 1.f, 25.f);
	g_iFeature[FEATURE_V_TRACTION]			= g_pSettings->addFeature(-1, handing, "牵引力", feat_slider, "vehTraction", 1.f, 25.f);
	g_iFeature[FEATURE_V_SUSPENSION_FORCE]	= g_pSettings->addFeature(-1, handing, "悬挂支撑力", feat_slider, "vehSuspensionForce", 0.f, 25.f);
	g_iFeature[FEATURE_V_SUSPENSION_HEIGH]	= g_pSettings->addFeature(-1, handing, "悬挂高度", feat_slider, "vehSuspensionHeigh", 0.f, 1.f);
	g_iFeature[FEATURE_V_COLISION_DAMAGE_MP]= g_pSettings->addFeature(-1, handing, "撞击伤害倍数", feat_slider, "vehColisionDamage", 0.f, 25.f);
	g_iFeature[FEATURE_V_WEAPON_DAMAGE_MP]	= g_pSettings->addFeature(-1, handing, "武器伤害倍数", feat_slider, "vehWeaponDamage", 0.f, 25.f);	
	g_iFeature[FEATURE_V_DEFORMATION]		= g_pSettings->addFeature(-1, handing, "变形倍数", feat_slider, "vehDeform", 0.f, 25.f);
	g_iFeature[FEATURE_V_ENGINE_DAMAGE_MP]	= g_pSettings->addFeature(-1, handing, "引擎伤害倍数", feat_slider, "vehEngineDamage", 0.f, 25.f);

	g_pSettings->addFeature(3, -1, "导航点", feat_teleport, tp_waypoint);
	g_pSettings->addFeature(3, -1, "目标点", feat_teleport, tp_objective);
	addFeature(3, -1, "向前", feat_btn, &hack::forwardTeleport, 5.f);

	int interior = g_pSettings->addFeature(3, -1, "室内 >>", feat_parent);
	g_pSettings->addFeature(-1, interior, "FIB大楼楼顶", feat_teleport, tp_static, 136.0f, -750.f, 262.f);
	g_pSettings->addFeature(-1, interior, "服装厂", feat_teleport, tp_static, 712.716f, -962.906f, 30.6f);
	g_pSettings->addFeature(-1, interior, "富兰克林家", feat_teleport, tp_static, 7.119f, 536.615f, 176.2f);
	g_pSettings->addFeature(-1, interior, "麦克家", feat_teleport, tp_static, -813.603f, 179.474f, 72.5f);
	g_pSettings->addFeature(-1, interior, "崔佛家", feat_teleport, tp_static, 1972.610f, 3817.040f, 33.65f);
	g_pSettings->addFeature(-1, interior, "丹尼斯阿姨家", feat_teleport, tp_static, -14.380f, -1438.510f, 31.3f);
	g_pSettings->addFeature(-1, interior, "弗洛伊德家", feat_teleport, tp_static, -1151.770f, -1518.138f, 10.85f);
	g_pSettings->addFeature(-1, interior, "莱斯特家", feat_teleport, tp_static, 1273.898f, -1719.304f, 54.8f);
	g_pSettings->addFeature(-1, interior, "脱衣舞俱乐部", feat_teleport, tp_static, 97.271f, -1290.994f, 29.45f);
	g_pSettings->addFeature(-1, interior, "银行金库（太平洋标准）", feat_teleport, tp_static, 255.85f, 217.f, 101.9f);
	g_pSettings->addFeature(-1, interior, "喜剧俱乐部", feat_teleport, tp_static, 378.100f, -999.964f, -98.6f);
	g_pSettings->addFeature(-1, interior, "人道实验室", feat_teleport, tp_static, 3614.394f, 3744.803f, 28.9f);
	g_pSettings->addFeature(-1, interior, "人道实验室地道", feat_teleport, tp_static, 3525.201f, 3709.625f, 21.2f);
	g_pSettings->addFeature(-1, interior, "IAA办公室", feat_teleport, tp_static, 113.568f, -619.001f, 206.25f);
	g_pSettings->addFeature(-1, interior, "刑讯室", feat_teleport, tp_static, 142.746f, -2201.189f, 4.9f);
	g_pSettings->addFeature(-1, interior, "军事基地高塔", feat_teleport, tp_static, -2358.132f, 3249.754f, 101.65f);
	g_pSettings->addFeature(-1, interior, "矿井", feat_teleport, tp_static, -595.342f, 2086.008f, 131.6f);
	int saved = g_pSettings->addFeature(3, -1, "自定义保存点 >>", feat_parent);
	g_pSettings->addFeature(-1, saved, "保存点1", feat_teleport, "pos0", tp_saved);
	g_pSettings->addFeature(-1, saved, "保存点2", feat_teleport, "pos1", tp_saved);
	g_pSettings->addFeature(-1, saved, "保存点3", feat_teleport, "pos2", tp_saved);
	g_pSettings->addFeature(-1, saved, "保存点4", feat_teleport, "pos3", tp_saved);
	g_pSettings->addFeature(-1, saved, "保存点5", feat_teleport, "pos4", tp_saved);
	g_pSettings->addFeature(-1, saved, "保存点6", feat_teleport, "pos5", tp_saved);
	g_pSettings->addFeature(3, -1, "洛圣都改车王", feat_teleport, tp_static, -365.425f, -131.809f, -225.f);//38.9f);
	g_pSettings->addFeature(3, -1, "LS机场", feat_teleport, tp_static, -1336.f, -3044.f, -225.f);//14.15f);
	g_pSettings->addFeature(3, -1, "桑迪海岸机场", feat_teleport, tp_static, 1747.f, 3273.f, -225.f);//41.35f);
	g_pSettings->addFeature(3, -1, "千年山", feat_teleport, tp_static, 489.979f, 5587.527f, 794.3f);

	int session = g_pSettings->addFeature(4, -1, "战局 >>", feat_parent);
	for (size_t i = 0; i < sizeof(tbl_SessionMItems)/sizeof(tbl_SessionMItems[0]); i++)
		addFeature(-1, session, tbl_SessionMItems[i].second, feat_btn, &hack::loadSession, tbl_SessionMItems[i].first);
	int olService = g_pSettings->addFeature(4, -1, "线上 >>", feat_parent);
	addFeature(-1, olService, "坐进个人载具", feat_btn, &hack::intoPV, -1.f);
	g_iFeature[FEATURE_P_MONERY_DROP] = g_pSettings->addFeature(-1, olService, "钱袋刷钱", feat_toggle, "moneyDrop");
	//g_iFeature[FEATURE_P_PLAYER_LIST] = g_pSettings->addFeature(3, -1, "玩家列表 >>", feat_parent);
	//for (size_t i = 0; i < sizeof(g_iFeaturePlayerList)/sizeof(g_iFeaturePlayerList[0]); i++)
	//{
	//	g_iFeaturePlayerList[i] = g_pSettings->addFeature(-1, g_iFeature[FEATURE_P_PLAYER_LIST],"线上 >>", feat_parent);
	//}
	int vehSpawn = g_pSettings->addFeature(-1, olService, "刷车 >>", feat_parent);
	for (size_t i = 0; i < vehiclePreview.size(); i++)
	{
		int temp = g_pSettings->addFeature(-1, vehSpawn, vehiclePreview[i].first, feat_parent);
		for (size_t j = 0; j < vehiclePreview[i].second.size(); j++)
			addFeature(-1, temp, vehiclePreview[i].second[j].VName, feat_btn, &hack::spawnVehicle, i * 1000 + j);
	}

	int tunable = g_pSettings->addFeature(-1, olService, "可调参数 >>", feat_parent);
	g_iFeature[FEATURE_T_ANTI_IDLE_KICK] = g_pSettings->addFeature(-1, tunable, "AFK反挂机踢出", feat_toggle, "AntiIdleKick");
	g_iFeature[FEATURE_T_ORBITAL_CANNON] = g_pSettings->addFeature(-1, tunable, "天基炮无冷却", feat_toggle, "OrbitalCannon");
	g_iFeature[FEATURE_T_RP_MP]			   = g_pSettings->addFeature(-1, tunable, "RP倍数", feat_slider,"RP", 1.f, 1000.f , (float)1.f / 9.f);
	g_iFeature[FEATURE_T_AP_MP]			   = g_pSettings->addFeature(-1, tunable, "AP倍数", feat_slider, "AP", 1.f, 1000.f, (float)1.f / 9.f);
	g_iFeature[FEATURE_T_MISSION_PAYOUT]   = g_pSettings->addFeature(-1, tunable, "最小任务金额", feat_slider, "MinMissionPayout", 0.f, 100000.f);
	int recovery = g_pSettings->addFeature(-1, olService, "解锁&恢复 >>", feat_parent);
	addFeature(-1, recovery, "角色属性全满", feat_btn, &hack::fillSkillLevels, -1.f);
	addFeature(-1, recovery, "补满零食、防弹衣", feat_btn, &hack::fillAllSnacks, -1.f);
	int casino = g_pSettings->addFeature(-1, recovery, "赌场豪劫 >>", feat_parent);
	addFeature(-1, casino, "清除冷却时间", feat_btn, &hack::casinoStat, 1.f);
	int bitSet1 = g_pSettings->addFeature(-1, casino, "第一块计划板 >>", feat_parent);
	addFeature(-1, bitSet1, "解锁所有探查点", feat_btn, &hack::casinoStatBitSet1, 7.f);
	addFeature(-1, bitSet1, "解锁所有兴趣点", feat_btn, &hack::casinoStatBitSet1, 8.f);
	int approach = g_pSettings->addFeature(-1, bitSet1, "抢劫方式 >>", feat_parent);
	addFeature(-1, approach, "潜行匿踪", feat_btn, &hack::casinoStatBitSet1, 0.f);
	addFeature(-1, approach, "兵不厌诈", feat_btn, &hack::casinoStatBitSet1, 1.f);
	addFeature(-1, approach, "气势汹汹", feat_btn, &hack::casinoStatBitSet1, 2.f);
	int target = g_pSettings->addFeature(-1, bitSet1, "抢劫物品 >>", feat_parent);
	addFeature(-1, target, "现金", feat_btn, &hack::casinoStatBitSet1, 3.f);
	addFeature(-1, target, "黄金", feat_btn, &hack::casinoStatBitSet1, 4.f);
	addFeature(-1, target, "艺术品", feat_btn, &hack::casinoStatBitSet1, 5.f);
	addFeature(-1, target, "钻石", feat_btn, &hack::casinoStatBitSet1, 6.f);
	int bitSet2 = g_pSettings->addFeature(-1, casino, "第二块计划板 >>", feat_parent);
	addFeature(-1, bitSet2, "一键解锁、最高等级", feat_btn, &hack::casinoStatBitSet2, 11.f);
	addFeature(-1, bitSet2, "削弱敌人装备", feat_btn, &hack::casinoStatBitSet2, 0.f);
	addFeature(-1, bitSet2, "钥匙卡最高级", feat_btn, &hack::casinoStatBitSet2, 1.f);
	addFeature(-1, bitSet2, "最高级抢手", feat_btn, &hack::casinoStatBitSet2, 2.f);
	addFeature(-1, bitSet2, "最高级司机", feat_btn, &hack::casinoStatBitSet2, 3.f);
	addFeature(-1, bitSet2, "最高级黑客", feat_btn, &hack::casinoStatBitSet2, 4.f);
	int vehs = g_pSettings->addFeature(-1, bitSet2, "逃亡载具 >>", feat_parent);
	addFeature(-1, vehs, "0", feat_btn, &hack::casinoStatBitSet2, 5.f);
	addFeature(-1, vehs, "1", feat_btn, &hack::casinoStatBitSet2, 6.f);
	addFeature(-1, vehs, "2", feat_btn, &hack::casinoStatBitSet2, 7.f);
	addFeature(-1, vehs, "3", feat_btn, &hack::casinoStatBitSet2, 8.f);
	int weaps = g_pSettings->addFeature(-1, bitSet2, "武器 >>", feat_parent);
	addFeature(-1, weaps, "0", feat_btn, &hack::casinoStatBitSet2, 9.f);
	addFeature(-1, weaps, "1", feat_btn, &hack::casinoStatBitSet2, 10.f);
	int unlock = g_pSettings->addFeature(-1, recovery, "解锁 >>", feat_parent);
	addFeature(-1, unlock, "解锁改车配件", feat_btn, &hack::unlockLSC, -1.f);
	addFeature(-1, unlock, "解锁武器涂装", feat_btn, &hack::unlockWeaponCamos, -1.f);
	addFeature(-1, unlock, "解锁抢劫车辆批发价", feat_btn, &hack::unlockHeistCars, -1.f);
	addFeature(-1, unlock, "解锁奖章", feat_btn, &hack::unlockAllAwards, -1.f);
	addFeature(-1, unlock, "解锁衣服", feat_btn, &hack::unlockClothes, -1.f);
	g_iFeature[FEATURE_T_BUNKER_RESEARCH] = g_pSettings->addFeature(-1, unlock, "解锁所有地堡研究(临时)", feat_toggle, "BunkerResearch");
	addFeature(4, -1, "GitHub - 关于", feat_btn, &hack::about, 0.f);
	addFeature(4, -1, "检查更新", feat_btn, &hack::about, 1.f);


	g_pSettings->setActiveCat(0);			//this needs to be called so we can fill the current feature buffer

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON1));
	wc.hbrBackground = dynamic_cast<HBRUSH>(CreateSolidBrush(RGB(0, 0, 0)));//(HBRUSH)COLOR_WINDOW;
	wc.lpszClassName = "sub1toOverlay";

	RegisterClassEx(&wc);
	g_hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED | WS_EX_TOOLWINDOW,		//dwExStyle [in]
							"sub1toOverlay",										//lpClassName [in, optional]
							szWindowTitle,											//lpWindowName [in, optional]
							WS_POPUP,												//dwStyle [in]
							0,														//x [in]
							0,														//y [in]
							300,													//nWidth [in]
							300,													//nHeight [in]
							nullptr,												//hWndParent [in, optional]
							nullptr,												//hMenu [in, optional]
							hInstance,												//hInstance [in, optional]		A handle to the instance of the module to be associated with the window.
							nullptr);												//lpParam [in, optional]

	SetLayeredWindowAttributes(g_hWnd, 0, 0, LWA_ALPHA);
	SetLayeredWindowAttributes(g_hWnd, RGB(0, 0, 0), 0, LWA_COLORKEY);

	MARGINS margins {1, 1, 1, 1};
	DwmExtendFrameIntoClientArea(g_hWnd, &margins);

	ShowWindow(g_hWnd, SW_SHOWNORMAL);

	CreateThread(	NULL,
					0,
					threadAttach,
					NULL,
					0,
					nullptr);
	Sleep(100);
	CreateThread(	NULL,
					0,
					threadRender,
					NULL,
					0,
					nullptr);
	Sleep(100);
	CreateThread(	NULL,
					0,
					threadHack,
					NULL,
					0,
					nullptr);

	MSG msg;
	while(true)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))	// Check to see if any messages are waiting in the queue
		{
			TranslateMessage(&msg);		//Translate the message and dispatch it to WindowProc()
			DispatchMessage(&msg);
		}

		if(msg.message == WM_QUIT)
			break;

		Sleep(100);
	}

	return (int) msg.wParam;
}

//main message handler
LRESULT __stdcall WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		break;
	}

	return DefWindowProc (hWnd, message, wParam, lParam); //default behaviour for any unhandled messages
}

int addFeature(int cat, int parent, std::string name, featType type, void (hack::* func)(float*), float arg)
{
	(*g_pCBMap)[g_iIndex] = new CallbackProxy<hack, float>;
	(*g_pCBMap)[g_iIndex]->Set(g_pHack, func);
	return g_pSettings->addFeature(cat, parent, name, type, g_iIndex++, arg);
}

DWORD __stdcall threadAttach(LPVOID lpParam)
{
	while(!g_bKillSwitch)
	{
		if(g_pMemMan->attach() && g_pMemMan->findWindow())
		{
			HWND	fgWnd	= GetForegroundWindow(),
					tgWnd	= g_pMemMan->getWindow();
			if(g_pD3D9Render->getViewport())
				MoveWindow(g_hWnd, g_pD3D9Render->m_screen.x, g_pD3D9Render->m_screen.y, g_pD3D9Render->m_screen.w, g_pD3D9Render->m_screen.h, true);

			if(fgWnd != tgWnd && fgWnd != g_hWnd)
			{
				SetWindowPos(g_hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
			else if(g_hWnd == fgWnd)
			{
				ShowWindow(tgWnd, SW_SHOW);
				SetForegroundWindow(tgWnd);
			}
			else if(tgWnd == fgWnd && !(GetWindowLong(g_hWnd, GWL_EXSTYLE) & WS_EX_TOPMOST))
			{
				SetWindowPos(g_hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
			}
		}
		else
		{
			MessageBox(nullptr, "请确保GTA5正在运行!", "subVersion加载失败", MB_OK | MB_ICONERROR);
			g_bKillAttach = true;
			killProgram();
		}

		Sleep(0x30);
	}
	g_bKillAttach	= true;
	return 1;
}

DWORD __stdcall threadRender(LPVOID lpParam)
{
	g_pD3D9Render->init(g_hWnd);
	while(!g_bKillSwitch)
	{
		g_pD3D9Render->render();
		Sleep(0x10);
	}
	g_bKillRender	= true;
	return 0;
}

DWORD __stdcall threadHack(LPVOID lpParam)
{
	g_pHack->m_hModule = g_pMemMan->getModuleAddress("GTA5.exe");
	while(!g_bKillSwitch)
	{
		BYTE btInit	= g_pHack->initPointers();
		g_pHack->checkKeys();

		if(!(btInit & INITPTR_INVALID_WORLD) && !(btInit & INITPTR_INVALID_PLAYER))
		{
			if(g_pSettings->getFeature(g_iFeature[FEATURE_P_GOD])->m_bOn || g_pSettings->getFeature(g_iFeature[FEATURE_P_TRUEGOD])->m_bOn)
				g_pHack->restoreHealth();
			if(g_pSettings->getFeature(g_iFeature[FEATURE_P_ANTINPC])->m_bOn)
				g_pHack->killNpc();
			if(g_pSettings->getFeature(g_iFeature[FEATURE_P_NEVERWANTED])->m_bOn)
				g_pHack->notWanted();
			if(g_pSettings->getFeature(g_iFeature[FEATURE_P_STAMINA])->m_bOn)
				g_pHack->restoreStamina();

			g_pHack->neverWanted(g_pSettings->getFeature(g_iFeature[FEATURE_P_NEVERWANTED]));
			g_pHack->wanted(g_pSettings->getFeature(g_iFeature[FEATURE_P_WANTED]));
			g_pHack->runSpeed(g_pSettings->getFeature(g_iFeature[FEATURE_P_RUNSPD]));
			g_pHack->swimSpeed(g_pSettings->getFeature(g_iFeature[FEATURE_P_SWIMSPD]));
			g_pHack->npcIgnore(g_pSettings->getFeature(g_iFeature[FEATURE_P_NPC_IGNORE]));
			g_pHack->godMode(g_pSettings->getFeature(g_iFeature[FEATURE_P_TRUEGOD]));
			g_pHack->noRagdoll(g_pSettings->getFeature(g_iFeature[FEATURE_P_NORAGDOLL]));
			g_pHack->seatbelt(g_pSettings->getFeature(g_iFeature[FEATURE_V_SEATBELT]));
			g_pHack->superPunch(g_pSettings->getFeature(g_iFeature[FEATURE_P_SUPER_PUNCH]));
			if (g_pHack->m_player.loadPlayerData())
			{
				g_pHack->undeadOffradar(g_pSettings->getFeature(g_iFeature[FEATURE_P_UNDEAD_OFFRADAR]));
				g_pHack->waterProof(g_pSettings->getFeature(g_iFeature[FEATURE_P_WATER_PROOF]));
			}

			g_pHack->frameFlags(	g_pSettings->getFeature(g_iFeature[FEATURE_P_SUPERJUMP]),
									g_pSettings->getFeature(g_iFeature[FEATURE_P_EXPLOSIVEMELEE]),
									g_pSettings->getFeature(g_iFeature[FEATURE_W_FIREAMMO]),
									g_pSettings->getFeature(g_iFeature[FEATURE_W_EXPLOSIVEAMMO]));


			if(!(btInit & INITPTR_INVALID_VEHICLE))
			{
				if(g_pSettings->getFeature(g_iFeature[FEATURE_V_GOD])->m_bOn)
					g_pHack->restoreVehicleHealth();

				g_pHack->vehicleGod(g_pSettings->getFeature(g_iFeature[FEATURE_V_TRUEGOD]));
				g_pHack->vehicleGravity(g_pSettings->getFeature(g_iFeature[FEATURE_V_GRAVITY]));
				g_pHack->vehicleBulletproofTires(g_pSettings->getFeature(g_iFeature[FEATURE_V_BULLETPROOFTIRES]));
				g_pHack->boost(g_pSettings->getFeature(g_iFeature[FEATURE_V_BOOST]));
				g_pHack->vehicleRocketRechargeSpeed(g_pSettings->getFeature(g_iFeature[FEATURE_V_RECHARGE_SPEED]));

				if(g_pHack->m_vehicle.loadHandling())
				{
					g_pHack->vehicleAccel(g_pSettings->getFeature(g_iFeature[FEATURE_V_ACCELERATION]));
					g_pHack->vehicleBrake(g_pSettings->getFeature(g_iFeature[FEATURE_V_BRAKEFORCE]));
					g_pHack->vehicleTraction(g_pSettings->getFeature(g_iFeature[FEATURE_V_TRACTION]));
					g_pHack->vehicleDeformation(g_pSettings->getFeature(g_iFeature[FEATURE_V_DEFORMATION]));
					g_pHack->vehicleUpShift(g_pSettings->getFeature(g_iFeature[FEATURE_V_UPSHIFT]));
					g_pHack->vehicleDownShift(g_pSettings->getFeature(g_iFeature[FEATURE_V_DOWNSHIFT]));
					g_pHack->vehicleSuspensionForce(g_pSettings->getFeature(g_iFeature[FEATURE_V_SUSPENSION_FORCE]));
					g_pHack->vehicleMass(g_pSettings->getFeature(g_iFeature[FEATURE_V_MASS]));
					g_pHack->vehicleBuoyancy(g_pSettings->getFeature(g_iFeature[FEATURE_V_BUOYANCY]));
					g_pHack->vehicleHandbrakeForce(g_pSettings->getFeature(g_iFeature[FEATURE_V_HANDBRAKEFORCE]));
					g_pHack->vehicleSuspensionHeigh(g_pSettings->getFeature(g_iFeature[FEATURE_V_SUSPENSION_HEIGH]));
					g_pHack->vehicleColisionDamageMult(g_pSettings->getFeature(g_iFeature[FEATURE_V_COLISION_DAMAGE_MP]));
					g_pHack->vehicleWeaponDamageMult(g_pSettings->getFeature(g_iFeature[FEATURE_V_WEAPON_DAMAGE_MP]));
					g_pHack->vehicleEngineDamageMult(g_pSettings->getFeature(g_iFeature[FEATURE_V_ENGINE_DAMAGE_MP]));
				}
			}

			if(!(btInit & INITPTR_INVALID_WEAPON) && g_pHack->m_weapon.loadWeapon())
			{
				g_pHack->weaponBulletEdit(g_pSettings->getFeature(g_iFeature[FEATURE_W_BULLET_EDIT]));
				g_pHack->noSpread(g_pSettings->getFeature(g_iFeature[FEATURE_W_SPREAD]));
				g_pHack->noRecoil(g_pSettings->getFeature(g_iFeature[FEATURE_W_RECOIL]));
				g_pHack->quickReload(g_pSettings->getFeature(g_iFeature[FEATURE_W_RELOAD]));
				g_pHack->bulletDamage(g_pSettings->getFeature(g_iFeature[FEATURE_W_DAMAGE]));
				g_pHack->weaponRange(g_pSettings->getFeature(g_iFeature[FEATURE_W_RANGE]));
				g_pHack->weaponSpin(g_pSettings->getFeature(g_iFeature[FEATURE_W_SPINUP]));
				g_pHack->weaponForceOnPed(g_pSettings->getFeature(g_iFeature[FEATURE_W_FORCE_ON_PED]));
				g_pHack->weaponForceOnVehicle(g_pSettings->getFeature(g_iFeature[FEATURE_W_FORCE_ON_VEHICLE]));
				g_pHack->weaponForceOnHeli(g_pSettings->getFeature(g_iFeature[FEATURE_W_FORCE_ON_HELI]));
				g_pHack->bulletBatch(g_pSettings->getFeature(g_iFeature[FEATURE_W_BULLET_BATCH]));
				g_pHack->batchSpread(g_pSettings->getFeature(g_iFeature[FEATURE_W_BATCH_SPREAD]));
				g_pHack->muzzleVelocity(g_pSettings->getFeature(g_iFeature[FEATURE_W_MUZZLE_VELOCITY]));
				g_pHack->infAmmo(g_pSettings->getFeature(g_iFeature[FEATURE_W_AMMO]));
				g_pHack->noReload(g_pSettings->getFeature(g_iFeature[FEATURE_W_NORELOAD]));
			}

			if (!(btInit & INITPTR_INVALID_TUNABLE))
			{
				g_pHack->tunableRpMult(g_pSettings->getFeature(g_iFeature[FEATURE_T_RP_MP]));
				g_pHack->tunableApMult(g_pSettings->getFeature(g_iFeature[FEATURE_T_AP_MP]));
				g_pHack->tunableMissionPayout(g_pSettings->getFeature(g_iFeature[FEATURE_T_MISSION_PAYOUT]));
				g_pHack->tunableOrbitalCannonCooldown(g_pSettings->getFeature(g_iFeature[FEATURE_T_ORBITAL_CANNON]));
				g_pHack->tunableBunkerResearch(g_pSettings->getFeature(g_iFeature[FEATURE_T_BUNKER_RESEARCH]));
				g_pHack->tunableAntiIdleKick(g_pSettings->getFeature(g_iFeature[FEATURE_T_ANTI_IDLE_KICK]));
			}

			if (!(btInit & INITPTR_INVALID_GLOBAL))
			{
				g_pHack->selfDropMoney(g_pSettings->getFeature(g_iFeature[FEATURE_P_MONERY_DROP]));
				g_pHack->consumeStatQueue();
			}

			//g_pHack->renderPlayerList(g_iFeature[FEATURE_P_PLAYER_LIST], g_iFeaturePlayerList);
		}
		Sleep(1);
	}
	g_bKillHack = true;
	return 0;
}

//Kill the program; Prototype in stdafx.h
void	killProgram()
{
	g_bKillSwitch = true;				//enable thread killswitch
	g_pSettings->m_iniParser.write();	//save options

	//make sure we shut down all threads before deleting the objects
	while(!g_bKillAttach || !g_bKillRender || !g_bKillHack)
		Sleep(1);

	//restore patched code
	feat dummyFeat;
	dummyFeat.m_bRestored = false;
	g_pHack->noReload(&dummyFeat);
	dummyFeat.m_bRestored = false;
	g_pHack->infAmmo(&dummyFeat);

	delete	g_pHack;
	delete	g_pD3D9Render;
	delete	g_pSettings;
	delete	g_pMemMan;

	exit(0);
}