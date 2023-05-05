#include "daisy.h"

#define TCAADDR 0x70

namespace daisy
{
  class I2CMultiplexer
  {
  public:
    I2CMultiplexer(){};

    void Init(I2CHandle &i2c, uint8_t addr = 0)
    {
      _i2c = &i2c;
      _addr = addr;
    }

    inline void SetChannel(uint8_t chan)
    {
      if (chan > 7)
        return;

      uint8_t buf = 1 << chan;
      _i2c->TransmitBlocking(TCAADDR + _addr, &buf, 1, 1000);
    }

  private:
    I2CHandle *_i2c;
    uint8_t _addr;
  };
}