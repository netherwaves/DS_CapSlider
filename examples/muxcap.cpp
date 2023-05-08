#include "daisy_seed.h"
#include "daisysp.h"

#include "OSCMessage.h"
#include "CapTouchWireling.h"
#include "I2CMultiplexer.h"

using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;

#define NUM_SLIDERS 2

DaisySeed        hw;
I2CHandle        i2c;
I2CMultiplexer   muxer;
CapTouchWireling sliders[NUM_SLIDERS];
Led              leds[NUM_SLIDERS];
Oscillator       osc;

// global rates
float cb_rate, sample_rate;

void AudioCallback(AudioHandle::InterleavingInputBuffer  in,
                   AudioHandle::InterleavingOutputBuffer out,
                   size_t                                size)
{
    // update LEDs
    for(int i = 0; i < NUM_SLIDERS; i++)
    {
        leds[i].Update();
    }

    // update soundce
    for(size_t i = 0; i < size; i += 2)
    {
        out[i] = out[i + 1] = osc.Process();
    }
}

/**
 * @brief initialize slider on specific TCA9548A channel
 *
 * @param addr index of channel
 * @param led_pin corresponding LED pin
 */
void initSlider(uint8_t addr, Pin led_pin)
{
    hw.Print("initializing slider %d ... ", addr);
    muxer.SetChannel(addr);
    if(int result = sliders[addr].begin(i2c))
    {
        hw.PrintLine("ERROR");
        asm("bkpt 255");
    }
    else
    {
        hw.PrintLine("OK");
    }

    leds[addr].Init(led_pin, false, cb_rate);
}

/**
 * @brief update LED depending on slider state
 *
 * @param addr index of channel
 */
void updateSlider(uint8_t addr)
{
    muxer.SetChannel(addr);
    sliders[addr].update();

    if(sliders[addr].isTouched())
        leds[addr].Set(255);
    else
        leds[addr].Set(0);
}

int main(void)
{
    hw.Configure();
    hw.Init();

    // audio config
    hw.SetAudioBlockSize(4);

    // set global rates
    cb_rate     = hw.AudioCallbackRate();
    sample_rate = hw.AudioSampleRate();

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

    // initialize multiplexer
    muxer.Init(i2c);

    // initialize sliders and LEDs
    initSlider(0, D15);
    initSlider(1, D16);

    // start audio
    hw.StartAudio(AudioCallback);

    // main loop
    for(;;)
    {
        updateSlider(0);
        updateSlider(1);
    }
}