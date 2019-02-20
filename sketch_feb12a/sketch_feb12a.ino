/* PARKING SETTINGS */
const int spots_available=15;
int spots_current=15;

/* DO NOT MODIFY BELOW*/
const int motorSpins = 700; // steps
const int motorInterval = 3;
const int sensorThreshold = 3;

/* ENTRY GATE */
// Motor
const int entry1_motor_pin0 = 2;
const int entry1_motor_pin1 = 3;
const int entry1_motor_pin2 = 4;
const int entry1_motor_pin3 = 5;
unsigned long entry1_motor_millis = 0;
int entry1_motor_currentSpins = 0;
int entry1_motor_step = 0;
int entry1_motor_status=-1;
// Ultrasound
const int entry1_sensor_pinTrigger = 10;
const int entry1_sensor_pinEcho = 11;
int entry1_sensor_status = 0;
unsigned long entry1_sensor_micros = 0;
// LED
const int entry1_LED_pin = 13;

/* EXIT GATE */
// Motor
const int exit1_motor_pin0 = 6;
const int exit1_motor_pin1 = 7;
const int exit1_motor_pin2 = 8;
const int exit1_motor_pin3 = 9;
unsigned long exit1_motor_millis = 0;
int exit1_motor_currentSpins = 0;
int exit1_motor_step = 0;
int exit1_motor_status=-1;

void setup() {
  pinMode(entry1_motor_pin0, OUTPUT);
  pinMode(entry1_motor_pin1, OUTPUT);
  pinMode(entry1_motor_pin2, OUTPUT);
  pinMode(entry1_motor_pin3, OUTPUT);
  
  pinMode(exit1_motor_pin0, OUTPUT);
  pinMode(exit1_motor_pin1, OUTPUT);
  pinMode(exit1_motor_pin2, OUTPUT);
  pinMode(exit1_motor_pin3, OUTPUT);
  pinMode(entry1_sensor_pinTrigger, OUTPUT); // Sets the trigpin as an Output
  pinMode(entry1_sensor_pinEcho, INPUT); // Sets the echopin as an Input
  pinMode(entry1_LED_pin, OUTPUT); // Sets the entry1_LED_pin as Output
  Serial.begin(9600); // Starts the serial communication
}

void loop(){
  unsigned long currentMicros=micros();
  unsigned long currentMillis=millis();
  check_entry1(currentMicros,currentMillis);
  currentMicros=micros();
  currentMillis=millis();
  check_exit1(currentMicros,currentMillis);
}

void check_entry1(long currentMicros,long currentMillis){
  switch(entry1_motor_status){
    case 1:
      Serial.println("Entry gate: open");
      entry1_sensorMeasure(currentMicros);
      break;
    case 0:
      Serial.println("Entry gate: closing...");
      entry1_motorClose(currentMillis);
      break;
    case -1:
      Serial.println("Entry gate: closed");
      entry1_motorOpen(currentMillis);
      break;
  }
}

void check_exit1(long currentMicros,long currentMillis){
  switch(exit1_motor_status){
    case 1:
      Serial.println("Exit gate: open");
      exit1_sensorMeasure(currentMicros);
      break;
    case 0:
      Serial.println("Exit gate: closing...");
      exit1_motorClose(currentMillis);
      break;
    case -1:
      Serial.println("Exit gate: closed");
      exit1_motorOpen(currentMillis);
      break;
  }
}

void entry1_sensorMeasure(unsigned long currentMicros) {
  switch(entry1_sensor_status){
    case 0: // reset
      entry1_sensor_status = 1;
      entry1_sensor_micros = currentMicros;
      digitalWrite(entry1_sensor_pinTrigger, LOW);
      break;
    case 1: // beam
      if (currentMicros - entry1_sensor_micros < 2) return;
      entry1_sensor_status = 2;
      entry1_sensor_micros = currentMicros;
      digitalWrite(entry1_sensor_pinTrigger, HIGH);
      break;
    case 2: // read
      if (currentMicros - entry1_sensor_micros < 10) return;
      entry1_sensor_status=0;
      entry1_sensor_micros = currentMicros;
      digitalWrite(entry1_sensor_pinTrigger, LOW);
      // Reads the echopin, returns the sound wave travel time in microseconds
      long entry1_sensor_duration = pulseIn(entry1_sensor_pinEcho, HIGH);
      // Calculating the distance
      int entry1_sensor_distance = entry1_sensor_duration * 0.034 / 2;
      Serial.print("Distance: ");
      Serial.println(entry1_sensor_distance);
      if(entry1_sensor_distance<sensorThreshold) entry1_motor_status=0;
      break;
  }
}

void entry1_motorOpen(unsigned long currentMillis) {
  if(entry1_motor_status==1) return;
  if(currentMillis - entry1_motor_millis < motorInterval) return;
  digitalWrite(entry1_LED_pin, HIGH); // turn the LED on (HIGH is the voltage level)
  entry1_motor_millis = currentMillis;
  switch(entry1_motor_step) {
    case 0:
      digitalWrite(entry1_motor_pin0, HIGH);
      digitalWrite(entry1_motor_pin1, LOW);
      digitalWrite(entry1_motor_pin2, LOW);
      digitalWrite(entry1_motor_pin3, LOW);
      break;
    case 1:
      digitalWrite(entry1_motor_pin0, LOW);
      digitalWrite(entry1_motor_pin1, HIGH);
      digitalWrite(entry1_motor_pin2, LOW);
      digitalWrite(entry1_motor_pin3, LOW);
      break;
    case 2:
      digitalWrite(entry1_motor_pin0, LOW);
      digitalWrite(entry1_motor_pin1, LOW);
      digitalWrite(entry1_motor_pin2, HIGH);
      digitalWrite(entry1_motor_pin3, LOW);
      break;
    case 3:
      digitalWrite(entry1_motor_pin0, LOW);
      digitalWrite(entry1_motor_pin1, LOW);
      digitalWrite(entry1_motor_pin2, LOW);
      digitalWrite(entry1_motor_pin3, HIGH);
      break;
  }
  entry1_motor_step++;
  if (entry1_motor_step > 3) {
    entry1_motor_step = 0;
  }
  entry1_motor_currentSpins++;
  if (entry1_motor_currentSpins >= motorSpins) {
    entry1_motor_status=1;
    entry1_motor_currentSpins = 0;
  }
}
void entry1_motorClose(unsigned long currentMillis) {
  if(entry1_motor_status==-1) return;
  if(currentMillis - entry1_motor_millis < motorInterval) return;
  digitalWrite(entry1_LED_pin, HIGH); // turn the LED on (HIGH is the voltage level)
  entry1_motor_millis = currentMillis;
  switch (entry1_motor_step) {
    case 0:
      digitalWrite(entry1_motor_pin0, LOW);
      digitalWrite(entry1_motor_pin1, LOW);
      digitalWrite(entry1_motor_pin2, LOW);
      digitalWrite(entry1_motor_pin3, HIGH);
      break;
    case 1:
      digitalWrite(entry1_motor_pin0, LOW);
      digitalWrite(entry1_motor_pin1, LOW);
      digitalWrite(entry1_motor_pin2, HIGH);
      digitalWrite(entry1_motor_pin3, LOW);
      break;
    case 2:
      digitalWrite(entry1_motor_pin0, LOW);
      digitalWrite(entry1_motor_pin1, HIGH);
      digitalWrite(entry1_motor_pin2, LOW);
      digitalWrite(entry1_motor_pin3, LOW);
      break;
    case 3:
      digitalWrite(entry1_motor_pin0, HIGH);
      digitalWrite(entry1_motor_pin1, LOW);
      digitalWrite(entry1_motor_pin2, LOW);
      digitalWrite(entry1_motor_pin3, LOW);
      break;
  }
  entry1_motor_step++;
  if (entry1_motor_step > 3) {
    entry1_motor_step = 0;
  }
  entry1_motor_currentSpins++;
  if (entry1_motor_currentSpins >= motorSpins) {
    entry1_motor_status = -1;
    digitalWrite(entry1_LED_pin, LOW); // turn the LED on (HIGH is the voltage level)
    entry1_motor_currentSpins = 0;
  }
}

void exit1_sensorMeasure(unsigned long currentMicros) {
  switch(exit1_sensor_status){
    case 0: // reset
      exit1_sensor_status = 1;
      exit1_sensor_micros = currentMicros;
      digitalWrite(exit1_sensor_pinTrigger, LOW);
      break;
    case 1: // beam
      if (currentMicros - exit1_sensor_micros < 2) return;
      exit1_sensor_status = 2;
      exit1_sensor_micros = currentMicros;
      digitalWrite(exit1_sensor_pinTrigger, HIGH);
      break;
    case 2: // read
      if (currentMicros - exit1_sensor_micros < 10) return;
      exit1_sensor_status=0;
      exit1_sensor_micros = currentMicros;
      digitalWrite(exit1_sensor_pinTrigger, LOW);
      // Reads the echopin, returns the sound wave travel time in microseconds
      long exit1_sensor_duration = pulseIn(exit1_sensor_pinEcho, HIGH);
      // Calculating the distance
      int exit1_sensor_distance = exit1_sensor_duration * 0.034 / 2;
      Serial.print("Distance: ");
      Serial.println(exit1_sensor_distance);
      if(exit1_sensor_distance<sensorThreshold) exit1_motor_status=0;
      break;
  }
}

void exit1_motorOpen(unsigned long currentMillis) {
  if(exit1_motor_status==1) return;
  if(currentMillis - exit1_motor_millis < motorInterval) return;
  digitalWrite(exit1_LED_pin, HIGH); // turn the LED on (HIGH is the voltage level)
  exit1_motor_millis = currentMillis;
  switch(exit1_motor_step) {
    case 0:
      digitalWrite(exit1_motor_pin0, HIGH);
      digitalWrite(exit1_motor_pin1, LOW);
      digitalWrite(exit1_motor_pin2, LOW);
      digitalWrite(exit1_motor_pin3, LOW);
      break;
    case 1:
      digitalWrite(exit1_motor_pin0, LOW);
      digitalWrite(exit1_motor_pin1, HIGH);
      digitalWrite(exit1_motor_pin2, LOW);
      digitalWrite(exit1_motor_pin3, LOW);
      break;
    case 2:
      digitalWrite(exit1_motor_pin0, LOW);
      digitalWrite(exit1_motor_pin1, LOW);
      digitalWrite(exit1_motor_pin2, HIGH);
      digitalWrite(exit1_motor_pin3, LOW);
      break;
    case 3:
      digitalWrite(exit1_motor_pin0, LOW);
      digitalWrite(exit1_motor_pin1, LOW);
      digitalWrite(exit1_motor_pin2, LOW);
      digitalWrite(exit1_motor_pin3, HIGH);
      break;
  }
  exit1_motor_step++;
  if (exit1_motor_step > 3) {
    exit1_motor_step = 0;
  }
  exit1_motor_currentSpins++;
  if (exit1_motor_currentSpins >= motorSpins) {
    exit1_motor_status=1;
    exit1_motor_currentSpins = 0;
  }
}
void exit1_motorClose(unsigned long currentMillis) {
  if(exit1_motor_status==-1) return;
  if(currentMillis - exit1_motor_millis < motorInterval) return;
  digitalWrite(exit1_LED_pin, HIGH); // turn the LED on (HIGH is the voltage level)
  exit1_motor_millis = currentMillis;
  switch (exit1_motor_step) {
    case 0:
      digitalWrite(exit1_motor_pin0, LOW);
      digitalWrite(exit1_motor_pin1, LOW);
      digitalWrite(exit1_motor_pin2, LOW);
      digitalWrite(exit1_motor_pin3, HIGH);
      break;
    case 1:
      digitalWrite(exit1_motor_pin0, LOW);
      digitalWrite(exit1_motor_pin1, LOW);
      digitalWrite(exit1_motor_pin2, HIGH);
      digitalWrite(exit1_motor_pin3, LOW);
      break;
    case 2:
      digitalWrite(exit1_motor_pin0, LOW);
      digitalWrite(exit1_motor_pin1, HIGH);
      digitalWrite(exit1_motor_pin2, LOW);
      digitalWrite(exit1_motor_pin3, LOW);
      break;
    case 3:
      digitalWrite(exit1_motor_pin0, HIGH);
      digitalWrite(exit1_motor_pin1, LOW);
      digitalWrite(exit1_motor_pin2, LOW);
      digitalWrite(exit1_motor_pin3, LOW);
      break;
  }
  exit1_motor_step++;
  if (exit1_motor_step > 3) {
    exit1_motor_step = 0;
  }
  exit1_motor_currentSpins++;
  if (exit1_motor_currentSpins >= motorSpins) {
    exit1_motor_status = -1;
    digitalWrite(exit1_LED_pin, LOW); // turn the LED on (HIGH is the voltage level)
    exit1_motor_currentSpins = 0;
  }
}
