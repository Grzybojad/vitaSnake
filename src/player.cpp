#include "player.hpp"

// Initialize variables
Player::Player()
{
	isClose = false;
	speed_mod = 1.0f;

	memset( &pad, 0, sizeof( pad ) );

	// Set sampling mode to analog, so that the analog sticks return proper values
	sceCtrlSetSamplingMode( SCE_CTRL_MODE_ANALOG );

	// Initialize parts
	vec3 newPart( SCREEN_WIDTH / 6, SCREEN_HEIGHT / 2, M_PI / 2 );
	snakeParts.push_back( newPart );
	for( int i = 1; i < 4; ++i )
	{
		vec3 newPart;
		snakeParts.push_back( newPart );
	}
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

void Player::setMode()
{
	switch( GAME_MODE )
	{
		case ModeMenu::classic:
			break;
		case ModeMenu::timeTrial:
			break;
		case ModeMenu::hyper:
			break;
		case ModeMenu::fibonacci:
			PLAYER_SET_SPEED *= 0.5;
			PLAYER_SET_ROTATION_SPEED *= 0.5;
			break;
		case ModeMenu::lazy:
			break;
	}
}


// Handle input
void Player::handleInput()
{
	sceCtrlPeekBufferPositive( 0, &pad, 1 );

	float analogInput = (float)((pad.lx - 128.0f) / 128.0f);

	// Calculate rotation
	if (CONTROL_STYLE == 0)
	{
		if (pad.buttons & SCE_CTRL_LEFT)
			snakeParts[0].r -= PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
		else if (pad.buttons & SCE_CTRL_RIGHT)
			snakeParts[0].r += PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;

		else if( analogInput > ANALOG_DEADZONE || analogInput < -ANALOG_DEADZONE )
				snakeParts[0].r += analogInput * PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;

		else if( gInput.isTouched() || gInput.isBackTouched() )
		{
			int touchX = 0;
			int touchY = 0;
			if( gInput.isTouched() )
			{
				touchX = gInput.getTouchX();
				touchY = gInput.getTouchY();
			}
			else if( gInput.isBackTouched() )
			{
				touchX = gInput.getBackTouchX();
				touchY = gInput.getBackTouchY();
			}

			// Touch steering uses the same logic as analog steering
			float touchAngle = atan2( touchY-snakeParts[0].y, touchX-snakeParts[0].x ) + (M_PI / 2);
			if( touchAngle < 0 ) touchAngle += M_PI * 2;

			if( snakeParts[0].r < M_PI )
			{
				if( touchAngle > snakeParts[0].r && touchAngle < (snakeParts[0].r + M_PI) )
					snakeParts[0].r += PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
				else
					snakeParts[0].r -= PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
			}
			else if( snakeParts[0].r > M_PI )
			{
				if( touchAngle < (snakeParts[0].r - M_PI) )
					touchAngle += 2 * M_PI;

				if( touchAngle > snakeParts[0].r && touchAngle < (snakeParts[0].r + M_PI) )
					snakeParts[0].r += PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
				else
					snakeParts[0].r -= PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
			}
		}
	}
	else if( CONTROL_STYLE == 1 )
	{
		// Find the "angle" at which the dpad was pressed
		float dpadAngle = -1;

		if( pad.buttons & SCE_CTRL_LEFT && pad.buttons & SCE_CTRL_UP )
			dpadAngle = ( 7 * M_PI ) / 4;
		else if( pad.buttons & SCE_CTRL_LEFT && pad.buttons & SCE_CTRL_DOWN )
			dpadAngle = ( 5 * M_PI ) / 4;
		else if( pad.buttons & SCE_CTRL_RIGHT && pad.buttons & SCE_CTRL_UP )
			dpadAngle = M_PI / 4;
		else if( pad.buttons & SCE_CTRL_RIGHT && pad.buttons & SCE_CTRL_DOWN )
			dpadAngle = ( 3 * M_PI ) / 4;

		else if( pad.buttons & SCE_CTRL_LEFT )
			dpadAngle = 3 * M_PI / 2;
		else if( pad.buttons & SCE_CTRL_RIGHT )
			dpadAngle = M_PI / 2;
		else if( pad.buttons & SCE_CTRL_UP )
			dpadAngle = 0;
		else if( pad.buttons & SCE_CTRL_DOWN )
			dpadAngle = M_PI;

		else if( analogInput > ANALOG_DEADZONE || analogInput < -ANALOG_DEADZONE )
		{
			float analogX = pad.lx - 128;
			float analogY = pad.ly - 128;

			float analogAngle = atan2( analogY, analogX ) + (M_PI / 2);
			if( analogAngle < 0 ) analogAngle += M_PI * 2;

			if( snakeParts[0].r < M_PI )
			{
				if( analogAngle > snakeParts[0].r && analogAngle < (snakeParts[0].r + M_PI) )
					snakeParts[0].r += PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
				else
					snakeParts[0].r -= PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
			}
			else if( snakeParts[0].r > M_PI )
			{
				if( analogAngle < (snakeParts[0].r - M_PI) )
					analogAngle += 2 * M_PI;

				if( analogAngle > snakeParts[0].r && analogAngle < (snakeParts[0].r + M_PI) )
					snakeParts[0].r += PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
				else
					snakeParts[0].r -= PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
			}
		}

		else if( gInput.isTouched() || gInput.isBackTouched() )
		{
			int touchX = 0;
			int touchY = 0;
			if( gInput.isTouched() )
			{
				touchX = gInput.getTouchX();
				touchY = gInput.getTouchY();
			}
			else if( gInput.isBackTouched() )
			{
				touchX = gInput.getBackTouchX();
				touchY = gInput.getBackTouchY();
			}

			// Touch steering uses the same logic as analog steering
			float touchAngle = atan2( touchY - snakeParts[0].y, touchX - snakeParts[0].x ) + (M_PI / 2);
			if( touchAngle < 0 ) touchAngle += M_PI * 2;

			if( snakeParts[0].r < M_PI )
			{
				if( touchAngle > snakeParts[0].r && touchAngle < (snakeParts[0].r + M_PI) )
					snakeParts[0].r += PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
				else
					snakeParts[0].r -= PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
			}
			else if( snakeParts[0].r > M_PI )
			{
				if( touchAngle < (snakeParts[0].r - M_PI) )
					touchAngle += 2 * M_PI;

				if( touchAngle > snakeParts[0].r && touchAngle < (snakeParts[0].r + M_PI) )
					snakeParts[0].r += PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
				else
					snakeParts[0].r -= PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
			}
		}

		// Calculate player rotation based on dpad input
		if( dpadAngle != -1 )
		{
			if( snakeParts[0].r < M_PI )
			{
				if( dpadAngle > snakeParts[0].r && dpadAngle < (snakeParts[0].r + M_PI) )
					snakeParts[0].r += PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
				else
					snakeParts[0].r -= PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
			}
			else if( snakeParts[0].r > M_PI )
			{
				if( dpadAngle < (snakeParts[0].r - M_PI) )
					dpadAngle += 2 * M_PI;

				if( dpadAngle > snakeParts[0].r && dpadAngle < (snakeParts[0].r + M_PI) )
					snakeParts[0].r += PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
				else
					snakeParts[0].r -= PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
			}
		}
	}

	// Analog stick controls with deadzone
	else if( analogInput > ANALOG_DEADZONE || analogInput < -ANALOG_DEADZONE )
	{
		if( CONTROL_STYLE == 0 )
		{
			snakeParts[0].r += analogInput * PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
		}
		else if( CONTROL_STYLE == 1 )
		{
			float analogX = pad.lx - 128;
			float analogY = pad.ly - 128;

			float analogAngle = atan2( analogY, analogX ) + ( M_PI / 2 );
			if( analogAngle < 0 ) analogAngle += M_PI * 2;	

			if( snakeParts[0].r < M_PI )
			{
				if( analogAngle > snakeParts[0].r && analogAngle < ( snakeParts[0].r + M_PI ) )
					snakeParts[0].r += PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
				else
					snakeParts[0].r -= PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
			}
			else if( snakeParts[0].r > M_PI )
			{
				if( analogAngle < ( snakeParts[0].r - M_PI ) )
					analogAngle += 2 * M_PI;

				if( analogAngle > snakeParts[0].r && analogAngle < ( snakeParts[0].r + M_PI ) )
					snakeParts[0].r += PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
				else
					snakeParts[0].r -= PLAYER_SET_ROTATION_SPEED * timestep * speed_mod;
			}
		}
	}

	// Boost snake speed with X
	if( pad.buttons & SCE_CTRL_CROSS )
		speed = 1.5f * -PLAYER_SET_SPEED * timestep * speed_mod;
	else
		speed = -PLAYER_SET_SPEED * timestep * speed_mod;
}


// Move the player
void Player::move()
{
	// Normalize player rotation
	if( snakeParts[0].r < 0 )
		snakeParts[0].r += ( 2 * M_PI );
	else if( snakeParts[0].r > ( 2 * M_PI ) )
		snakeParts[0].r = snakeParts[0].r - ( 2 * M_PI );

	// Calcule player position
	snakeParts[0].x -= sin( snakeParts[0].r ) * speed;
	snakeParts[0].y += cos( snakeParts[0].r ) * speed;

	// Trap the player inside the screen
	if( snakeParts[0].x < PLAYER_HEIGHT * 0.45 )
		snakeParts[0].x = PLAYER_HEIGHT * 0.45;
	else if( snakeParts[0].x > SCREEN_WIDTH - PLAYER_HEIGHT * 0.45 )
		snakeParts[0].x = SCREEN_WIDTH - PLAYER_HEIGHT * 0.45;
	if( snakeParts[0].y < PLAYER_HEIGHT * 0.45 )
		snakeParts[0].y = PLAYER_HEIGHT * 0.45;
	else if( snakeParts[0].y > SCREEN_HEIGHT - PLAYER_HEIGHT * 0.45 )
		snakeParts[0].y = SCREEN_HEIGHT - PLAYER_HEIGHT * 0.45;
}

// Move the body parts
void Player::follow()
{
	// Don't bother calculating positions of the end of a very long snake
	for( int i = 1; i < snakeParts.size() && i < 1000; ++i )
	{
		if( snakeParts[i].r < 0 )
			snakeParts[i].r += ( 2 * M_PI );
		else if( snakeParts[i].r > ( 2 * M_PI ) )
			snakeParts[i].r -= ( 2 * M_PI );

		// Calculate distance to previous part
		float distance = sqrt( pow( ( snakeParts[i-1].x - snakeParts[i].x ), 2) + pow( ( snakeParts[i-1].y - snakeParts[i].y ), 2 ) );

		// Rotate to look at previous part
		snakeParts[i].r = atan2( snakeParts[i-1].y - snakeParts[i].y, snakeParts[i-1].x - snakeParts[i].x ) + ( M_PI / 2 );
		

		// If the distance is too far, move to the correct distance
		if( distance > FOLLOW_DISTANCE )
		{
			snakeParts[i].x += sin( snakeParts[i].r ) * ( distance - FOLLOW_DISTANCE );
			snakeParts[i].y -= cos( snakeParts[i].r ) * ( distance - FOLLOW_DISTANCE );
		}

		// Trap the part inside the screen
		if( snakeParts[i].x < 0 )
			snakeParts[i].x = 0;
		else if( snakeParts[i].x > SCREEN_WIDTH )
			snakeParts[i].x = SCREEN_WIDTH;
		if( snakeParts[i].y < 0 )
			snakeParts[i].y = 0;
		else if( snakeParts[i].y > SCREEN_HEIGHT )
			snakeParts[i].y = SCREEN_HEIGHT;
	}
}

void Player::handleDrag()
{
	int touch_zone = 50;
	
	if( gInput.isTouched() )
	{
		if( 
			( ( snakeParts[0].x + touch_zone ) > gInput.getTouchX() ) &&
			( gInput.getTouchX() > ( snakeParts[0].x - touch_zone ) ) &&
			( ( snakeParts[0].y + touch_zone ) > gInput.getTouchY() ) &&
			( gInput.getTouchY() > ( snakeParts[0].y - touch_zone ) )
		)
		{
			vec3 lastPos = snakeParts[0];

			snakeParts[0].x = gInput.getTouchX();
			snakeParts[0].y = gInput.getTouchY();
			snakeParts[0].r = snakeParts[1].r;
		}
	}
}

// Tail collision
bool Player::checkCollision()
{
	if( snakeParts.size() > START_SNAKE_LENGTH )
	{
		// Don't bother calculating collisions of the end of a very long snake
		for( int i = START_SNAKE_LENGTH+1; i < snakeParts.size() && i < 1000; ++i )
		{
			// Check collisions only if parts don't overlap
			if( snakeParts[ i ].x != snakeParts[ i-1 ].x || snakeParts[ i ].y != snakeParts[ i-1 ].y )
			{
				float distance = sqrt( pow( ( snakeParts[i].x - snakeParts[0].x ), 2) + pow( ( snakeParts[i].y - snakeParts[0].y ), 2 ) );
				if( distance < COLLISION_DISTANCE )
					return true;
			}
		}
	}
	
	return false;
}

// Die from touching a wall on hardcore
bool Player::wallDeath()
{
	if( GAME_DIFFICULTY == DifficultyMenu::hardcore )
	{
		if( snakeParts[0].x <= PLAYER_HEIGHT * 0.45 )
			return true;
		else if( snakeParts[0].x >= SCREEN_WIDTH - PLAYER_HEIGHT * 0.45 )
			return true;
		if( snakeParts[0].y <= PLAYER_HEIGHT * 0.45 )
			return true;
		else if( snakeParts[0].y >= SCREEN_HEIGHT - PLAYER_HEIGHT * 0.45 )
			return true;
	}

	return false;
}

void Player::render()
{
	// Render tail first
	drawPlayer( tail, snakeParts[ snakeParts.size()-1 ].x, snakeParts[ snakeParts.size()-1 ].y, snakeParts[ snakeParts.size()-1 ].r );

	// Render the body parts from tail to head, but don't bother drawing the snake parts if it's too long
	for( int i = snakeParts.size()-2; i > 0; --i )
	{
		// but don't bother drawing the snake parts if it's too long
		if( i < 1000)
			// Draw snakePart only if it doesn't overlap
			if( snakeParts[ i ].x != snakeParts[ i-1 ].x || snakeParts[ i ].y != snakeParts[ i-1 ].y )
				drawPlayer( body, snakeParts[ i ].x, snakeParts[ i ].y, snakeParts[ i ].r );
	}
		
	
	// Render the head
	if( isClose )
		drawPlayer( headOpen, snakeParts[ 0 ].x, snakeParts[ 0 ].y, snakeParts[ 0 ].r );
	else
		drawPlayer( head, snakeParts[ 0 ].x, snakeParts[ 0 ].y, snakeParts[ 0 ].r );
}

// Reset player position
void Player::reset()
{
	for( int i = 0; i < snakeParts.size()-1; ++i )
	{
		snakeParts[i].x = SCREEN_WIDTH / 6;
		snakeParts[i].y = SCREEN_HEIGHT / 2;
		snakeParts[i].r = M_PI / 2;
	}

	speed_mod = 1.0f;
}

// Add the specified amount of parts to the snake
void Player::addParts( int i )
{
	for( ; i > 0; --i )
	{
		vec3 newPart( snakeParts.end()[-1].x, snakeParts.end()[-1].y, snakeParts.end()[-1].r );
		snakeParts.push_back( newPart );
	}
}

// Set the snake length
void Player::setSize( int i )
{
	if( snakeParts.size() > i )
		while( snakeParts.size() > i )
			snakeParts.pop_back();
	else if( snakeParts.size() < i )
	{
		while( snakeParts.size() < i )
		{
			vec3 newPart;
			snakeParts.push_back( newPart );
		}
	}
}

void Player::speedUp()
{
	speed_mod *= SPEED_UP_MODIFIER;
}

// Get position coordinates
vec3 Player::get_pos()
{
	return snakeParts[0];
}

int Player::getSize()
{
	return snakeParts.size();
}