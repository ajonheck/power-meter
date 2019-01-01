#include "src/MomentarySwitch/MomentarySwitch.h"
#include "src/Meter/Meter.h"

#define GREEN_BUTTON 7
#define RED_BUTTON 6
#define VOLT_METER A1
#define AMP_METER A2
#define GREEN_LED 3
#define RED_LED 2
#define BAUDRATE 9600

/* Declare global variables */
MomentarySwitch gbttn(GREEN_BUTTON);
Meter v_met(VOLT_METER, 0.0048828125, 0);
Meter i_met(AMP_METER, 0.003522068478, 0.01629657612);
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
  current_state = gbttn.isPressed();
  if(current_state == true && current_state != previous_state)
  {
    press_count ++;
    Serial.print("Green button pressed: ");
    Serial.print(press_count);
    Serial.print("\n");
    v_met.reset();
    i_met.reset();
  }

  gbttn.isHeld() ? digitalWrite(GREEN_LED, HIGH) : digitalWrite(GREEN_LED, LOW);
  
  previous_state = current_state;
  v_met.read();
  i_met.read();

  interrupt_count ++;
  if(interrupt_count % 100 == 0)
  {
    isr_1000ms();
    interrupt_count = 0;
  }
}

void isr_1000ms()
{
  Serial.print("Voltmeter readings:");
  Serial.print("\n\tinstant: ");
  Serial.print(v_met.read(),5);
  Serial.print("\n\tave: ");
  Serial.print(v_met.getAverage(),5);
  Serial.print("\n");
  Serial.print("Ampmeter readings:");
  Serial.print("\n\tinstant: ");
  Serial.print(i_met.read(),5);
  Serial.print("\n\tave: ");
  Serial.print(i_met.getAverage(),5);
  Serial.print("\n");
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
