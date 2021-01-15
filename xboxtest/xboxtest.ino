#include <XBOXRECV.h>
#include <Servo.h>
#include "Constants.h"
#include "SyRenSimplified.h"
#include "Sabertooth.h"
//#ifdef dobogusinclude
//#include <spi4teensy3.h>
//#endif

USB Usb;
XBOXRECV Xbox(&Usb);

SyRenSimplified DOME(SRSerial);   // We'll name the SyRen object DOME b/c it controls the dome
Sabertooth LEGS(128, STSerial);   // We'll name the Sabertooth object LEGS by the same logic

void setup()
{
  TCCR1B = TCCR1B & 0b11111000 | 0x02;
  Serial.begin(115200);
  if (Usb.Init() == -1)
  {
    Serial.print(F("\r\nOSC did not start"));
    while(1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));

  // Begin serial communications
  SRSerial.begin(9600);   // Start SyRen communication (dome motors). MUST be 9600 baud
                          // To configure DIP switches, look up "SyRen DIP Wizard" or go here:
                          // https://www.dimensionengineering.com/datasheets/SyrenDIPWizard/start.htm
  MP3Serial.begin(38400); // Start MP3 trigger soundboard communication. MUST be 38400 baud
                          // To change the baud rate, an initialization file is needed. See:
                          // https://learn.sparkfun.com/tutorials/mp3-trigger-hookup-guide-v24/
  STSerial.begin(9600);   // Start Sabertooth communication (leg motors).
                          // To configure DIP switches, look up "Sabertooth DIP Wizard" or go here:
                          // https://www.dimensionengineering.com/datasheets/SabertoothDIPWizard/start.htm
  
  pinMode(LED_BUILTIN, OUTPUT);

  // Set sound to max
  MP3Serial.write('v');
  MP3Serial.write(vol);

  // Set digital pins to output for holo-projectors
  pinMode(HP1_LED, OUTPUT);
  HP1S1.attach(HP1_S1);
  HP1S2.attach(HP1_S2);
  pinMode(HP2_LED, OUTPUT);
  HP2S1.attach(HP2_S1);
  HP2S2.attach(HP2_S2);
  pinMode(HP3_LED, OUTPUT);
  HP3S1.attach(HP3_S1);
  HP3S2.attach(HP3_S2);

  // Set initial positions for holo-projector servos
  HP1S1.write(HP1S1_center); // Center for HP 1 servo 1
  HP1S2.write(HP1S2_center); // Center for HP 1 servo 2
  HP2S1.write(HP2S1_center); // Center for HP 2 servo 1
  HP2S2.write(HP2S2_center); // Center for HP 2 servo 2
  HP3S1.write(HP3S1_center); // Center for HP 3 servo 1
  HP3S2.write(HP3S2_center); // Center for HP 3 servo 2
}


void loop()
{ 
  Usb.Task();
  if(Xbox.XboxReceiverConnected) 
  {
    for(uint8_t i=0;i<1;i++) 
    {

      
      // ================= SOUND CONTROLS =================

      if (Xbox.getButtonClick(L1, i)) {
        changeVolume(-4);
        MP3Serial.write('v');
        MP3Serial.write(vol);
        continue;
      }
      if (Xbox.getButtonClick(R1, i)) {
        changeVolume(4);
        MP3Serial.write('v');
        MP3Serial.write(vol);
        continue;
      }
      
      if (Xbox.getButtonPress(DOWN, i)) {
        if (Xbox.getButtonClick(A, i)) {
          MP3Serial.write('t');
          MP3Serial.write(theme);
        }
        else if (Xbox.getButtonClick(B, i)) {
          MP3Serial.write('t');
          MP3Serial.write(cantina);
        }
        else if (Xbox.getButtonClick(Y, i)) {
          MP3Serial.write('t');
          MP3Serial.write(emperor);
        }
        else if (Xbox.getButtonClick(X, i)) {
          MP3Serial.write('t');
          MP3Serial.write(chorus);
        }
        continue;
      }
      if (Xbox.getButtonPress(UP, i)) {
        if (Xbox.getButtonClick(A, i)) {
          MP3Serial.write('t');
          MP3Serial.write(scream);
        }
        else if (Xbox.getButtonClick(B, i)) {
          MP3Serial.write('t');
          MP3Serial.write(annoyed);
        }
        else if (Xbox.getButtonClick(Y, i)) {
          MP3Serial.write('t');
          MP3Serial.write(misc17);
        }
        else if (Xbox.getButtonClick(X, i)) {
          MP3Serial.write('t');
          MP3Serial.write(chortle);
        }
        continue;
      }
      if (Xbox.getButtonPress(RIGHT, i)) {
        if (Xbox.getButtonClick(A, i)) {
          MP3Serial.write('t');
          MP3Serial.write(sent3);
        }
        else if (Xbox.getButtonClick(B, i)) {
          MP3Serial.write('t');
          MP3Serial.write(doodoo);
        }
        else if (Xbox.getButtonClick(Y, i)) {
          MP3Serial.write('t');
          MP3Serial.write(wolfwhistle);
        }
        else if (Xbox.getButtonClick(X, i)) {
          MP3Serial.write('t');
          MP3Serial.write(shortcircuit);
        }
        continue;
      }
      if (Xbox.getButtonPress(LEFT, i)) {
        if (Xbox.getButtonClick(A, i)) {
          MP3Serial.write('t');
          MP3Serial.write(leia);
        }
        else if (Xbox.getButtonClick(B, i)) {
          MP3Serial.write('t');
          MP3Serial.write(byte(ceil(random(0,54))));
        }
        else if (Xbox.getButtonClick(Y, i)) {
          MP3Serial.write('t');
          MP3Serial.write(sent17);
        }
        else if (Xbox.getButtonClick(X, i)) {
          MP3Serial.write('t');
          MP3Serial.write(ooh1);
        }
        continue;
      }
      
      // ================= LEG CONTROLS =================

//      LEGS.turn(mapCtrl_legs(Xbox.getAnalogHat(LeftHatX, i)));
//      LEGS.drive(mapCtrl_legs(Xbox.getAnalogHat(LeftHatY, i)));

      
      // ================= DOME CONTROLS =================

      DOME.motor(-mapCtrl_dome(Xbox.getButtonPress(L2, i)) + mapCtrl_dome(Xbox.getButtonPress(R2, i)));
      
      // ================= HOLO-PROJECTOR CONTROLS =================
        if (Xbox.getButtonPress(B, i)) {
          HP1S1.write(HP1S1_center + mapCtrl_holo(Xbox.getAnalogHat(LeftHatX, i)));
          HP1S2.write(HP1S2_center + mapCtrl_holo(Xbox.getAnalogHat(LeftHatY, i)));
        }
      
    }

  // Add brief delay between checking different controllers
  delay(1);
  }
}


int mapCtrl_dome(int input) {
  static const int maxIn = 255;
  static const int maxOut = 255;
  static const int deadzone = 20;
  int output = map(max(input,deadzone),deadzone,maxIn,0,maxOut);
  return output;
}


int mapCtrl_legs(int input) {
  static const int maxIn = 32767;
  static const int maxOut = 127;
  static const int deadzone = 14000;
  int output = map(constrain(abs(input),deadzone,maxIn),deadzone,maxIn,0,maxOut) * sgn(input);
  return output;
}

int mapCtrl_holo(int input){
  static const int maxIn = 32767;
  static const int maxOut = servo_rot_max;
  int output = map(max(input,-maxIn),0,maxIn,0,maxOut);
  return output;
}


void changeVolume(int delta) {
  static int v = (int) vol - delta;
  v = (v>=40) ? 40 : ((v<0) ? 0 : v); // the max volume is 0, and the min volume is 40.
  vol = byte(v);
}


template <typename T> int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}
