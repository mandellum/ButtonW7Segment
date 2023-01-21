// Countup with button for MAX7219 8 Digit 7-Segment display
//

#include <LedControl.h> // LedControl Library created by Eberhard Fahle at http://playground.arduino.cc/Main/LedControl

#define DEBUG false // false is Debug off, true is Debug on

#if DEBUG == true
#define debug(x) Serial.print(x)
#define debugln(x) Serial.println(x)
#else
#define debug(x)
#define debugln(x)
#endif

#define Max7219DIN 12 // Pin 12 connected to DIN (DataIN)
#define Max7219CLK 11 // Pin 11 connected to CLK
#define Max7219CS 10  // Pin 10 connected to CS
#define BUTTON_PIN 2  // Pin 2 connected to BUTTON Input
#define RELAY_PIN 4   // Pin 3 connected to RELAY

LedControl lc = LedControl(Max7219DIN, Max7219CLK, Max7219CS, 1); // Last number represent the number of Max7219 Modules connected

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
int previousButtonState = HIGH; // Not pressed = high
unsigned long lastPress = 0;
unsigned long millisLedIdle = 0;    // Time that LED was last turned on
unsigned long idleWaitTime = 15000; // 15s
unsigned long blinkDelay = 700;     // 0.7s

long int sendCount = 0; // for sending count thru serial
long int recieveMessageBuffer = 0;
long int buttonCount;

bool recievedCountUpdate = false; // Did we recieve update from PC?
bool idleLightsOn = false;
bool idleState = false;

void setup()
{
  Serial.begin(9600); // initialize serial baud rate at 9600 - in tests, it worked well

  lc.shutdown(0, false); // Wake up the display
  lc.setIntensity(0, 5); // Set Brightness 0-15
  lc.clearDisplay(0);    // Clear display

  pinMode(BUTTON_PIN, INPUT_PULLUP); // set button to pull-up mode
  pinMode(RELAY_PIN, OUTPUT);        // set up Relay
  pinMode(LED_BUILTIN, OUTPUT);      // set up built-in LED
  randomSeed(analogRead(0));
  // buttonCount = random(0, 9990);
  buttonCount = 8888; // initialize button count
}

void loop()
{
  updateCount();

  int buttonState = digitalRead(BUTTON_PIN); // read new state

  if (buttonState == LOW)
  {                                  // if button is pressed
    digitalWrite(LED_BUILTIN, HIGH); // turn on LED
    digitalWrite(RELAY_PIN, HIGH);   // turn on Relay

    if (previousButtonState == HIGH)
    { // if button was last unpressed
      onButtonPress();
    }
  }
  else if (buttonState == HIGH && previousButtonState == LOW) // button is not pressed && was just pressed
  {
    digitalWrite(LED_BUILTIN, LOW); // turn off LED
    digitalWrite(RELAY_PIN, LOW);   // turn off Relay
  }

  //  Display number on Display depending on number of digits remaining
  updateSevenSegmentDisplay();
  if (millis() > lastPress + idleWaitTime) // if time passed idle wait time
  {
    idleState = true;
    idleProcess();
  }
  previousButtonState = buttonState;
}

void updateSevenSegmentDisplay()
{
  if (buttonCount > 9999999)
  {
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
  }
  else
  {
    if (buttonCount > 999999)
    {
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
    }
    else
    {
      if (buttonCount > 99999)
      {
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
      }
      else
      {
        if (buttonCount > 9999)
        {
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
        }
        else
        {
          if (buttonCount > 999)
          {
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
          }
          else
          {
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
}

void onButtonPress()
{
  lastPress = millis(); // resets Attract Loop
  idleState = false;
  Serial.println("Down"); // Outputs command to PC
  buttonCount++;
  delay(25); // reduces accidental double inputs
}

void idleProcess()
{
  updatePCDuringIdle();
  idleAnimation();
}

void updatePCDuringIdle()
{
  if (sendCount != buttonCount)
  {
    sendCount = buttonCount;
    if (recievedCountUpdate == true)
    {
      Serial.println(sendCount); // updates sendCount and outputs it to PC
      debugln("Updating count to PC");
    }
  }
}

void idleAnimation()
{
  if (millis() > millisLedIdle + blinkDelay && idleLightsOn == true)
  {
    digitalWrite(RELAY_PIN, LOW);   // turn off relay
    digitalWrite(LED_BUILTIN, LOW); // turn off built-in
    idleLightsOn = false;
  }

  if (millis() > millisLedIdle + 3 * blinkDelay && idleLightsOn == false)
  {
    digitalWrite(RELAY_PIN, HIGH);   // turn on relay
    digitalWrite(LED_BUILTIN, HIGH); // turn on built-in
    idleLightsOn = true;
    millisLedIdle = millis();
  }
}

void updateCount() // runs once per loop()
{
  // Check to see if anything exists in the serial receive buffer
  if (Serial.available() > 0)
  {
    if (recievedCountUpdate == false) // flip boolean to true once
    {
      recievedCountUpdate = !recievedCountUpdate;
    }

    while (Serial.available() > 0) // runs WHILE there's something in the serial recieve buffer
    {
      char c = Serial.read();
      if (c == '\n')
      {
        // Full message received
        buttonCount = recieveMessageBuffer; // moves count
        recieveMessageBuffer = 0;           // clears buffer
        debug("buttonCount updated: ");
        debugln(buttonCount);
      }
      else
      {
        // take Serial character, convert to number, then shift into next digit.
        recieveMessageBuffer = recieveMessageBuffer * 10;
        recieveMessageBuffer = recieveMessageBuffer + (c - '0'); // Subtract '0' to adjust from ascii back to real numbers
      }
    }
  }
}