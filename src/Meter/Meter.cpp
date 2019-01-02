#include "Meter.h"
#include "Arduino.h"

Meter::Meter(uint8_t pin, double slope, double intercept)
{
	_pin = pin;
	_slope = slope;
	_intercept = intercept;
	
	reset();
}

double Meter::getLastReading()
{
	return _interpolate((double)_instant);
}

double Meter::read()
{
	_instant = analogRead(_pin);
	_count ++;
	_min = _instant < _min ? _instant : _min;
	_max = _instant > _max ? _instant : _max;
	_ave = _ave * ((double)_count - 1)/((double)_count)
		+ (double)_instant * (1/(double)_count);
		
	_movingAveragePtr > MOVING_AVERAGE_COUNT - 1 ? _movingAveragePtr = 0 : _movingAveragePtr ++;
	_movingAverage[_movingAveragePtr] = _instant;
	
	return getLastReading();
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

double Meter::getMovingAverage()
{
	int sum;
	for(int i = 0; i <= MOVING_AVERAGE_COUNT-1; i ++)
	{
		sum += _movingAverage[i];
	}
	double ave = (double)(sum) / (double)MOVING_AVERAGE_COUNT;
	return _interpolate(ave);
}

double Meter::reset()
{
	_max = -32768;
	_min = 32767;
    _ave = 0;
    _instant = 0;
	_count = 0;
	_movingAveragePtr = 0;
	
	for(int i = 0; i <= MOVING_AVERAGE_COUNT-1; i ++)
	{
		_movingAverage[i] = 0;
	}
}