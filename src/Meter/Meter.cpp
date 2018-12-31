#include "Meter.h"
#include "Arduino.h"

Meter::Meter(uint8_t pin, double slope, double intercept)
{
	_pin = pin;
	_slope = slope;
	_intercept = intercept;
	
	reset();
}

double Meter::read()
{
	_instant = analogRead(_pin);
	_count ++;
	if(_instant < _min)
	{
		_min = _instant;
	}
	if(_instant > _max)
	{
		_max = _instant;
	}
	_ave = _ave * ((double)_count - 1)/((double)_count)
		+ (double)_instant * (1/(double)_count);
		
	return _interpolate((double)_instant);
}

double Meter::_interpolate(double data_point)
{
	return data_point * _slope + _intercept;
}

double Meter::getMin()
{
	return _interpolate((double)_min);
}

double Meter::getMax()
{
	return _interpolate((double)_max);
}

double Meter::getAverage()
{
	return _interpolate(_ave);
}

double Meter::reset()
{
	_max = -32768;
	_min = 32767;
    _ave = 0;
    _instant = 0;
	_count = 0;
}