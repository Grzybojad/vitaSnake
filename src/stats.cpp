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
    int statsFontSize = 50;
    int descFontSize = 30;

    int spacing = 340;

    int headerHeight = 50;
    int statsHeight = 180;
    int descHeight = 230;

	drawBackground();

	int text_width;
	text_width = vita2d_font_text_width( gFont[ (int)(headerFontSize * FONT_SCALE) ], (int)(headerFontSize * FONT_SCALE), "Extras" );
	vita2d_font_draw_text( gFont[ (int)(headerFontSize * FONT_SCALE) ], ( ( SCREEN_WIDTH - text_width ) / 2 ), headerHeight, MAIN_FONT_COLOR, (int)(headerFontSize * FONT_SCALE), "Extras" );

    text_width = vita2d_font_text_width( gFont[ (int)(statsFontSize * FONT_SCALE) ], (int)(statsFontSize * FONT_SCALE), std::to_string(applesEaten).c_str() );
	vita2d_font_draw_textf( gFont[ (int)(statsFontSize * FONT_SCALE) ], (SCREEN_WIDTH/2)-spacing-(text_width/2), statsHeight, MAIN_FONT_COLOR, (int)(statsFontSize * FONT_SCALE), "%i", applesEaten );
    text_width = vita2d_font_text_width( gFont[ (int)(descFontSize * FONT_SCALE) ], (int)(descFontSize * FONT_SCALE), "Apples eaten" );
    vita2d_font_draw_text( gFont[ (int)(descFontSize * FONT_SCALE) ], (SCREEN_WIDTH/2)-spacing-(text_width/2), descHeight, MAIN_FONT_COLOR, (int)(descFontSize * FONT_SCALE), "Apples eaten" );

    text_width = vita2d_font_text_width( gFont[ (int)(statsFontSize * FONT_SCALE) ], (int)(statsFontSize * FONT_SCALE), timeToString(timePlayed).c_str() );
	vita2d_font_draw_textf( gFont[ (int)(statsFontSize * FONT_SCALE) ], (SCREEN_WIDTH/2)-(text_width/2), statsHeight, MAIN_FONT_COLOR, (int)(statsFontSize * FONT_SCALE), "%s", timeToString(timePlayed).c_str() );
    text_width = vita2d_font_text_width( gFont[ (int)(descFontSize * FONT_SCALE) ], (int)(descFontSize * FONT_SCALE), "Time spent playing" );
    vita2d_font_draw_text( gFont[ (int)(descFontSize * FONT_SCALE) ], (SCREEN_WIDTH/2)-(text_width/2), descHeight, MAIN_FONT_COLOR, (int)(descFontSize * FONT_SCALE), "Time spent playing" );

    text_width = vita2d_font_text_width( gFont[ (int)(statsFontSize * FONT_SCALE) ], (int)(statsFontSize * FONT_SCALE), std::to_string(totalDeaths).c_str() );
	vita2d_font_draw_textf( gFont[ (int)(statsFontSize * FONT_SCALE) ], (SCREEN_WIDTH/2)+spacing-(text_width/2), statsHeight, MAIN_FONT_COLOR, (int)(statsFontSize * FONT_SCALE), "%i", totalDeaths );
    text_width = vita2d_font_text_width( gFont[ (int)(descFontSize * FONT_SCALE) ], (int)(descFontSize * FONT_SCALE), "Total deaths" );
    vita2d_font_draw_text( gFont[ (int)(descFontSize * FONT_SCALE) ], (SCREEN_WIDTH/2)+spacing-(text_width/2), descHeight, MAIN_FONT_COLOR, (int)(descFontSize * FONT_SCALE), "Total deaths" );

	drawBackText();
}