#ifndef EDMRelay_h
#define EDMRelay_h

#include <inttypes.h>
#include <Arduino.h>
#include <SerialComms.h>

class EDMRelay {
private:
	uint8_t _pin;
	bool _normallyOpen = true;
	bool _sendingSignal = false;
	SerialComms* _serialComms = NULL;

public:
	inline EDMRelay(uint8_t pin, SerialComms* serialComms) { _pin = pin; _serialComms = serialComms; }
	
	inline void setNormallyOpen(bool val) { _normallyOpen = val; }
	inline bool isOpen() { return !(_normallyOpen && _sendingSignal); }
	inline void toggleState();

	inline void open() {
		digitalWrite(_pin, !_normallyOpen);
		_sendingSignal = !_normallyOpen;
    _serialComms->sendMessage(mtRelayOpen, "");
	}

	inline void close() {
		digitalWrite(_pin, _normallyOpen);
		_sendingSignal = _normallyOpen;
    _serialComms->sendMessage(mtRelayClose, "");
	}
};

#endif
