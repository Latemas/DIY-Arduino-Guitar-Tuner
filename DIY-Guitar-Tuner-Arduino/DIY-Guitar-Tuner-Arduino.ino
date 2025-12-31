/*
  Project: Arduino Guitar Tuner
  Author: Salih Mete Alkan
  Date: 2025
  Version: 1.1 (Final - Universal English Codebase)
  License: MIT License
  
  Description:
  This project is an automatic Arduino Guitar Tuner. 
  It utilizes frequency analysis (zero-crossing) to detect notes and 
  controls a stepper motor to tune the guitar peg automatically.
  
  Features:
  - Minimalist OLED Interface (SSD1306)
  - Auto-centering UI logic
  - Persistent Hz display during tuning
  - Visual Arrow Indicators (Inward pointing for intuitive guidance)
  
  Hardware:
  - Arduino Nano / Uno
  - 0.96" OLED Display (I2C)
  - 28BYJ-48 Stepper Motor & ULN2003 Driver
  - Microphone Module (Generic LM393 or MAX9814 Amplifier)
  - LEDs (Red, Green, Yellow)
*/

#include <Stepper.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// --- PIN DEFINITIONS ---
const int sensorPin = A0;   // Microphone Input
const int buttonPin = 2;    // Button to switch strings

// LED Definitions (Updated to English)
const int ledRed = 3;       // Tune Down (Note is too Sharp/High)
const int ledGreen = 4;     // Perfect Pitch (In Tune)
const int ledYellow = 5;    // Tune Up (Note is too Flat/Low)

// --- MOTOR SETTINGS ---
// Sequence: IN1-IN3-IN2-IN4 (Standard for 28BYJ-48)
Stepper myStepper(2038, 8, 10, 9, 11);

// --- TUNING SETTINGS ---
const int threshold = 525;  // Noise threshold
const int TOTAL_STRINGS = 6;
const char* stringNotes[] = {"E", "A", "D", "G", "B", "e"};
float targetFreqs[] = {82.41, 110.00, 146.83, 196.00, 246.94, 329.63};
float tolerance = 0.9;      // Acceptable error margin (+- Hz)

// --- VARIABLES ---
int currentString = 0; 
unsigned long minPeriod, maxPeriod, lastCrossingTime = 0;
float readings[5]; 
int readIndex = 0;
float lastStableFreq = 0;
int confirmCount = 0;
unsigned long lastDebounceTime = 0;
int lastButtonState = HIGH;
float currentMeasuredFreq = 0;
bool isPerfect = false; 

void setup() {
  Serial.begin(115200);
  Wire.begin();
  Wire.setClock(400000); // Increase I2C clock speed

  pinMode(buttonPin, INPUT_PULLUP);
  // Setup LEDs with new names
  pinMode(ledRed, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledYellow, OUTPUT);
  
  // Initialize Display
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED Error"));
    for(;;); 
  }
  
  drawModernUI(); 
  myStepper.setSpeed(8); 
  
  // Optimize ADC (Analog Read) Speed for better frequency detection
  ADCSRA &= ~(bit (ADPS0) | bit (ADPS1) | bit (ADPS2));
  ADCSRA |= bit (ADPS2) | bit (ADPS0);
  
  setFilter(currentString);
  lastButtonState = digitalRead(buttonPin);
}

void loop() {
  checkButton();

  unsigned long now = micros();
  int val = analogRead(sensorPin);
  static bool previousStateHigh = false;
  bool currentStateHigh = (val > threshold); 

  if (!previousStateHigh && currentStateHigh) {
    unsigned long period = now - lastCrossingTime;
    
    if (period > minPeriod && period < maxPeriod) {
      float freq = 1000000.0 / period;
      float target = targetFreqs[currentString];
      
      if (freq > target * 0.75 && freq < target * 1.25) {
        readings[readIndex] = freq;
        readIndex = (readIndex + 1) % 5; 
        
        if (readIndex == 0) { 
           float average = 0;
           for(int k=0; k<5; k++) average += readings[k];
           average /= 5.0;
           
           if (abs(average - lastStableFreq) < 1.2) confirmCount++;
           else confirmCount = 0;
           lastStableFreq = average;

           if (confirmCount >= 2) { 
              currentMeasuredFreq = average;
              processMotorLogic(average);
              confirmCount = 0; 
           }
        }
      }
    }
    lastCrossingTime = now;
  }
  previousStateHigh = currentStateHigh;
}

void drawModernUI() {
  display.clearDisplay();
  display.setTextColor(WHITE);

  // 1. TOP HEADER
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.print("ARDUINO TUNER"); 
  
  display.setCursor(95, 0);
  if(currentMeasuredFreq > 0) {
    display.print((int)currentMeasuredFreq);
    display.print("Hz");
  } else {
    display.print("---");
  }
  display.drawLine(0, 10, 128, 10, WHITE); 

  // 2. CENTER NOTE (Auto-Centered)
  display.setTextSize(5);
  display.setCursor(49, 22); 
  display.print(stringNotes[currentString]);

  // 3. ARROWS (Reversed: Pointing INWARDS to the note)
  if(currentMeasuredFreq > 0 && !isPerfect) {
    float target = targetFreqs[currentString];
    float diff = currentMeasuredFreq - target;

    if (abs(diff) > tolerance) {
        // LEFT ARROWS (TUNE UP) -> Points RIGHT (>)
        if (diff < -tolerance) { 
          display.fillTriangle(40, 42, 30, 32, 30, 52, WHITE); 
          if (diff < -3.0) display.drawTriangle(25, 42, 15, 32, 15, 52, WHITE); 
          if (diff < -5.0) display.drawLine(5, 32, 5, 52, WHITE); 
        }
        
        // RIGHT ARROWS (TUNE DOWN) -> Points LEFT (<)
        if (diff > tolerance) { 
          display.fillTriangle(88, 42, 98, 32, 98, 52, WHITE); 
          if (diff > 3.0) display.drawTriangle(103, 42, 113, 32, 113, 52, WHITE); 
          if (diff > 5.0) display.drawLine(123, 32, 123, 52, WHITE); 
        }
    }
  }
  display.display(); 
}

void processMotorLogic(float currentFreq) {
  float target = targetFreqs[currentString];
  float diff = currentFreq - target;
  
  allLedsOff();

  if (abs(diff) <= tolerance) {
    // PERFECT PITCH
    digitalWrite(ledGreen, HIGH); // Green Light
    isPerfect = true; 
    drawModernUI();   
    stopMotor();
    delay(2000);      
    
    allLedsOff();
    isPerfect = false;
    currentMeasuredFreq = 0; 
    drawModernUI(); 
  }
  else {
    isPerfect = false;
    if (currentFreq < (target - tolerance)) {
      // NOTE IS FLAT (Low) -> Turn Motor CCW, Light Yellow
      digitalWrite(ledYellow, HIGH); 
      drawModernUI();
      myStepper.step(-160); 
    } else {
      // NOTE IS SHARP (High) -> Turn Motor CW, Light Red
      digitalWrite(ledRed, HIGH); 
      drawModernUI();
      myStepper.step(160); 
    }
    stopMotor();
    allLedsOff();
  }
}

void checkButton() {
  int reading = digitalRead(buttonPin);
  if (reading == LOW && lastButtonState == HIGH) {
    if (millis() - lastDebounceTime > 400) {
      currentString = (currentString + 1) % TOTAL_STRINGS;
      setFilter(currentString);
      lastDebounceTime = millis();
      allLedsOff();
      currentMeasuredFreq = 0;
      isPerfect = false;
      drawModernUI(); 
    }
  }
  lastButtonState = reading;
}

void setFilter(int idx) {
  float target = targetFreqs[idx];
  minPeriod = 1000000.0 / (target * 1.3); 
  maxPeriod = 1000000.0 / (target * 0.7); 
}

void allLedsOff() {
  digitalWrite(ledRed, LOW); 
  digitalWrite(ledGreen, LOW); 
  digitalWrite(ledYellow, LOW);
}

void stopMotor() {
  for(int i=8; i<=11; i++) digitalWrite(i, LOW);
}
