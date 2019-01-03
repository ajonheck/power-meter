#include "PowerDisplay.h"
PowerDisplay::PowerDisplay()
{
	Adafruit_SSD1306 _display = Adafruit_SSD1306(128, 32, &Wire);
	_displayedMeasurements[VOLTAGE_LINE] = INST_AVERAGE;
	_displayedMeasurements[CURRENT_LINE] = INST_AVERAGE;
	_displayedMeasurements[POWER_LINE] = INST_AVERAGE;

}

void PowerDisplay::boot()
{	
  _display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // Address 0x3C for 128x32
  _display.display();
  delay(1000);
  // Clear the buffer.
  _display.clearDisplay();
  _display.display();
  // text display tests
  _display.setTextSize(1);
  _display.setTextColor(WHITE);
  _display.setCursor(0,0);
  _display.println("1");
  _display.println("2");
  _display.println("3");
  _display.println("4");
  _display.setCursor(0,0);
  _display.display(); // actually display all of the above
}

void PowerDisplay::changeDisplayUnits(Line line_number)
{
	if(_displayedMeasurements[line_number] == INST_AVERAGE)
	{
		_displayedMeasurements[line_number] = INST_MIN;
	}
	else if(_displayedMeasurements[line_number] == INST_MIN)
	{
		_displayedMeasurements[line_number] = INST_MAX;
	}
	else if (_displayedMeasurements[line_number] == INST_MAX)
	{
		_displayedMeasurements[line_number] = INST_AVERAGE;
	}
}

void PowerDisplay::updateDisplay()
{
	// Clear the buffer.
    _display.clearDisplay();
	
    _display.setCursor(0,0);
	
	// Set voltage display
	if(_displayedMeasurements[VOLTAGE_LINE] == INST_AVERAGE)
	{
		_displayLine("V", values[INST_VOLTAGE], "A", values[AVE_VOLTAGE], "V");
	}
	else if (_displayedMeasurements[VOLTAGE_LINE] == INST_MIN)
	{
		_displayLine("V", values[INST_VOLTAGE], "m", values[MIN_VOLTAGE], "V");
	}
	else
	{
		_displayLine("V", values[INST_VOLTAGE], "M", values[MAX_VOLTAGE], "V");
	}
	
	// Set current Display
	if(_displayedMeasurements[CURRENT_LINE] == INST_AVERAGE)
	{
		_displayLine("I", values[INST_CURRENT], "A", values[AVE_CURRENT], "A");
	}
	else if (_displayedMeasurements[CURRENT_LINE] == INST_MIN)
	{
		_displayLine("I", values[INST_CURRENT], "m", values[MIN_CURRENT], "A");
	}
	else
	{
		_displayLine("I", values[INST_CURRENT], "M", values[MAX_CURRENT], "A");
	}
	
	// Set power Display
	if(_displayedMeasurements[POWER_LINE] == INST_AVERAGE)
	{
		_displayLine("P", values[INST_POWER], "A", values[AVE_POWER], "W");
	}
	else if (_displayedMeasurements[POWER_LINE] == INST_MIN)
	{
		_displayLine("P", values[INST_POWER], "m", values[MIN_POWER], "W");
	}
	else
	{
		_displayLine("P", values[INST_POWER], "M", values[MAX_POWER], "W");
	}
	
	char energy_str[32];
	dtostrf(values[ENERGY]/60/60, 7, 4, energy_str);
	
	_display.println( String("") + "E: " + energy_str + "kWh");

	
    _display.setCursor(0,0);
    _display.display(); // actually display all of the above
}

void PowerDisplay::setVoltageValues(double instant, double min, double max, double average)
{
	values[INST_VOLTAGE] = instant;
	values[MIN_VOLTAGE] = min;
	values[MAX_VOLTAGE] = max;
	values[AVE_VOLTAGE] = average;
}
void PowerDisplay::setCurrentValues(double instant, double min, double max, double average)
{
	values[INST_CURRENT] = instant;
	values[MIN_CURRENT] = min;
	values[MAX_CURRENT] = max;
	values[AVE_CURRENT] = average;
}
void PowerDisplay::setPowerValues(double instant, double min, double max, double average)
{
	values[INST_POWER] = instant;
	values[MIN_POWER] = min;
	values[MAX_POWER] = max;
	values[AVE_POWER] = average;
}
void PowerDisplay::setEnergyUsage(double energy)
{
	values[ENERGY] = energy;
}
void PowerDisplay::_displayLine(String label1, double value1, String label2, double value2, String unit)
{
	char str1[8];
	char str2[8];
	dtostrf(value1, 4, 3, str1);
	dtostrf(value2, 4, 3, str2);
	_display.println( String("") + label1 + ": " + str1 + unit + ", " + label2 + ": " + str2 + unit);
}