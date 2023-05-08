#include "daisy_seed.h"
#include "daisysp.h"

#include "CapTouchWireling.h"

using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;

DaisySeed        hw;
I2CHandle        i2c;
CapTouchWireling c1;
Oscillator       osc;

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    for(size_t i = 0; i < size; i += 2)
    {
        out[i] = out[i + 1] = osc.Process();
    }
}

int main(void)
{
    hw.Configure();
    hw.Init();

    // audio config
    hw.SetAudioBlockSize(4);
    float sample_rate = hw.AudioSampleRate();

    // log
    hw.StartLog(true);
    System::Delay(1000);

    // initialize oscillator
    osc.Init(sample_rate);
    osc.SetAmp(0.5);
    osc.SetFreq(1000);
    osc.SetWaveform(Oscillator::WAVE_SIN);

    // initialize i2c
    I2CHandle::Config cfg;
    cfg.periph         = I2CHandle::Config::Peripheral::I2C_1;
    cfg.speed          = I2CHandle::Config::Speed::I2C_400KHZ;
    cfg.mode           = I2CHandle::Config::Mode::I2C_MASTER;
    cfg.pin_config.scl = D11;
    cfg.pin_config.sda = D12;
    i2c.Init(cfg);

    //  initialize slider
    if(int result = c1.begin(i2c))
    {
        hw.PrintLine("ERROR");
    }
    else
    {
        hw.PrintLine("yay");
    }

    // start audio
    hw.StartAudio(AudioCallback);

    // main loop
    for(;;)
    {
        bool completedTouch = c1.update();
        if(completedTouch)
        {
            uint32_t             touchMillis  = c1.duration();
            float                touchSeconds = touchMillis / 1000.0;
            FixedCapStr<5, char> str;
            str.AppendFloat(touchSeconds);
            hw.PrintLine("Completed Touch: %ss", str.Cstr());
        }

        if(c1.isTouched())
        {
            int16_t pos
                = c1.getPosition(); // getPosition() returns a position value from 0 to 100 across the sensor
            int16_t mag
                = c1.getMagnitude(); // getMagnitude() returns a relative 'pressure' value of about 50 to 500
            for(int i = 0; i < 100; i++)
            {
                if(abs(i - pos) <= (mag / 100))
                {
                    hw.Print("|");
                }
                else
                {
                    hw.Print(" ");
                }
            }

            hw.PrintLine("%d\t%d\t%ld", pos, mag, c1.duration());
        }
    }
}