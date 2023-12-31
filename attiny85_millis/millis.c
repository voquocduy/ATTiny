#define F_CPU 8000000UL
#include <avr/io.h>
#include <inttypes.h>
#include <avr/interrupt.h>

void setup(void);
void loop(void);
int main(void);

uint64_t _millis = 0;
uint16_t _1000us = 0;

uint64_t old_millis = 0;

/* interrupts routines */
// timer overflow occur every 0.256 ms
ISR(TIM0_OVF_vect) {
  _1000us += 256;
  while (_1000us > 1000) {
    _millis++;
    _1000us -= 1000;
  }
}

uint64_t millis() {
  uint64_t m;
  cli();
  m = _millis;
  sei();
  return m;
}

void setup(void) {
  // LED IO
  DDRB |= _BV(PB1); // set LED pin as output
  PORTB |= _BV(PB1); // turn the LED on

  // prescale timer0 to 1/8th the clock rate
  // overflow timer0 every 0.256 ms
  TCCR0B |= (1<<CS01);
  // enable timer overflow interrupt
  TIMSK  |= 1<<TOIE0;

  // Enable global interrupts
  sei();
}

void loop(void) {
  // every 1000ms toggle LED
  if ((millis() - old_millis) > 100) {
   // Toggle Port B pin 3 output state
   PORTB ^= 1<<PB3;
   old_millis = millis();
  }
}

int main(void) {
  setup();
  for(;;) {
    loop();
  }
};
