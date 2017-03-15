#ifndef EDMFreezerCtrl_h
#define EDMFreezerCtrl_h

#include <EDMTempSensor.h>
#include <EDMRelay.h>
#include <SerialComms.h>
#include <inttypes.h>

// EDMFreezerCtrl. Controls a freezer to keep it in the defined temperature range (default: 0 to 4 deg celsius).
// Purpose: keep beer from freezing (the termostat included in most freezers will allow freezing temperatures even in the lowest settings).
// This can also be used for a fermentation chamber.
class EDMFreezerCtrl {
private:
	EDMTempSensor* _tempSensor;
	EDMRelay* _relay;
	float _minTemp = 0;
	float _maxTemp = 4;

public:
	EDMFreezerCtrl(uint8_t tempSensorPin, uint8_t relayPin, SerialComms* serialComms);
	~EDMFreezerCtrl();

	void process(); // Needs to be called inside the loop() function
	void setTempLimits(float minTemp, float maxTemp);

	float getMinTemp() { return _minTemp; }
	float getMaxTemp() { return _maxTemp; }
	
	EDMTempSensor* getTempSensor() { return _tempSensor; }
	EDMRelay* getRelay() { return _relay; }
};

#endif
