#include "stats.hpp"

Stats::Stats()
{
    applesEaten = 0;
    timePlayed = 0;
    totalDeaths = 0;
}

void Stats::readStats()
{
    std::ifstream stats;
	stats.open( "ux0:/data/vitaSnake/stats.txt", std::ifstream::in );
		
	stats >> applesEaten;
    stats >> timePlayed;
    stats >> totalDeaths;

    stats.close();
}

void Stats::saveStats()
{
    std::ofstream stats;
	stats.open( "ux0:/data/vitaSnake/stats.txt" );

	stats << applesEaten << "\n";
    stats << timePlayed << "\n";
    stats << totalDeaths;

	stats.close();
}

void Stats::renderStatsPage()
{
    int headerFontSize = 50;
    int horizontalSpacing = 340;
    int headerHeight = 50;
    int statsHeight = 180;

	drawBackground();

	int text_width;
	text_width = vita2d_font_text_width( gFont[ (int)(headerFontSize * FONT_SCALE) ], (int)(headerFontSize * FONT_SCALE), "Extras" );
	vita2d_font_draw_text( gFont[ (int)(headerFontSize * FONT_SCALE) ], ( ( SCREEN_WIDTH - text_width ) / 2 ), headerHeight, MAIN_FONT_COLOR, (int)(headerFontSize * FONT_SCALE), "Extras" );

    drawStats( (SCREEN_WIDTH/2) - horizontalSpacing, statsHeight, std::to_string(applesEaten).c_str(), "Apples eaten" );
    drawStats( (SCREEN_WIDTH/2), statsHeight, timeToString(timePlayed).c_str(), "Time spent playing" );
    drawStats( (SCREEN_WIDTH/2) + horizontalSpacing, statsHeight, std::to_string(totalDeaths).c_str(), "Total deaths" );

    extrasMenu.renderMenu();
    extrasMenu.menuNav();

	drawBackText();
}

void Stats::drawStats( float x, float y, std::string statData, std::string statName )
{
    int statsFontSize = 50;
    int descFontSize = 30;
    int verticalSpacing = 50;
    int text_width;

    text_width = vita2d_font_text_width( gFont[ (int)(statsFontSize * FONT_SCALE) ], (int)(statsFontSize * FONT_SCALE), statData.c_str() );
	vita2d_font_draw_textf( gFont[ (int)(statsFontSize * FONT_SCALE) ], x - (text_width/2), y, MAIN_FONT_COLOR, (int)(statsFontSize * FONT_SCALE), statData.c_str() );
    text_width = vita2d_font_text_width( gFont[ (int)(descFontSize * FONT_SCALE) ], (int)(descFontSize * FONT_SCALE), statName.c_str() );
    vita2d_font_draw_text( gFont[ (int)(descFontSize * FONT_SCALE) ], x - (text_width/2), y + verticalSpacing, MAIN_FONT_COLOR, (int)(descFontSize * FONT_SCALE), statName.c_str() );
}