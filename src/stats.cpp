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