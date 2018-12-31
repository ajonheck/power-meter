#ifndef Meter_h
#define Meter_h

#include "Arduino.h"
class Meter
{
	public:
		Meter(uint8_t pin, double slope, double intercept);
		double read();
		double getMin();
		double getMax();
		double getAverage();
		double reset();
	private:
		uint8_t _pin;
		double _slope;
		double _intercept;
		short _min;
		short _max;
		double _ave;
		short _instant;
		int _count;
		double _interpolate(double data_point);
};

#endif