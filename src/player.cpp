#include "player.hpp"

// Initialize variables
Player::Player()
{
	// Initialize position
	xPos = SCREEN_WIDTH / 6;
	yPos = SCREEN_HEIGHT / 2;

	// Set starting rotation to 90 degrees (pi rad / 2)
	rotation = M_PI / 2;

	isClose = false;

	memset( &pad, 0, sizeof( pad ) );

	// Set sampling mode to analog, so that the analog sticks return proper values
	sceCtrlSetSamplingMode( SCE_CTRL_MODE_ANALOG );
}

// Set difficulty
void Player::setDifficulty()
{
	switch( GAME_DIFFICULTY )
	{
		case DifficultyMenu::classic:
			PLAYER_SET_SPEED = 5.0f;
			PLAYER_SET_ROTATION_SPEED = 0.07f;
			break;
		case DifficultyMenu::hardcore:
			PLAYER_SET_SPEED = 6.0f;
			PLAYER_SET_ROTATION_SPEED = 0.08f;
			break;
	}

	speed = -PLAYER_SET_SPEED;
}

// Handle input
void Player::handleInput()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	// Calculate rotation
	// DPAD controls
	if( pad.buttons & SCE_CTRL_LEFT )
		rotation -= PLAYER_SET_ROTATION_SPEED;
	else if( pad.buttons & SCE_CTRL_RIGHT )
		rotation += PLAYER_SET_ROTATION_SPEED;

	// Analog stick controls with deadzone
	float analogInput = (float)( ( pad.lx - 128.0f ) / 128.0f );
	if( analogInput > ANALOG_DEADZONE || analogInput < -ANALOG_DEADZONE )
	{
		if( CONTROL_STYLE == 0 )
		{
			rotation += analogInput * PLAYER_SET_ROTATION_SPEED;
		}
		else if( CONTROL_STYLE == 1 )
		{
			float analogX = pad.lx - 128;
			float analogY = pad.ly - 128;

			float analogAngle = atan2( analogY, analogX ) + ( M_PI / 2 );
			if( analogAngle < 0 ) analogAngle += M_PI * 2;	

			if( rotation < M_PI )
			{
				if( analogAngle > rotation && analogAngle < ( rotation + M_PI ) )
					rotation += PLAYER_SET_ROTATION_SPEED;
				else
					rotation -= PLAYER_SET_ROTATION_SPEED;
			}
			else if( rotation > M_PI )
			{
				if( analogAngle < ( rotation - M_PI ) )
					analogAngle += 2 * M_PI;

				if( analogAngle > rotation && analogAngle < ( rotation + M_PI ) )
					rotation += PLAYER_SET_ROTATION_SPEED;
				else
					rotation -= PLAYER_SET_ROTATION_SPEED;
			}
		}
	}

	// Boost snake speed with X
	if( pad.buttons & SCE_CTRL_CROSS )
		speed = 1.5f * -PLAYER_SET_SPEED;
	else
		speed = -PLAYER_SET_SPEED;
}

// Move the player
void Player::move()
{
	if( rotation < 0 )
		rotation += ( 2 * M_PI );
	else if( rotation > ( 2 * M_PI ) )
		rotation = rotation - ( 2 * M_PI );

	// Calcule player position
	xPos -= sin( rotation ) * speed;
	yPos += cos( rotation ) * speed;

	// Trap the player inside the screen
	if( xPos < PLAYER_HEIGHT * 0.45 )
		xPos = PLAYER_HEIGHT * 0.45;
	else if( xPos > SCREEN_WIDTH - PLAYER_HEIGHT * 0.45 )
		xPos = SCREEN_WIDTH - PLAYER_HEIGHT * 0.45;
	if( yPos < PLAYER_HEIGHT * 0.45 )
		yPos = PLAYER_HEIGHT * 0.45;
	else if( yPos > SCREEN_HEIGHT - PLAYER_HEIGHT * 0.45 )
		yPos = SCREEN_HEIGHT - PLAYER_HEIGHT * 0.45;
}

// Move the body parts
void Player::follow( Player part )
{
	if( rotation < 0 )
		rotation += ( 2 * M_PI );
	else if( rotation > ( 2 * M_PI ) )
		rotation = rotation - ( 2 * M_PI );

	// Calculate distance to previous part
	float distance = sqrt( pow( ( part.get_xPos() - xPos ), 2) + pow( ( part.get_yPos() - yPos ), 2 ) );

	// Rotate to look at previous part
	rotation = atan2( part.get_yPos() - yPos, part.get_xPos() - xPos ) + ( M_PI / 2 );

	// If the distance is too far, move to the correct distance
	if( distance > FOLLOW_DISTANCE )
	{
		xPos += sin( rotation ) * ( distance - FOLLOW_DISTANCE );
		yPos -= cos( rotation ) * ( distance - FOLLOW_DISTANCE );
	}

	// Trap the part inside the screen
	if( xPos < 0 )
		xPos = 0;
	else if( xPos > SCREEN_WIDTH )
		xPos = SCREEN_WIDTH;
	if( yPos < 0 )
		yPos = 0;
	else if( yPos > SCREEN_HEIGHT )
		yPos = SCREEN_HEIGHT;
}

// Tail collision
bool Player::checkCollision( Player part )
{
	float distance = sqrt( pow( ( part.get_xPos() - xPos ), 2) + pow( ( part.get_yPos() - yPos ), 2 ) );
	if( distance < COLLISION_DISTANCE )
		return true;
	else
		return false;
}

// Die from touching a wall on hardcore
bool Player::wallDeath()
{
	if( GAME_DIFFICULTY == DifficultyMenu::hardcore )
	{
		if( xPos <= PLAYER_HEIGHT * 0.45 )
			return true;
		else if( xPos >= SCREEN_WIDTH - PLAYER_HEIGHT * 0.45 )
			return true;
		if( yPos <= PLAYER_HEIGHT * 0.45 )
			return true;
		else if( yPos >= SCREEN_HEIGHT - PLAYER_HEIGHT * 0.45 )
			return true;
	}

	return false;
}

// Render head or body
void Player::render( part part )
{
	switch( part )
	{
		case head:
			if( isClose )
				vita2d_draw_texture_rotate( gSnakeHeadOpenTexture.texture, xPos, yPos, rotation );
			else
				vita2d_draw_texture_rotate( gSnakeHeadTexture.texture, xPos, yPos, rotation );
			break;
		case body:
			vita2d_draw_texture_rotate( gSnakeBodyTexture.texture, xPos, yPos, rotation );
			break;
		case tail:
			vita2d_draw_texture_rotate( gSnakeTailTexture.texture, xPos, yPos, rotation );
			break;
	}
}

// Reset player position
void Player::resetPos()
{
	xPos = SCREEN_WIDTH / 6;
	yPos = SCREEN_HEIGHT / 2;
	rotation = M_PI / 2;
}

// Get position coordinates
float Player::get_xPos()
{
	return xPos;
}

float Player::get_yPos()
{
	return yPos;
}