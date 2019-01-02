#ifndef Meter_h
#define Meter_h

#define MOVING_AVERAGE_COUNT 10

#include "Arduino.h"
class Meter
{
	public:
		Meter(uint8_t pin, double slope, double intercept);
		double read();
		double getMin();
		double getMax();
		double getAverage();
		double getMovingAverage();
		double getLastReading();
		double reset();
	private:
		uint8_t _pin;
		double _slope;
		double _intercept;
		short _min;
		short _max;
		short _movingAverage[MOVING_AVERAGE_COUNT];
		uint8_t _movingAveragePtr;
		double _ave;
		short _instant;
		int _count;
		double _interpolate(double data_point);
};

#endif