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
int			g_iFeature[MAX_MENU_FEATURES]	= {};

bool		g_bKillSwitch	= false;
bool		g_bKillRender	= false;
bool		g_bKillAttach	= false;
bool		g_bKillHack		= false;

long		ADDRESS_WORLD	= 0;
long		ADDRESS_WAYPOINT= 0;
long		ADDRESS_AMMO	= 0;
long		ADDRESS_MAGAZINE= 0;
long		ADDRESS_TUNABLE = 0;
//fuction prototypes
LRESULT	__stdcall	WindowProc(	HWND	hWnd,
								UINT	message,
								WPARAM	wParam,
								LPARAM	lParam);
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

	g_pMemMan		= new memManager;
	g_pSettings		= new settings;
	g_pD3D9Render	= new D3D9Render;
	g_pHack			= new hack;

	//LPCSTR	szWindowTitleTarget	= "Untitled - Notepad";
	LPCSTR	szWindowTitleTarget	= "Grand Theft Auto V";
	LPCSTR	szWindowTitle		= "subVersion mAsk°重制版";
	g_pMemMan->setWindowName(szWindowTitleTarget);
	g_pD3D9Render->m_szWindowTitle = szWindowTitle;

	g_pSettings->addFeatureCategory("玩家");		//0
	g_pSettings->addFeatureCategory("武器");		//1
	g_pSettings->addFeatureCategory("载具");		//2
	g_pSettings->addFeatureCategory("传送");	//3
	

	g_iFeature[FEATURE_P_TRUEGOD]			= g_pSettings->addFeature(0, -1, "无敌", feat_toggle, "trueGodMode");
	g_iFeature[FEATURE_P_GOD]				= g_pSettings->addFeature(0, -1, "半无敌", feat_toggle, "godMode");
	g_iFeature[FEATURE_P_WANTED]			= g_pSettings->addFeature(0, -1, "通缉等级", feat_slider, "wanted", 0.f, 5.f, .2f);
	g_iFeature[FEATURE_P_NEVERWANTED]		= g_pSettings->addFeature(0, -1, "永不通缉", feat_toggle, "neverWanted");
	g_iFeature[FEATURE_P_ANTINPC]			= g_pSettings->addFeature(0, -1, "杀死攻击NPC", feat_toggle, "antiNpc");
	g_iFeature[FEATURE_P_RUNSPD]			= g_pSettings->addFeature(0, -1, "奔跑速度", feat_slider, "runSpd", 1.f, 5.f);
	g_iFeature[FEATURE_P_SWIMSPD]			= g_pSettings->addFeature(0, -1, "游泳速度", feat_slider, "swimSpd", 1.f, 5.f);
	g_iFeature[FEATURE_P_SUPERJUMP]			= g_pSettings->addFeature(0, -1, "超级跳跃", feat_toggle, "superJump");
	g_iFeature[FEATURE_P_EXPLOSIVEMELEE]	= g_pSettings->addFeature(0, -1, "爆炸近战", feat_toggle, "explMelee");
	g_iFeature[FEATURE_P_NORAGDOLL]			= g_pSettings->addFeature(0, -1, "无布娃娃", feat_toggle, "noRagdoll");
	g_iFeature[FEATURE_P_STAMINA]			= g_pSettings->addFeature(0, -1, "无限耐力", feat_toggle, "infStam");

	g_iFeature[FEATURE_W_SPREAD]			= g_pSettings->addFeature(1, -1, "无扩散", feat_toggle, "noSpread");	
	g_iFeature[FEATURE_W_RECOIL]			= g_pSettings->addFeature(1, -1, "无后座", feat_toggle, "noRecoil");	
	g_iFeature[FEATURE_W_NORELOAD]			= g_pSettings->addFeature(1, -1, "无需换弹", feat_toggle, "noReload");
	g_iFeature[FEATURE_W_RELOAD]			= g_pSettings->addFeature(1, -1, "快速换弹", feat_slider, "quickReload", 1.f, 10.f);
	g_iFeature[FEATURE_W_DAMAGE]			= g_pSettings->addFeature(1, -1, "武器伤害系数", feat_slider, "bulletDamage", 1.f, 10.f);
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
	g_iFeature[FEATURE_V_BULLETPROOFTIRES]	= g_pSettings->addFeature(2, -1, "防爆轮胎", feat_toggle, "vehBulletproofTires");
	g_iFeature[FEATURE_V_SEATBELT]			= g_pSettings->addFeature(2, -1, "安全带", feat_toggle, "seatbelt");
	g_iFeature[FEATURE_V_DEFORMATION]		= g_pSettings->addFeature(2, -1, "变形系数", feat_slider, "vehDeform", 0.f, 1.f);
	g_iFeature[FEATURE_V_ACCELERATION]		= g_pSettings->addFeature(2, -1, "加速度", feat_slider, "vehAccel", 1.f, 10.f);
	g_iFeature[FEATURE_V_UPSHIFT]			= g_pSettings->addFeature(2, -1, "高速挡", feat_slider, "vehUpShift", 1.f, 25.f);
	g_iFeature[FEATURE_V_BRAKEFORCE]		= g_pSettings->addFeature(2, -1, "刹车制动力", feat_slider, "vehBrakeForce", 1.f, 10.f);
	g_iFeature[FEATURE_V_TRACTION]			= g_pSettings->addFeature(2, -1, "牵引力", feat_slider, "vehTraction", 1.f, 2.f);
	g_iFeature[FEATURE_V_GRAVITY]			= g_pSettings->addFeature(2, -1, "重力", feat_slider, "vehGravity", 0.f, 25.f);
	g_iFeature[FEATURE_V_SUSPENSION_FORCE]	= g_pSettings->addFeature(2, -1, "悬挂", feat_slider, "vehSuspensionForce", 0.f, 2.f);
	//g_iFeature[FEATURE_V_DISABLE_DOORS]		= g_pSettings->addFeature(2, -1, "Disable Doors", feat_toggle, "vehDisableDoors");
	//g_iFeature[FEATURE_V_INF_CAR_ALARM]		= g_pSettings->addFeature(2, -1, "Infinite Alarm", feat_toggle, "vehInfAlarm");

	g_pSettings->addFeature(3, -1, "导航点", feat_teleport, tp_waypoint);
	g_pSettings->addFeature(3, -1, "目标点", feat_teleport, tp_objective);

	int interior = g_pSettings->addFeature(3, -1, "室内 >>", feat_parent);
	g_pSettings->addFeature(-1, interior, "FIB Building Top", feat_teleport, tp_static, 136.0f, -750.f, 262.f);
	g_pSettings->addFeature(-1, interior, "Garment Factory", feat_teleport, tp_static, 712.716f, -962.906f, 30.6f);
	g_pSettings->addFeature(-1, interior, "Franklin's House", feat_teleport, tp_static, 7.119f, 536.615f, 176.2f);
	g_pSettings->addFeature(-1, interior, "Michael's House", feat_teleport, tp_static, -813.603f, 179.474f, 72.5f);
	g_pSettings->addFeature(-1, interior, "Trevor's House", feat_teleport, tp_static, 1972.610f, 3817.040f, 33.65f);
	g_pSettings->addFeature(-1, interior, "Aunt Denise's House", feat_teleport, tp_static, -14.380f, -1438.510f, 31.3f);
	g_pSettings->addFeature(-1, interior, "Floyd's House", feat_teleport, tp_static, -1151.770f, -1518.138f, 10.85f);
	g_pSettings->addFeature(-1, interior, "Lester's House", feat_teleport, tp_static, 1273.898f, -1719.304f, 54.8f);
	g_pSettings->addFeature(-1, interior, "Vanilla Unicorn Office", feat_teleport, tp_static, 97.271f, -1290.994f, 29.45f);
	g_pSettings->addFeature(-1, interior, "Bank Vault (Pacific Standard)", feat_teleport, tp_static, 255.85f, 217.f, 101.9f);
	g_pSettings->addFeature(-1, interior, "Comedy Club", feat_teleport, tp_static, 378.100f, -999.964f, -98.6f);
	g_pSettings->addFeature(-1, interior, "Humane Labs", feat_teleport, tp_static, 3614.394f, 3744.803f, 28.9f);
	g_pSettings->addFeature(-1, interior, "Humane Labs Tunnel", feat_teleport, tp_static, 3525.201f, 3709.625f, 21.2f);
	g_pSettings->addFeature(-1, interior, "IAA Office", feat_teleport, tp_static, 113.568f, -619.001f, 206.25f);
	g_pSettings->addFeature(-1, interior, "Torture Room", feat_teleport, tp_static, 142.746f, -2201.189f, 4.9f);
	g_pSettings->addFeature(-1, interior, "Fort Zancudo Tower", feat_teleport, tp_static, -2358.132f, 3249.754f, 101.65f);
	g_pSettings->addFeature(-1, interior, "Mine Shaft", feat_teleport, tp_static, -595.342f, 2086.008f, 131.6f);

	g_pSettings->addFeature(3, -1, "自定义保存点1", feat_teleport, "pos0", tp_saved);
	g_pSettings->addFeature(3, -1, "自定义保存点2", feat_teleport, "pos1", tp_saved);
	g_pSettings->addFeature(3, -1, "自定义保存点3", feat_teleport, "pos2", tp_saved);
	g_pSettings->addFeature(3, -1, "自定义保存点4", feat_teleport, "pos3", tp_saved);
	g_pSettings->addFeature(3, -1, "自定义保存点5", feat_teleport, "pos4", tp_saved);
	g_pSettings->addFeature(3, -1, "自定义保存点6", feat_teleport, "pos5", tp_saved);
	g_pSettings->addFeature(3, -1, "洛圣都改车王", feat_teleport, tp_static, -365.425f, -131.809f, -225.f);//38.9f);
	g_pSettings->addFeature(3, -1, "军事基地", feat_teleport, tp_static, -1336.f, -3044.f, -225.f);//14.15f);
	g_pSettings->addFeature(3, -1, "桑迪海岸机场", feat_teleport, tp_static, 1747.f, 3273.f, -225.f);//41.35f);
	g_pSettings->addFeature(3, -1, "千年山", feat_teleport, tp_static, 489.979f, 5587.527f, 794.3f);

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
	g_hWnd = CreateWindowEx(WS_EX_TOPMOST | WS_EX_TRANSPARENT | WS_EX_LAYERED,		//dwExStyle [in]
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
			g_pHack->godMode(g_pSettings->getFeature(g_iFeature[FEATURE_P_TRUEGOD]));
			g_pHack->noRagdoll(g_pSettings->getFeature(g_iFeature[FEATURE_P_NORAGDOLL]));
			g_pHack->seatbelt(g_pSettings->getFeature(g_iFeature[FEATURE_V_SEATBELT]));

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
				//g_pHack->vehicleDisableDoors(g_pSettings->getFeature(g_iFeature[FEATURE_V_DISABLE_DOORS]));			THIS ONLY WORKS CLIENT SIDE
				//g_pHack->vehicleInfAlarm(g_pSettings->getFeature(g_iFeature[FEATURE_V_INF_CAR_ALARM]));

				if(g_pHack->m_vehicle.loadHandling())
				{
					g_pHack->vehicleAccel(g_pSettings->getFeature(g_iFeature[FEATURE_V_ACCELERATION]));
					g_pHack->vehicleBrake(g_pSettings->getFeature(g_iFeature[FEATURE_V_BRAKEFORCE]));
					g_pHack->vehicleTraction(g_pSettings->getFeature(g_iFeature[FEATURE_V_TRACTION]));
					g_pHack->vehicleDeformation(g_pSettings->getFeature(g_iFeature[FEATURE_V_DEFORMATION]));
					g_pHack->vehicleUpShift(g_pSettings->getFeature(g_iFeature[FEATURE_V_UPSHIFT]));
					g_pHack->vehicleSuspensionForce(g_pSettings->getFeature(g_iFeature[FEATURE_V_SUSPENSION_FORCE]));
				}
			}

			if(!(btInit & INITPTR_INVALID_WEAPON) && g_pHack->m_weapon.loadWeapon())
			{
				g_pHack->noSpread(g_pSettings->getFeature(g_iFeature[FEATURE_W_SPREAD]));
				g_pHack->noRecoil(g_pSettings->getFeature(g_iFeature[FEATURE_W_RECOIL]));
				g_pHack->quickReload(g_pSettings->getFeature(g_iFeature[FEATURE_W_RELOAD]));
				g_pHack->bulletDamage(g_pSettings->getFeature(g_iFeature[FEATURE_W_DAMAGE]));
				g_pHack->weaponRange(g_pSettings->getFeature(g_iFeature[FEATURE_W_RANGE]));
				g_pHack->weaponSpin(g_pSettings->getFeature(g_iFeature[FEATURE_W_SPINUP]));
				g_pHack->bulletBatch(g_pSettings->getFeature(g_iFeature[FEATURE_W_BULLET_BATCH]));
				g_pHack->batchSpread(g_pSettings->getFeature(g_iFeature[FEATURE_W_BATCH_SPREAD]));
				g_pHack->muzzleVelocity(g_pSettings->getFeature(g_iFeature[FEATURE_W_MUZZLE_VELOCITY]));
				g_pHack->infAmmo(g_pSettings->getFeature(g_iFeature[FEATURE_W_AMMO]));
				g_pHack->noReload(g_pSettings->getFeature(g_iFeature[FEATURE_W_NORELOAD]));
			}
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