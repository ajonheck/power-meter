#define WAIT_COUNT 99
#define READ_COUNT 4
#define HELD_COUNT 100

#include "MomentarySwitch.h"
#include "Arduino.h"

MomentarySwitch::MomentarySwitch(uint8_t pin)
{
	_pin = pin;
	_pressed = false;
	_held = 0;
	pinMode(_pin, INPUT);
}

bool MomentarySwitch::isPressed()
{
	bool inconclusive_reading = false;
	char readings[READ_COUNT];
	
	// Perform debounce
	for(int i = 0; i <=READ_COUNT-1; i ++)
	{
		readings[i] = digitalRead(_pin);
		for(int wait = 0; wait <= WAIT_COUNT; wait ++);
		if( i >= 1 && readings[i] != readings[i - 1])
		{
			inconclusive_reading = true;
			break;
		}
	}
	
	if(!inconclusive_reading)
	{
		_pressed = readings[0];
	}
	
	if(_pressed && _held <= HELD_COUNT)
	{
		_held ++;
	}
	else if(!_pressed)
	{
		_held = 0;
	}
	
	return _pressed;
}

bool MomentarySwitch::isHeld()
{
	if(_held >= HELD_COUNT)
	{
		return true;
	}
	else
	{
		return false;
	}
}