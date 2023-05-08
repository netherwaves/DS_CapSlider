#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisy::seed;
using namespace daisysp;

DaisySeed   hw;
Oscillator  osc;
NeoPixelI2C neopixel;

#define NUMLEDS 20

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

    // initialize Neopixel
    NeoPixelI2C::Config cfg;
    // seesaw address
    cfg.transport_config.address = 0x49;
    // recommended I2C speed for seesaw;
    // 400khz is a maximum so yeah better not go that far
    cfg.transport_config.speed = I2CHandle::Config::Speed::I2C_100KHZ;
    cfg.numLEDs                = NUMLEDS;
    cfg.output_pin             = 9;

    if(neopixel.Init(cfg) == NeoPixelI2C::Result::ERR)
    {
        hw.PrintLine("ERROR INITIALIZING NEOPIXEL");
        asm("bkpt 255");
    }

    neopixel.Clear();
    for(int i = 0; i < NUMLEDS; i++)
    {
        neopixel.SetPixelColor(i,
                               neopixel.Color(127 * i / NUMLEDS,
                                              127 * (NUMLEDS - i) / NUMLEDS,
                                              0));
    }
    neopixel.Show();
    hw.PrintLine("shown");

    // initialize oscillator
    osc.Init(sample_rate);
    osc.SetAmp(0.5);
    osc.SetFreq(1000);
    osc.SetWaveform(Oscillator::WAVE_SIN);

    // start audio
    hw.StartAudio(AudioCallback);

    // main loop
    for(;;) {}
}