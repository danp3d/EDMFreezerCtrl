#ifndef EDMFreezerCtrl_h
#define EDMFreezerCtrl_h

#include <EDMTempSensor.h>
#include <EDMRelay.h>
#import <inttypes.h>

// EDMFreezerCtrl. Controls a freezer to keep it in the defined temperature range (default: 0 to 4 deg celsius).
// Purpose: keep beer from freezing (the termostat included in most freezers will allow freezing temperatures even in the lowest settings).
// This can also be used for a fermentation chamber.
class EDMFreezerCtrl {
private:
	EDMTempSensor* _tempSensor;
	EDMRelay* _relay;
	float _minTemp = 0;
	float _maxTemp = 4;
	
	// Timers	
	unsigned long _ONTimerStart = 0;
	unsigned long _ONTimerStop = 0;
	unsigned long _OFFTimerStart = 0;
	unsigned long _OFFTimerStop = 0;
public:
	EDMFreezerCtrl(uint8_t tempSensorPin, uint8_t relayPin);
	~EDMFreezerCtrl();

	void process(); // Needs to be called inside the loop() function
	void setTempLimits(float minTemp, float maxTemp);

	float getMinTemp() { return _minTemp; }
	float getMaxTemp() { return _maxTemp; }

	void startONTimer();
	void stopONTimer();
	void startOFFTimer();
	void stopOFFTimer();

	// How many seconds the Compressor was ON/OFF. E.g:
	// Compressor turned on 5 minutes ago (and still on)
	// Compressor was off for 30 minutes prior to being turned on again
	unsigned long getSecondsON();
	unsigned long getSecondsOFF();
	
	EDMTempSensor* getTempSensor() { return _tempSensor; }
	EDMRelay* getRelay() { return _relay; }
};

#endif
