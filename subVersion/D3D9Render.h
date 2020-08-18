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
#ifndef D3D9_RENDER
#define D3D9_RENDER

#define FONT_BUFFER_SIZE	0x10									//max amount of fonts
#define VERTEX_FORMAT		(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)		//custom vertex format

//Layout defines
#define LAYOUT_PADDING_TOP	50
#define LAYOUT_PADDING_LEFT	50

#define LAYOUT_BORDER_SIZE		2
#define LAYOUT_ELEMENT_WIDTH	300
#define LAYOUT_ELEMENT_HEIGHT	25

#define LAYOUT_MB_WIDTH			300
#define LAYOUT_MB_HEIGHT		100

#define LAYOUT_SCROLLBAR_WIDTH	0x08
#define LAYOUT_SCROLLBAR_HEIGHT	0x40

#define LAYOUT_COLOR_BACKGROUND		D3DCOLOR_ARGB(0xff, 0x38, 0x78, 0xe2)
#define LAYOUT_COLOR_BORDER			D3DCOLOR_ARGB(0xff, 1, 1, 1)
#define LAYOUT_COLOR_TEXT			D3DCOLOR_ARGB(0xff, 1, 1, 1)
#define LAYOUT_COLOR_VALUE_TEXT		D3DCOLOR_ARGB(0xff, 255,255,255)
#define LAYOUT_COLOR_SLIDER_BG		D3DCOLOR_ARGB(0xff, 0x28, 0x28, 0x28)
#define LAYOUT_COLOR_SLIDER_BTN		D3DCOLOR_ARGB(0xff, 0x73, 0x73, 0x73)
#define LAYOUT_COLOR_ACTIVE_BG		D3DCOLOR_ARGB(0xff, 0x85, 0xac, 0xed)
#define LAYOUT_COLOR_ACTIVE_BORDER	D3DCOLOR_ARGB(0xff, 1, 1, 1)
#define LAYOUT_COLOR_SELECTED		D3DCOLOR_ARGB(0xff, 0xff, 0xff, 0xff)


struct Vertex
{
    FLOAT x, y, z, rhw;    // from the D3DFVF_XYZRHW flag
    DWORD color;    // from the D3DFVF_DIFFUSE flag
};

class D3D9Render
{
	public:
		struct scrnVars
		{
			int		w = 800,		//screen width
					h = 600,
					x = 0,
					y = 0;		//screen height
		} m_screen;

		std::wstring		m_szWindowTitle;
		bool			m_bMBShowing;
		std::wstring		m_sTitle;
		std::wstring		m_sDetail;

				D3D9Render	();
				~D3D9Render	();

		bool	init		(HWND hWnd);
		bool	render		();
		bool	createFont	(char *font, int size, bool bold, bool italic);
		void	releaseFont	();
		bool	getViewport	();
		void	showMessageBox(std::wstring title, std::wstring detail);

		void	drawBox			(int x, int y, int w, int h, D3DCOLOR color);
		void	drawBoxInline	(int x, int y, int w, int h, int size, D3DCOLOR color);
		void	drawBoxBorder	(int x, int y, int w, int h, int borderSize, D3DCOLOR color, D3DCOLOR borderColor);
		void	drawText		(std::wstring str, int x, int y, int font, D3DCOLOR color);
		void	drawText		(std::wstring str, int x, int y, int w, int h, int font, D3DCOLOR color, DWORD flags = NULL);
	protected:

		LPDIRECT3D9				m_pD3d;			// the pointer to Direct3D interface
		LPDIRECT3DDEVICE9		m_pD3dDev;		// the pointer to the device
		LPDIRECT3DVERTEXBUFFER9	m_pVertexBuffer; //pointer to the vertex buffer interface
		LPD3DXFONT				m_pFont[FONT_BUFFER_SIZE];
		int						m_nFont;
		D3DPRESENT_PARAMETERS	m_d3dParam;				//d3d9 device params
};

extern D3D9Render*	g_pD3D9Render;

#endif