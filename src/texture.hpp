#ifndef TEXTURE_HPP
#define TEXTURE_HPP

#include <vita2d.h>

#include "texture.hpp"

class Texture
{
public:
	void loadTexture( const char *filename );

	void draw( float x, float y );

	void freeTexture();

	vita2d_texture *texture;	
};

#endif // TEXTURE_HPP