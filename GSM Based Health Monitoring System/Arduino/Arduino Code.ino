#include <LiquidCrystal.h>
#include <PulseSensorPlayground.h>
#include <SoftwareSerial.h>

// Pin Definitions
const int PulseWire = A0;
const int PressurePin = A2;
const int TempPin = A1;

// Constants
const int Threshold = 550;
const int HeartbeatLowerLimit = 60;
const int HeartbeatUpperLimit = 70;
const float TempLowerLimit = 34.0;
const float TempUpperLimit = 38.0;
const float PressureLowerLimit = 90.0;
const float PressureUpperLimit = 120.0;

// Global objects
PulseSensorPlayground pulseSensor;
LiquidCrystal lcd(13, 12, 11, 10, 9, 8);
SoftwareSerial gsm(2, 3);

void setup() {
  Serial.begin(9600);
  lcd.begin(20, 4);
  pulseSensor.analogInput(PulseWire);
  pulseSensor.setThreshold(Threshold);
  if (pulseSensor.begin()) {
    
  }
}

void loop() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Health Monitoring");
  lcd.setCursor(0, 2);
  lcd.print("Measuring Data");
  lcd.setCursor(0, 3);
  lcd.print("Wait Some Time...");

  delay(5000);

  int heartRate = pulseSensor.getBeatsPerMinute();
  float temperature_C = readTemperature();
  float pressure_kPa = readPressure();
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Heart Rate: ");
  lcd.print(heartRate);
  lcd.print(" BPM");

  lcd.setCursor(0, 1);
  lcd.print("Temperature: ");
  lcd.print(temperature_C, 1);
  lcd.print(" C");

  lcd.setCursor(0, 2);
  lcd.print("Pressure: ");
  lcd.print(pressure_kPa, 1);
  lcd.print(" kPa");

  if (heartRate < HeartbeatLowerLimit || heartRate > HeartbeatUpperLimit) {
    Serial.println("Abnormal heartbeat detected!");
    lcd.setCursor(0, 3);
    sendSMS("Abnormal heartbeat detected!");
  }
  if (temperature_C < TempLowerLimit || temperature_C > TempUpperLimit) {
    Serial.println("Abnormal temperature detected!");
    lcd.setCursor(0, 3);
    sendSMS("Abnormal temperature detected!");
  }
  if (pressure_kPa < PressureLowerLimit || pressure_kPa > PressureUpperLimit) {
    Serial.println("Abnormal blood pressure detected!");
    lcd.setCursor(0, 3);
    sendSMS("Abnormal blood pressure detected!");
  }

  delay(1000);
}

float readPressure() {
  int pressureValue = analogRead(PressurePin);
  float pressure_kPa = ((float)pressureValue / 1023.0) * 100;
  return pressure_kPa;
}

float readTemperature() {
  int temperatureValue = analogRead(TempPin);
  float voltage = (temperatureValue / 1023.0) * 5.0;
  float temperature_C = voltage * 100.0;
  return temperature_C;
}

void sendSMS(String message) {
  Serial.println("Sending SMS...");
  gsm.println("AT+CMGF=1");
  delay(1000);
  gsm.print("AT+CMGS=\"");
  gsm.print("+251949055321");
  gsm.println("\"");
  delay(1000);
  gsm.println(message);
  delay(100);
  gsm.write(0x1A);
  delay(1000);
  Serial.println("SMS sent.");
}
