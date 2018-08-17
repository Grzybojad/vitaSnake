#ifndef MENU_HPP
#define MENU_HPP

#include <psp2/kernel/processmgr.h>
#include <vita2d.h>

#include "global.hpp"

class Menu
{
	public:
		Menu();

		struct MenuItem
		{
			itemName name;
			float x;
			float y;
		};

		itemName cursor;

		MenuItem item[ 3 ];

		void selectUp();
		void selectDown();
		void renderBackground();
		void renderCursor( MenuItem item );

	private:
		vita2d_texture *menuTexture;
		vita2d_texture *cursorTexture;
};

#endif // MENU_HPP