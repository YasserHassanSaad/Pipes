#include "FirebaseESP8266.h"  // Install Firebase ESP8266 library
#include <ESP8266WiFi.h>

//define sound velocity in cm/uS
#define SOUND_VELOCITY 0.034
#define FIREBASE_HOST "arduinoteampipes-2f6d2-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "3pg6fP754yNR8NR8n5sxfxaVhS3RqausUkOzc44D"
#define WIFI_SSID "moussa home"
#define WIFI_PASSWORD "$@@dH@$$an"

const int trigPin = 2;  //D4
const int echoPin = 0;  //D3
int led = D5;     // Connect LED to D5
int pipe1Motor; 
int pipe2Motor; 
int pipe3Motor; 

long duration;
float distanceCm;
float distanceInch;

//Define FirebaseESP8266 data object
FirebaseData firebaseData;
FirebaseData ledData;
FirebaseData pipe1Data; 
FirebaseData pipe2Data; 
FirebaseData pipe3Data; 

FirebaseJson json;

void setup() {
  Serial.begin(9600); // Starts the serial communication
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  pinMode(led,OUTPUT);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Firebase.reconnectWiFi(true);
}

void loop() {
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);

  // Calculate the distance
  distanceCm = duration * SOUND_VELOCITY / 2;

  // Prints the distance on the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);

  if (Firebase.setFloat(firebaseData, "/FirebaseIOT/WaterLevel", distanceCm))
  {
    Serial.println("PASSED");
    Serial.println("PATH: " + firebaseData.dataPath());
    Serial.println("TYPE: " + firebaseData.dataType());
    Serial.println("ETag: " + firebaseData.ETag());
    Serial.println("------------------------------------");
    Serial.println();
  }
  else
  {
    Serial.println("FAILED");
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println("------------------------------------");
    Serial.println();
  }

  if (Firebase.getString(ledData, "/FirebaseIOT/Led")){
    Serial.println(ledData.stringData());
    if (ledData.stringData() == "1") {
    digitalWrite(led, HIGH);
    }
  else if (ledData.stringData() == "0"){
    digitalWrite(led, LOW);
    }
  }

  if (Firebase.getString(pipe1Data, "/FirebaseIOT/Pipe1")){
    if (pipe1Data.stringData() == "1") {
    digitalWrite(pipe1Motor, HIGH);
    }
  else if (pipe1Data.stringData() == "0"){
    digitalWrite(pipe1Motor, LOW);
    }
  }

  if (Firebase.getString(pipe2Data, "/FirebaseIOT/Pipe2")){
    if (pipe2Data.stringData() == "1") {
    digitalWrite(pipe2Data, HIGH);
    }
  else if (pipe2Data.stringData() == "0"){
    digitalWrite(pipe2Data, LOW);
    }
  }

  if (Firebase.getString(pipe3Data, "/FirebaseIOT/Pipe3")){
    if (pipe3Data.stringData() == "1") {
    digitalWrite(pipe3Data, HIGH);
    }
  else if (pipe3Data.stringData() == "0"){
    digitalWrite(pipe3Data, LOW);
    }
  }
  
  delay(500);
}
