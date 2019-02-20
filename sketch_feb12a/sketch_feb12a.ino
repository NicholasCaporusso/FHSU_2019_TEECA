/* */
const int gateSpins = 700;

/* ENTRY GATE */
unsigned long entryGate_millis = 0;
const long entryGateInterval = 3;
int entryGateCurrentSpins=0;
int entryGate_Pin0 = 2;
int entryGate_Pin1 = 3;
int entryGate_Pin2 = 4;
int entryGate_Pin3 = 5;
int entryGate_step = 0;
int entryGate_status=-1;

/* EXIT GATE */
unsigned long exitGate_millis = 0;
const long exitGateInterval = 2000;
int exitGate_Pin0 = 6;
int exitGate_Pin1 = 7;
int exitGate_Pin2 = 8;
int exitGate_Pin3 = 9;
int exitGate_step = 0;
int isExitGateOpen = true;

// defines pins numbers
const int entryUltrasound_PinTrigger = 10;
const int entryUltrasound_PinEcho = 11;
const int LEDpin = 13;
const int threshold = 3;
int entryUltrasoundStatus = 0;
unsigned long entryUltrasoundStatus_micros = 0;
// defines variables
long entryUltrasound_duration;
int entryUltrasound_distance;

void setup() {
  pinMode(entryGate_Pin0, OUTPUT);
  pinMode(entryGate_Pin1, OUTPUT);
  pinMode(entryGate_Pin2, OUTPUT);
  pinMode(entryGate_Pin3, OUTPUT);
  pinMode(exitGate_Pin0, OUTPUT);
  pinMode(exitGate_Pin1, OUTPUT);
  pinMode(exitGate_Pin2, OUTPUT);
  pinMode(exitGate_Pin3, OUTPUT);
  pinMode(entryUltrasound_PinTrigger, OUTPUT); // Sets the trigPin as an Output
  pinMode(entryUltrasound_PinEcho, INPUT); // Sets the echoPin as an Input
  pinMode(LEDpin, OUTPUT); // Sets the LEDpin as Output
  Serial.begin(9600); // Starts the serial communication
}

void loop(){
  unsigned long currentMicros=micros();
  unsigned long currentMillis=millis();
  switch(entryGate_status){
    case 1:
      Serial.println("Open");
      entryUltrasound(currentMicros);
      break;
    case 0:
      Serial.println("Closing");
      entryGateClose(currentMillis);
      break;
    case -1:
      Serial.println("Opening");
      entryGateOpen(currentMillis);
      break;
  }
}

void entryUltrasound(unsigned long currentMicros) {
  switch(entryUltrasoundStatus){
    case 0: // reset
      digitalWrite(entryUltrasound_PinTrigger, LOW);
      entryUltrasoundStatus = 1;
      entryUltrasoundStatus_micros = currentMicros;
      break;
    case 1: // beam
      if (currentMicros - entryUltrasoundStatus_micros < 2) return;
      digitalWrite(entryUltrasound_PinTrigger, HIGH);
      entryUltrasoundStatus = 2;
      entryUltrasoundStatus_micros = currentMicros;
      break;
    case 2: // read
      if (currentMicros - entryUltrasoundStatus_micros < 10) return;
      digitalWrite(entryUltrasound_PinTrigger, LOW);
      // Reads the echoPin, returns the sound wave travel time in microseconds
      entryUltrasound_duration = pulseIn(entryUltrasound_PinEcho, HIGH);
      entryUltrasoundStatus=0;
      // Calculating the distance
      entryUltrasound_distance = entryUltrasound_duration * 0.034 / 2;
      Serial.print("Distance: ");
      Serial.println(entryUltrasound_distance);
      if(entryUltrasound_distance<threshold) entryGate_status=0;
      break;
  }
}

void entryGateOpen(unsigned long currentMillis) {
  if(entryGate_status==1) return;
  if(currentMillis - entryGate_millis < entryGateInterval) return;
  digitalWrite(LEDpin, HIGH); // turn the LED on (HIGH is the voltage level)
  entryGate_millis = currentMillis;
  switch (entryGate_step) {
    case 0:
      digitalWrite(entryGate_Pin0, HIGH);
      digitalWrite(entryGate_Pin1, LOW);
      digitalWrite(entryGate_Pin2, LOW);
      digitalWrite(entryGate_Pin3, LOW);
      break;
    case 1:
      digitalWrite(entryGate_Pin0, LOW);
      digitalWrite(entryGate_Pin1, HIGH);
      digitalWrite(entryGate_Pin2, LOW);
      digitalWrite(entryGate_Pin3, LOW);
      break;
    case 2:
      digitalWrite(entryGate_Pin0, LOW);
      digitalWrite(entryGate_Pin1, LOW);
      digitalWrite(entryGate_Pin2, HIGH);
      digitalWrite(entryGate_Pin3, LOW);
      break;
    case 3:
      digitalWrite(entryGate_Pin0, LOW);
      digitalWrite(entryGate_Pin1, LOW);
      digitalWrite(entryGate_Pin2, LOW);
      digitalWrite(entryGate_Pin3, HIGH);
      break;
  }
  entryGate_step++;
  if (entryGate_step > 3) {
    entryGate_step = 0;
  }
  entryGateCurrentSpins++;
  if (entryGateCurrentSpins >= gateSpins) {
    entryGate_status=1;
    entryGateCurrentSpins = 0;
  }
}
void entryGateClose(unsigned long currentMillis) {
  if (entryGate_status==-1) return;
  if (currentMillis - entryGate_millis < entryGateInterval) return;
  digitalWrite(LEDpin, HIGH); // turn the LED on (HIGH is the voltage level)
  entryGate_millis = currentMillis;
  switch (entryGate_step) {
    case 0:
      digitalWrite(entryGate_Pin0, LOW);
      digitalWrite(entryGate_Pin1, LOW);
      digitalWrite(entryGate_Pin2, LOW);
      digitalWrite(entryGate_Pin3, HIGH);
      break;
    case 1:
      digitalWrite(entryGate_Pin0, LOW);
      digitalWrite(entryGate_Pin1, LOW);
      digitalWrite(entryGate_Pin2, HIGH);
      digitalWrite(entryGate_Pin3, LOW);
      break;
    case 2:
      digitalWrite(entryGate_Pin0, LOW);
      digitalWrite(entryGate_Pin1, HIGH);
      digitalWrite(entryGate_Pin2, LOW);
      digitalWrite(entryGate_Pin3, LOW);
      break;
    case 3:
      digitalWrite(entryGate_Pin0, HIGH);
      digitalWrite(entryGate_Pin1, LOW);
      digitalWrite(entryGate_Pin2, LOW);
      digitalWrite(entryGate_Pin3, LOW);
      break;
  }
  entryGate_step++;
  if (entryGate_step > 3) {
    entryGate_step = 0;
  }
  entryGateCurrentSpins++;
  if (entryGateCurrentSpins >= gateSpins) {
    entryGate_status = -1;
    digitalWrite(LEDpin, LOW); // turn the LED on (HIGH is the voltage level)
    entryGateCurrentSpins = 0;
  }
}
