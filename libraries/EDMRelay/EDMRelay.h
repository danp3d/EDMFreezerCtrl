#ifndef EDMRelay_h
#define EDMRelay_h

#import <inttypes.h>
#include <Arduino.h>

class EDMRelay {
private:
	uint8_t _pin;
	bool _normallyOpen = true;
	bool _sendingSignal = false;

public:
	inline EDMRelay(uint8_t pin) { _pin = pin; }
	
	inline void setNormallyOpen(bool val) { _normallyOpen = val; }
	inline bool isOpen() { return !(_normallyOpen && _sendingSignal); }
	inline void toggleState();

	inline void open() {
		digitalWrite(_pin, !_normallyOpen);
		_sendingSignal = !_normallyOpen;
	}

	inline void close() {
		digitalWrite(_pin, _normallyOpen);
		_sendingSignal = _normallyOpen;
	}
};

#endif
