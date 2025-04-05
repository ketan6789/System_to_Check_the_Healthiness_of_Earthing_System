#include <Adafruit_ADS1X15.h>
#include <LiquidCrystal.h>

// Initialize ADS1015
Adafruit_ADS1015 ads;  // Create an instance of ADS1015

// Initialize LCD (16x4)
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

// ACS712 Current Sensing Variables
const int ACS_PIN_1 = A1;  // ACS712 for soil resistivity and earth resistance
const int ACS_PIN_2 = A0;  // ACS712 for leakage current
const int buzzerPinCurrent2 = 8; // Pin for the current sensing buzzer (for ACS_PIN_2)
const int buzzerPinContinuity = 10;  // Pin for the continuity buzzer
const int buzzerPinEarthRes = 9;  // Pin for earth resistance buzzer (replacing high current buzzer)
const float sensitivity = 100.0;  // Sensitivity for ACS712 (mV per Amp)
const float currentThreshold = 0.01;  // Threshold for current sensing (in Amps)
const float offsetCurrent = 0.024;  // Measured offset current value

// Soil Resistivity Calculation Variables
const float a = 0.5;  // Distance between electrodes in meters

// Earth Resistance Calculation Variables
const float L = 15.0 / 100.0;  // Length of the ground rod in meters (15 cm)
const float d = 0.3 / 100.0;  // Diameter of the ground rod in meters (0.3 cm)
const float earthResistanceThreshold = 2.0;  // Threshold for earth resistance buzzer (in Ohms)

// Continuity Check Variables
const int sensorPinContinuity = A2;  // Analog input pin for continuity checking
const int continuityThreshold = 500; // Threshold for continuity (change as needed)

void setup() {
  Serial.begin(9600);  // Initialize serial communications at 9600 bps

  // Initialize the ADS1015
  ads.begin();  // Start the ADS1015

  // Initialize the LCD
  lcd.begin(16, 4);  // For a 16x4 LCD display
  lcd.print("Initializing...");
  delay(1000);
  lcd.clear();

  // Set buzzer pins as output
  pinMode(buzzerPinCurrent2, OUTPUT);
  pinMode(buzzerPinContinuity, OUTPUT);
  pinMode(buzzerPinEarthRes, OUTPUT);  // Set earth resistance buzzer pin as output
  digitalWrite(buzzerPinCurrent2, LOW);  // Ensure current buzzer is off initially
  digitalWrite(buzzerPinContinuity, LOW);  // Ensure continuity buzzer is off initially
  digitalWrite(buzzerPinEarthRes, LOW);  // Ensure earth resistance buzzer is off initially
}

float readCurrent(int pin) {
  float totalCurrent = 0;
  const int numReadings = 10;  // Number of readings to average

  for (int i = 0; i < numReadings; i++) {
    int sensorValue = analogRead(pin);
    float voltage = (sensorValue / 1023.0) * 5000;  // Convert to mV
    float current = (voltage - 2500) / sensitivity;  // Calculate current in Amps
    current -= offsetCurrent;  // Subtract the offset
    totalCurrent += current;
    delay(10);  // Short delay between readings
  }

  return totalCurrent / numReadings;  // Return the average current
}

void loop() {
  // Read currents
  float current1 = readCurrent(ACS_PIN_1);  // Soil resistivity current
  float current2 = readCurrent(ACS_PIN_2);  // Leakage current

  // Read voltage from ADS1015 (using AIN0)
  int16_t adc0 = ads.readADC_SingleEnded(0);  // Reading from AIN0
  float busVoltage = (adc0 * 0.003);  // Convert ADC value to voltage

  // Calculate soil resistivity using the formula ρ = 2πa(V/I)
  float soilResistivity = (2 * 3.14159 * a) * (busVoltage / current1);

  // Calculate earth resistance using the formula:
  float earthResistance = (soilResistivity / (2 * 3.14159 * L)) * (log((8 * L) / d) - 1);

  // Display soil resistivity on the LCD
  lcd.setCursor(0, 0);
  lcd.print("Soil Res: ");
  lcd.print(soilResistivity, 3);  // Display resistivity with 3 decimal places
  lcd.print(" Ωm");

  // Display the earth resistance on the LCD
  lcd.setCursor(0, 1);
  lcd.print("Earth Res: ");
  lcd.print(earthResistance, 3);  // Display resistance with 3 decimal places
  lcd.print(" Ω");

  // Display leakage current (current2) on the LCD
  lcd.setCursor(0, 2);
  lcd.print("Leak Curr: ");
  lcd.print(current2, 3);  // Display leakage current with 3 decimal places
  lcd.print(" A");

  // Display continuity status on the LCD
  int sensorValueContinuity = analogRead(sensorPinContinuity);  // Read input from A2
  if (sensorValueContinuity > continuityThreshold) {
    lcd.setCursor(0, 3);
    lcd.print("Continuous   ");
    digitalWrite(buzzerPinContinuity, LOW);  // Buzzer off when continuous
  } else {
    lcd.setCursor(0, 3);
    lcd.print("Discontinuous");
    digitalWrite(buzzerPinContinuity, HIGH);  // Buzzer on when discontinuous
  }

  // Control the buzzer for ACS712 sensor 2 (leakage current)
  if (current2 > currentThreshold) {
    digitalWrite(buzzerPinCurrent2, HIGH);  // Turn on the buzzer for current sensing
  } else {
    digitalWrite(buzzerPinCurrent2, LOW);   // Turn off the buzzer for current sensing
  }

  // Control the earth resistance buzzer
  if (earthResistance >= earthResistanceThreshold) {
    digitalWrite(buzzerPinEarthRes, HIGH);  // Turn on the buzzer for earth resistance warning
  } else {
    digitalWrite(buzzerPinEarthRes, LOW);   // Turn off the buzzer for earth resistance warning
  }

  delay(1000);  // Delay in milliseconds between readings
}
