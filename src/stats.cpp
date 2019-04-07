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
    int statsHeight = 150;

	drawBackground();

    drawText_position( centeredX, SCREEN_WIDTH / 2, headerHeight, headerFontSize, "Extras" );

    drawStats( (SCREEN_WIDTH/2) - horizontalSpacing, statsHeight, std::to_string(applesEaten).c_str(), "Apples eaten" );
    drawStats( (SCREEN_WIDTH/2), statsHeight, timeToString(timePlayed).c_str(), "Time spent playing" );
    drawStats( (SCREEN_WIDTH/2) + horizontalSpacing, statsHeight, std::to_string(totalDeaths).c_str(), "Total deaths" );

	drawBackText();
}

void Stats::drawStats( float x, float y, std::string statData, std::string statName )
{
    int statsFontSize = 50;
    int descFontSize = 30;
    int verticalSpacing = 50;

    drawText_position( centeredX, x, y, statsFontSize, statData.c_str() );
    drawText_position( centeredX, x, y + verticalSpacing, descFontSize, statName.c_str() );
}