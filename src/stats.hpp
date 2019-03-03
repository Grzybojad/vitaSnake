#ifndef STATS_HPP
#define STATS_HPP

#include <fstream>

class Stats
{
    public:
        Stats();

        void readStats();
        void saveStats();

        int applesEaten;
        int timePlayed;
        int totalDeaths;
};

#endif // STATS_HPP