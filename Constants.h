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
#define channelD  3
#define channelE  4

#define channel_legL  channelA
#define channel_dome  channelB
#define channel_legR  channelC
#define channel_mp3   channelD
#define channel_holo  channelE

uint8_t channel = 0;
uint8_t channel_max = 5;


/*  Note: Servos and motors combined draw too much power for the batteries to handle.
 *        To resolve this issue, we set channels for different components to run on.
 *        During the main loop(), only one channel is activated per loop. */


// ================= HARDWARE SERIAL PORTS =================

#define MP3Serial Serial3 // Pins 15/14 = RX/TX to the MP3 trigger soundboard.
#define SRSerial  Serial2 // Pins 17/16 = RX/TX to the SyRen controller.

/*  Note: We need to use the built-in hardware serial ports because SoftwareSerial uses
 *        interrupts which cause the digital pin outputs to be stopped and restarted.
 *        
 *        An example of how this may cause issues: a digital pin outputs a PWM signal at
 *        50% duty cycle. SoftwareSerial communication interrupts the digital pin before
 *        the duty cycle is 50% complete, and thus the digital pin outputs HIGH, gets
 *        interrupted, and begins outputting HIGH again without ever going LOW. */


// ================= SOUND VARIABLES =================

bool mp3Enabled = false; // allow volume adjustment only when true
byte vol = byte(25); // initial volume is 10, with 0 = highest and 40 = lowest

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

int cmd_dome = 0;  // initial dome rotation rate


// ================= LEG VARIABLES =================

const int max_leg_speed = 60; // max foot motor speed. Goes up to 90.
Servo LegL;
Servo LegR;

// ================= HOLO-PROJECTOR VARIABLES =================

const uint8_t HP_LED[3] = {31, 38, 45}; // digital pinouts for holo-projector LEDs
const uint8_t HP_S1_pin[3]  = {33, 42, 47}; // digital pinouts for holo-projector servo 1
const uint8_t HP_S2_pin[3]  = {35, 40, 49}; // digital pinouts for holo-projector servo 2

const uint8_t HP_S1_center[3] = {75, 80, 110};  // neutral angle of servo 1 for each holoprojector (S1)
const uint8_t HP_S2_center[3] = {75, 90, 75};  // neutral angle of servo 2 for each holoprojector (S2)
const uint8_t servo_rot_max = 30; // max rotation of holo-projector servos about neutral point

// LED modes - initialize off
uint8_t LED_mode[3] = {0, 0, 0}; // which mode the HP LED is on
const uint8_t max_LED_mode = 4; // how many modes of HP LEDs there are.
unsigned long LED_timer[3] = {0, 0, 0}; // timer for blinking LED
bool LED_state[3] = {false, false, false};


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
Servo HP_S1[3] = {HP1S1, HP2S1, HP3S1};
Servo HP_S2[3] = {HP1S2, HP2S2, HP3S2};
