#pragma once
#include<vector>

struct WeaponPreview {
	std::wstring Name;
	char* Weapon;
	char* Pickup;
	char* Model;
};

static const std::vector<WeaponPreview> Pistol{
	{L"穿甲手枪","weapon_appistol","pickup_weapon_appistol","w_pi_appistol" },
	{L"战斗手枪","weapon_combatpistol","pickup_weapon_combatpistol","w_pi_combatpistol" },
	{L"信号枪","weapon_flaregun","pickup_weapon_flaregun","w_pi_flaregun"},
	{L"重型手枪","weapon_heavypistol","pickup_weapon_heavypistol","w_pi_heavypistol" },
	{L"射手手枪","weapon_marksmanpistol","pickup_weapon_marksmanpistol","w_pi_marksmanpistol"},
	{L"手枪","weapon_pistol","pickup_weapon_pistol","w_pi_pistol" },
	{L"手枪 MK2","weapon_pistol_mk2","pickup_weapon_pistol_mk2","w_pi_pistol_mk2"},
	{L".5口径手枪","weapon_pistol50","pickup_weapon_pistol50","w_pi_pistol50" },
	{L"冲锋手枪","weapon_raypistol","pickup_weapon_raypistol","w_pi_raygun" },
	{L"重型左轮手枪","weapon_revolver","pickup_weapon_revolver","w_pi_revolver"},
	{L"重型左轮手枪 MK2","weapon_revolver_mk2","pickup_weapon_revolver_mk2","w_pi_revolver_mk2"},
	{L"劣质手枪","weapon_snspistol","pickup_weapon_snspistol","w_pi_sns_pistol "},
	{L"劣质手枪 MK2","weapon_snspistol_mk2","pickup_weapon_snspistol_mk2","w_pi_sns_pistol_mk2"},
	{L"电击枪","weapon_stungun","pickup_weapon_stungun","w_pi_stungun" },
	{L"老式手枪","weapon_vintagepistol","pickup_weapon_vintagepistol","w_pi_vintage_pistol" },
};
static const std::vector<WeaponPreview> Rifle{
	{L"高级步枪", "weapon_advancedrifle", "pickup_weapon_advancedrifle", "w_ar_advancedrifle" },
	{L"突击步枪AK47","weapon_assaultrifle","pickup_weapon_assaultrifle","w_ar_assaultrifle" },
	{L"突击步枪AK47 MK2","weapon_assaultrifle_mk2","pickup_weapon_assaultrifle_mk2","w_ar_assaultrifle_mk2"},
	{L"无托式步枪QBZ95","weapon_bullpuprifle","pickup_weapon_bullpuprifle","w_ar_bullpuprifle" },
	{L"无托式步枪QBZ95 MK2","weapon_bullpuprifle_mk2","pickup_weapon_bullpuprifle_mk2","w_ar_bullpuprifle_mk2"},
	{L"卡宾步枪M4","weapon_carbinerifle","pickup_weapon_carbinerifle","w_ar_carbinerifle" },
	{L"卡宾步枪M4 MK2","weapon_carbinerifle_mk2","pickup_weapon_carbinerifle_mk2","w_ar_carbinerifle_mk2"},
	{L"紧凑型步枪","weapon_compactrifle","pickup_weapon_compactrifle","w_ar_compactrifle"},
	{L"老式火枪","weapon_musket","pickup_weapon_musket","w_ar_musket"},
	{L"特制卡宾步枪","weapon_specialcarbine","pickup_weapon_specialcarbine","w_ar_specialcarbine"},
	{L"特制卡宾步枪 MK2","weapon_specialcarbine_mk2","pickup_weapon_specialcarbine_mk2","w_ar_specialcarbine_mk2"},
};
static const std::vector<WeaponPreview> SMG{
	{L"突击冲锋枪","weapon_assaultsmg","pickup_weapon_assaultsmg","w_sb_assaultsmg" },
	{L"作战自卫冲锋枪","weapon_combatpdw","pickup_weapon_combatpdw","w_sb_pdw"},
	{L"UZI","weapon_microsmg","pickup_weapon_microsmg","w_sb_microsmg" },
	{L"汤姆逊冲锋枪","weapon_gusenberg","pickup_weapon_gusenberg","w_sb_gusenberg" },
	{L"冲锋枪","weapon_smg","pickup_weapon_smg","w_sb_smg" },
	{L"冲锋枪 MK2","weapon_smg_mk2","pickup_weapon_smg_mk2","w_sb_smg_mk2"},
	{L"微型冲锋枪","weapon_minismg","pickup_weapon_minismg","w_sb_minismg" },
};
static const std::vector<WeaponPreview> MG{
	{L"战斗机枪","weapon_combatmg","pickup_weapon_combatmg","w_mg_combatmg" },
	{L"战斗机枪 MK2","weapon_combatmg_mk2","pickup_weapon_combatmg_mk2","w_mg_combatmgmk2"},
	{L"机枪","weapon_mg","pickup_weapon_mg","w_mg_mg" },
};
static const std::vector<WeaponPreview> Shotgun{
	{L"突击霰弹枪","weapon_assaultshotgun","pickup_weapon_assaultshotgun","w_sg_assaultshotgun" },
	{L"冲锋霰弹枪","weapon_autoshotgun","pickup_weapon_autoshotgun","w_sg_autoshotgun"},
	{L"无托式霰弹枪","weapon_bullpupshotgun","pickup_weapon_bullpupshotgun","w_sg_bullpupshotgun" },
	{L"双管霰弹枪","weapon_dbshotgun","pickup_weapon_dbshotgun","w_sg_dbshotgun"},
	{L"重型霰弹枪","weapon_heavyshotgun","pickup_weapon_heavyshotgun","w_sg_heavyshotgun" },
	{L"泵动式霰弹枪","weapon_pumpshotgun","pickup_weapon_pumpshotgun","w_sg_pumpshotgun" },
	{L"泵动式霰弹枪 MK2","weapon_pumpshotgun_mk2","pickup_weapon_pumpshotgun_mk2","w_sg_pumpshotgun_mk2"},
	{L"短管霰弹枪","weapon_sawnoffshotgun","pickup_weapon_sawnoffshotgun","w_sg_sawnoff" },
};
static const std::vector<WeaponPreview> Sniper{
	{L"重型狙击步枪","weapon_heavysniper","pickup_weapon_heavysniper","w_sr_heavysniper" },
	{L"重型狙击步枪 MK2","weapon_heavysniper_mk2","pickup_weapon_heavysniper_mk2","w_sr_heavysniper_mk2"},
	{L"射手步枪","weapon_marksmanrifle","pickup_weapon_marksmanrifle","w_sr_marksmanrifle" },
	{L"射手步枪 MK2","weapon_marksmanrifle_mk2","pickup_weapon_marksmanrifle_mk2","w_sr_marksmanrifle_mk2"},
	{L"狙击步枪","weapon_sniperrifle","pickup_weapon_sniperrifle","w_sr_sniperrifle" },
};
static const std::vector<WeaponPreview> Heavy{
	{L"电磁步枪","weapon_railgun","pickup_weapon_railgun","w_ar_railgun"},
	{L"加特林","weapon_minigun","pickup_weapon_minigun","w_mg_minigun" },
	{L"紧凑型榴弹发射器","weapon_compactlauncher","pickup_weapon_compactlauncher","w_lr_compactlauncher"},
	{L"烟花发射器","weapon_firework","pickup_weapon_firework","w_lr_firework" },
	{L"榴弹发射器","weapon_grenadelauncher","pickup_weapon_grenadelauncher","w_lr_grenadelauncher" },
	{L"制导火箭发射器","weapon_hominglauncher","pickup_weapon_hominglauncher","w_lr_homing"},
	{L"RPG","weapon_rpg","pickup_weapon_rpg","w_lr_rpg" },
};

static const std::vector<std::pair<std::wstring, std::vector<WeaponPreview>>> weaponPreview{
	{L"手枪 >>",Pistol},
	{L"步枪 >>",Rifle},
	{L"冲锋枪 >>",SMG},
	{L"轻机枪 >>",MG},
	{L"霰弹枪 >>",Shotgun},
	{L"狙击枪 >>",Sniper},
	{L"重武器 >>",Heavy},
};