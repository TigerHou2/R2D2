#include <XBOXRECV.h>
#include <Servo.h>
#include "Constants.h"
#include "SyRenSimplified.h"

USB Usb;
XBOXRECV Xbox(&Usb);

SyRenSimplified DOME(SRSerial);   // We'll name the SyRen object DOME b/c it controls the dome

void setup()
{
  TCCR1B = TCCR1B & 0b11111000 | 0x02;
  Serial.begin(BAUD_SERIAL);
  
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while(1); //halt
  }
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));

  // Begin serial communications
  SRSerial.begin(BAUD_SYREN);   // Start SyRen communication (dome motors). MUST be 9600 baud
                                // To configure DIP switches, look up "SyRen DIP Wizard" or go here:
                                // https://www.dimensionengineering.com/datasheets/SyrenDIPWizard/start.htm
  MP3Serial.begin(BAUD_MP3);    // Start MP3 trigger soundboard communication. MUST be 38400 baud
                                // To change the baud rate, an initialization file is needed. See:
                                // https://learn.sparkfun.com/tutorials/mp3-trigger-hookup-guide-v24/

  // Set initial volume
  MP3Serial.write('v');
  MP3Serial.write(vol);

  LegR.attach(22);
  LegL.attach(24);
  LegR.write(90);
  LegL.write(90);
  
  // Set digital pins to output mode for holo-projectors
  //    and attach digital pins to motor outputs
  pinMode(HP_LED[0], OUTPUT);
  HP1S1.attach(HP_S1_pin[0]);
  HP1S2.attach(HP_S2_pin[0]);
  pinMode(HP_LED[1], OUTPUT);
  HP2S1.attach(HP_S1_pin[1]);
  HP2S2.attach(HP_S2_pin[1]);
  pinMode(HP_LED[2], OUTPUT);
  HP3S1.attach(HP_S1_pin[2]);
  HP3S2.attach(HP_S2_pin[2]);

  // Set initial positions for holo-projector servos
  //    these values should be calibrated after setting the servo to 90
  //    and then attaching the servo arms parallel to the motor mount
  HP1S1.write(HP_S1_center[0]); // Center for HP 1 servo 1
  HP1S2.write(HP_S2_center[0]); // Center for HP 1 servo 2
  HP2S1.write(HP_S1_center[1]); // Center for HP 2 servo 1
  HP2S2.write(HP_S2_center[1]); // Center for HP 2 servo 2
  HP3S1.write(HP_S1_center[2]); // Center for HP 3 servo 1
  HP3S2.write(HP_S2_center[2]); // Center for HP 3 servo 2

}


void loop()
{ 
  Usb.Task();
  if(Xbox.XboxReceiverConnected) 
  {
    for(uint8_t i=0;i<1;i++) 
    {

      
      // ================= SOUND CONTROLS =================
      //    Trigger a sound effect by pressing and holding any button on the D-pad
      //      and clicking any of the X/Y/A/B buttons.
      //    Control volume by pressing and holding any button on the D-pad
      //      and clicking the L1/R1 bumpers for -/+, respectively.
      
      if (channel_mp3 == channel) {
        mp3Enabled = true;
        if (Xbox.getButtonPress(DOWN, i)) {
          if      (Xbox.getButtonClick(A, i)) { MP3Serial.write('t'); MP3Serial.write(theme); }
          else if (Xbox.getButtonClick(B, i)) { MP3Serial.write('t'); MP3Serial.write(cantina); }
          else if (Xbox.getButtonClick(Y, i)) { MP3Serial.write('t'); MP3Serial.write(emperor); }
          else if (Xbox.getButtonClick(X, i)) { MP3Serial.write('t'); MP3Serial.write(chorus); }
        Serial.println("Here");
        }
        else if (Xbox.getButtonPress(UP, i)) {
          if      (Xbox.getButtonClick(A, i)) { MP3Serial.write('t'); MP3Serial.write(scream); }
          else if (Xbox.getButtonClick(B, i)) { MP3Serial.write('t'); MP3Serial.write(annoyed); }
          else if (Xbox.getButtonClick(Y, i)) { MP3Serial.write('t'); MP3Serial.write(misc17); }
          else if (Xbox.getButtonClick(X, i)) { MP3Serial.write('t'); MP3Serial.write(chortle); }
        }
        else if (Xbox.getButtonPress(RIGHT, i)) {
          if      (Xbox.getButtonClick(A, i)) { MP3Serial.write('t'); MP3Serial.write(sent3); }
          else if (Xbox.getButtonClick(B, i)) { MP3Serial.write('t'); MP3Serial.write(doodoo); }
          else if (Xbox.getButtonClick(Y, i)) { MP3Serial.write('t'); MP3Serial.write(wolfwhistle); }
          else if (Xbox.getButtonClick(X, i)) { MP3Serial.write('t'); MP3Serial.write(shortcircuit); }
        }
        else if (Xbox.getButtonPress(LEFT, i)) {
          if      (Xbox.getButtonClick(A, i)) { MP3Serial.write('t'); MP3Serial.write(leia); }
          else if (Xbox.getButtonClick(B, i)) { MP3Serial.write('t'); MP3Serial.write(byte(ceil(random(0,54)))); }
          else if (Xbox.getButtonClick(Y, i)) { MP3Serial.write('t'); MP3Serial.write(sent17); }
          else if (Xbox.getButtonClick(X, i)) { MP3Serial.write('t'); MP3Serial.write(ooh1); }
        }
        else { mp3Enabled = false; }
        
        if (mp3Enabled) {
          if (Xbox.getButtonClick(L1, i)) {
            changeVolume(-2); MP3Serial.write('v'); MP3Serial.write(vol);}
          if (Xbox.getButtonClick(R1, i)) {
            changeVolume(2); MP3Serial.write('v'); MP3Serial.write(vol);}
          mp3Enabled = false;
        }
      }

      
      // ================= LEG CONTROLS =================
      //    Drive the R2D2 using the left joystick.

      if (channel_legL == channel || channel_legL == channel-1) {
          LegL.write(mapCtrl_legs(-Xbox.getAnalogHat(LeftHatY, i))+mapCtrl_legs(Xbox.getAnalogHat(LeftHatX, i))-90); 
          Serial.println("Left leg:");
          Serial.println(mapCtrl_legs(-Xbox.getAnalogHat(LeftHatY, i)));     
      }

      if (channel_legR == channel || channel_legR == channel-1) {
          LegR.write(mapCtrl_legs(-Xbox.getAnalogHat(LeftHatY, i))-mapCtrl_legs(Xbox.getAnalogHat(LeftHatX, i))+90);
          Serial.println("Right leg:");
          Serial.println(mapCtrl_legs(-Xbox.getAnalogHat(LeftHatY, i)));
      }
      
      // ================= DOME CONTROLS =================
      //    Rotate the dome by pressing L2/R2 triggers.

      if (channel_dome == channel) {
        cmd_dome = - mapCtrl_dome(Xbox.getButtonPress(L2, i)) 
                   + mapCtrl_dome(Xbox.getButtonPress(R2, i));
        DOME.motor(cmd_dome);
      }

      
      // ================= HOLO-PROJECTOR CONTROLS =================
      //    Toggle individual holo-projector controls by double clicking X/Y/B
      //      and using the right joystick to control active holo-projectors.
      //    Press and hold X/Y/B and click L1/R1 bumpers to cycle holo-projector light modes.
      
      if (channel_holo == channel) {

        if (Xbox.getButtonClick(X, i) && checkDoubleClick(1)) {
          hpEnable[0] = !hpEnable[0];
        }
        if (Xbox.getButtonClick(A, i) && checkDoubleClick(2)) {
          hpEnable[1] = !hpEnable[1];
        }
        if (Xbox.getButtonClick(B, i) && checkDoubleClick(3)) {
          hpEnable[2] = !hpEnable[2];
        }
          
        for(int j = 0; j < 3; j++) { // loop through each HP

          if (hpEnable[j]) {
            if (j==1) { // glued in servo 2 incorrectly so need to fix controls with software 
              HP_S1[j].write(HP_S1_center[j] - mapCtrl_holo(Xbox.getAnalogHat(RightHatX, i))); 
            }
            else { HP_S1[j].write(HP_S1_center[j] + mapCtrl_holo(Xbox.getAnalogHat(RightHatX, i))); }
            HP_S2[j].write(HP_S2_center[j] + mapCtrl_holo(Xbox.getAnalogHat(RightHatY, i)));
            if (Xbox.getButtonClick(Y, i)) {
              LED_mode[j] += 1;
              LED_mode[j] = LED_mode[j] % max_LED_mode;
            }
          }

          if (LED_mode[j] == 0) {
            // always off
            digitalWrite(HP_LED[j], LOW);
          }
          else if (LED_mode[j] == 1) { 
            // always on
            digitalWrite(HP_LED[j], HIGH);
          }
          
          else if (LED_mode[j] == 2) {
            // blink in 1 sec intervals
            if (millis() >= LED_timer[j] + 1000) {
              LED_timer[j] = millis();
              LED_state[j] = !LED_state[j];
            }
            if (LED_state[j]) {
              digitalWrite(HP_LED[j], HIGH);
            }
            else {
              digitalWrite(HP_LED[j], LOW);
            }
          }
          
          else if (LED_mode[j] == 3) {
            // blink in 0.5  sec intervals
            if (millis() >= LED_timer[j] + 500) {
              LED_timer[j] = millis();
              LED_state[j] = !LED_state[j];
            }
            if (LED_state[j]) {
              digitalWrite(HP_LED[j], HIGH);
            }
            else {
              digitalWrite(HP_LED[j], LOW);
            }
              
          }
        }
      }
    }

    // switch channels after every loop()
    //    different parts of the R2D2 electronics are active on different channels
    //    only one channel is active per loop()
    //    this is done to prevent components overdrawing current and misbehaving
    channel += 1;
    channel = channel % channel_max;
  }
}


int mapCtrl_dome(int input) {   // map Xbox R2/L2 controls to dome motor inputs
  static const int maxIn = 255;
  static const int maxOut = 255;
  static const int deadzone = 20;
  int output = map(max(input,deadzone),deadzone,maxIn,0,maxOut);
  return output;
}


int mapCtrl_legs(int input) {   // map Xbox left joystick controls to leg motor inputs
  static const int maxIn = 32767;
  static const int deadzone = 14000;
  int temp = max(deadzone,abs(min(max(-maxIn,input),maxIn)));
  int output = map(temp,deadzone,maxIn,0,max_leg_speed) * sgn(input)+90;
  return output;
}


int mapCtrl_holo(int input){    // map Xbox right joystick controls to holo-projector servo inputs
  static const int maxIn = 32767;
  static const int maxOut = servo_rot_max;
  int output = map(max(input,-maxIn),0,maxIn,0,maxOut);
  return output;
}


bool checkDoubleClick(int holoID) {
  thisClick = millis();
  holoID = holoID - 1;
  if ( ((thisClick-lastClick[holoID]) > doubleClickMinDelay) 
     &&((thisClick-lastClick[holoID]) < doubleClickMaxDelay) ) {
    Serial.println(thisClick-lastClick[holoID]);
    return true;  // is a double click
  }
  else {
    lastClick[holoID] = thisClick;
    return false; // is not a double click
  }
}


void changeVolume(int delta) {  // map Xbox R1/L1 controls to MP3 speaker volume controls
  int v = (int) vol - delta;
  v = (v>=40) ? 40 : ((v<0) ? 0 : v); // the max volume is 0, and the min volume is 40
  vol = byte(v);
}


template <typename T> int sgn(T val) {  // returns the integer sign (-1 or 1) of the input
    return (T(0) < val) - (val < T(0));
}
