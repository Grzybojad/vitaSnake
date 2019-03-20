#ifndef STATS_HPP
#define STATS_HPP

#include <vita2d.h>

#include <fstream>
#include <string>
#include "global.hpp"

class Stats
{
    public:
        Stats();

        void readStats();
        void saveStats();

        void renderStatsPage();

        int applesEaten;
        int timePlayed;
        int totalDeaths;
};

#endif // STATS_HPP