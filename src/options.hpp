#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>
#include <psp2/touch.h>
#include <psp2/io/stat.h> 
#include <vita2d.h>

#include "collectable.hpp"
#include "global.hpp"

class OptionsMenu
{
public:
	OptionsMenu();

	struct Option
	{
		int id;
		const char *name;
		float name_x, name_y;
		float slct_x, slct_y;
		float desc_x, desc_y;
		int selected;
		int nr_selectables;
	};
	static const int MENU_ITEMS = 5;
	Option option[ MENU_ITEMS ];
	
	int cursor;

	void selectUp();
	void selectDown();
	void menuNav();

	void changeSelectable( Option & option );
	bool touchSelect( Option option );

	void renderOptions();
	void renderCursor( Option option, int text_width );

	void updateNrTextures();

	void writeSettings();
	void readSettings();

private:
	SceTouchData touch_old[ SCE_TOUCH_PORT_MAX_NUM ];
	SceTouchData touch[ SCE_TOUCH_PORT_MAX_NUM ];

	static const int ITEM_X = 400;
	static const int DESC_X = 550;

	int drawSelectable( const char *name, int y );
	void drawPlayerModel();

	// A collectable object for displaying the selected apple style
	Collectable appleModel;
};


#endif // OPTIONS_HPP
