#include "PowerDisplay.h"
PowerDisplay::PowerDisplay()
{
	Adafruit_SSD1306 _display = Adafruit_SSD1306(128, 32, &Wire);
	_voltageUnits = INST_AVERAGE;
	_currentUnits = INST_AVERAGE;
	_powerUnits = INST_AVERAGE;

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

void PowerDisplay::changeDisplayUnits(uint8_t line_number)
{
	if(line_number == 1 && (_voltageUnits ++) > INST_MAX)
	{
		_voltageUnits = INST_AVERAGE;
	}
	else if(line_number == 2 && (_currentUnits ++) > INST_MAX)
	{
		_currentUnits = INST_AVERAGE;
	} else if (line_number == 3 && (_powerUnits ++) > INST_MAX)
	{
		_powerUnits = INST_AVERAGE;
	}
}

void PowerDisplay::updateDisplay()
{
	// Clear the buffer.
    _display.clearDisplay();
	
    _display.setCursor(0,0);
	
	// Set voltage display
	if(_voltageUnits == INST_AVERAGE)
	{
		_displayLine("V", values[INST_VOLTAGE], "Ave", values[AVE_VOLTAGE], "V");
	}
	else if (_voltageUnits == INST_MIN)
	{
		_displayLine("V", values[INST_VOLTAGE], "Min", values[MIN_VOLTAGE], "V");
	}
	else
	{
		_displayLine("V", values[INST_VOLTAGE], "Max", values[MAX_VOLTAGE], "V");
	}
	
	// Set current Display
	if(_currentUnits == INST_AVERAGE)
	{
		_displayLine("I", values[INST_CURRENT], "Ave", values[AVE_CURRENT], "A");
	}
	else if (_currentUnits == INST_MIN)
	{
		_displayLine("I", values[INST_CURRENT], "Min", values[MIN_CURRENT], "A");
	}
	else
	{
		_displayLine("I", values[INST_CURRENT], "Max", values[MAX_CURRENT], "A");
	}
	
	// Set power Display
	if(_powerUnits == INST_AVERAGE)
	{
		_displayLine("P", values[INST_POWER], "Ave", values[AVE_POWER], "W");
	}
	else if (_powerUnits == INST_MIN)
	{
		_displayLine("P", values[INST_POWER], "Min", values[MIN_POWER], "W");
	}
	else
	{
		_displayLine("P", values[INST_POWER], "Max", values[MAX_POWER], "W");
	}
	
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

void PowerDisplay::_displayLine(String label1, double value1, String label2, double value2, String unit)
{
	_display.println( String("") + label1 + ": " + value1 + unit + ", " + label2 + ": " + value2 + unit);
}