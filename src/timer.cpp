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

    int text_width;
    if( time < 10 )
        text_width = vita2d_font_text_width( gFont[ (int)(font_size * FONT_SCALE) ], (int)(font_size * FONT_SCALE), "0.00" );
    else if( time < 100 )
        text_width = vita2d_font_text_width( gFont[ (int)(font_size * FONT_SCALE) ], (int)(font_size * FONT_SCALE), "00.00" );
    else
        text_width = vita2d_font_text_width( gFont[ (int)(font_size * FONT_SCALE) ], (int)(font_size * FONT_SCALE), "000.00" );
    
    vita2d_font_draw_textf( gFont[ (int)(font_size * FONT_SCALE) ], (SCREEN_WIDTH - text_width)/2, 35, MAIN_FONT_COLOR, (int)(font_size * FONT_SCALE), "%.*f", 2, time );
}

void Timer::drawCountdown( int timeLimit )
{
    int font_size = 45;

    float time = ( timeLimit - (float)get_ticks() ) / (float)1000000;

    int text_width;
    if( time < 10 )
        text_width = vita2d_font_text_width( gFont[ (int)(font_size * FONT_SCALE) ], (int)(font_size * FONT_SCALE), "0.00" );
    else if( time < 100 )
        text_width = vita2d_font_text_width( gFont[ (int)(font_size * FONT_SCALE) ], (int)(font_size * FONT_SCALE), "00.00" );
    else
        text_width = vita2d_font_text_width( gFont[ (int)(font_size * FONT_SCALE) ], (int)(font_size * FONT_SCALE), "000.00" );

    // Prevent drawing negative time
    if( time > 0)
        vita2d_font_draw_textf( gFont[ (int)(font_size * FONT_SCALE) ], (SCREEN_WIDTH - text_width)/2, 35, MAIN_FONT_COLOR, (int)(font_size * FONT_SCALE), "%.*f", 2, time );
    else
        vita2d_font_draw_text( gFont[ (int)(font_size * FONT_SCALE) ], (SCREEN_WIDTH - text_width)/2, 35, MAIN_FONT_COLOR, (int)(font_size * FONT_SCALE), "0.00" );
}