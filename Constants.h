#include <Servo.h>


// ================= BAUD RATES =================

#define BAUD_SERIAL   115200
#define BAUD_SYREN    9600
#define BAUD_MP3      38400
#define BAUD_SABER    9600


// ================= CHANNELS =================

#define channelA  0
#define channelB  1
#define channelC  2

#define channel_legs  channelA
#define channel_dome  channelB
#define channel_mp3   channelC
#define channel_holo  channelC

uint8_t channel = 0;
uint8_t channel_max = 3;

/*  Note: Servos and motors combined draw too much power for the batteries to handle.
 *        To resolve this issue, we set channels for different components to run on.
 *        During the main loop(), only one channel is activated per loop. */


// ================= HARDWARE SERIAL PORTS =================

#define MP3Serial Serial1 // Pins 15/14 = RX/TX to the MP3 trigger soundboard.
#define SRSerial  Serial2 // Pins 17/16 = RX/TX to the SyRen controller.
#define STSerial  Serial3 // Pins 19/18 = RX/TX to the Sabertooth controller.

/*  Note: We need to use the built-in hardware serial ports because SoftwareSerial uses
 *        interrupts which cause the digital pin outputs to be stopped and restarted.
 *        
 *        An example of how this may cause issues: a digital pin outputs a PWM signal at
 *        50% duty cycle. SoftwareSerial communication interrupts the digital pin before
 *        the duty cycle is 50% complete, and thus the digital pin outputs HIGH, gets
 *        interrupted, and begins outputting HIGH again without ever going LOW. */


// ================= SOUND VARIABLES =================

bool mp3Enabled = false; // allow volume adjustment only when true
byte vol = byte(10); // initial volume is 10, with 0 = highest and 40 = lowest

const byte scream = byte(1);
const byte annoyed = byte(8);
const byte misc17 = byte(21);
const byte chortle = byte(2);

const byte sent3 = byte(34);
const byte doodoo = byte(3);
const byte wolfwhistle = byte(4);
const byte shortcircuit = byte(6);

const byte theme = byte(9);
const byte cantina = byte(10);
const byte emperor = byte(11);
const byte chorus = byte(12);

const byte leia = byte(54);
byte random1; // assign this byte when button is pressed
const byte sent17 = byte(48);
const byte ooh1 = byte(25);

/*  Note: The MP3 trigger soundboard takes 1- or 2-byte commands. The R2D2 plays soundtracks
 *        loaded on the microSD card, which is in turn mounted to the MP3 trigger sounboard.
 *        
 *        The files stored on the microSD should be named XXXNNN.mp3, where NNN is a zero-padded
 *        number from 001 to 999, and XXX is any string of any length.
 *        
 *        Send one byte (byte("t")) to indicate track selection, and then send the second byte
 *        to indicate the track number (byte(NNN)). */


// ================= DOME VARIABLES =================

int cmd_dome = 0;  // dome rotation rate


// ================= HOLO-PROJECTOR VARIABLES =================

const uint8_t HP1_LED = 30;   // digital pinouts for holo-projector 1
const uint8_t HP1_S1  = 32;   //    includes 1 LED and 2 servos
const uint8_t HP1_S2  = 34;

const uint8_t HP2_LED = 38;   // digital pinouts for holo-projector 2
const uint8_t HP2_S1  = 40;
const uint8_t HP2_S2  = 42;

const uint8_t HP3_LED = 46;   // digital pinouts for holo-projector 3
const uint8_t HP3_S1  = 48;
const uint8_t HP3_S2  = 50;

const uint8_t HP1S1_center = 75;  // neutral angle of holo-projector 1, servo 1 (1S1)
const uint8_t HP1S2_center = 80;  // neutral angle of holo-projector 1, servo 2 (1S2)
const uint8_t HP2S1_center = 90;
const uint8_t HP2S2_center = 80;  // these values should be calibrated after setting the servo to 90
const uint8_t HP3S1_center = 110; //    and then attaching the servo arms parallel to the motor mount.
const uint8_t HP3S2_center = 75;  // when the holo-projector is pointed down, the motors should be
                              //    arranged in a clockwise order with arms pointed clockwise also.

const uint8_t servo_rot_max = 30; // max rotation of holo-projector servos about neutral point

const unsigned long doubleClickMinDelay = 30;
const unsigned long doubleClickMaxDelay = 500;
unsigned long lastClick[3] = {0,0,0};
unsigned long thisClick = 0;

bool hpEnable[3] = {false,false,false};

Servo HP1S1;  // create servo objects
Servo HP1S2;
Servo HP2S1;
Servo HP2S2;
Servo HP3S1;
Servo HP3S2;

