#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

Oscillator osc;
DaisySeed  hw;

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
    hw.SetAudioBlockSize(4);
    float sample_rate = hw.AudioSampleRate();

    // initialize oscillator
    osc.Init(sample_rate);
    osc.SetAmp(0.5);
    osc.SetFreq(1000);
    osc.SetWaveform(Oscillator::WAVE_SIN);

    // start audio
    hw.StartAudio(AudioCallback);

    // loop
    for(;;) {}
}