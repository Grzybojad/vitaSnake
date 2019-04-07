#include "timer.hpp"

Timer::Timer()
{
    startTicks = 0;
    pausedTicks = 0;

    paused = false;
    started = false;
}

void Timer::start()
{
    startTicks = sceKernelGetProcessTimeWide();

    paused = false;
    started = true;
}

void Timer::stop()
{
    paused = false;
    started = false;
}

void Timer::pause()
{
    if( started && !paused )
    {
        paused = true;

        pausedTicks = sceKernelGetProcessTimeWide() - startTicks;
    }
}

void Timer::unpause()
{
    if( paused )
    {
        paused = false;

        startTicks = sceKernelGetProcessTimeWide() - pausedTicks;

        pausedTicks = 0;
    }
}

int Timer::get_ticks()
{
    if( started )
        if( paused )
            return pausedTicks;
        else
            return sceKernelGetProcessTimeWide() - startTicks;

    return 0;
}

bool Timer::is_started()
{
    return started;
}

bool Timer::is_paused()
{
    return paused;
}

void Timer::drawTime()
{
    int font_size = 45;

    float time = (float)get_ticks() / (float)1000000;

    drawTextf_position( centeredX, SCREEN_WIDTH/2, 35, font_size, "%.*f", 2, time );
}

void Timer::drawCountdown( int timeLimit )
{
    int font_size = 45;

    float time = ( timeLimit - (float)get_ticks() ) / (float)1000000;

    // Prevent drawing negative time
    if( time > 0)
        drawTextf_position( centeredX, SCREEN_WIDTH/2, 35, font_size, "%.*f", 2, time );
    else
        drawText_position( centeredX, SCREEN_WIDTH/2, 35, font_size, "0.00" );
}