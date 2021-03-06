#ifndef EDMTempSensor_h
#define EDMTempSensor_h


#include <inttypes.h>
#include <OneWire.h>
#include <DallasTemperature.h>

class EDMTempSensor;

typedef void (*temperatureAlert)(void*,EDMTempSensor*,uint8_t,float); // callerptr, pointer to the Sensor object, sensor index and temperature

class EDMTempSensor {
private:
	// Actual pin to which the sensor is connected
	uint8_t _pin;
	OneWire* _oneWire;
	DallasTemperature* _dallas;
	temperatureAlert _temperatureAlert; // Event handler
	void* _callerPtr;

	// Boundaries - if any sensor reads a temperature outside of the following readings, execute the event handler
	float _minTemperature = 0;
	float _maxTemperature = 4;
public:
	// Ctor/Dtor
	EDMTempSensor(uint8_t pin);
	~EDMTempSensor();

	// Wrappers to the DallasTemperature class
	inline void readSensors(void);
	inline uint8_t getSensorCount();
	inline float getTemperature(uint8_t index) { return _dallas->getTempCByIndex(index); }

	// Define temperature limits (if readings are outside of those values, we call the temperatureAlert function)
	inline void setTemperatureLimits(float minTemperature, float maxTemperature) { _minTemperature = minTemperature; _maxTemperature = maxTemperature; } 

	// Events
	inline temperatureAlert getTemperatureAlertHandler() { return _temperatureAlert; }
	inline void setTemperatureAlertHandler(void* callerPtr, temperatureAlert handler) { _callerPtr = callerPtr; _temperatureAlert = handler; }

	// to run on main loop
	void process(void);
};
#endif
