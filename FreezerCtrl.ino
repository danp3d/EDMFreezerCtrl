#include <OneWire.h>
#include <DallasTemperature.h>
#include <EDMRelay.h>
#include <EDMTempSensor.h>
#include <EDMFreezerCtrl.h>
#include <PCD8544.h>

// ==== Display pins
#define RST 12
#define CE 11
#define DC 10
#define DIN 8
#define CLK 7

PCD8544* lcd = new PCD8544(CLK,DIN,DC,RST,CE); // Display object

// Temperature sensor and relay pins
#define sensorPin 9
#define relayPin 13
EDMFreezerCtrl* freezerCtrl = NULL;

// Executed once when initialising the circuit
void setup() {
  // Initialise the display and print the title
  lcd->begin(84, 48);
  lcd->setCursor(0, 0);
  lcd->clear();
  lcd->print("Dan's Freezer");
  
  // Configure the Relay pin as Output. No need to touch the sensor pin - the modified OneWire
  // lib will handle it, including the pullup bit.
  pinMode(relayPin, OUTPUT);
  freezerCtrl = new EDMFreezerCtrl(sensorPin, relayPin);
}

// Executed every duty cicle
void loop() {
  // First thing - we need to let the freezer control object do its thing
  freezerCtrl->process();
  
  // Print the current temperature
  lcd->setCursor(0, 1);
  lcd->print("Temp: " + floatToStr(freezerCtrl->getTempSensor()->getTemperature(0)) + "C     ");
  
  // Print the current state of the compressor (on or off)
  lcd->setCursor(0, 2);
  lcd->print("State: ");
  lcd->print(freezerCtrl->getRelay()->isOpen() ? "OFF          " : "ON          ");
  
  // Time on (or time spent on before turning off)
  lcd->setCursor(0,3);
  lcd->print("ON: " + formatTime(freezerCtrl->getSecondsON()) + "          ");
  
  // Time off (or time spent off before turning on)
  lcd->setCursor(0,4);
  lcd->print("OFF: " + formatTime(freezerCtrl->getSecondsOFF()) + "          ");
}

String formatTime(long seconds) {
  return toHours(seconds) + ':' + toRemainingMins(seconds) + ':' + toRemainingSecs(seconds);
}

String toHours(long seconds) {
  return String(seconds / 3600);
}

String toRemainingMins(long seconds) {
  return String((seconds % 3600) / 60);
}

String toRemainingSecs(long seconds) {
  return String(((seconds % 3600) % 60));
}

String floatToStr(float val) {
  char charVal[10] = "         ";
  dtostrf(val, 4, 2, charVal);
  
  String strVal = "";
  for (int i = 0; i < sizeof(charVal); i++) {
    if (charVal[i] != ' ')
      strVal += charVal[i];
     else
       break;
  }
  
  return strVal;
}
