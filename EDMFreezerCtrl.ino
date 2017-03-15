#include <OneWire.h>
#include <DallasTemperature.h>
#include <EDMRelay.h>
#include <EDMTempSensor.h>
#include <EDMFreezerCtrl.h>
#include <PCD8544.h>
#include <SerialComms.h>

// Serial
SerialComms* serialComms = NULL;


// ==== Display pins
#define RST 12
#define CE 11
#define DC 10
#define DIN 8
#define CLK 7

PCD8544* lcd = new PCD8544(CLK,DIN,DC,RST,CE); // Display object

// Temperature sensor and relay pins
#define sensorPin 2
#define relayPin 3
EDMFreezerCtrl* freezerCtrl = NULL;

// Old temperature (to check if temp has changed)
float oldTemp = 0;

// Executed once when initialising the circuit
void setup() {
  serialComms = new SerialComms();
  
  // Initialise the display and print the title
  lcd->begin(84, 48);
  lcd->setCursor(0, 0);
  lcd->clear();
  lcd->print("Ze Kegmeister");
    
  // Configure the Relay pin as Output. No need to touch the sensor pin - the modified OneWire
  // lib will handle it, including the pullup bit.
  pinMode(relayPin, OUTPUT);
  freezerCtrl = new EDMFreezerCtrl(sensorPin, relayPin, serialComms);

  serialComms->sendMessage(mtCircuitReady, "ready");
}

// Executed every duty cicle
void loop() {
  // Let the serial read/write messages first
  serialComms->process();
  if (serialComms->hasMessage()) {
    // Process message
    message msg = serialComms->readMessage();

    if (msg.type == mtChangeUpperTempLimit) {
      freezerCtrl->setTempLimits(msg.message.toFloat(), freezerCtrl->getMaxTemp());
    }
    if (msg.type == mtChangeLowerTempLimit) {
      freezerCtrl->setTempLimits(freezerCtrl->getMinTemp(), msg.message.toFloat());
    }
  }
  
  // now we need to let the freezer control object do its thing
  freezerCtrl->process();
  float currentTemp = freezerCtrl->getTempSensor()->getTemperature(0);
  String tempStr = floatToStr(currentTemp);
  if (abs(currentTemp - oldTemp) > 0.1) {
    serialComms->sendMessage(mtTemperatureChange, tempStr);
    oldTemp = currentTemp;
  }
  
  // Print the current temperature
  lcd->setCursor(0, 1);
  lcd->print("Temp: " + tempStr + "C     ");
  
  // Print the current state of the compressor (on or off)
  lcd->setCursor(0, 2);
  lcd->print("State: ");
  lcd->print(freezerCtrl->getRelay()->isOpen() ? "OFF          " : "ON          ");

  // Print settings
  lcd->setCursor(0, 3);
  lcd->print("--- Limits:");
  lcd->setCursor(0, 4);
  lcd->print("Low: " + floatToStr(freezerCtrl->getMinTemp()) + "C       ");
  lcd->setCursor(0, 5);
  lcd->print("High: " + floatToStr(freezerCtrl->getMaxTemp()) + "C");
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
