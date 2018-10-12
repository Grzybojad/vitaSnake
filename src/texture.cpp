#include "texture.hpp"

// Load texture from file
void Texture::loadTexture( const char *filename )
{
	texture = vita2d_load_PNG_file( filename );
}

// Draw the texture in the specified position
void Texture::draw( float x, float y )
{
	vita2d_draw_texture( texture, x, y );
}

// Fill the screen by with texture tiles
void Texture::fill_tile()
{
	int rows = 544 / get_height()*2 + 1;
	int columns = 960 / get_width()*2 + 1;

	for( int i = 0; i <= rows; ++i )
	{
		for( int j = 0; j <= columns; ++j )
		{
			vita2d_draw_texture_scale( texture, i * get_width(), j * get_height(), 2.0f, 2.0f );
		}
	}
}

// Set clipping rectangles for sprite sheets
void Texture::setClips( int i, int x, int y, int w, int h )
{
	this->clips[i].x = x;
	this->clips[i].y = y;
	this->clips[i].w = w;
	this->clips[i].h = h;
}

// Free the texture from memory
void Texture::freeTexture()
{
	if( texture != NULL )
	{
		vita2d_free_texture( texture );
		texture = NULL;
	}	
}

// Get texture width
int Texture::get_width()
{
	return vita2d_texture_get_width( texture );
}

// Get texture height
int Texture::get_height()
{
	return vita2d_texture_get_height( texture );
}