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
#ifndef SETTINGS_H
#define SETTINGS_H

#define MAX_SAVEDPOS					0x06		//max saved positions (to teleport to)
#define MAX_PROPERTIES					0x60		//max iniparser properties
#define MAX_KEYS						0x10		//max indexed keys (keybinds)
#define MAX_MENU_TABS					0x08		//max amount of overlay menu tabs
#define MAX_MENU_FEATURES				0xFFF		//max amount of features
#define MAX_MENU_FEATURES_DISPLAYED		0x14		//max features displayed
#define MENU_FEATURE_SCROLL_PADDING		0x02
#define NAME_BUFFER_SIZE				0x20

#define FFB_CATEGORY	1 << 0
#define FFB_PARENT		1 << 1

//Required keys, loaded in settings::settings
#define keyExit			0x0
#define keyMenu			0x1
#define keyMenuUp		0x2
#define keyMenuDown		0x3
#define keyMenuLeft		0x4
#define keyMenuRight	0x5
#define keyMenuSelect	0x6
#define keyMenuTabNext	0x7
#define keyMenuTabPrev	0x8
#define keyMenuSave		0x9
#define keyHotTeleport	0xA
#define keyHotWanted	0xB
#define keyHotHealth	0xC
#define keyHotAmmo		0xD
#define keyMenuBack		0xE

class hack;

class featCat
{
	public:
		int			m_iId,
					m_iActiveFeatureRet = 0,
					m_iDisplayOffsetRet = 0;
		std::wstring	m_szName;
};

enum featType	{ feat_btn, feat_toggle, feat_slider, feat_teleport, feat_parent };
enum teleType	{ tp_saved, tp_waypoint, tp_static, tp_objective };

class feat
{
	public:
		int			m_iId,
					m_iCat,
					m_iParent;
		featType	m_type;
		std::wstring	m_szName;
		std::string	m_szIniKey;
		bool		m_bOn		= false,
					m_bRestored	= true;

						feat();
		virtual			~feat();
		virtual	void	toggle();
		virtual void	inc();
		virtual void	dec();
};

template<typename... Args>
class featBtn : public feat
{
	public:
		std::function<void()> m_Action;

		featBtn(void(hack::* const func)(Args...), Args... args)
		{
			m_Action = [=]()
			{
				std::thread t([=]()
				{
					std::lock_guard<std::mutex> lock(g_pHack->m_mutex);
					(g_pHack->*func)(args...);
				});
				t.detach();
			};
		}
		~featBtn() {};

		void toggle()
		{
			if (m_Action)
				std::invoke(m_Action);
		}
};

class featSlider : public feat
{
	public:
		float		m_fValue,
					m_fMin,
					m_fMax,
					m_fMod = .10f;

				featSlider();
				~featSlider();
		void	inc();
		void	dec();
};

class featTeleport : public feat
{
	public:
		teleType	m_tpType;
		v3			m_v3Pos;

				featTeleport();
				~featTeleport();
		void	toggle();
};

class featParent : public feat
{
	public:
		int		m_iActiveFeatureRet = 0,
				m_iDisplayOffsetRet = 0;

				featParent();
				~featParent();
		void	toggle();
};


typedef struct
{
    std::string    key,
                   value;
    int            section;
} iniProperty;

class iniParser
{
	public:
		std::string	m_szFile;

					iniParser();
					iniParser(std::string szFile);
					~iniParser();

		void		read();
		void		write();

		int			findKey(std::string szKey, std::string szSection = "");
		int			createKey(std::string szKey, std::string szSection = "");


		template	<typename rT>
		rT			getValue(std::string szKey, std::string szSection = "")
		{
			rT		r{};
			int		i	= this->findKey(szKey, szSection);
			if(i == -1 || (szSection != "" && (m_key[i].section < 0 || m_section[m_key[i].section] != szSection)))
				return r;
			std::stringstream	ss(m_key[i].value);
			ss	>> r;
			return r;
		}

		template	<>
		std::string	getValue(std::string szKey, std::string szSection)
		{
			std::string		r{};
			int				i	= this->findKey(szKey, szSection);
			if(i == -1 || (szSection != "" && (m_key[i].section < 0 || m_section[m_key[i].section] != szSection)))
				return r;
			return m_key[i].value;
		}

		template	<typename wT>
		bool		setValue(std::string szKey, wT value, std::string szSection = "")
		{
			int i	= findKey(szKey, szSection);
			if(i == -1 || (szSection != "" && (m_key[i].section < 0 || m_section[m_key[i].section] != szSection)))
				i	= createKey(szKey, szSection);
			m_key[i].value = std::to_string(value);
			return true;
		}
	protected:
		std::vector<std::string>	m_section;
		std::vector<iniProperty>	m_key;
};

class settings
{
	public:
		int			m_iKeys[MAX_KEYS];
		iniParser	m_iniParser;
			
					settings();
					~settings();

		void		toggleMenu();
		bool		isMenuActive();
		void		menuUp();		//prev feature
		void		menuDown();		//next feature
		void		menuLeft();		//float value down
		void		menuRight();	//float value up
		void		menuSelect();	//toggle feature/teleport or w/e
		void		menuBack();
		void		menuTabLeft();	//prev tab
		void		menuTabRight();	//next tab

		int			addFeatureCategory(std::wstring name);
		int			getFeatureCategoryCount();
		featCat*	getFeatureCategory(int id);
		int			setActiveCat(int);
		int			getActiveCat();
		bool		fillFeatureCurBuffer(int i, BYTE flags);

		int			addFeature(int cat, int parent, std::wstring name, featType type);
		int			addFeature(int cat, int parent, std::wstring name, featType type, std::string iniKey);
		int			addFeature(int cat, int parent, std::wstring name, featType type, std::string iniKey, float min, float max);
		int			addFeature(int cat, int parent, std::wstring name, featType type, std::string iniKey, float min, float max, float mod);
		int			addFeature(int cat, int parent, std::wstring name, featType type, std::string iniKey, teleType tpType);
		int			addFeature(int cat, int parent, std::wstring name, featType type, teleType tpType);
		int			addFeature(int cat, int parent, std::wstring name, featType type, teleType tpType, float x, float y, float z);

		template	<typename... Args>
		int			addFeature(int cat, int parent, std::wstring name, featType type, void(hack::* const func)(Args...), Args... args)
		{
			this->m_pCurrentFeatBtn = new featBtn<Args...>(func, args...);
			int id = this->addFeature(cat, parent, name, type);
			if (id < 0)
				return id;

			return id;
		}

		int			updataFeature(int id, int cat, int parent, std::wstring name, featType type);
		int			updataFeature(int id, int cat, int parent, std::wstring name, featType type, teleType tpType, float x, float y, float z);
		int			getFeatureCurCount();
		feat*		getFeatureCur(int i);
		int			getFeatureCount();
		feat*		getFeature(int id);
		int			getActiveFeature();
		int			getDisplayOffset();

		bool		getFeatureCurLock();
		bool		lockFeatureCur();
		bool		unlockFeatureCur();

	protected:
		feat*		m_pCurrentFeatBtn;
		featCat*	m_pFeatureCat[MAX_MENU_TABS];
		int			m_nFeatureCat = 0;
		int			m_iActiveCat = 0;			//index for featureParent [should be the same as id]
		feat*		m_pFeature[MAX_MENU_FEATURES];
		int			m_nFeature = 0;						//total amount of features
		int			m_iActiveFeature = 0;				//index for featureCur [DOES NOT HOLD ID!]
		feat*		m_pFeatureCur[MAX_MENU_FEATURES];	//list of features from current category
		bool		m_bFeatureCurLock = false;
		int			m_nFeatureCur	= 0;			//amount of features in current tab
		bool		m_bMenuActive = true;
		int			m_iFeatureCurDisplayOffset = 0;
};

extern settings*		g_pSettings;

#endif