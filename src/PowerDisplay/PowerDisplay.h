#ifndef PowerDiplay_h
#define PowerDiplay_h
// OLED libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "Arduino.h"

enum DisplayedMeasurements: uint8_t {
	INST_AVERAGE,
	INST_MIN,
	INST_MAX
};

enum Line: uint8_t {
	VOLTAGE_LINE,
	CURRENT_LINE,
	POWER_LINE
};

enum ValueKeys: uint8_t {
	INST_VOLTAGE,
	MIN_VOLTAGE,
	MAX_VOLTAGE,
	AVE_VOLTAGE,
	INST_CURRENT,
	MIN_CURRENT,
	MAX_CURRENT,
	AVE_CURRENT,
	INST_POWER,
	MIN_POWER,
	MAX_POWER,
	AVE_POWER,
	ENERGY
};

class PowerDisplay
{
	public:
		PowerDisplay();
		void setVoltageValues(double instant, double min, double max, double average);
		void setCurrentValues(double instant, double min, double max, double average);
		void setPowerValues(double instant, double min, double max, double average);
		void setEnergyUsage(double energy);
		void changeDisplayUnits(Line line_number);
		void updateDisplay();
		void boot();
	private:
		DisplayedMeasurements _displayedMeasurements[3];
		Adafruit_SSD1306 _display;
		double values[13];
		void _displayLine(String label1, double value1, String label2, double value2, String unit);
};


#endif