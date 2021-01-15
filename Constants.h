#include <Servo.h>


// DEBUG
int alternator = 0;


// CHANNELS
// servos and motors combined draw too much power so we set channels for different components
// during the main loop(), only one channel is activated per loop.
#define channelA 0
#define channelB 1
#define channelC 2

#define channel_legs channelA
#define channel_dome channelB
#define channel_mp3  channelC
#define channel_holo channelC

int channel = 0;
int channel_max = 3;


// HARDWARE SERIAL PORTS
#define MP3Serial Serial1 // Pins 15/14 = RX/TX to the MP3 trigger soundboard.
#define SRSerial  Serial2 // Pins 17/16 = RX/TX to the SyRen controller.
#define STSerial  Serial3 // Pins 19/18 = RX/TX to the Sabertooth controller.


// SOUND VARIABLES
byte vol = byte(0);

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


// DOME VARIABLES
int cmd_dome = 0;


// HOLO-PROJECTOR VARIABLES
const int HP1_LED = 30;
const int HP1_S1  = 32;
const int HP1_S2  = 34;

const int HP2_LED = 38;
const int HP2_S1  = 40;
const int HP2_S2  = 42;

const int HP3_LED = 46;
const int HP3_S1  = 48;
const int HP3_S2  = 50;

const int HP1S1_center = 80;
const int HP1S2_center = 70;
const int HP2S1_center = 80;
const int HP2S2_center = 70;
const int HP3S1_center = 80;
const int HP3S2_center = 70;
const int servo_rot_max = 30;

Servo HP1S1;
Servo HP1S2;
Servo HP2S1;
Servo HP2S2;
Servo HP3S1;
Servo HP3S2;

