#include "daisy_seed.h"
#include "daisysp.h"

using namespace daisy;
using namespace daisysp;

// hardware
DaisySeed hw;

// DSP
Oscillator osc;

// SD card
WavPlayer sampler;
SdmmcHandler sdcard;
FatFSInterface fsi;

// audio callback
void AudioCallback(AudioHandle::InterleavingInputBuffer in, AudioHandle::InterleavingOutputBuffer out, size_t size)
{
  for (size_t i = 0; i < size; i += 2)
  {
    out[i] = out[i+1] = (osc.Process() + s162f(sampler.Stream())) * 0.5f;
  }
}

// main
int main(void)
{
  // configure hardware
  hw.Configure();
  hw.Init();
  hw.SetAudioBlockSize(4);
  float sample_rate = hw.AudioSampleRate();

  // initialize oscillator
  osc.Init(sample_rate);
  osc.SetAmp(0.5);
  osc.SetFreq(1000);
  osc.SetWaveform(Oscillator::WAVE_SIN);

  // initialize SD card
  SdmmcHandler::Config cfg;
  cfg.Defaults();
  sdcard.Init(cfg);

  // initialize FatFS + mount SD card
  fsi.Init(FatFSInterface::Config::MEDIA_SD);
  f_mount(&fsi.GetSDFileSystem(), fsi.GetSDPath(), 1);

  // initialize sampler
  sampler.Init(fsi.GetSDPath());

  // start audio
  hw.StartAudio(AudioCallback);

  // main loop
  for(;;)
  {
    sampler.Prepare();
  }
}