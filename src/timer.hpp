#ifndef TIMER_HPP
#define TIMER_HPP

#include <psp2/kernel/processmgr.h>
#include <vita2d.h>

#include "global.hpp" 

class Timer
{
    private:
        int startTicks;
        int pausedTicks;

        bool paused;
        bool started;

    public:
        Timer();

        void start();
        void stop();
        void pause();
        void unpause();

        int get_ticks();

        bool is_started();
        bool is_paused();

        // Draw a timer at the top of the screen
        void drawTime();

        void drawCountdown( int timeLimit );
};

#endif // TIMER_HPP