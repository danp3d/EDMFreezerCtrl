#ifndef SERIALCOMMS_h
#define SERIALCOMMS_h

#include <Arduino.h>

enum messageTypes {
  mtUnknown = 0,
  mtTemperatureChange,
  mtRelayOpen,
  mtRelayClose,
  mtChangeUpperTempLimit,
  mtChangeLowerTempLimit,
  mtCircuitReady
};

struct message {
  messageTypes type;
  String message;
};

class SerialComms {
  private:
    bool _isReceiving;
    message _buffer;

  public:
    inline SerialComms() { _isReceiving = false; _buffer.message = ""; _buffer.type = mtUnknown; Serial.begin(9600); }

    inline bool isReceiving() { return _isReceiving; }
    inline bool hasMessage() { return !_isReceiving && (_buffer.message != ""); }
    message readMessage();
    bool sendMessage(messageTypes t, String message);
    
    void process();
};

#endif

