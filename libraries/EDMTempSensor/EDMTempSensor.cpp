#include "EDMTempSensor.h"
#include <OneWire.h>
#include <DallasTemperature.h>

EDMTempSensor::EDMTempSensor(uint8_t pin) {
	_pin = pin;	
	_oneWire = new OneWire(pin);
	_dallas = new DallasTemperature(_oneWire); 
	_dallas->begin();
	_temperatureAlert = NULL;
}

EDMTempSensor::~EDMTempSensor() {
	delete _dallas;
	delete _oneWire;
}

void EDMTempSensor::readSensors(void) {
	_dallas->requestTemperatures();
}

uint8_t EDMTempSensor::getSensorCount() {
	return _dallas->getDeviceCount();
}

// Function to run on the main loop
void EDMTempSensor::process(void) {
	if (!_temperatureAlert)
		return;

	this->readSensors();

	// Loop through each sensor checking the current temperature
	for (uint8_t i = 0; i < this->getSensorCount(); i++) {
		float temp = this->getTemperature(i);

		// If the temperature is outside the boundaries, run the alert
		if ((temp <= _minTemperature) || (temp >= _maxTemperature))
			_temperatureAlert(_callerPtr, this, i, temp);
	}
}
