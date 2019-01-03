#ifndef MomentarySwitch_h
#define MomentarySwitch_h

#include "Arduino.h"
class MomentarySwitch
{
	public:
		MomentarySwitch(uint8_t pin);
		void poll();
		bool isHeld();
		bool wasTapped();
		void resetStatus();
	private:
		uint8_t _pin;
		bool _pressed;
		uint8_t _held;
		bool _tapped;
};

#endif