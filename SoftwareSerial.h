/*
SoftwareSerial.h

SoftwareSerial.cpp - Implementation of the Arduino software serial for ESP8266.
Copyright (c) 2015-2016 Peter Lerup. All rights reserved.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

*/

#ifndef SoftwareSerial_h
#define SoftwareSerial_h

#include <inttypes.h>
#include <Stream.h>


// This class is compatible with the corresponding AVR one,
// the constructor however has an optional rx buffer size.
// Speed up to 115200 can be used.


class SoftwareSerial : public Stream {
public:
  SoftwareSerial(int receivePin, int transmitPin, bool inverse_logic = false, unsigned int buffSize = 64, bool edge_triggered = false);
  virtual ~SoftwareSerial();

  void begin(long speed);
  long baudRate();
  // Transmit control pin
  void setTransmitEnablePin(int transmitEnablePin);
  // Enable or disable interrupts during tx
  void enableIntTx(bool on);

  bool overflow();
  int peek();

  virtual size_t write(uint8_t byte);
  virtual int read();
  virtual int available();
  virtual void flush();
  operator bool() {return m_rxValid || m_txValid;}

  // Disable or enable interrupts on the rx pin
  void enableRx(bool on);
  // One wire control
  void enableTx(bool on);

  void rxRead();

  volatile boolean SerialBusy = false;

  // AVR compatibility methods
  bool listen() { enableRx(true); return true; }
  void end() { stopListening(); }
  bool isListening() { return m_rxEnabled; }
  bool stopListening() { enableRx(false); return true; }

  using Print::write;

  void setWaitingForStart();
  void setStartBit(unsigned long start);
  bool propgateBits(bool level, int pulseBitLength);

private:
  bool isValidGPIOpin(int pin);

  // Member variables
  bool m_edge;
  bool m_oneWire;
  int m_rxPin, m_txPin, m_txEnablePin;
  bool m_rxValid, m_rxEnabled;
  bool m_txValid, m_txEnableValid;
  bool m_invert;
  bool m_overflow;
  unsigned long m_bitTime;
  bool m_intTxEnabled;
  unsigned int m_inPos, m_outPos;
  int m_buffSize;
  uint8_t *m_buffer;

  // Edge detection management
  enum GetByteState { awaitingStart, gotStart, readingBits };

  volatile GetByteState m_getByteState = awaitingStart;

  volatile unsigned long m_byteStart;
  volatile unsigned long m_pulseStart;

  volatile uint8_t m_rec = 0;
  volatile uint8_t m_bitNo = 0;

};

// If only one tx or rx wanted then use this as parameter for the unused pin
#define SW_SERIAL_UNUSED_PIN -1


#endif
