#include "src/MomentarySwitch/MomentarySwitch.h"

#define GREEN_BUTTON 7
#define RED_BUTTON 6
#define GREEN_LED 3
#define RED_LED 2
#define BAUDRATE 9600

/* Declare global variables */
MomentarySwitch gbttn(GREEN_BUTTON);
bool previous_state = false;
bool current_state = false;
uint8_t press_count = 0;
uint8_t interrupt_count = 0;

/**
 * Setup variables before starting polling loop
 */
void setup() {
  Serial.begin(BAUDRATE);
  pinMode(GREEN_LED, OUTPUT);
  setup_isr_timer();
}

SIGNAL(TIMER2_COMPA_vect)
{
  isr_10ms();
}

/**
 * Interrupt routine called every ~9.984 ms (10 ms)
 */
void isr_10ms()
{

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


void loop() {
}
