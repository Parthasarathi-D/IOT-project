#include "arduino_secrets.h"
#include "thingProperties.h"
#include "Servo.h"
Servo servo;
int alcohol_measured = A0;
const int D2_PIN = D2; // Digital pin connected to the DO pin of the gas sensor

void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(9600);
  // This delay gives the chance to wait for a Serial Monitor without blocking if none is found
  delay(1500);
  servo.attach(D0);
  servo.write(0);//door servo
  pinMode(D6, OUTPUT);//engine off 6
  pinMode(D5, OUTPUT);//driver state  7
  pinMode(D8, OUTPUT);//door state   8
  pinMode(D1, OUTPUT);//engine controll
  pinMode(D4, OUTPUT);//buzzer
  pinMode(D2_PIN, INPUT); // D2 pin for achohol
  pinMode(D3,INPUT);//password

  // Defined in thingProperties.h
  initProperties();

  // Connect to Arduino IoT Cloud
  ArduinoCloud.begin(ArduinoIoTPreferredConnection);

  setDebugMessageLevel(2);
  ArduinoCloud.printDebugInfo();
}
bool flag;

void loop() {
  ArduinoCloud.update();
  int sensorValue = digitalRead(D2_PIN); // Read digital value from D2 pin;
  int anologValue = analogRead(alcohol_measured);
  
  // Convert analog value to percentage (adjust this based on your sensor's characteristics)
  alcohol_level = map(anologValue, 0, 1023, 0, 100);

  if (sensorValue == LOW) {
    flag = true;
    alcohol_level=75;
    Serial.println("Alcohol detected!"); // Print message to serial monitor
    digitalWrite(D5, HIGH);
    digitalWrite(D4, HIGH);
    status = "Alcohol level is HIGH";
  }
  else {
    flag = false;
    alcohol_level=4;
    Serial.println("No alcohol detected"); // Print message to serial monitor
    digitalWrite(D5, LOW);
    digitalWrite(D4, LOW);
    status = "Safe to Drive";
  }
  delay(500);
}


void onDoorChange()  {
  
  if (door and flag){
  
    servo.write(90);
    digitalWrite(D8, HIGH);
    door_status="locked";
    
    
  } else {
    servo.write(0);
    digitalWrite(D8, LOW);
    door_status="unlocked";
  }

}   

void onEngineChange()  {
  if (engine and flag) {
    
    digitalWrite(D1, HIGH);
    digitalWrite(D6, HIGH);
  } else {
    digitalWrite(D1, LOW);
    digitalWrite(D6, LOW);
  }
}


void onAlcoholLevelChange()  {
}

void onStatusChange()  {
}

void onDoorStatusChange()  {
}
