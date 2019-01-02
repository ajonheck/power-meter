// Project specific includes
#include "src/MomentarySwitch/MomentarySwitch.h"
#include "src/Meter/Meter.h"
#include "src/PowerDisplay/PowerDisplay.h"

/* Constant definitions */
#define GREEN_BUTTON 2
#define RED_BUTTON 3
#define VOLT_METER A6
#define AMP_METER A7
#define BAUDRATE 9600
#define INTERRUPT_TIME 0.00992

/* Functional definitions */
#define CHK_BIT(c, n) ((c) & (1<<(n)))
#define CLR_BIT(c, n) ((c) &= ~((1) << (n)))
#define SET_BIT(c, n) ((c) |= (1<<(n)))

/* Enumerated Types */
enum IsrMask: uint8_t {
  ISR_10_ms    = 0,
  ISR_20_ms    = 1,
  ISR_50_ms    = 2,
  ISR_100_ms   = 3,
  ISR_200_ms   = 4,
  ISR_500_ms   = 5,
  ISR_1000_ms  = 6,
  ISR_2000_ms  = 7
};

/* Declare global variables */
MomentarySwitch gbttn(GREEN_BUTTON);
MomentarySwitch rbttn(RED_BUTTON);

Meter vMet(VOLT_METER, 0.0048828125, 0);
Meter iMet(AMP_METER, 0.003522068478, 0.01629657612);

volatile char isr_flag = 0;
uint8_t interrupt_count = 0;
bool isRunning;

double p, minP, maxP, averageP, energy;
int readingCount = 0;

PowerDisplay display = PowerDisplay();

/**
 * Setup
 */
void setup() {
  isRunning = false;
  p = 0;
  minP = 100000;
  maxP = -1;
  averageP = 0;
  
  Serial.begin(BAUDRATE);
  display.boot();
  setup_isr_timer();
}

/**
 * Setsup timer 2 for ~ 10 ms interrupts
 */
void setup_isr_timer()
{
  TCCR2A = 0b11000010;
  OCR2A = 155;
  TIMSK2 = 0b00000010;
  TCCR2B = 0b00000111;
  sei();
}

void isr_10_ms()
{
  //Handle buttons
  if(isRunning)
  {
    if(rbttn.isPressed())
    {
      isRunning = false;
    }
  }
  else
  {
    if(gbttn.isPressed())
    {
      isRunning = true;
    }
  }

  if(gbttn.isHeld() && rbttn.isHeld())
  {
    isRunning = false;
    p = 0;
    minP = 100000;
    maxP = -1;
    averageP = 0;
    vMet.reset();
    iMet.reset();
    display.setVoltageValues(0, 0, 0, 0);
    display.setCurrentValues(0, 0, 0, 0);
    display.setPowerValues(0, 0, 0,0);
    display.updateDisplay();
  } else {
    if(gbttn.isHeld())
    {
      display.changeDisplayUnits(1);
    }
    if(rbttn.isHeld())
    {
      display.changeDisplayUnits(2);      
    }
  }

  if(isRunning)
  {
    p = vMet.read() * iMet.read();
    readingCount ++;
    minP = p < minP ? p : minP;
    maxP = p > maxP ? p : maxP;
    averageP = averageP * ((double)readingCount - 1)/((double)readingCount)
      + (double)p * (1/(double)readingCount);
    energy += p * INTERRUPT_TIME;
    CLR_BIT(isr_flag, ISR_10_ms);
  }
}

void isr_2000_ms()
{
  // update display values
  display.setVoltageValues(vMet.getLastReading(), vMet.getMin(), vMet.getMax(), vMet.getAverage());
  display.setCurrentValues(iMet.getLastReading(), iMet.getMin(), iMet.getMax(), iMet.getAverage());
  display.setPowerValues(p, minP, maxP, averageP);
  display.updateDisplay();
  CLR_BIT(isr_flag, ISR_2000_ms);
}

void loop() {
  if(CHK_BIT(isr_flag, ISR_10_ms))
  {
    isr_10_ms();
  }
  if(CHK_BIT(isr_flag, ISR_2000_ms))
  {
    isr_2000_ms();
  }
}


SIGNAL(TIMER2_COMPA_vect)
{
  SET_BIT(isr_flag, ISR_10_ms);
  interrupt_count ++;
  if(interrupt_count % 2 == 0)
  {
    SET_BIT(isr_flag, ISR_20_ms);
  }
  if(interrupt_count % 5 == 0)
  {
    SET_BIT(isr_flag, ISR_50_ms);
  }
  if(interrupt_count % 10 == 0)
  {
    SET_BIT(isr_flag, ISR_100_ms);
  }
  if(interrupt_count % 20 == 0)
  {
    SET_BIT(isr_flag, ISR_200_ms);
  }
  if(interrupt_count % 50 == 0)
  {
    SET_BIT(isr_flag, ISR_500_ms);
  }
  if(interrupt_count % 100 == 0)
  {
    SET_BIT(isr_flag, ISR_1000_ms);
  }
  if(interrupt_count % 200 == 0)
  {
    SET_BIT(isr_flag, ISR_2000_ms);
    interrupt_count = 0;
  }
}
