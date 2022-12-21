/* Countup with button for MAX7219 8 Digit 7-Segment display

*/
#include "LedControl.h"  // LedControl Library created by Eberhard Fahle at http://playground.arduino.cc/Main/LedControl

#define DEBUG 1 // 0 is Debug off, 1 is Debug on

#if DEBUG == 1
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

#define Max7219DIN 12  // Pin 12 connected to DIN (DataIN)
#define Max7219CLK 11  // Pin 11 connected to CLK
#define Max7219CS 10   // Pin 10 connected to CS
#define BUTTON_PIN 2   // Pin 2 connected to BUTTON Input
#define RELAY_PIN 4    // Pin 3 connected to RELAY



LedControl lc = LedControl(Max7219DIN, Max7219CLK, Max7219CS, 1);  // Last number represent the number of Max7219 Modules connected

// Variables to store individual numbers
int firstnum = 0;
int secondnum = 0;
int thirdnum = 0;
int fournum = 0;
int fivenum = 0;
int sixnum = 0;
int sevennum = 0;
int eightnum = 0;

// Variables for button
int previousButtonState = HIGH;  // Not pressed = high
unsigned long lastPress = 0;
unsigned long lastBlink = 0;
unsigned long idleWaitTime = 10000;  // 10s
unsigned long blinkDelay = 700;      // 0.7s

long int sentCount = 0;     // for sending count thru serial
long int buttonCount = 0;
long int incomingByte = 0;  // for incoming serial data

void setup() {
  Serial.begin(9600);  // initialize serial baud rate at 9600 - in tests, it worked well

  lc.shutdown(0, false);  // Wake up the display
  lc.setIntensity(0, 2);  // Set Brightness 0-15
  lc.clearDisplay(0);     // Clear display

  pinMode(BUTTON_PIN, INPUT_PULLUP);  //set button to pull-up mode
  pinMode(LED_BUILTIN, OUTPUT);       // set up built-in LED
  randomSeed(analogRead(0));
  // buttonCount = random(0, 9990);
  buttonCount = 3268;
}

void loop() {

  if (Serial.available() > 0) {  // Prints if serial recieves message
    incomingByte = Serial.read();
    debug("I received: ");
    debugln(incomingByte, DEC);
  }

  int buttonState = digitalRead(BUTTON_PIN);  // read new state

  if (buttonState == LOW) {           // if button is pressed
    digitalWrite(LED_BUILTIN, HIGH);  // turn on LED
    digitalWrite(RELAY_PIN, HIGH);    // turn on Relay

    if (previousButtonState == HIGH) {  // if button was last unpressed
      onButtonPress();
    }
  } else if (buttonState == HIGH) {
    digitalWrite(LED_BUILTIN, LOW);  // turn off LED
    digitalWrite(RELAY_PIN, LOW);    // turn off Relay
  }

  //  Display number on Display depending on number of digits remaining
  if (buttonCount > 9999999) {
    firstnum = ((buttonCount / 10000000) % 10);
    secondnum = buttonCount / 1000000 % 10;
    thirdnum = buttonCount / 100000 % 10;
    fournum = buttonCount / 10000 % 10;
    fivenum = buttonCount / 1000 % 10;
    sixnum = buttonCount / 100 % 10;
    sevennum = buttonCount / 10 % 10;
    eightnum = buttonCount % 10;

    lc.setDigit(0, 7, firstnum, false);
    lc.setDigit(0, 6, secondnum, false);
    lc.setDigit(0, 5, thirdnum, false);
    lc.setDigit(0, 4, fournum, false);
    lc.setDigit(0, 3, fivenum, false);
    lc.setDigit(0, 2, sixnum, false);
    lc.setDigit(0, 1, sevennum, false);
    lc.setDigit(0, 0, eightnum, false);
  } else {
    if (buttonCount > 999999) {
      //      firstnum = ((buttonCount/10000000)%10);
      secondnum = buttonCount / 1000000 % 10;
      thirdnum = buttonCount / 100000 % 10;
      fournum = buttonCount / 10000 % 10;
      fivenum = buttonCount / 1000 % 10;
      sixnum = buttonCount / 100 % 10;
      sevennum = buttonCount / 10 % 10;
      eightnum = buttonCount % 10;

      lc.setChar(0, 7, ' ', false);
      lc.setDigit(0, 6, secondnum, false);
      lc.setDigit(0, 5, thirdnum, false);
      lc.setDigit(0, 4, fournum, false);
      lc.setDigit(0, 3, fivenum, false);
      lc.setDigit(0, 2, sixnum, false);
      lc.setDigit(0, 1, sevennum, false);
      lc.setDigit(0, 0, eightnum, false);
    } else {
      if (buttonCount > 99999) {
        //          firstnum = ((buttonCount/10000000)%10);
        //          secondnum = buttonCount/1000000%10;
        thirdnum = buttonCount / 100000 % 10;
        fournum = buttonCount / 10000 % 10;
        fivenum = buttonCount / 1000 % 10;
        sixnum = buttonCount / 100 % 10;
        sevennum = buttonCount / 10 % 10;
        eightnum = buttonCount % 10;

        lc.setChar(0, 7, ' ', false);
        lc.setChar(0, 6, ' ', false);
        lc.setDigit(0, 5, thirdnum, false);
        lc.setDigit(0, 4, fournum, false);
        lc.setDigit(0, 3, fivenum, false);
        lc.setDigit(0, 2, sixnum, false);
        lc.setDigit(0, 1, sevennum, false);
        lc.setDigit(0, 0, eightnum, false);
      } else {
        if (buttonCount > 9999) {
          //             firstnum = ((buttonCount/10000000)%10);
          //             secondnum = buttonCount/1000000%10;
          //             thirdnum = buttonCount/100000%10;
          fournum = buttonCount / 10000 % 10;
          fivenum = buttonCount / 1000 % 10;
          sixnum = buttonCount / 100 % 10;
          sevennum = buttonCount / 10 % 10;
          eightnum = buttonCount % 10;

          lc.setChar(0, 7, ' ', false);
          lc.setChar(0, 6, ' ', false);
          lc.setChar(0, 5, ' ', false);
          lc.setDigit(0, 4, fournum, false);
          lc.setDigit(0, 3, fivenum, false);
          lc.setDigit(0, 2, sixnum, false);
          lc.setDigit(0, 1, sevennum, false);
          lc.setDigit(0, 0, eightnum, false);
        } else {
          if (buttonCount > 999) {
            //              firstnum = ((buttonCount/10000000)%10);
            //              secondnum = buttonCount/1000000%10;
            //              thirdnum = buttonCount/100000%10;
            //              fournum = buttonCount/10000%10;
            fivenum = buttonCount / 1000 % 10;
            sixnum = buttonCount / 100 % 10;
            sevennum = buttonCount / 10 % 10;
            eightnum = buttonCount % 10;

            lc.setChar(0, 7, ' ', false);
            lc.setChar(0, 6, ' ', false);
            lc.setChar(0, 5, ' ', false);
            lc.setChar(0, 4, ' ', false);
            lc.setDigit(0, 3, fivenum, false);
            lc.setDigit(0, 2, sixnum, false);
            lc.setDigit(0, 1, sevennum, false);
            lc.setDigit(0, 0, eightnum, false);
          } else {
            //              firstnum = ((buttonCount/10000000)%10);
            //              secondnum = buttonCount/1000000%10;
            //              thirdnum = buttonCount/100000%10;
            //              fournum = buttonCount/10000%10;
            //              fivenum = buttonCount/1000%10;
            sixnum = buttonCount / 100 % 10;
            sevennum = buttonCount / 10 % 10;
            eightnum = buttonCount % 10;

            lc.setChar(0, 7, ' ', false);
            lc.setChar(0, 6, ' ', false);
            lc.setChar(0, 5, ' ', false);
            lc.setChar(0, 4, ' ', false);
            lc.setChar(0, 3, ' ', false);
            lc.setDigit(0, 2, sixnum, false);
            lc.setDigit(0, 1, sevennum, false);
            lc.setDigit(0, 0, eightnum, false);
          }
        }
      }
    }
  }

  // Idle animation (blinks once for every three counts)
  if (millis() > lastPress + idleWaitTime) {
    if (millis() > lastBlink + 2 * blinkDelay) {
      digitalWrite(RELAY_PIN, HIGH);    // turn on relay
      digitalWrite(LED_BUILTIN, HIGH);  // turn on built-in
      delay(blinkDelay);
      if (sentCount != buttonCount) {
        sentCount = buttonCount;
        Serial.println(sentCount); // Outputs count to PC
      }
      lastBlink = millis();  // Loops pattern
    }
  }

  previousButtonState = buttonState;
}

void onButtonPress() {
  lastPress = millis();    // resets Attract Loop
  Serial.println("Down");  // Outputs command to PC
  buttonCount++;
  delay(25);
}
