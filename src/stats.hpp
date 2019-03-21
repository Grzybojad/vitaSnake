#ifndef STATS_HPP
#define STATS_HPP

#include <vita2d.h>

#include <fstream>
#include <string>
#include "menu.hpp"
#include "global.hpp"

class Stats
{
    public:
        Stats();

        void readStats();
        void saveStats();

        void renderStatsPage();
        void drawStats( float x, float y, std::string statData, std::string statName );

        int applesEaten;
        int timePlayed;
        int totalDeaths;
    
    private:
        ExtrasMenu extrasMenu;
};

#endif // STATS_HPP