#include <LedControl.h>

const int batteryRead = A0;
const int relayControl = 4;

const float voltDivideRatio = 12.90 / 4.1;
const float chargeCutoffVoltage = 12.7;
const float A2V = 5.0 / 1023.0;
const float V2A = 1.0 / A2V;
const float highVolt = 12.5; // Voltage at which CBI has all LED lit
const float lowVolt = 11; // Voltage at which CBI has no LED lit
const float CBILedStep = floor((highVolt - lowVolt)/25); // Voltage step size for each CBI LED to turn on
const float greenVolt = 12.4; // Voltage above which CBI green LED is on
const float yellowVolt = 12; // Voltage above which CBI yellow LED is on


const long chargeTime = 1200000; // ms, 20 min
const long settleTime = 120000; // ms, 2 min

long prevTime;
long currTime;
boolean settled = false;

float val = 0;

LedControl lc=LedControl(13,12,11,2); 
// Connect (D)ataIn to pin 13
// Connect (C)LK to pin 12
// Connect (L)oad to pin 11

void setup() {
  Serial.begin(9600);
  pinMode(relayControl, OUTPUT);
  digitalWrite(relayControl, LOW);

  delay(5000);
  val = analogRead(batteryRead) *A2V * voltDivideRatio;
  updateCBI(val, CBILedStep, lowVolt, greenVolt, yellowVolt);
  
  prevTime = millis();
  currTime = prevTime;
}

void loop() {
  currTime = millis();
  
  // Update DP Logic  
   animateDPLogic(currTime);

   // disconnect, settle battery voltage
  if (currTime - prevTime > settleTime && !settled)
  {
    // check voltage to determine if battery needs charge
    val = analogRead(batteryRead) *A2V * voltDivideRatio;
    Serial.println(val);
    settled = true;

    if ( val < chargeCutoffVoltage)
    {
      digitalWrite(relayControl, HIGH);
    }
    updateCBI(val, CBILedStep, lowVolt, greenVolt, yellowVolt);
    prevTime = millis();
  }

  if (currTime - prevTime > chargeTime && settled)
  { 
    settled = false;
    digitalWrite(relayControl, LOW);
    prevTime = millis();
  }
}

  
void updateCBI(unsigned long val, float CBILedStep, float lowVolt, float greenVolt, float yellowVolt)
{
  int totLEDOn = floor((val - lowVolt) / CBILedStep);
  int numRowsOn = floor(totLEDOn / 5);  // Find out how many rows are fully on
  int numLEDOn = totLEDOn % 5; // Find out how many LEDs are on in the next row

  // set rows that are all on 
  if (numRowsOn >= 5)
  {
    numRowsOn = 5;
    numLEDOn = 0;
  }

  // Set LEDs in a partially ON row
  for (int col = 0; col++; col < 5)
  {
    if (col < numLEDOn)
    {
      lc.setLed(0, numRowsOn, col, true);
    }
    else {lc.setLed(0, numRowsOn, col, false);}
  }

  // Update red, yellow, green indicators
  if (val > greenVolt)
  {
    lc.setLed(0, 4, 5, true); // green
    lc.setLed(0, 5, 5, false); // yellow
    lc.setLed(0, 6, 5, false); // red
  }
  else if (val > yellowVolt)
  {
    lc.setLed(0, 4, 5, false); // green
    lc.setLed(0, 5, 5, true); // yellow
    lc.setLed(0, 6, 5, false); // red
  }
  else {
    lc.setLed(0, 4, 5, false); // green
    lc.setLed(0, 5, 5, false); // yellow
    lc.setLed(0, 6, 5, true); // red
  }
}
  
void animateDPLogic(unsigned long elapsed)
{
  static unsigned long timeLast=0;
  if ((elapsed - timeLast) < 750) return;
  timeLast = elapsed; 

  int DPdev=1;
    for (int row=0; row<6; row++)
      lc.setRow(DPdev,row,random(0,256));
}  
