#ifndef BUTTONS_HPP
#define BUTTONS_HPP

#include <string.h>

#include <psp2/kernel/processmgr.h>
#include <psp2/ctrl.h>

class Input
{
	public:
		Input();

		enum buttonIndex
		{
			select = 0,
			start = 1,

			up = 2,
			right = 3,
			down = 4,
			left = 5,
			
			lTrigger = 6,
			rTrigger = 7,

			triangle = 8,
			circle = 9,
			cross = 10,
			square = 11
		};

		// Update button states
		bool wasPressed( buttonIndex id );


	private:
		// Inputs
		SceCtrlData pad;

		// Buttons
		bool button[ 12 ];
};

#endif // BUTTONS_HPP