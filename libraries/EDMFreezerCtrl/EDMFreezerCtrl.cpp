#include "EDMFreezerCtrl.h"
#include <EDMTempSensor.h>
#include <EDMRelay.h>
#include <Arduino.h>

unsigned long seconds() {
	return millis() / 1000;
}

void temperatureOutsideBounds(void* freezerCtrl, EDMTempSensor* tempSensor, uint8_t index, float temperature) {
	EDMFreezerCtrl* ctrl = static_cast<EDMFreezerCtrl*>(freezerCtrl);

	// If the temperature is greater than maxTemp, turn on the compressor
	if (temperature >= ctrl->getMaxTemp()) {
		// Close relay (allow current to go through it). This is going to turn on our compressor
		ctrl->getRelay()->close();

		// Stop the ON timer and start the OFF
		ctrl->stopOFFTimer();
		ctrl->startONTimer();
	}
	// If it's already too cold, turn off the compressor
	else if (temperature <= ctrl->getMinTemp()) {
		// Open relay (stop current from going through it). This is going to turn the compressor off
		ctrl->getRelay()->open();

		// Stop OFF timer and start the ON
		ctrl->stopONTimer();
		ctrl->startOFFTimer();
	}
}

EDMFreezerCtrl::EDMFreezerCtrl(uint8_t tempSensorPin, uint8_t relayPin) {
	_relay = new EDMRelay(relayPin);

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

// Timespan (in seconds) that the compressor has been left ON (e.g: it was turned on 5 minutes ago - and is still on - or it was on for 10 minutes - and it's currently off)
unsigned long EDMFreezerCtrl::getSecondsON() {
	if (getRelay()->isOpen()) {
		return _ONTimerStop - _ONTimerStart;
	}
	
	return seconds() - _ONTimerStart;
}

// Timespan (in seconds) that the compressor has been left OFF
unsigned long EDMFreezerCtrl::getSecondsOFF() {
	if (getRelay()->isOpen()) {
		return seconds() - _OFFTimerStart;
	}

	return _OFFTimerStop - _OFFTimerStart;
}

/// --- Timers

void EDMFreezerCtrl::startONTimer() {
	_ONTimerStart = seconds();
	_ONTimerStop = _ONTimerStart;
}

void EDMFreezerCtrl::stopONTimer() {
	_ONTimerStop = seconds();
}

void EDMFreezerCtrl::startOFFTimer() {
	_OFFTimerStart = seconds();
	_OFFTimerStop = _OFFTimerStart;
}

void EDMFreezerCtrl::stopOFFTimer() {
	_OFFTimerStop = seconds();
}
