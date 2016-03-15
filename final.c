// Declare libraries
#include <wiringPi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

// Define variables

// 74HC595 register
#define   SDI   0   //serial data input
#define   RCLK  1   //memory clock input(STCP)
#define   SRCLK 2   //shift register clock input(SHCP)

// For the game itself
#define PIN_LED 29 // The LED that turns on randomly
#define PIN_BUTTON 4 // The button that has to be pressed

unsigned char LED[8] = {0x01,0x02,0x04,0x08,0x10,0x20,0x40,0x80}; // Corresesponding LED's
int randomTimer = 0; // Randomized timer

// Define functions

// Turns LED on for measuring reaction time
void reactionLedOn () {
  randomTimer = rand() % 10 + 1;
  delay(randomTimer * 1000);
  digitalWrite(PIN_LED, 1);
  printf("LED aan\n");
}

// Clock for 74HC595 register
void pulse(int pin)
{
	digitalWrite(pin, 0);
	digitalWrite(pin, 1);
}

// Set pin (parameter) to high
void SIPO (unsigned char byte) {
	int i;
	for (i = 0; i < 8; i++) {
		digitalWrite(SDI, ((byte & (0x80 >> i)) > 0));
		pulse(SRCLK);
	}
}

// Initializes all pins
int init()
{
	pinMode(SDI, OUTPUT); //make P0 output
	pinMode(RCLK, OUTPUT); //make P0 output
	pinMode(SRCLK, OUTPUT); //make P0 output
	pinMode(PIN_LED, OUTPUT); //make P0 output
  pinMode(PIN_BUTTON, INPUT); //make P0 output
  pinMode(PIN_BUTTON, PUD_UP); //make P0 output

	digitalWrite(SDI, 0);
	digitalWrite(RCLK, 0);
	digitalWrite(SRCLK, 0);

  if(wiringPiSetup() == -1){ //when initialize wiring failed,print messageto screen
		printf("setup wiringPi failed !");
		return 1;
	}
}

// Calculates reactiontime in seconds
int calcTimeInS (clock_t time) {
  float m = (((float)time)/CLOCKS_PER_SEC);
  int n = (int) (m < 0 ? (m - 0.5) : (m + 0.5));
  return n;
}

// Calculates reactiontime in ms
float calcTimeInMS (clock_t time) {
  float m = (((float)time)/CLOCKS_PER_SEC);
  return m*10;
}

// Calculates reactiontime in ms (rounded)
float calcTimeInMSRounded (clock_t time) {
  float m = (((float)time)/CLOCKS_PER_SEC);
  int x = m * 10;
  int y = (int) (x < 0 ? (x - 0.5) : (x + 0.5));
  return y;
}

// Sets output of pins according to reactiontime
int setBitLED (int t) {
  int u;
  if (t == 1) {
    SIPO(LED[4]);
    pulse(RCLK);
  } else if (t == 2) {
    SIPO(LED[3]);
    pulse(RCLK);
  } else if (t == 3) {
    for (u = 0; u < 8; u++) {
      SIPO(LED[4]);
      pulse(RCLK);
      delay(200);
      SIPO(LED[3]);
      pulse(RCLK);
      delay(200);
    }
  } else if (t == 4) {
    SIPO(LED[2]);
    pulse(RCLK);
  } else if (t == 5) {
    for (u = 0; u < 8; u++) {
      SIPO(LED[4]);
      pulse(RCLK);
      delay(200);
      SIPO(LED[2]);
      pulse(RCLK);
      delay(200);
    }
  } else if (t == 6) {
    for (u = 0; u < 8; u++) {
      SIPO(LED[3]);
      pulse(RCLK);
      delay(200);
      SIPO(LED[2]);
      pulse(RCLK);
      delay(200);
    }
  } else if (t == 7) {
    for (u = 0; u < 8; u++) {
      SIPO(LED[4]);
      pulse(RCLK);
      delay(200);
      SIPO(LED[3]);
      pulse(RCLK);
      delay(200);
      SIPO(LED[2]);
      pulse(RCLK);
      delay(200);
    }
  } else if (t == 8) {
    SIPO(LED[1]);
    pulse(RCLK);
  } else if (t == 9) {
    for (u = 0; u < 8; u++) {
      SIPO(LED[4]);
      pulse(RCLK);
      delay(200);
      SIPO(LED[1]);
      pulse(RCLK);
      delay(200);
    }
  } else if (t == 10) {
    for (u = 0; u < 8; u++) {
      SIPO(LED[3]);
      pulse(RCLK);
      delay(200);
      SIPO(LED[1]);
      pulse(RCLK);
      delay(200);
    }
  } else {
    for (u = 0; u < 8; u++) {
      SIPO(LED[4]);
      pulse(RCLK);
      delay(200);
      SIPO(LED[3]);
      pulse(RCLK);
      delay(200);
      SIPO(LED[2]);
      pulse(RCLK);
      delay(200);
      SIPO(LED[1]);
      pulse(RCLK);
      delay(200);
    }
  }
  return 0;
}

// Displays reactiontime in binary code
int timeInBits (time) {
    int c, k;
    for (c = 3; c >= 0; c--) {
      k = time >> c;
      if (k & 1) {
        printf("1");
      } else {
        printf("0");
      }
    }
  printf("\n");
  return 1;
}

// Main function
int main (void) {
	init(); // Initializes pins and wiringPi library

	int buttonIsPressed = 0; // Variable for game button

  // Turn LED on rancomly if it isn't already
	while(PIN_LED == 0); {
		reactionLedOn();
	}

  // Initialize reactiontime and get current time before button press
	clock_t reactionTime;
	reactionTime = clock();

  // Main logic
	while (buttonIsPressed != 1) {
		buttonIsPressed = digitalRead(PIN_BUTTON);

		if (buttonIsPressed == 1) {
	  	digitalWrite(PIN_LED, 0);
	  	printf("LED on\n");

      // Calculate reactiontime
	  	reactionTime = clock() - reactionTime;
      int timeInS = calcTimeInS(reactionTime);
      float timeInMS = calcTimeInMS(reactionTime);
      int timeInMSRounded = calcTimeInMSRounded(reactionTime);

      // Print according output
	  	printf("Jouw reactietijd: %d secondes. \n", timeInS);
      printf("Jouw reactietijd: %f milisecondes. \n", timeInMS);
      printf("Jouw reactietijd: %d afgeronde milisecondes.\n", timeInMSRounded);
      printf("Jouw tijd uitgedrukt in bits: ");
      timeInBits(timeInMSRounded);
      setBitLED(timeInMSRounded);
    }
	}
	return 0;
}
