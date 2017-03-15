#include "SerialComms.h"
#include <Arduino.h>

bool SerialComms::sendMessage(messageTypes t, String message) {
  if (isReceiving())
    return false;

  Serial.println("{[" + String((int)t) + "]" + message + "}");
  return true;
}

message SerialComms::readMessage() {
  message result;

  if (!hasMessage()) {
    result.type = mtUnknown;
    result.message = "";
    return result;
  }

  result.type = this->_buffer.type;
  result.message = this->_buffer.message;
  this->_buffer.type = mtUnknown;
  this->_buffer.message = "";
  return result;
}

void SerialComms::process() {
  while (Serial.available()) {
    _isReceiving = true;
    char in = (char)Serial.read();
    this->_buffer.message += in;

    if (in == '}') {
      messageTypes type = mtUnknown;

      int endType = this->_buffer.message.indexOf(']') + 1;
      String strType = this->_buffer.message.substring(2, endType);

      this->_buffer.type = (messageTypes)strType.toInt();
      this->_buffer.message = this->_buffer.message.substring(endType, this->_buffer.message.length() - 1);

      _isReceiving = false;
    }
  }
}
