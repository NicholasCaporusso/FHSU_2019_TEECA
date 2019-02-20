/* PARKING SETTINGS */
const int sensorThreshold = 3;
const int parkingSpots=15;
int echo_pins={};
int trigger_pins={};
int LED_pins={};

/* DO NOT MODIFY BELOW*/
/* SENSORS */
int sensors_status[parkingSpots];
unsigned long sensors_millis[parkingSpots];

void setup() {
  for(int i=0;i<parkingSpots;i++){
	pinMode(trigger_pins[i], OUTPUT);
    pinMode(echo_pins[i], INPUT); 
    pinMode(LED_pins[i], OUTPUT);
	sensors_status[i]=0;
	sensors_millis[i]=0;
  }
  Serial.begin(9600); // Starts the serial communication
}

void loop(){
  unsigned long currentMicros=micros();
  unsigned long currentMillis=millis();
  for(int i=0;i<parkingSpots;i++){
	  checkSpot(i);
  }
}

void checkSpot(int sensor, unsigned long currentMicros){
  switch(sensors_status[sensor]){
    case 0: // reset
      sensors_status[sensor] = 1;
      sensors_millis[sensor] = currentMicros;
      digitalWrite(trigger_pins[sensor], LOW);
      break;
    case 1: // beam
      if (currentMicros - sensors_millis[sensor] < 2) return;
      sensors_status[sensor] = 2;
      sensors_millis[sensor] = currentMicros;
      digitalWrite(trigger_pins[sensor], HIGH);
      break;
    case 2: // read
      if (currentMicros - sensors_millis[sensor] < 10) return;
      sensors_status[sensor]=0;
      sensors_millis[sensor] = currentMicros;
      digitalWrite(trigger_pins[sensor], LOW);
      long duration = pulseIn(echo_pins[sensor], HIGH); // Reads the echopin, returns the sound wave travel time in microseconds
      int distance = duration * 0.034 / 2; // Calculating the distance
      Serial.print("Distance: ");
      Serial.println(distance);
      if(distance<sensorThreshold) digitalWrite(LED_pins[sensor],LOW);
	  else digitalWrite(LED_pins[sensor],HIGH);
      break;
  }
}