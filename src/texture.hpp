#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <math.h>

#include <vita2d.h>

class Texture
{
	public:
		void loadTexture( const char *filename );

		void draw( float x, float y );

		void fill_tile();

		void setClips( int i, int x, int y, int w, int h );

		void freeTexture();

		// Get texture dimensions
		int get_width();
		int get_height();

		vita2d_texture *texture;

		// Clipping positons for spritesheets
		struct SheetClips
		{
			int x;
			int y;
			int w;
			int h;
		};

		SheetClips clips[ 16 ];
};


#endif // TEXTURE_HPP