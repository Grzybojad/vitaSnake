#include "texture.hpp"

// Load texture from file
void Texture::loadTexture( const char *filename )
{
	texture = vita2d_load_PNG_file( filename );
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

// Draw the texture in the specified position
void Texture::draw( float x, float y )
{
	vita2d_draw_texture( texture, x, y );
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