#include <string.h>
#include <cmath>

#include <psp2/ctrl.h>
#include <psp2/kernel/processmgr.h>

#include <vita2d.h>

#include "player.hpp"
#include "global.hpp"
#include "collectable.hpp"

#define DEBUG false


int main()
{
	// Inputs
	SceCtrlData pad;

	// PGF text
	vita2d_pgf *pgf;

	// Set sampling mode to analog, so that the analog sticks return proper values
	sceCtrlSetSamplingMode( SCE_CTRL_MODE_ANALOG );

	vita2d_init();
	vita2d_set_clear_color( RGBA8( 0x00, 0x00, 0x00, 0xFF ) );

	pgf = vita2d_load_default_pgf();

	memset( &pad, 0, sizeof( pad ) );

	bool showDebug = false;
	bool selectPressed = false;

	// Nr of snake parts (including the head)
	static int SNAKE_LENGTH = 3;
	// This is a buffer for the length of the snake, any longer will crash the game
	Player snakePart[ 255 ];

	// Set snake textures
	snakePart[0].setTexture( "app0:/img/head.png" );
	for( int part = 1; part < SNAKE_LENGTH-1; ++part )
	{
		snakePart[ part ].setTexture( "app0:/img/body.png" );
	}
	snakePart[ SNAKE_LENGTH-1 ].setTexture( "app0:/img/tail.png" );

	// Collectable apple
	Collectable apple;


	// Main game loop
	while ( true ) {
		sceCtrlPeekBufferPositive( 0, &pad, 1 );

		// Close the program if the user presses START
		if( pad.buttons & SCE_CTRL_START )
			break;
		
		// player controls
		snakePart[0].handleInput();

		// Calcule player position
		snakePart[0].move();

		// Check collectable collisions
		if( apple.checkCollision(snakePart[0]) )
		{
			apple.collect();

			snakePart[ SNAKE_LENGTH-1 ].setTexture( "app0:/img/body.png" );
			SNAKE_LENGTH++;
			snakePart[ SNAKE_LENGTH-1 ].setTexture( "app0:/img/tail.png" );
		}

		// Move the following parts
		for( int part = 1; part < SNAKE_LENGTH; ++part )
			snakePart[ part ].follow( snakePart[ part - 1 ] );

		
		// Start rendering
		vita2d_start_drawing();
		vita2d_clear_screen();

		// Check snake collisions
		/*
		for( int part = 4; part < SNAKE_LENGTH; ++part )
		{
			if( snakePart[0].checkCollision( snakePart[part] ) )
				vita2d_pgf_draw_text( pgf, 700, 60, RGBA8( 255, 0, 0, 255 ), 1.0f, "COLLISION!" );
		}
		*/

		// Draw snake
		for( int part = 0; part < SNAKE_LENGTH; ++part )
		{
			snakePart[ part ].render();
		}

		// Draw collectable
		apple.render();

		// Draw the score counter
		apple.renderScore();

		// Debug 
		if( DEBUG )
		{
			for( int part = 4; part < SNAKE_LENGTH; ++part )
			{
				vita2d_pgf_draw_textf( pgf, 700, 30*part, RGBA8(0,255,0,255), 1.0f, "%d distance: %f.2", part, 
					sqrt( pow( ( snakePart[part].get_xPos() - snakePart[0].get_xPos() ), 2) + pow( ( snakePart[part].get_yPos() - snakePart[0].get_yPos() ), 2 ) ) );
			}
		}

		vita2d_end_drawing();
		vita2d_swap_buffers();
	}

	vita2d_fini();
	vita2d_free_pgf( pgf );

	// Destroy snake textures
	for( int part = 0; part < SNAKE_LENGTH; ++part )
	{
		snakePart[ part ].destroyTexture();
	}
	// Destroy collectable texture
	apple.destroyTexture();
	

	sceKernelExitProcess( 0 );
	return 0;
}
