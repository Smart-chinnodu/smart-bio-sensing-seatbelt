/*
 * Smart Bio-Sensing Seat Belt System
 * Main Arduino Sketch
 * 
 * Description: This system monitors driver vital signs and consciousness
 * to prevent accidents caused by fatigue or health issues.
 * 
 * Author: Smart Chinnodu
 * Institution: Dadi Institute of Engineering and Technology
 * Date: November 2025
 * License: MIT
 */

// Include required libraries
#include <Wire.h>
#include <MAX30100_PulseOximeter.h>
#include <SoftwareSerial.h>
#include <TinyGPS++.h>

// Pin Definitions
#define HEART_RATE_SDA    A4  // I2C Data line
#define HEART_RATE_SCL    A5  // I2C Clock line
#define BP_SENSOR_PIN     A0  // Blood pressure analog input
#define ALCOHOL_SENSOR    A1  // Alcohol sensor analog input
#define BCI_RX            10  // BCI module RX
#define BCI_TX            11  // BCI module TX
#define BLUETOOTH_RX      2   // Bluetooth RX
#define BLUETOOTH_TX      3   // Bluetooth TX
#define GPS_RX            4   // GPS RX
#define GPS_TX            5   // GPS TX
#define BUZZER_PIN        8   // Buzzer output
#define RELAY_PIN         9   // Vehicle control relay
#define LED_GREEN         6   // Normal status LED
#define LED_YELLOW        7   // Warning LED
#define LED_RED          13   // Emergency LED

// Threshold Values
#define HEART_RATE_MIN    50  // Minimum safe heart rate (BPM)
#define HEART_RATE_MAX    120 // Maximum safe heart rate (BPM)
#define BP_MIN            80  // Minimum safe blood pressure
#define BP_MAX            140 // Maximum safe blood pressure
#define ALCOHOL_THRESHOLD 200 // Alcohol sensor threshold
#define DROWSINESS_THRESHOLD 30 // BCI drowsiness threshold

// Alert Levels
enum AlertLevel {
  NORMAL,
  WARNING,
  CRITICAL
};

// Global Variables
PulseOximeter pox;
SoftwareSerial bluetooth(BLUETOOTH_RX, BLUETOOTH_TX);
SoftwareSerial gpsSerial(GPS_RX, GPS_TX);
TinyGPSPlus gps;

float heartRate = 0;
float spO2 = 0;
int bloodPressure = 0;
int alcoholLevel = 0;
int drowsinessLevel = 0;
AlertLevel currentAlert = NORMAL;

unsigned long lastUpdate = 0;
const unsigned long UPDATE_INTERVAL = 1000; // Update every 1 second

bool emergencyActive = false;

/*
 * Setup Function
 * Initializes all sensors, communication modules, and pins
 */
void setup() {
  // Initialize Serial Communication
  Serial.begin(9600);
  bluetooth.begin(9600);
  gpsSerial.begin(9600);
  
  Serial.println("Smart Bio-Sensing Seat Belt System");
  Serial.println("Initializing...");
  
  // Initialize I2C for Heart Rate Sensor
  Wire.begin();
  
  // Initialize Pulse Oximeter
  if (!pox.begin()) {
    Serial.println("ERROR: Failed to initialize pulse oximeter!");
    while(1); // Halt if sensor fails
  } else {
    Serial.println("Pulse oximeter initialized successfully");
  }
  
  // Configure pulse oximeter
  pox.setIRLedCurrent(MAX30100_LED_CURR_50MA);
  
  // Initialize Digital Pins
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  
  // Set initial states
  digitalWrite(BUZZER_PIN, LOW);
  digitalWrite(RELAY_PIN, LOW);
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  
  // Initialize Analog Pins
  pinMode(BP_SENSOR_PIN, INPUT);
  pinMode(ALCOHOL_SENSOR, INPUT);
  
  Serial.println("System ready!");
  bluetooth.println("SYSTEM_READY");
  
  delay(1000);
}

/*
 * Main Loop Function
 * Continuously monitors sensors and updates system state
 */
void loop() {
  unsigned long currentTime = millis();
  
  // Update pulse oximeter
  pox.update();
  
  // Read all sensors at regular intervals
  if (currentTime - lastUpdate >= UPDATE_INTERVAL) {
    lastUpdate = currentTime;
    
    // Read Heart Rate and SpO2
    heartRate = pox.getHeartRate();
    spO2 = pox.getSpO2();
    
    // Read Blood Pressure (simulated from analog sensor)
    bloodPressure = map(analogRead(BP_SENSOR_PIN), 0, 1023, 60, 180);
    
    // Read Alcohol Level
    alcoholLevel = analogRead(ALCOHOL_SENSOR);
    
    // Read Drowsiness from BCI (simulated)
    drowsinessLevel = readDrowsinessLevel();
    
    // Read GPS data
    while (gpsSerial.available() > 0) {
      gps.encode(gpsSerial.read());
    }
    
    // Analyze vital signs and determine alert level
    analyzeVitalSigns();
    
    // Take appropriate action based on alert level
    handleAlerts();
    
    // Send data via Bluetooth
    sendDataToBluetooth();
    
    // Print data to Serial Monitor
    printSensorData();
  }
}

/*
 * Read drowsiness level from BCI module
 * Returns: Drowsiness value (0-100)
 */
int readDrowsinessLevel() {
  // TODO: Implement actual BCI reading protocol
  // This is a placeholder simulation
  
  // For now, return a simulated value
  return random(0, 50);
}

/*
 * Analyze vital signs and determine alert level
 */
void analyzeVitalSigns() {
  bool criticalCondition = false;
  bool warningCondition = false;
  
  // Check Heart Rate
  if (heartRate < HEART_RATE_MIN || heartRate > HEART_RATE_MAX) {
    if (heartRate > 0) { // Valid reading
      criticalCondition = true;
    }
  }
  
  // Check Blood Pressure
  if (bloodPressure < BP_MIN || bloodPressure > BP_MAX) {
    warningCondition = true;
  }
  
  // Check Alcohol Level
  if (alcoholLevel > ALCOHOL_THRESHOLD) {
    criticalCondition = true;
  }
  
  // Check Drowsiness
  if (drowsinessLevel > DROWSINESS_THRESHOLD) {
    if (drowsinessLevel > 50) {
      criticalCondition = true;
    } else {
      warningCondition = true;
    }
  }
  
  // Set alert level
  if (criticalCondition) {
    currentAlert = CRITICAL;
  } else if (warningCondition) {
    currentAlert = WARNING;
  } else {
    currentAlert = NORMAL;
  }
}

/*
 * Handle alerts based on current alert level
 */
void handleAlerts() {
  switch (currentAlert) {
    case NORMAL:
      handleNormalState();
      break;
      
    case WARNING:
      handleWarningState();
      break;
      
    case CRITICAL:
      handleCriticalState();
      break;
  }
}

/*
 * Handle normal operating state
 */
void handleNormalState() {
  digitalWrite(LED_GREEN, HIGH);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, LOW);
  digitalWrite(BUZZER_PIN, LOW);
  
  emergencyActive = false;
}

/*
 * Handle warning state - mild fatigue detected
 */
void handleWarningState() {
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, HIGH);
  digitalWrite(LED_RED, LOW);
  
  // Intermittent buzzer
  if (millis() % 2000 < 100) {
    digitalWrite(BUZZER_PIN, HIGH);
  } else {
    digitalWrite(BUZZER_PIN, LOW);
  }
  
  bluetooth.println("ALERT:WARNING");
}

/*
 * Handle critical state - emergency protocols
 */
void handleCriticalState() {
  digitalWrite(LED_GREEN, LOW);
  digitalWrite(LED_YELLOW, LOW);
  digitalWrite(LED_RED, HIGH);
  
  // Continuous buzzer
  digitalWrite(BUZZER_PIN, HIGH);
  
  if (!emergencyActive) {
    emergencyActive = true;
    activateEmergencyProtocol();
  }
}

/*
 * Activate emergency protocol
 */
void activateEmergencyProtocol() {
  Serial.println("\n*** EMERGENCY ACTIVATED ***");
  
  // Send emergency alert via Bluetooth
  bluetooth.println("EMERGENCY:ACTIVE");
  
  // Get GPS coordinates
  if (gps.location.isValid()) {
    String gpsData = String(gps.location.lat(), 6) + "," + 
                    String(gps.location.lng(), 6);
    bluetooth.print("GPS:");
    bluetooth.println(gpsData);
    Serial.print("GPS Location: ");
    Serial.println(gpsData);
  }
  
  // Activate vehicle slowdown (via relay)
  digitalWrite(RELAY_PIN, HIGH);
  
  // TODO: Implement actual emergency call functionality
  // This would require GSM module integration
  Serial.println("Emergency services notified");
  
  delay(100);
}

/*
 * Send sensor data via Bluetooth
 */
void sendDataToBluetooth() {
  // Send data in JSON-like format
  bluetooth.print("{");
  bluetooth.print("\"hr\":");
  bluetooth.print(heartRate);
  bluetooth.print(",\"spo2\":");
  bluetooth.print(spO2);
  bluetooth.print(",\"bp\":");
  bluetooth.print(bloodPressure);
  bluetooth.print(",\"alcohol\":");
  bluetooth.print(alcoholLevel);
  bluetooth.print(",\"drowsiness\":");
  bluetooth.print(drowsinessLevel);
  bluetooth.print(",\"alert\":");
  bluetooth.print(currentAlert);
  bluetooth.println("}");
}

/*
 * Print sensor data to Serial Monitor
 */
void printSensorData() {
  Serial.println("\n--- Sensor Readings ---");
  Serial.print("Heart Rate: ");
  Serial.print(heartRate);
  Serial.println(" BPM");
  
  Serial.print("SpO2: ");
  Serial.print(spO2);
  Serial.println(" %");
  
  Serial.print("Blood Pressure: ");
  Serial.print(bloodPressure);
  Serial.println(" mmHg");
  
  Serial.print("Alcohol Level: ");
  Serial.println(alcoholLevel);
  
  Serial.print("Drowsiness: ");
  Serial.print(drowsinessLevel);
  Serial.println("%");
  
  Serial.print("Alert Level: ");
  switch(currentAlert) {
    case NORMAL:
      Serial.println("NORMAL");
      break;
    case WARNING:
      Serial.println("WARNING");
      break;
    case CRITICAL:
      Serial.println("CRITICAL");
      break;
  }
  Serial.println("----------------------");
}
