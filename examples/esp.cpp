#include "daisy_seed.h"
#include "daisysp.h"
#include "wifi.h"

using namespace daisy;
using namespace daisy::seed;

DaisySeed   hw;
UartHandler uart;
WiFi        wifi;

int main(void)
{
    hw.Configure();
    hw.Init();

    hw.StartLog(true);
    System::Delay(1000);

    // UART
    UartHandler::Config uart_cfg;
    uart_cfg.periph        = UartHandler::Config::Peripheral::USART_1;
    uart_cfg.mode          = UartHandler::Config::Mode::TX_RX;
    uart_cfg.baudrate      = 115200;
    uart_cfg.pin_config.rx = D14;
    uart_cfg.pin_config.tx = D13;

    // configure wifi
    WiFi::Config cfg;
    cfg.mode     = WiFi::Config::Mode::BOTH;
    cfg.uart_cfg = uart_cfg;
    cfg.mux      = true;

    // init wifi
    hw.Print("init wifi...");
    if(wifi.Init(cfg) != WiFi::Result::OK)
    {
        hw.PrintLine("ERROR");
        asm("bkpt 255");
    }
    hw.PrintLine("OK");

    // connect to router
    hw.Print("connecting...");
    if(wifi.Connect("CHOUFIVER", "f19v10reiy") != WiFi::Result::OK)
    {
        hw.PrintLine("ERROR");
        asm("bkpt 255");
    }
    hw.PrintLine("OK");

    // for posterity ...
    hw.SetLed(1);

    // loop
    for(;;) {}
}