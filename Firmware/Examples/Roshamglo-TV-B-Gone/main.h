/*
Roshamglo branch
Date: 07 Mar. 2017
By SparkFun Electronics https://www.sparkfun.com/
Modified to run on ATTiny84-based Roshamglo board

Last Updated: 22 Oct. 2016
By Gabriel Staples, http://www.ElectricRCAircraftGuy.com 
-My contact info is available by clicking the "Contact Me" tab at the top of my website.
*/

#include <avr/pgmspace.h>

// The TV-B-Gone for Arduino can use either the EU (European Union) or the NA (North America) database of POWER CODES
// EU is for Europe, Middle East, Australia, New Zealand, and some countries in Africa and South America
// NA is for North America, Asia, and the rest of the world not covered by EU

// Two regions!
#define NA 1
#define EU 0
#define REGION NA       // Set to NA or EU

// What pins do what
#define LED 7           //LED indicator pin (usually 13)
#define IRLED 5         //the IR sender LED 
#define TRIGGER 6       //the button pin; NB: this pin is "hard-coded" in the sleepNow() function in the primary .ino file by means of using external interrupt 0, which is hard-wired to pin 2

// Lets us calculate the size of the NA/EU databases
#define NUM_ELEM(x) (sizeof (x) / sizeof (*(x)));

// set define to 0 to turn off debug output
#define DEBUG 0
#define DEBUGP(x) if (DEBUG == 1) { x ; }

// Shortcut to insert single, non-optimized-out nop
#define NOP __asm__ __volatile__ ("nop")

// Tweak this if neccessary to change timing
// -for 8MHz Arduinos, a good starting value is 11
// -for 16MHz Arduinos, a good starting value is 25
#define DELAY_CNT 11

// Makes the codes more readable. the OCRA is actually
// programmed in terms of 'periods' not 'freqs' - that
// is, the inverse!
#define freq_to_timerval(x) (F_CPU / 8 / x - 1)

// The structure of compressed code entries
struct IrCode {
  uint8_t timer_val;
  uint8_t numpairs;
  uint8_t bitcompression;
  uint16_t const *times;
  uint8_t const *codes;
};
