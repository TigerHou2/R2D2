const int batteryRead = A0;
const int relayControl = 4;

const float voltDivideRatio = 12.90 / 4.1;
const float chargeCutoffVoltage = 12.8;
const float A2V = 5.0 / 1023.0;
const float V2A = 1.0 / A2V;

const long chargeTime = 20000; // ms
const long settleTime = 20000; // ms

float val = 0;

void setup() {
  Serial.begin(9600);
  pinMode(relayControl, OUTPUT);
}

void loop() {
  
  // disconnect, settle battery voltage
  digitalWrite(relayControl, LOW);
  delay(settleTime);

  // check voltage to determine if battery needs charge
  val = analogRead(batteryRead) *A2V * voltDivideRatio;
  Serial.println(val);
  if ( val < chargeCutoffVoltage )
  {
    digitalWrite(relayControl, HIGH);
  }
  
  // charge
  delay(chargeTime);
}
