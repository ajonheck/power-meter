#ifndef MomentarySwitch_h
#define MomentarySwitch_h

#include "Arduino.h"
class MomentarySwitch
{
	public:
		MomentarySwitch(uint8_t pin);
		bool isPressed();
		bool isHeld();
	private:
		uint8_t _pin;
		bool _pressed;
		uint8_t _held;
};

#endif