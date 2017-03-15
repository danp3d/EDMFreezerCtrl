#include "EDMFreezerCtrl.h"
#include <EDMTempSensor.h>
#include <EDMRelay.h>
#include <Arduino.h>
#include <SerialComms.h>

void temperatureOutsideBounds(void* freezerCtrl, EDMTempSensor* tempSensor, uint8_t index, float temperature) {
	EDMFreezerCtrl* ctrl = static_cast<EDMFreezerCtrl*>(freezerCtrl);

	// If the temperature is greater than maxTemp, turn on the compressor
	if (temperature >= ctrl->getMaxTemp()) {
		// Close relay (allow current to go through it). This is going to turn on our compressor
    if (ctrl->getRelay()->isOpen())
      ctrl->getRelay()->close();
	}
	// If it's already too cold, turn off the compressor
	else if (temperature <= ctrl->getMinTemp()) {
		// Open relay (stop current from going through it). This is going to turn the compressor off
    if (!ctrl->getRelay()->isOpen())
      ctrl->getRelay()->open();
	}
}

EDMFreezerCtrl::EDMFreezerCtrl(uint8_t tempSensorPin, uint8_t relayPin, SerialComms* serialComms) {
	_relay = new EDMRelay(relayPin, serialComms);

	_tempSensor = new EDMTempSensor(tempSensorPin);
	_tempSensor->setTemperatureAlertHandler(this, &temperatureOutsideBounds);
	_tempSensor->setTemperatureLimits(_minTemp, _maxTemp);
}

EDMFreezerCtrl::~EDMFreezerCtrl() {
	delete _tempSensor;
	delete _relay;
}

void EDMFreezerCtrl::process() {
	_tempSensor->process();
}

void EDMFreezerCtrl::setTempLimits(float minTemp, float maxTemp) {
	_minTemp = minTemp;
	_maxTemp = maxTemp;
	
	_tempSensor->setTemperatureLimits(_minTemp, _maxTemp);
}

