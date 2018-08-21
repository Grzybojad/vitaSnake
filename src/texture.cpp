#include "texture.hpp"

void Texture::loadTexture( const char *filename )
{
	texture = vita2d_load_PNG_file( filename );
}

void Texture::freeTexture()
{
	if( texture != NULL )
	{
		vita2d_free_texture( texture );
		texture = NULL;
	}	
}

void Texture::draw( float x, float y )
{
	vita2d_draw_texture( texture, x, y );
}