/*
ATtiny841Lib.cpp - Last modified 24 Feb 2020

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

Written by Ben Rose for TinyCircuits.

The latest version of this library can be found at https://tinycircuits.com/
*/

#include "ATtiny841Lib.h"
#include <inttypes.h>

namespace daisy
{
  ATtiny841Lib::ATtiny841Lib(I2CHandle *i2c) : _i2c(i2c)
  {
  }

  void ATtiny841Lib::writeByte(uint8_t b1)
  {
    _i2c->TransmitBlocking(address, &b1, 1, 1000);
  }

  void ATtiny841Lib::writeByte(uint8_t b1, uint8_t b2)
  {
    uint8_t data[2] = {b1, b2};
    _i2c->TransmitBlocking(address, data, 2, 1000);
  }

  void ATtiny841Lib::writeByte(uint8_t b1, uint8_t b2, uint8_t b3)
  {
    uint8_t data[3] = {b1, b2, b3};
    _i2c->TransmitBlocking(address, data, 3, 1000);
  }

  void ATtiny841Lib::writeCommand(uint8_t cmd, uint16_t val)
  {
    uint8_t data[3] = {
        cmd,
        (uint8_t)(val & 0xff), (uint8_t)((val >> 8) & 0xff)};
    _i2c->TransmitBlocking(address, data, 3, 1000);
  }

  void ATtiny841Lib::writeCommand(uint8_t cmd, uint16_t val1, uint16_t val2)
  {
    uint8_t data[5] = {
        cmd,
        (uint8_t)(val1 & 0xff), (uint8_t)((val1 >> 8) & 0xff),
        (uint8_t)(val2 & 0xff), (uint8_t)((val2 >> 8) & 0xff)};
    _i2c->TransmitBlocking(address, data, 5, 1000);
  }

  void ATtiny841Lib::writeCommand(uint8_t cmd, uint16_t val1, uint16_t val2, uint16_t val3, uint16_t val4)
  {
    uint8_t data[9] = {
        cmd,
        (uint8_t)(val1 & 0xff),
        (uint8_t)((val1 >> 8) & 0xff),
        (uint8_t)(val2 & 0xff),
        (uint8_t)((val2 >> 8) & 0xff),
        (uint8_t)(val3 & 0xff),
        (uint8_t)((val3 >> 8) & 0xff),
        (uint8_t)(val4 & 0xff),
        (uint8_t)((val4 >> 8) & 0xff),
    };
    _i2c->TransmitBlocking(address, data, 9, 1000);
  }

  uint8_t ATtiny841Lib::read(uint8_t reg)
  {
    writeByte(reg);
    _i2c->ReceiveBlocking(address, &_buf, 1, 1000);
    return _buf;
  }

  void ATtiny841Lib::setFailsafe(uint16_t ms)
  {
    if (ms > 0x3FFF)
      ms = 0x3FFF;
    writeCommand(COMMAND_SET_FAILSAFE_TIMEOUT, ms * 4); // using defualt settings- really ~1.024ms
  }

  void ATtiny841Lib::setTimerPrescaler(uint8_t prescaler)
  {
    if (prescaler >= T841_TIMER_PRESCALER_0 && prescaler <= T841_TIMER_PRESCALER_1024)
    {
      writeByte(COMMAND_PRESCALER_1, prescaler);
      writeByte(COMMAND_PRESCALER_2, prescaler);
    }
  }

  void ATtiny841Lib::powerDown()
  {
    writeByte(COMMAND_SET_SLEEP_MODE, T841_SLEEP_MODE_PWR_DOWN);
    writeByte(COMMAND_SLEEP);
  }
}